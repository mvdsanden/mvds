#ifndef __INC_MVDS_FILE_HH__
#define __INC_MVDS_FILE_HH__

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cerrno>
#include <cassert>

#include "../IOError/ioerror.hh"
#include "../Timer/timer.hh"

namespace mvds {

  class File {
  public:

	static File stdin;
	static File stdout;
	static File stderr;

	enum Flags {

	  Nothing = 0x00,
	  Read    = O_RDONLY,
	  Write   = O_WRONLY,
	  RW      = O_RDWR,

	};

    File();

    File(int fd);

	File(std::string const &pathname, int flags);

	~File();

	File(File const &other);

	//    operator bool() const;
	//	operator int const();

	File &operator=(File const &other);

    bool operator==(File const &other) const;
    bool operator!=(File const &other) const;
    bool operator>(File const &other) const;
    bool operator<(File const &other) const;
    bool operator>=(File const &other) const;
    bool operator<=(File const &other) const;

	void close();

	bool ok() const;

    size_t read(char *buf, size_t size);

    size_t readExactly(char *buf, size_t size);

	size_t readExactly(char *buf, size_t size, TimeSpec const &timeout);

    size_t write(char const *buf, size_t size);

    size_t write(std::string const &str);

    size_t writeExactly(char const *buf, size_t size);

    int fcntl(int cmd);
    int fcntl(int cmd, long arg);
    int fcntl(int cmd, struct flock *lock);

    int setBlocking(bool blocking);
    bool blocking();

	int fd() const;

	void grab();

	bool release();

  protected:

    void copy(File const &other);
    void destroy();

	virtual bool preClose();
	virtual void postClose();

	int d_fd;

    unsigned *d_refcnt;

  };

  inline void File::copy(File const &other)
  {
	d_fd     = other.d_fd;
	d_refcnt = other.d_refcnt;
	++*d_refcnt;
  }

  inline void File::destroy() {
	if (!(--(*d_refcnt))) {
	  if (d_fd != -1)
		{
		  if (preClose()) {
			::close(d_fd);
		  }
		  postClose();

		  d_fd = -1;
		}
	  delete d_refcnt;
	  d_refcnt = 0;
	}
  }

  inline File::File()
	: d_fd(-1),
	  d_refcnt(new unsigned(1))
  {
  }

  inline File::File(int fd)
	: d_fd(fd),
	  d_refcnt(new unsigned(1))
  {
  }

  inline File::File(File const &other)
  {
	copy(other);
  }

  inline File::File(std::string const &pathname, int flags)
	: d_refcnt(new unsigned(1))
  {
	d_fd = open(pathname.c_str(),flags);

	if (d_fd < 0)
	  throw IOError(errno,pathname+": opening this file");
  }

  inline File::~File()
  {
	destroy();
  }

  /*
  inline File::operator int const()
  {
	return d_fd;
  }
  */

  inline File &File::operator=(File const &other)
  {
	if (this != &other) {
	  destroy();
	  copy(other);
	}
	return *this;
  }

  inline bool File::operator==(File const &other) const
  { return (d_fd == other.d_fd); }

  inline bool File::operator!=(File const &other) const
  { return (d_fd != other.d_fd); }

  inline bool File::operator>(File const &other) const
  { return (d_fd > other.d_fd); }

  inline bool File::operator<(File const &other) const
  { return (d_fd < other.d_fd); }

  inline bool File::operator>=(File const &other) const
  { return (d_fd >= other.d_fd); }

  inline bool File::operator<=(File const &other) const
  { return (d_fd <= other.d_fd); }

  /*
  inline File::operator bool() const
  {
	return (d_fd != -1);
  }
  */

  inline void File::close()
  {
	if (preClose()) {
	  ::close(d_fd);
	}
	postClose();
	d_fd = -1;
  }

  inline bool File::ok() const
  {
	return d_fd >= 0;
  }

  inline size_t File::read(char *buf, size_t size)
  {
	int res = ::read(d_fd,buf,size);

	if (res < 0){
	  if (errno != EAGAIN)
		throw IOError(errno,"reading from file");
	  else
		res = 0;
	}
	return static_cast<size_t>(res);
  }

  inline size_t File::readExactly(char *buf, size_t size)
  {
	size_t total;
	for (total = 0;
		 total < size;
		 total += read(buf + total,size-total));
	return total;
  }

  inline size_t File::readExactly(char *buf, size_t size, TimeSpec const &timeout)
  {
	Timer timer;
	timer.tick();

	size_t total = 0;
	while (true) {
	  total += read(buf + total,size-total);

	  if (total < size) {
		if (timer.tock() > timeout)
		  throw std::runtime_error("File::readExactly timeout");
		usleep(100);
	  } else
		break;
	}
	return total;
  }


  inline size_t File::write(char const *buf, size_t size)
  {
	assert(d_fd >= 0);
	int res = ::write(d_fd,buf,size);
	if (res < 0) {
	  if (errno != EAGAIN)
		throw IOError(errno,"writing to file");
	  else
		res = 0;
	}
	return static_cast<size_t>(res);
  }

  inline size_t File::write(std::string const &str)
  {
	return write(str.c_str(),str.length());
  }

  inline size_t File::writeExactly(char const *buf, size_t size)
  {
	size_t total;
	for (total = 0;
		 total < size;
		 total += write(buf + total,size-total));
	return total;
  }

  inline int File::fcntl(int cmd)
  {
	return ::fcntl(d_fd,cmd);
  }

  inline int File::fcntl(int cmd, long arg)
  {
	return ::fcntl(d_fd,cmd,arg);
  }

  inline int File::fcntl(int cmd, struct flock *lock)
  {
	return ::fcntl(d_fd,cmd,lock);
  }

  inline int File::setBlocking(bool blocking)
  {
	int flags = fcntl(F_GETFL);

	if (blocking && !(flags & O_NONBLOCK))
	  return fcntl(F_SETFL,flags & ~O_NONBLOCK);
	else
	  return fcntl(F_SETFL,flags | O_NONBLOCK);
  }

  inline bool File::blocking()
  {
	int flags = fcntl(F_GETFL);
	return !(flags & O_NONBLOCK);
  }

  inline bool File::preClose()
  {
	return true;
  }

  inline void File::postClose()
  {
  }

  inline int File::fd() const
  {
	return d_fd;
  }

  inline void File::grab()
  {
	++(*d_refcnt);
  }

  inline bool File::release()
  {
	return !--(*d_refcnt);
  }


};


#endif // __INC_MVDS_FILE_HH__
