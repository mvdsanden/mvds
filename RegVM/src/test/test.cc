#include "../RegVM/regvm.hh"

#include <iostream>
#include <cstdlib>

using namespace std;
using namespace mvds;

int main()
{

  char text[1024];

  char *p = text;

  *(p++) = RegVM::opShift;

  *(p++) = RegVM::opTest;

  *(p++) = 'a';

  *(p++) = RegVM::opJmpNE;

  *(reinterpret_cast<uint32_t*>(p)) = 32; p += sizeof(uint32_t);

  *(p++) = RegVM::opShiftS;

  *(reinterpret_cast<uint32_t*>(p)) = 0; p += sizeof(uint32_t);

  *(p++) = RegVM::opJump;

  *(reinterpret_cast<uint32_t*>(p)) = 1; p += sizeof(uint32_t);

  p = text + 32;

  *(p++) = RegVM::opTest;

  *(p++) = 'b';

  *(p++) = RegVM::opJmpNE;

  *(reinterpret_cast<uint32_t*>(p)) = 64; p += sizeof(uint32_t);

  *(p++) = RegVM::opShiftS;

  *(reinterpret_cast<uint32_t*>(p)) = 1; p += sizeof(uint32_t);

  *(p++) = RegVM::opJump;

  *(reinterpret_cast<uint32_t*>(p)) = 1; p += sizeof(uint32_t);

  *(reinterpret_cast<uint32_t*>(p)) = 'b'; p += sizeof(uint32_t);

  p = text + 64;

  *(p++) = RegVM::opSWJump

  *(reinterpret_cast<uint32_t*>(p)) = 512; p += sizeof(uint32_t);

  //  *(p++

  p = text + 128;

  *(p++) = RegVM::opHalt;

  *(reinterpret_cast<uint32_t*>(p)) = 0; p += sizeof(uint32_t);

  vector<string> out;

  RegVM vm;

  RegVMProg prog(1024,text,2);

  int res = vm.execute(prog,"aaaabbbbababc",out);

  if (res < 0) {

    cerr << "Error: " << vm.error() << endl;

  } else {
    cerr << "Succes!" << endl;

    for (vector<string>::iterator i = out.begin();
	 i != out.end(); ++i)
      cerr << "- '" << *i << "'" << endl;
  }

  return 0;
}
