#include "server.ih"

#include "../Logger/logger.hh"

Server::EventDispatch::EventDispatch(Server &server, rf<SocketInfo> info)
  : d_server(server), d_info(info)
{
}

void Server::EventDispatch::operator()(File &fd, unsigned flags)
{

  Logger::instance().log() << "Server: Event (" << fd.fd() << ", " << flags << ")." << endl;

  if (flags & Select::Delete) {
	// signal socket deletion.
	d_server.deleteConnection(d_info);

   	map<Socket,rf<SocketInfo> >::iterator i = d_server.d_connections.find(d_info->socket);

	// Erase it from the list.
	if (i != d_server.d_connections.end())
	  d_server.d_connections.erase(i);

	return;
  }

  switch (d_info->state) {

  case cs_read:
	if (flags & Select::Read)
	  if (d_server.readyToRead(d_info)) {
		d_server.d_select.change(d_info->socket,Select::Write | Select::DispatchWrite);
		d_info->state = cs_write;
	  }
	break;

  case cs_write:
	if (flags & Select::Write)
	  if (d_server.readyToWrite(d_info)) {
		d_server.d_select.change(d_info->socket,Select::Read | Select::DispatchRead);
		d_info->state = cs_read;
	  }
	break;

  };

}

