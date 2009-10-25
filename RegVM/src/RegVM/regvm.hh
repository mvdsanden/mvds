#ifndef __INC_MVDS_REGVM_HH__
#define __INC_MVDS_REGVM_HH__

#include "../RegVMProg/regvmprog.hh"

#include <string>
#include <vector>

namespace mvds {

  /** Regular Virtual Machine
   *
   *  RegVM is a vm with only very few opcodes and one direct usable register.
   *  It is intended to run regular expression byte code.
   *
   *  It only has a few opcodes:
   *  - test x     --  Compare %reg to x, the result is saved in the flag register.
   *  - add x      --  Add immediate value x to %reg.
   *  - sub x      --  Substract immediate value x from %reg.
   *  - mul x      --  Multiply %reg with immediate value x.
   *  - div x      --  Divide %reg with immediate value x.
   *  - lshft x    --  Left bit shift, shift %reg x bits to the left.
   *  - rshft x    --  Left bit shift, shift %reg x bits to the right.
   *  - jmpeq t    --  When the equality flag is set, jump to immediate value t.
   *  - jmpne t    --  When the equality flag is not set, jump to immediate value t.
   *  - jmpst t    --  When the smaller than flag is set, jump to immediate value t.
   *  - jmplt t    --  When the larger than flag is set, jump to immediate value t.
   *  - jmpse t    --  When the equality or smaller than flag is set, jump to immediate value t.
   *  - jmple t    --  When the equality or larger than flag is set, jump to immediate value t.
   *  - swjmp x    --  Switch Jump, lookup %reg in immediate data at p and jump to it if match.
   *  - shift      --  Shift to the next input character.
   *  - shift x    --  Save %reg to the end of output x and then shift to the next input character.
   *  - halt x     --  Stop with exit code x, we are done.
   */
  class RegVM {
  public:

    enum OpCode {

      opNone   = 0,
      opTest   = 1,
      opAdd    = 2,
      opSub    = 3,
      opMul    = 4,
      opDiv    = 5,
      opLShft  = 6,
      opRShft  = 7,
      opJump   = 8,
      opJmpEQ  = 9,
      opJmpNE  = 10,
      opJmpST  = 11,
      opJmpLT  = 12,
      opJmpSE  = 13,
      opJmpLE  = 14,
      opSWJmp  = 15,
      opShift  = 16,
      opShiftS = 17,
      opHalt   = 18,
      opHighest

    };

    enum ErrCode {

      errNone,
      errOutOfText,
      errOutputBounds,
      errPrematureEnd,

    };

    int execute(RegVMProg const &program, std::string const &input, std::vector<std::string> &output);

    ErrCode error() const;

  private:

    ErrCode d_error;

    static char *s_ops[opHighest];

  };

  inline RegVM::ErrCode RegVM::error() const
  {
    return d_error;
  }

};

#endif // __INC_MVDS_REGVM_HH__
