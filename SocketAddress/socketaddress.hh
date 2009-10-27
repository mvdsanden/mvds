/*
 *  Socket Class
 *
 *  Author: Mart van de Sanden
 *
 *  License: GPL
 */

#ifndef __INC_MVDS_SOCKETADDRESS_HH_
#define __INC_MVDS_SOCKETADDRESS_HH_

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <cerrno>
#include <string>

namespace mvds {

  class SocketAddress {

    friend std::ostream &operator<<(std::ostream &, const SocketAddress &);

    union sock {
      struct sockaddr s;
      struct sockaddr_in i;
	  struct sockaddr_un u;
    };

    sock d_sockAddress;

	unsigned d_length;

    void NSLookup(std::string const &_host_name);

    void copy(SocketAddress const &_other);
    void destroy();

  public:

    SocketAddress();

    SocketAddress(SocketAddress const &_other);

    SocketAddress(int family, int port, in_addr_t addr = INADDR_ANY);

    SocketAddress(int port, in_addr_t = INADDR_ANY);

    SocketAddress(int family, int port, std::string const &hostname);

    SocketAddress(int port, std::string const &hostname);
	
	/**
	 *  For use with unix sockets.
	 */
	SocketAddress(std::string const &path);

    SocketAddress &operator=(SocketAddress const &_other);

    ~SocketAddress();

    int family() const;
    int port() const;

    operator sockaddr*();

    operator sockaddr_in*();

    unsigned length() const;

  };

  inline int SocketAddress::family() const
  {
	return d_sockAddress.i.sin_family;
  }

  inline int SocketAddress::port() const
  {
	return ntohs(d_sockAddress.i.sin_port);
  }

  inline SocketAddress::operator sockaddr*()
  {
	return &(d_sockAddress.s);
  }

  inline SocketAddress::operator sockaddr_in*()
  {
	return &(d_sockAddress.i);
  }

  inline unsigned SocketAddress::length() const
  {
	return d_length; //sizeof(d_sockAddress);
  }

  std::ostream &operator<<(std::ostream &, const SocketAddress &);

};

#endif //  __INC_MVDS_SOCKETADDRESS_HH_
