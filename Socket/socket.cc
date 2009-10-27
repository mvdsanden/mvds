/*
 *  Socket Class
 *
 *  Author: Mart van de Sanden
 *
 *  License: GPL
 */

#include "socket.ih"

Socket::Socket(int domain, int type, int protocol)
  : File(::socket(domain,type,protocol))
{
  if (d_fd == -1)
	throw IOError(errno,"creating socket");
}
