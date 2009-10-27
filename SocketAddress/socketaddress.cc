/*
 *  Socket Class
 *
 *  Author: Mart van de Sanden
 *
 *  License: GPL
 */

#include "socketaddress.ih"

#include <netdb.h>
#include <arpa/inet.h>

#include <stdexcept>
#include <iostream>

void SocketAddress::NSLookup(std::string const &hostname)
{
  struct hostent *h = gethostbyname(hostname.c_str());

  if (!h)
	throw std::runtime_error(hstrerror(h_errno));

  memcpy(&(d_sockAddress.i.sin_addr),*(h->h_addr_list),sizeof(struct in_addr));
}

void SocketAddress::copy(SocketAddress const &other)
{
  memcpy(&d_sockAddress,&(other.d_sockAddress),sizeof(d_sockAddress));
  d_length = other.d_length;
}

void SocketAddress::destroy()
{
}

SocketAddress::SocketAddress()
{
  // Default to internet sockets.
  d_sockAddress.i.sin_family = AF_INET;
  d_sockAddress.i.sin_port = 0;

  d_length = sizeof(d_sockAddress.i);
}

SocketAddress::SocketAddress(SocketAddress const &other)
{
  copy(other);
}

SocketAddress::SocketAddress(int family, int port, in_addr_t addr)
{
  d_sockAddress.i.sin_family = family;
  d_sockAddress.i.sin_port = htons(port);
  d_sockAddress.i.sin_addr.s_addr = addr;
  d_length = sizeof(d_sockAddress.i);
}

SocketAddress::SocketAddress(int port, in_addr_t addr)
{
  d_sockAddress.i.sin_family = AF_INET;
  d_sockAddress.i.sin_port = htons(port);
  d_sockAddress.i.sin_addr.s_addr = addr;
  d_length = sizeof(d_sockAddress.i);
}

SocketAddress::SocketAddress(int family, int port, std::string const &hostname)
{
  d_sockAddress.i.sin_family = family;
  d_sockAddress.i.sin_port = htons(port);
  d_length = sizeof(d_sockAddress.i);

  NSLookup(hostname);
}

SocketAddress::SocketAddress(int port, std::string const &hostname)
{
  d_sockAddress.i.sin_family = AF_INET;
  d_sockAddress.i.sin_port = htons(port);
  d_length = sizeof(d_sockAddress.i);

  NSLookup(hostname);
}

SocketAddress::SocketAddress(std::string const &path)
{
  if (path.length() > 100)
	throw std::runtime_error("SocketAddress: unix socket path to long");

  d_sockAddress.u.sun_family = AF_UNIX;
  //	d_sockAddress.u.sun_path   = new char [ path.length() + 1 ];

  strncpy(d_sockAddress.u.sun_path, path.c_str(), path.length());
  d_sockAddress.u.sun_path[path.length()] = 0;

  d_length = sizeof(d_sockAddress.u);
}

SocketAddress &SocketAddress::operator=(SocketAddress const &other)
{
  if (this != &other) {
	destroy();
	copy(other);
  }
  return *this;
}

SocketAddress::~SocketAddress()
{
  destroy();
}

std::ostream &mvds::operator<<(std::ostream &os, const SocketAddress &sa)
{

  os
	<< inet_ntoa(sa.d_sockAddress.i.sin_addr);
  os
	<< ":" << ntohs(sa.d_sockAddress.i.sin_port);
 
  return os;
   
}

