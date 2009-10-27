#ifndef __INC_MVDS_SYSINFO_HH__
#define __INC_MVDS_SYSINFO_HH__

#include "../TimeVal/timeval.hh"

#include <string>
#include <list>

namespace mvds {

  class SysInfo {
  public:

	struct User {
	  std::string name;
	  std::string host;
	  size_t pid;
	  TimeVal t;
	};

	typedef std::list<User> UserList;

	UserList users();

  };


};

#endif // __INC_MVDS_SYSINFO_HH__
