#include "select.ih"

#include <cerrno>
#include <cstring>
#include <iostream>

#include <signal.h>

bool Select::select(TimeVal &tv)
{

  fd_set readSet, writeSet, exceptSet;

  int maxDescr = 0;

  // Zero fd sets.
  FD_ZERO(&readSet);
  FD_ZERO(&writeSet);
  FD_ZERO(&exceptSet);

  // Process all deleted.
  for (set<File>::iterator i = d_deleteSet.begin();
	   i != d_deleteSet.end(); ++i) {

	iterator j = find(*i);

	// Find dispatch.
	std::map<File,Dispatch>::iterator k = d_dispatches.find(*i);

	// Do dispatch.
	if (j != end() &&
		(*j).second.first & DispatchDelete &&
		k != d_dispatches.end() &&
		(*k).second.deleteFunction) {
	  d_dispatchQueue.push_back(make_pair((*i),make_pair((*k).second.deleteFunction,Delete)));
	} else if (k != d_dispatches.end())
	  d_dispatches.erase(k);

	if (j != end())
	  erase(j);
  }

  d_deleteSet.clear();

  // Iterate through all socket connections.
  for (iterator i = begin(); i != end(); ++i) {

	//	cerr << "Adding file " << (*i).first.fd() << " to select pool" << endl;

	if ((*i).first.fd() >= 0) {
	  // Add to read set if flag is set.
	  if ((*i).second.first & Read)
		FD_SET((*i).first.fd(),&readSet);

	  // Add to write set if flag is set.
	  if ((*i).second.first & Write)
		FD_SET((*i).first.fd(),&writeSet);

	  // Always add to the exception set.
	  FD_SET((*i).first.fd(),&exceptSet);

	  // Set max descriptor.
	  if (maxDescr < (*i).first.fd())
		maxDescr = (*i).first.fd();

	} else {

	  d_deleteSet.insert((*i).first);

	}

  }

  // Select seems to hang sometimes for no apparent reason. Luckily
  // a signal should break this.. Hopefully it won't break something else :).
  // This is a problem for multithreaded programs, exactly where this
  // class is conveniant, so I need to find another solution...
  // It works though, as long as no ther threads use SIGALRM.
  signal(SIGALRM,sigAlarm);
  alarm(10);

  // Do the actual select.
  int res = ::select(maxDescr+1,&readSet,&writeSet,&exceptSet,tv);

  if (res == -1) {

	if (errno == EINTR)
	  cerr << "Select(): interrupted by signal." << endl;
	else {
	  alarm(0);
	  signal(SIGALRM,SIG_DFL);
	  throw runtime_error(std::string() + "select():" + strerror(errno));
	}

	alarm(0);
	signal(SIGALRM,SIG_DFL);

  } else if (res) {

	alarm(0);
	signal(SIGALRM,SIG_DFL);

	// Check select results.
	for (iterator i = begin(); i != end(); i++) {

	  (*i).second.second = 0;

	  if (((*i).second.first & Read) && FD_ISSET((*i).first.fd(),&readSet)) {
		(*i).second.second |= Read;

		if ((*i).second.first & DispatchRead) {

		  std::map<File,Dispatch>::iterator j = d_dispatches.find((*i).first);

		  if (j != d_dispatches.end() && (*j).second.readFunction) {
			d_dispatchQueue.push_back(make_pair((*i).first,
										 make_pair((*j).second.readFunction,
												   Read)));
		  }
		}
	  }

	  if (((*i).second.first & Write) && FD_ISSET((*i).first.fd(),&writeSet)) {
		(*i).second.second |= Write;

		if ((*i).second.first & DispatchWrite) {
		  std::map<File,Dispatch>::iterator j = d_dispatches.find((*i).first);

		  if (j != d_dispatches.end() && (*j).second.writeFunction) {
			d_dispatchQueue.push_back(make_pair((*i).first,
												make_pair((*j).second.writeFunction,
												   Write)));
		  }
		}
	  }

	  if (FD_ISSET((*i).first.fd(),&exceptSet)) {
		(*i).second.second |= Except;

		cerr << "Warning: select error: exception on fd " << (*i).first.fd() << ": " << strerror(errno) << "." << endl;

		if ((*i).second.first & DispatchExcept) {
		  std::map<File,Dispatch>::iterator j = d_dispatches.find((*i).first);

		  if (j != d_dispatches.end() && (*j).second.exceptFunction) {
			d_dispatchQueue.push_back(make_pair((*i).first,
												make_pair((*j).second.exceptFunction,
														  Write)));
		  }
		}
	  }

	}

	// Process all dispatches!
	while (!d_dispatchQueue.empty()) {

	  pair< File,pair< rf<DispatchFunction> ,unsigned> > d = d_dispatchQueue.front();
	  d_dispatchQueue.pop_front();

	  if (d_deleteSet.find(d.first) == d_deleteSet.end())
		(*d.second.first)(d.first,d.second.second);

	}

	return true;
  }

  return false;
}

Select::~Select()
{
}
