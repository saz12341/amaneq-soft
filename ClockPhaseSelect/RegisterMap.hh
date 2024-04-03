#ifndef REGISTER_HH
#define REGISTER_HH

#include"RegisterMapCommon.hh"

namespace LBUS{
  
  //-------------------------------------------------------------------------
  // CPS (Clock Phase Selection)
  //-------------------------------------------------------------------------
  namespace CPS{
    enum LocalAddress
    {
      kAddrPhaseStatus    = 0x50000000, // W,   [7:0]
      kAddrPhaseOperate   = 0x50100000, // W,   [7:0]
      kAddrShift          = 0x50200000, // R,   [32:0]
      kAddrEepromCenter   = 0x51100000, // W/R, [32:0]
      kAddrEepromLength   = 0x51200000  // W/R, [32:0]
    };
    enum Status
    {
      kRegIsReady = 0x1,
      kRegTimeout = 0x2
    };
  };
};


#endif
