#include "regvm.ih"


char *RegVM::s_ops[opHighest] =
  {
    "opNone",
    "opTest",
    "opAdd",
    "opSub",
    "opMul",
    "opDiv",
    "opLShft",
    "opRShft",
    "opJump",
    "opJmpEQ",
    "opJmpNE",
    "opJmpST",
    "opJmpLT",
    "opJmpSE",
    "opJmpLE",
    "opSWJmp",
    "opShift",
    "opShiftS",
    "opHalt"
  };
