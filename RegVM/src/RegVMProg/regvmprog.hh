#ifndef __INC_MVDS_REGVMPROG_HH__
#define __INC_MVDS_REGVMPROG_HH__

#include <cstdlib>
#include <cstring>

namespace mvds {

  /** Regular Virtual Machine
   *
   *  RegVMProg is a vm with only very few opcodes.
   *  It is intended to run regular expression byte code.
   */
  class RegVMProg {

    RegVMProg(RegVMProg const &); // NI
    RegVMProg &operator=(RegVMProg const &); // NI

  public:

    RegVMProg(size_t textSize, char const *text, size_t outputCount);

    ~RegVMProg();

    size_t textSize() const;

    char const *text() const;

    size_t outputCount() const;

  private:

    size_t d_textSize;

    char *d_text;

    size_t d_outputCount;

  };

  inline RegVMProg::RegVMProg(size_t textSize, char const *text, size_t outputCount)
    : d_textSize(textSize),
      d_outputCount(outputCount)
  {
    d_text = new char [ d_textSize ];
    memcpy(d_text,text,d_textSize);
  }

  inline RegVMProg::~RegVMProg()
  {
    delete [] d_text;
  }

  inline size_t RegVMProg::textSize() const
  {
    return d_textSize;
  }

  inline char const *RegVMProg::text() const
  {
    return d_text;
  }

  inline size_t RegVMProg::outputCount() const
  {
    return d_outputCount;
  }


};

#endif // __INC_MVDS_REGVMPROG_HH__
