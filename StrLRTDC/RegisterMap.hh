#ifndef REGISTER_HH
#define REGISTER_HH

#include"RegisterMapCommon.hh"

namespace LBUS{
  
  namespace SLT{
    //-------------------------------------------------------------------------
    // StrLRTDC
    //-------------------------------------------------------------------------
    namespace TDC{
      enum LocalAddress
	{
	  kTdcMaskMainU     = 0x1000'0000,  // W/R, [31:0] Channel mask for 0-31 ch
	  kTdcMaskMainD     = 0x1010'0000,  // W/R, [31:0] Channel mask for 32-63 ch
	  kTdcMaskMznU      = 0x1020'0000,  // W/R, [31:0] Channel mask for 64-95 ch
	  kTdcMaskMznD      = 0x1030'0000,  // W/R, [31:0] Channel mask for 96-127 ch
	  		    
	  kEnBypass         = 0x1040'0000,  // W/R, [1:0]
	  		    
	  kTotFilterCtrl    = 0x1050'0000,  // W/R, [1:0]
	  kTotMinTh         = 0x1060'0000,  // W/R, [15:0]
	  kTotMaxTh         = 0x1070'0000,  // W/R, [15:0]
	  		    
	  kTrgEmuCtrl       = 0x1080'0000,  // W/R, [1:0]
	  kTrgEmuDelay      = 0x1090'0000,  // W/R, [7:0]
	  kTrgEmuWidth      = 0x10A0'0000,  // W/R, [15:0]
			    
	  kHbfThrottCtrl    = 0x10B0'0000,  // W/R, [3:0]
			    
	  kHbdUserReg       = 0x10C0'0000,  // W,   [15:0]

	  kSelfRecoveryMode = 0x10D0'0000,  // W/R, [0:0]	  
	};

      enum Bypass
	{
	  kIndexTdcDelay = 0x1,
	  kIndexParing   = 0x2
	};

      enum TotFilter
	{
	  kIndexEnFiler         = 0x1,
	  kIndexTotZeroThrough  = 0x2
	};

      enum TrgEmuMode
	{
	  kTriggerMode     = 0x1,
	  kVetoMode        = 0x2
	};

      enum HbfThrottling
	{
	  kIndex1in2  = 0x1,
	  kIndex1in4  = 0x2,
	  kIndex1in8  = 0x4,
	  kIndex1in16 = 0x8
	};
    };

  };
};


#endif
