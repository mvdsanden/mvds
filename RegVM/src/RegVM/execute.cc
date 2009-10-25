#include "regvm.ih"

#include <cassert>
#include <iostream>

int RegVM::execute(RegVMProg const &program, std::string const &input, vector<string> &output)
{
  // Initialize text area.
  char const *text = program.text();
  char const *end  = text + program.textSize();

  // Initialize output vector.
  output.resize(program.outputCount());

  // Registers
  //  char const *pc = text;          // The program counter.

  union {

    char const *i8;
    uint32_t const *u32;
    int32_t const *i32;

  } pc;

  pc.i8 = text;

  char const *ic = input.c_str(); // Input counter.
  ssize_t flags  = 0;             // The flags register.
  char reg       = 0;             // The register.

  size_t outputIndex = 0;

  while (true) {

    // Check text bounds.
    if (pc.i8 < text || pc.i8 >= end) {
      d_error = errOutOfText;
      return -1;
    }

    // Read the next operation.
    char in = *(pc.i8++);

    // Do an assert.
    assert(in < opHighest);

    //    cerr << dec << (pc.i8 - text - 1) << dec << ":" << s_ops[static_cast<size_t>(in)] << "(" << static_cast<int>(in) << ")" << endl;

    switch (in) {

    case opNone:
      break;

    case opTest:
      flags = reg - *(pc.i8++);
      break;

    case opAdd: reg += *(pc.i8++); break;
    case opSub: reg -= *(pc.i8++); break;
    case opMul: reg *= *(pc.i8++); break;
    case opDiv: reg /= *(pc.i8++); break;

    case opLShft: reg <<= *(pc.i8++); break;
    case opRShft: reg >>= *(pc.i8++); break;

    case opJump:
      pc.i8 = text + *(pc.u32);
      break;

    case opJmpEQ:
      if (!flags)
	pc.i8 = text + *(pc.u32);
      else
	pc.u32++;
      break;

    case opJmpNE:
      if (flags)
	pc.i8 = text + *(pc.u32);
      else
	pc.u32++;
      break;

    case opJmpST:
      if (flags < 0)
	pc.i8 = text + *(pc.u32);
      else
	pc.u32++;
      break;

    case opJmpLT:
      if (flags > 0)
	pc.i8 = text + *(pc.u32);
      else
	pc.u32++;
      break;

    case opJmpSE:
      if (flags <= 0)
	pc.i8 = text +*(pc.u32);
      else
	pc.u32++;
      break;

    case opJmpLE:
      if (flags >= 0)
	pc.i8 = text + *(pc.u32);
      else
	pc.u32++;
      break;

    case opSWJmp:
      pc.i8 = text + *(text + *(pc.u32) + (reg*4));
      break;

    case opShiftS:
      outputIndex = *(pc.u32++);

      if (outputIndex >= output.size()) {
	d_error = errOutputBounds;
	return -1;
      }

      output[outputIndex] += reg;
      // Fall through!

    case opShift:
      reg = *(ic++);

      if (!reg) {
	d_error = errPrematureEnd;
	return -1;
      }
      break;

    case opHalt:
      return *(pc.i32++);

    };

  };

}

