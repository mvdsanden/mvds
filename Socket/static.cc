#include "socket.ih"

pair<Socket,Socket> Socket::pair()
{
  int sv[2];

  if (socketpair(PF_UNIX,SOCK_DGRAM,0,sv) == -1)
	throw IOError(errno,"creating socket pair");

	//    std::cerr << "Socket pair created: " << sv[0] << "," << sv[1] << std::endl;

  return make_pair(Socket(sv[0]),Socket(sv[1]));
}
