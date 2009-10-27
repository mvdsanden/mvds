#ifndef __INC_MVDS_CONDITIONSOCK_HH_
#define __INC_MVDS_CONDITIONSOCK_HH_

#include "../TimeSpec/timespec.hh"
#include "../Socket/socket.hh"
#include "../SocketAddress/socketaddress.hh"
//#include "../Logger/logger.hh"

#include <cstdlib>
#include <iostream>

namespace mvds {

  class ConditionSock {


    void destroy();

    ConditionSock(ConditionSock const &); // NI
    ConditionSock &operator=(ConditionSock const &); // NI

  public:

    ConditionSock();

    ~ConditionSock();

    int signal();

    int broadcast();

    int wait();

    int timedWait(TimeSpec const ts);

	//    int fd();

	File file();

    void readAll();

  private:

	std::pair<Socket,Socket> d_sockets;
    char                     d_buf[24];

  };

  inline void ConditionSock::readAll()
  {
	assert(d_sockets.second.fd() > 0);
    d_sockets.second.read(d_buf,1);
  }

  inline void ConditionSock::destroy()
  {
	std::cerr << "ConditionSock::destroy()" << std::endl;
	d_sockets.first.close();
	d_sockets.second.close();
  }

  inline ConditionSock::ConditionSock()
	: d_sockets(Socket::pair())
  {
  }

  inline ConditionSock::~ConditionSock() { destroy(); }

  inline int ConditionSock::signal()
  {
    d_sockets.first.write("1",1);
    return 0;
  }

  inline int ConditionSock::broadcast()
  {
    d_sockets.first.write("1",1);
    return 0;
  }

  inline int ConditionSock::wait()
  {
    fd_set read_set;
    int max_descr = d_sockets.second.fd() + 1;

    while (true) {
      FD_ZERO(&read_set);
      FD_SET(d_sockets.second.fd(),&read_set);

      int res = select(max_descr,&read_set,0,0,0);

      if (res == -1)
		throw IOError(errno,"ConditionSock::select()");
      else if (res && FD_ISSET(d_sockets.second.fd(),&read_set))
		break;
    }

    readAll();

    return 0;
  }

  inline int ConditionSock::timedWait(TimeSpec const ts)
  {
    fd_set read_set;

    int max_descr = d_sockets.second.fd() + 1;

    while (true) {
      FD_ZERO(&read_set);
      FD_SET(d_sockets.second.fd(),&read_set);

      int res = pselect(max_descr,&read_set,0,0,ts,0);

      if (res == -1)
		throw IOError(errno,"ConditionSock::pselect()");
      else if (res && FD_ISSET(d_sockets.second.fd(),&read_set))
		break;
    }

    readAll();

    return 0;
  }

  /*
  inline int ConditionSock::fd()
  {
    return d_sockets.second.fd();
  }
  */

  inline File ConditionSock::file()
  {
	return d_sockets.second;
  }

};


#endif // __INC_MVDS_CONDITIONSOCK_HH_
