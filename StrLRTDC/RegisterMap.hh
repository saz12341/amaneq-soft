#ifndef REGISTER_HH
#define REGISTER_HH

#include"RegisterMapCommon.hh"

namespace LBUS{
  
  namespace SLT{
    //-------------------------------------------------------------------------
    // LSTC Module
    //-------------------------------------------------------------------------
    namespace LSTC{
      enum LocalAddress
	{
	  kAddrRunStatus  = 0x00000000, // W/R, [0:0] Stcp RUN status
	  kAddrAutoUpdate = 0x00100000, // W/R, [0:0] Gate number autoupdate
	  kAddrHbcReset   = 0x00200000, // W,         Asert Hb counter reset
	  kAddrGateNumber = 0x00300000  // W/R, [7:0] Stcp gate number
	};
    };
  
    //-------------------------------------------------------------------------
    // DCT Module
    //-------------------------------------------------------------------------
    namespace DCT{
      enum LocalAddress
	{
	  kAddrMikuStatus   = 0x20000000, // W/R, [0:0] Internal MIKUMARI link up status
	  kAddrStcpStatus   = 0x20100000  // R,   [7:0] StcpFlag status
	};

    };

    //-------------------------------------------------------------------------
    // StrLRTDC
    //-------------------------------------------------------------------------
    namespace TDC{
      enum LocalAddress
	{
	  kTdcMaskMainU = 0x30000000, // W/R, [31:0] Channel mask for 0-31 ch
	  kTdcMaskMainD = 0x30100000, // W/R, [31:0] Channel mask for 32-63 ch
	  kTdcMaskMznU  = 0x30200000, // W/R, [31:0] Channel mask for 64-95 ch
	  kTdcMaskMznD  = 0x30300000  // W/R, [31:0] Channel mask for 96-127 ch
	};
    };

  };
};


#endif
