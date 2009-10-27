#ifndef __INC_MVDS_SOCKET_HH__
#define __INC_MVDS_SOCKET_HH__

#include "../File/file.hh"

#include <sys/socket.h>
#include <string>
#include <cassert>

#include "../SocketAddress/socketaddress.hh"

namespace mvds {

  class Socket : public File {
  public:

	static std::pair<Socket,Socket> pair();

	Socket();

	Socket(int socket);

    Socket(Socket const &other);

    Socket(int domain, int type, int protocol);

	Socket &operator=(Socket const &other);

    void setsockopt(int optname, int value);
    void setsockopt(int optname, bool value);

	void setReuseAddr(bool reuse);

    void connect(SocketAddress &serverAddress);

    void bind(SocketAddress &myAddress);

    void listen(int backlog);

    Socket accept(SocketAddress &address);

	size_t recvfrom(char *buf, size_t len, int flags, SocketAddress &from);
	size_t sendto(const char *buf, size_t len, int flags,SocketAddress &to);

	void shutdown();
    void shutdownRead();
    void shutdownWrite();

  };

  inline Socket::Socket()
  {
  }

  inline Socket::Socket(int socket)
	: File(socket)
  {
  }

  inline Socket::Socket(Socket const &other)
	: File(other)
  {
	//	copy(other);
  }

  inline Socket &Socket::operator=(Socket const &other)
  {
	if (this != &other) {
	  destroy();
	  copy(other);
	}
	return *this;
  }

  inline void Socket::setsockopt(int optname, int value)
  {
	if (::setsockopt(d_fd,SOL_SOCKET,optname,&value,sizeof(int)) == -1)
	  throw IOError(errno,"setsockopt()");
  }

  inline void Socket::setsockopt(int optname, bool value)
  {
	setsockopt(optname,value);
  }

  inline void Socket::setReuseAddr(bool reuse)
  {
	setsockopt(SO_REUSEADDR,reuse);
  }

  inline void Socket::connect(SocketAddress &serverAddress)
  {
	if (::connect(d_fd,serverAddress,serverAddress.length()) == -1)
	  throw IOError(errno,"connecting socket");
  }

  inline void Socket::bind(SocketAddress &myAddress)
  {
	if (::bind(d_fd,myAddress,myAddress.length()) == -1)
	  throw IOError(errno,"binding socket");
  }

  inline void Socket::listen(int backlog)
  {
	if (::listen(d_fd,backlog) == -1)
	  throw IOError(errno,"listening to socket");
  }

  inline Socket Socket::accept(SocketAddress &address)
  {
	socklen_t len = address.length();
	int sock = ::accept(d_fd,address,&len);
	if (sock == -1)
	  throw IOError(errno,"accepting connection");	  
	return sock;
  }

  inline size_t Socket::recvfrom(char *buf, size_t len, int flags, SocketAddress &from)
  {
	assert(d_fd);
	size_t fromlen = from.length();
	int res = ::recvfrom(d_fd,buf,len,flags,from,&fromlen);
	if (res < 0 && errno != EAGAIN)
	  throw IOError(errno,"receiving from socket");	  
	return static_cast<size_t>(res);
  }

  inline size_t Socket::sendto(const char *buf, size_t len, int flags,
							   SocketAddress &to)
  {
	assert(d_fd);
	int res = ::sendto(d_fd,buf,len,flags,to,to.length());
	if (res < 0 && errno != EAGAIN)
	  throw IOError(errno,"sending to socket");	  
	return static_cast<size_t>(res);
  }

  inline void Socket::shutdown()
  {
	if (::shutdown(d_fd, SHUT_RD | SHUT_WR) == -1)
	  throw IOError(errno,"shutting down socket read and write");
  }

  inline void Socket::shutdownRead()
  {
	if (::shutdown(d_fd, SHUT_RD) == -1)
	  throw IOError(errno,"shutting down socket read side");
  }
    
  inline void Socket::shutdownWrite()
  {
	if (::shutdown(d_fd, SHUT_WR) == -1)
	  throw IOError(errno,"shutting down socket write side");
  }

};

#endif //  __INC_MVDS_SOCKET_HH__
