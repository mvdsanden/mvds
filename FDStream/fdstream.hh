#ifndef __INC_MVDS_FDSTREAM_HH__
#define __INC_MVDS_FDSTREAM_HH__

#include <ext/stdio_filebuf.h>
#include <istream>

namespace mvds {

  class IFDStream : public std::istream {

  public:

	IFDStream(int fd);

	IFDStream();

	void open(int fd);

	~IFDStream();

  };


  class OFDStream : public std::ostream {

  public:

	OFDStream(int fd);

	OFDStream();

	void open(int fd);

	~OFDStream();

  };

  class IOFDStream : public std::iostream {

  public:

	IOFDStream(int fd);

	IOFDStream();

	void open(int fd);

	~IOFDStream();

  };


  inline IFDStream::IFDStream(int fd)
	: std::istream(new __gnu_cxx::stdio_filebuf<char>(fd,std::ios::in))
  {
	
  }

  inline IFDStream::IFDStream()
  {
	
  }

  inline void IFDStream::open(int fd)
  {
	std::streambuf *fb = rdbuf(new __gnu_cxx::stdio_filebuf<char>(fd,std::ios::in));

	if (fb)
	  delete fb;
  }

  inline IFDStream::~IFDStream()
  {
  }

  inline OFDStream::OFDStream(int fd)
	: std::ostream(new __gnu_cxx::stdio_filebuf<char>(fd,std::ios::out))
  {
  }

  inline OFDStream::OFDStream()
  {
  }

  inline void OFDStream::open(int fd)
  {
	std::streambuf *fb = rdbuf(new __gnu_cxx::stdio_filebuf<char>(fd,std::ios::out));

	if (fb)
	  delete fb;
  }

  inline OFDStream::~OFDStream()
  {
  }

  inline IOFDStream::IOFDStream(int fd)
	: std::iostream(new __gnu_cxx::stdio_filebuf<char>(fd,std::ios::in | std::ios::out))
  {
  }

  inline IOFDStream::IOFDStream()
  {
	
  }

  inline void IOFDStream::open(int fd)
  {
	std::streambuf *fb = rdbuf(new __gnu_cxx::stdio_filebuf<char>(fd,std::ios::in | std::ios::out));

	if (fb)
	  delete fb;
  }

  inline IOFDStream::~IOFDStream()
  {
  }


};

#endif // __INC_MVDS_FDSTREAM_HH__
