#ifndef __INC_MVDS_SERVER_HH_
#define __INC_MVDS_SERVER_HH_

#include "../Socket/socket.hh"
#include "../TimeVal/timeval.hh"
#include "../Select/select.hh"

#include "../Logger/logger.hh"

#include <list>

namespace mvds {

  /** TCP Network Server
   *
   *  With automated network event handling.
   */
  class Server {

  public:
	
    /**
     *  Implementations of Server should inherit this.
     */
    struct SocketData {};

    /**
     *  The connection states.
     */
    enum ConnectionState {
      cs_accepting,
      cs_read,
      cs_write
    };

    struct SocketInfo : public RefAble {

      Socket socket;
      SocketAddress address;
      SocketData *data;

      ConnectionState state;

      TimeVal lastTime;

	  bool operator<(SocketInfo const &other)
	  { return socket < other.socket; }

	  bool operator==(SocketInfo const &other)
	  { return socket == other.socket; }

	  ~SocketInfo()
	  { if (data) delete data; }

    };

  private:

	class AcceptDispatch : public Select::DispatchFunction {
	public:

	  AcceptDispatch(Server &server);

	  virtual void operator()(File &fd, unsigned flags);

	private:

	  Server &d_server;

	};

	class EventDispatch : public Select::DispatchFunction {
	public:

	  EventDispatch(Server &server, rf<SocketInfo> info);

	  virtual void operator()(File &fd, unsigned flags);

	private:

	  Server        &d_server;
	  rf<SocketInfo> d_info;

	};


    Server(Server const &); // NI
    Server &operator=(Server const &); //NI

  public:


    Server(unsigned port);

    virtual ~Server();

    void listen();

    void stop();

    void finish();

	bool running() const;

	void appendConnection(rf<SocketInfo> si, unsigned flags);

	/* Virtual functions */
	virtual bool acceptConnection(rf<SocketInfo> info) = 0;
    virtual bool readyToRead     (rf<SocketInfo> info) = 0;
    virtual bool readyToWrite    (rf<SocketInfo> info) = 0;
    virtual bool readyToReadOob  (rf<SocketInfo> info) = 0;
    virtual void deleteConnection(rf<SocketInfo> info) = 0;
	virtual void timeout() = 0;
	virtual void tick(TimeVal dt) = 0;
	virtual void stopped() = 0;

  protected:

    Socket d_socket;

	void close(Socket &socket);
	
	void setTimeout(TimeVal const &tv);

	void setTickInterval(TimeVal const &tv);

    bool d_running;

	std::map<Socket,rf<SocketInfo> > d_connections;

	Select d_select;

  private:

    void checkSelect();

    static int const s_defaultBacklog;
    static int const s_defaultTimeout;

	TimeVal d_timeout;
	TimeVal d_tickInterval;
	TimeVal d_lastTick;

	bool d_doTick;

    SocketAddress d_socketAddress;

  };

  inline Server::Server(unsigned port)
    : 
	  d_socket(PF_INET,SOCK_STREAM,0),
	  d_timeout(s_defaultTimeout,0),
	  d_tickInterval(0,0),
	  d_lastTick(TimeVal::now()),
	  d_doTick(false),
	  d_socketAddress(port)
  {

	Logger::instance().log() << "Server socket: " << d_socket.fd() << std::endl;

    // Bind the socket to a port.
    d_socket.bind(d_socketAddress);

	// Add the socket to the select pool.
	d_select.dispatch(d_socket,Select::Read,new AcceptDispatch(*this));

  }

  inline void Server::finish()
  {
	for (Select::iterator i = d_select.begin(); i != d_select.end(); ++i) {
	  d_select.remove((*i).first);
    }
	
	TimeVal t(0,0);
	d_select.select(t);
  }

  inline Server::~Server()
  {
    d_socket.close();
  }

  inline void Server::listen()
  {
    d_socket.listen(s_defaultBacklog);

    d_running = true;

    while (d_running)
      checkSelect();

    stopped();
  }

  inline void Server::stop()
  {
    d_running = false;
  }

  inline void Server::close(Socket &socket)
  {
	d_select.remove(socket);
  }

  inline bool Server::running() const
  {
	return d_running;
  }

  inline void Server::appendConnection(rf<SocketInfo> si, unsigned flags)
  {
	// Insert connection into the connections map.
	d_connections.insert(std::make_pair(si->socket,si));

	// Set an handler for connection events.
	d_select.dispatch(si->socket,Select::Read | Select::Write | Select::Delete,
					  new EventDispatch(*this,si));

	d_select.change(si->socket,flags | flags << 4);
  }

  inline void Server::setTimeout(TimeVal const &tv)
  {
	d_timeout = tv;
  }

  inline void Server::setTickInterval(TimeVal const &tv)
  {
	d_tickInterval = tv;

	if (tv > TimeVal(0,0)) {
	  d_doTick = true;
	  d_lastTick = TimeVal::now();
	}
  }

};

#endif // __INC_MVDS_SERVER_HH_
