#ifndef __INC_MVDS_SELECT_HH__
#define __INC_MVDS_SELECT_HH__

#include <map>
#include <list>
#include <set>

#include "../File/file.hh"
#include "../TimeVal/timeval.hh"

#include "../RefAble/refable.hh"
#include "../Ref/rf.hh"

namespace mvds {

  class Select : public std::map< File, std::pair<unsigned,unsigned> > {
  public:

	enum Flags {

	  Nothing = 0x00,
	  Read    = 0x01,
	  Write   = 0x02,
	  RW      = 0x03,
	  Except  = 0x04,
	  All     = 0x07,
	  
	  Delete  = 0x08,

	  DispatchNone   = 0x00,
	  DispatchRead   = 0x10,
	  DispatchWrite  = 0x20,
	  DispatchRW     = 0x30,
	  DispatchExcept = 0x40,
	  DispatchAll    = 0x70,
	  DispatchDelete = 0x80,

	};

	struct DispatchFunction : public RefAble {
	  virtual void operator()(File &fd, unsigned flags) = 0;
	};

	~Select();

	void insert(File const &fd, unsigned flags = All);

	void dispatch(File const &fd, unsigned flags, rf<DispatchFunction> func);

	void remove(File const &fd);

	void removeHard(File const &fd);

	void change(File const &fd, unsigned flags = All);

	bool select(TimeVal &tv);

	unsigned check(File const &fd) const;

	bool check(File const &fd, unsigned flags) const;

	void timedRun(TimeVal const &t);

  private:

	static void sigAlarm(int val);

	struct Dispatch {

	  unsigned flags;

	  rf<DispatchFunction> readFunction;
	  rf<DispatchFunction> writeFunction;
	  rf<DispatchFunction> exceptFunction;
	  rf<DispatchFunction> deleteFunction;

	};

	std::map<File,Dispatch> d_dispatches;

	std::list<std::pair< File,std::pair< rf<DispatchFunction> ,unsigned> > > d_dispatchQueue;

	std::set<File> d_deleteSet;

  };

  inline void Select::insert(File const &fd, unsigned flags)
  {
	iterator i = find(fd);

	if (i != end())
	  (*i).second.first |= flags;
	else
	  (*this)[fd] = std::make_pair(flags,(unsigned)0);
	//	  insert(std::make_pair(fd,std::make_pair(flags,(unsigned)0)));
  }

  inline void Select::dispatch(File const &fd, unsigned flags, rf<DispatchFunction> func)
  {
	insert(fd,flags | flags << 4);

	Dispatch &d = d_dispatches[fd];

	d.flags |= flags;

	if (flags & Read)   d.readFunction   = func;
	if (flags & Write)  d.writeFunction  = func;
	if (flags & Except) d.exceptFunction = func;
	if (flags & Delete) d.deleteFunction = func;
  }

  inline void Select::remove(File const &fd)
  {
	d_deleteSet.insert(fd);
  }

  inline void Select::change(File const &fd, unsigned flags)
  {
	iterator i = find(fd);

	if (i != end())
	  (*i).second.first = flags;
	else
	  (*this)[fd] = std::make_pair(flags,(unsigned)0);
  }

  inline unsigned Select::check(File const &fd) const
  {
	const_iterator i = find(fd);

	if (i == end())
	  return 0;
	else
	  return (*i).second.second;
  }

  inline bool Select::check(File const &fd, unsigned flags) const
  {
	return check(fd) & flags;
  }

  inline void Select::timedRun(TimeVal const &t)
  {
	TimeVal t0 = TimeVal::now(), dt;

	while (dt < t) {
	  TimeVal time = t - dt;
	  select(time);
	  dt = TimeVal::now() - t0;
	}
  }

};

#endif // __INC_MVDS_SELECT_HH__
