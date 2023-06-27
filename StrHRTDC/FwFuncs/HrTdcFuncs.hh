#ifndef DAQFUNCS_HH
#define DAQFUNCS_HH

#include<string>
#include<stdint.h>

namespace HUL{
  class FPGAModule;
};

namespace HUL::DAQ{
  void    DdrInitialize(HUL::FPGAModule& fmodule, bool enslot_up, bool enslot_low);
  void    CalibLUT(HUL::FPGAModule& fmodule, uint32_t base_addr);
};

#endif
