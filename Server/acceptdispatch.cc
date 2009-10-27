#include "server.ih"

Server::AcceptDispatch::AcceptDispatch(Server &server)
  : d_server(server)
{
}

void Server::AcceptDispatch::operator()(File &fd, unsigned flags)
{
  rf<SocketInfo> si = new SocketInfo;
  si->socket        = d_server.d_socket.accept(si->address);;
  si->state         = cs_accepting;

  Logger::instance().log() << "Accept socket: " << si->socket.fd() << endl;

  Select::iterator i = d_server.d_select.find(si->socket);

  // First remove any lingerings of the old descriptor
  if (i != d_server.d_select.end()) {
	d_server.deleteConnection(si);
	d_server.d_select.removeHard(si->socket);
  }

  if (d_server.acceptConnection(si)) {
	// Default state after accepting the connection.
	if (si->state == cs_accepting)
	  si->state = cs_read;

	// Insert connection into the connections map.
	d_server.d_connections.insert(make_pair(si->socket,si));

	// Set an handler for connection events.
	d_server.d_select.dispatch(si->socket,Select::Read | Select::Write,
							   new EventDispatch(d_server,si));

	d_server.d_select.change(si->socket,(si->state == cs_read?(Select::Read | Select::DispatchRead):(Select::Write | Select::DispatchWrite)));

  }
}

