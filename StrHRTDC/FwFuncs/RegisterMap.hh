#ifndef REGISTER_HH
#define REGISTER_HH

#include"RegisterMapCommon.hh"

namespace LBUS{
  // ------------------------------------------------------------------------
  // StrHRTDC BASE
  // ------------------------------------------------------------------------
  
  namespace SHT_BASE{
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
	  kAddrMikuStatus   = 0x20000000, // W/R, [1:0] Internal MIKUMARI link up status
	  kAddrInitCbt      = 0x20100000, // W,         Assert CBT init signal
	  kAddrInitDDR      = 0x20200000, // W,         Assert DDR Receiver initialize
	  kAddrCtrlReg      = 0x20300000, // W/R, [5:0] DDR receiver controll register
	  kAddrRcvStatus    = 0x20400000, // R,   [3:0] DDR receiver status
	  kAddrStcpStatusU  = 0x20500000, // R,   [7:0] StcpFlag form MZN-U
	  kAddrStcpStatusD  = 0x20600000  // R,   [7:0] StcpFlag form MZN-D
	};

      enum Ctrl
	{
	  kRegTestModeU  =0x1,
	  kRegTestModeD  =0x2,
	  kRegEnableU    =0x4,
	  kRegEnableD    =0x8,
	  kRegFRstU      =0x10,
	  kRegFRstD      =0x20,
	};

      enum Status
	{
	  kRegBitAlignedU = 0x1,
	  kRegBitAlignedD = 0x2,
	  kRegBitErrorU   = 0x4,
	  kRegBitErrorD   = 0x8
	};
    };

    //-------------------------------------------------------------------------
    // BctBusBridge Primary
    //-------------------------------------------------------------------------
    namespace BBP{
      enum BaseAddress
	{
	  kUpper  = 0x30000000,
	  kLower  = 0x40000000
	};
  
      enum LocalAddress
	{
	  kAddrTxd    = 0x00000000, // W, [31:0] write data to 2ndry FPGA
	  kAddrRxd    = 0x00100000, // R, [31:0] read data from 2ndry FPGA
	  kAddrExec   = 0x01000000  // W, Excecute Bus Bridge
	};  
    };

  };

  
  // ------------------------------------------------------------------------
  // Mezzanine StrHR-TDC
  // ------------------------------------------------------------------------
  namespace SHT_MZN{
    // ------------------------------------------------------------------------
    // DCT
    // ------------------------------------------------------------------------
    namespace DCT{
      enum LocalAddress
	{
	  kAddrTestMode  = 0x0000, // W/R [0:0] enable ddr test mode
	  kAddrExtraPath = 0x0010, // W/R [0:0] enable clock caliblation
	  kAddrGate      = 0x0020, // W/R [0:0] DAQ gate
	  kAddrEnBlocks  = 0x0030  // W/R [1:0] 0: leading, 1: trailing
	};

      enum EnableBlocks
	{
	  kEnLeading  = 1,
	  kEnTrailing = 2
	};
    };

    // ------------------------------------------------------------------------
    // TDC
    // ------------------------------------------------------------------------
    namespace TDC{
      enum LocalAddress
	{
	  kAddrControll   = 0x1010,  // W/R [2:0] Controll bits
	  kAddrReqSwitch  = 0x1020,  // W,  Assert manual switch pulse
	  kAddrStatus     = 0x1030,  // R,  [0:0] Read status
	  kAddrTdcMask    = 0x1040   // W/R [31:0] Channel mask
	};

      enum ControllBits
	{
	  kRegThrough     = 0x1,
	  kRegAutosw      = 0x2,
	  kRegStopDout    = 0x4
	};

      enum StatusBits
	{
	  kRegReadyLut    = 0x1
	};
    };

    // ------------------------------------------------------------------------
    // SDS
    // ------------------------------------------------------------------------
    namespace SDS{
      enum LocalAddress
	{
	  kAddrSdsStatus      = 0xC000,  // R   [7:0]
     
	  kAddrXadcDrpMode    = 0xC010,  // W/R [0:0]
	  kAddrXadcDrpAddr    = 0xC020,  // W/R [6:0]
	  kAddrXadcDrpDin     = 0xC030,  // W/R [15:0]
	  kAddrXadcDrpDout    = 0xC040,  // R   [15:0]
	  kAddrXadcExecute    = 0xC050,  // W
     
	  kAddrSemCorCount    = 0xC0A0,  // R   [15:0]
	  kAddrSemRstCorCount = 0xC0B0,  // W
	  kAddrSemErrAddr     = 0xC0C0,  // W   [39:0]
	  kAddrSemErrStrobe   = 0xC0D0   // W
	};

    };

    // ------------------------------------------------------------------------
    // BCT
    // ------------------------------------------------------------------------
    namespace BCT{
      enum LocalAddress
	{
	  kAddrReset   = 0xE000, // W, assert reset signal from BCT
	  kAddrVersion = 0xE010,// R, [31:0]
	  kAddrReConfig= 0xE020 // W, Reconfig FPGA by SPI
	};
    };
  };
};
#endif
