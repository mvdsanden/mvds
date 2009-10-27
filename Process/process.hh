#ifndef __INC_MVDS_PROCESS_HH__
#define __INC_MVDS_PROCESS_HH__

#include <string>
#include <istream>
#include <sys/wait.h>
#include <signal.h>
#include <cassert>

namespace mvds {

  class Process {

  public:

	Process();

	Process(std::string const &command);

	int execute(std::string const &command);

	int pid() const;

	int wait();

	int kill(int sig = SIGTERM);

	~Process();

	std::ostream &in();

	std::istream &out();

	std::istream &err();

	int fdIn();

	int fdOut();

	int fdErr();

  private:

	int d_pid;

	int d_inPipe[2];
	int d_outPipe[2];
	int d_errPipe[2];

	std::ostream *d_in;
	std::istream *d_out;
	std::istream *d_err;

  };

  inline Process::Process()
	: d_pid(0),
	  d_in(0),
	  d_out(0),
	  d_err(0)
  {
  }

  inline Process::Process(std::string const &command)
  {
	execute(command);
  }

  inline Process::~Process()
  {
	if (d_pid)
	  wait();

	if (d_in) {
	  delete d_in;
	  close(d_inPipe[1]);
	}

	if (d_out) {
	  delete d_out;
	  close(d_outPipe[0]);
	}

	if (d_err) {
	  delete d_err;
	  close(d_errPipe[0]);
	}
  }

  inline int Process::pid() const
  {
	return d_pid;
  }

  inline int Process::wait()
  {

	int status;

	for (int res = 0; res != d_pid; res = waitpid(d_pid,&status,0));

	d_pid = 0;

	close(d_inPipe[0]);
	close(d_outPipe[1]);
	close(d_errPipe[1]);

	close(d_inPipe[1]);
	close(d_outPipe[0]);
	close(d_errPipe[0]);

	delete d_in;
	delete d_out;
	delete d_err;

	d_in  = 0;
	d_out = 0;
	d_err = 0;

	if (WIFEXITED(status))
	  return WEXITSTATUS(status);
	else
	  return -1;

  }

  inline int Process::kill(int sig)
  {
	assert(d_pid);
	return ::kill(d_pid,sig);
  }

  inline std::ostream &Process::in()
  {
	assert(d_in);
	return *d_in;
  }

  inline std::istream &Process::out()
  {
	assert(d_out);
	return *d_out;
  }

  inline std::istream &Process::err()
  {
	assert(d_err);
	return *d_err;
  }

  inline int Process::fdIn()
  {
	return d_inPipe[1];
  }

  inline int Process::fdOut()
  {
	return d_outPipe[0];
  }

  inline int Process::fdErr()
  {
	return d_errPipe[0];
  }

};

#endif // __INC_MVDS_PROCESS_HH__

