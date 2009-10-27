#include "sysinfo.ih"

#include <fstream>
#include <utmp.h>

#include <iostream>

SysInfo::UserList SysInfo::users()
{

  UserList l;

  ifstream ut("/var/run/utmp");

  utmp entry;

  User user;

  while (ut) {

	ut.read(reinterpret_cast<char*>(&entry),sizeof(entry));

	if (!ut)
	  break;

	//	cout << "Type: " << entry.ut_type << endl
	//		 << "User: " << entry.ut_user << endl
	//		 << "Host: " << entry.ut_host << endl;

	if (entry.ut_type == USER_PROCESS) {

	  user.name = entry.ut_user;
	  user.host = entry.ut_host;
	  user.t    = entry.ut_tv;

	  l.push_back(user);

	}

  }

  return l;
}
