#ifndef REGISTER_HH
#define REGISTER_HH

#include"RegisterMapCommon.hh"

namespace LBUS{
  // ------------------------------------------------------------------------
  // StrHRTDC BASE
  // ------------------------------------------------------------------------
  
  namespace SHT_BASE{
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
    // MikumariUtil 
    // ------------------------------------------------------------------------
    namespace MUTIL{
      enum LocalAddress
	{
	  kAddrCbtLaneUp       = 0x0000, // R,   [31:0] CBT lann-up status
	  kAddrCbtTapValueIn   = 0x0010, // R,   [4:0] Tap value selected by CBT init process
	  kAddrCbtTapValueOut  = 0x0020, // W,   [4:0] Tap value for fixed idelay value mode
	  kAddrCbtBitSlipIn    = 0x0030, // R,   [3:0] Bitslip number by CBT init process
	  kAddrCbtInit         = 0x0040, // W,   [31:0] CBT init request
			  		        
	  kAddrMikumariUp      = 0x0050, // R,   [31:0] Mikumari link-up status
     			  
	  kAddrLaccpUp         = 0x0060, // R,   [31:0] LACCP link-up status
	  kAddrPartnerIpAddr   = 0x0070, // R,   [31:0] IP address in other side
	  kAddrHbcOffset       = 0x0080, // R,   [15:0] Hbc offset
	  kAddrLocalFineOffset = 0x0090, // R,   [15:0] Local fine offset
	  kAddrLaccpFineOffset = 0x00A0, // R,   [15:0] LACCP fine offset
	  kAddrHbfState        = 0x00B0, // W/R, [0:0] Hb frame state

	  kAddrRegIndex        = 0x0100, // W/R, [5:0] W/R channel number
	  kAddrNumLinks        = 0x0200  // R,   [5:0] Number of Mikumari Links
     
	};
    };

    
    // ------------------------------------------------------------------------
    // DCT
    // ------------------------------------------------------------------------
    namespace DCT{
      enum LocalAddress
	{
	  kAddrTestMode  = 0x1000, // W/R [0:0] enable ddr test mode
	  kAddrExtraPath = 0x1010  // W/R [0:0] enable clock caliblation
	};
    };

    // ------------------------------------------------------------------------
    // TDC
    // ------------------------------------------------------------------------
    namespace TDC{
      enum LocalAddress
	{
	  kAddrControll     = 0x2010,  // W/R [2:0] Controll bits
	  kAddrReqSwitch    = 0x2020,  // W,  Assert manual switch pulse
	  kAddrStatus       = 0x2030,  // R,  [0:0] Read status
	  kAddrTdcMask      = 0x2040,   // W/R [31:0] Channel mask

	  kEnBypass         = 0x2050,  // W/R, [1:0]
	  kTotFilterCtrl    = 0x2060,  // W/R, [1:0]
	  kTotMinTh         = 0x2070,  // W/R, [15:0]
	  kTotMaxTh         = 0x2080,  // W/R, [15:0]
	  kTrgEmuCtrl       = 0x2090,  // W/R, [1:0]
	  kTrgEmuDelay      = 0x20A0,  // W/R, [7:0]
	  kTrgEmuWidth      = 0x20B0,  // W/R, [15:0]
	  kHbfThrottCtrl    = 0x20C0,  // W/R, [3:0]
	  kHbdUserReg       = 0x20D0,  // W,   [15:0]
	  kSelfRecoveryMode = 0x20E0,  // W/R, [0:0]	  

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
    
    // ------------------------------------------------------------------------
    // Scaler
    // ------------------------------------------------------------------------
    // Module ID: 0x8
    namespace SCR{
      enum LocalAddress
	{
	  kAddrScrReset  = 0x8000, // W,  [0:0] Assert counter soft reset
	  kAddrLatchCnt  = 0x8010, // R,  [0:0] Read busy status, if busy is low, assert latch signal for counters
	  kAddrNumCh     = 0x8020, // R,  [7:0] Read number of scaler channel
	  kAddrReadFIFO  = 0x8100 // R,  FIFO read sequence
     
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
