#ifndef __INC_MVDS_IOERROR_HH__
#define __INC_MVDS_IOERROR_HH__

#include <stdexcept>
#include <cerrno>
#include <cstring>
#include <string>

namespace mvds {

  class IOError : public std::runtime_error {

  public:

	IOError(int no, std::string const &what);

	int no() const;

  private:

	int d_errno;

  };

  inline IOError::IOError(int no, std::string const &what)
	: std::runtime_error(std::string("'")+strerror(no)+"' while "+what),
	  d_errno(errno)
  {
  }

  inline int IOError::no() const
  {
	return d_errno;
  }

};

#endif // __INC_MVDS_IOERROR_HH__
