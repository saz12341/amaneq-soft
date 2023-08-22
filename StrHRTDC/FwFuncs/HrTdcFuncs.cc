#include<fstream>
#include<cstdlib>
#include<iostream>
#include<cstdio>
#include<csignal>
#include<list>
#include<unistd.h>

#include"HrTdcFuncs.hh"
#include"BctBusBridgeFunc.hh"
#include"RegisterMap.hh"
#include"UDPRBCP.hh"
#include"FPGAModule.hh"

namespace HUL::DAQ{

  // DdrInitialize --------------------------------------------------------
  void
  DdrInitialize(HUL::FPGAModule& fmodule, bool enslot_up, bool enslot_low)
  {
    using namespace LBUS;
    using namespace LBUS::SHT_BASE;
  
    uint32_t enable_up   = enslot_up  ? DCT::kRegEnableU : 0;
    uint32_t enable_down = enslot_low ? DCT::kRegEnableD : 0;

    std::cout << "#D : Do DDR initialize" << std::endl;

    // MZN
    if(enslot_up){
      WriteModuleIn2ndryFPGA(fmodule, BBP::kUpper,
			     SHT_MZN::DCT::kAddrTestMode, 1, 1 );
    }

    if(enslot_low){
      WriteModuleIn2ndryFPGA(fmodule, BBP::kLower,
			     SHT_MZN::DCT::kAddrTestMode, 1, 1 );
    }
  
    uint32_t reg =
      enable_up   |
      enable_down |
      DCT::kRegTestModeU |
      DCT::kRegTestModeD;

    // Base
    fmodule.WriteModule(DCT::kAddrCtrlReg, reg);
    fmodule.WriteModule(DCT::kAddrInitDDR, 0);

    sleep(1);
    uint32_t ret = fmodule.ReadModule(DCT::kAddrRcvStatus, 1);

    if(enslot_up){
      if( ret & DCT::kRegBitAlignedU){
	std::cout << "#D : DDR initialize succeeded (MZN-U)" << std::endl;
      }else{
	std::cout << "#E : Failed (MZN-U)" << std::endl;
	exit(-1);
      }
    }// bit aligned ?

    if(enslot_low){
      if( ret & DCT::kRegBitAlignedD){
	std::cout << "#D : DDR initialize succeeded (MZN-D)" << std::endl;
      }else{
	std::cout << "#E : Failed (MZN-D)" << std::endl;
	exit(-1);
      }
    }// bit aligned ?

    // Set DAQ mode

    if(enslot_up){
      WriteModuleIn2ndryFPGA(fmodule, BBP::kUpper,
			     SHT_MZN::DCT::kAddrTestMode, 0, 1 );

    }

    if(enslot_low){
      WriteModuleIn2ndryFPGA(fmodule, BBP::kLower,
			     SHT_MZN::DCT::kAddrTestMode, 0, 1 );
    }

    reg = enable_up | enable_down;
    fmodule.WriteModule(DCT::kAddrCtrlReg, reg);
  
  }// DdrInitialize

  // CalibLUT ---------------------------------------------------------------
  void
  CalibLUT(HUL::FPGAModule& fmodule, uint32_t base_addr)
  {
    using namespace LBUS;
    using namespace LBUS::SHT_BASE;
  
    WriteModuleIn2ndryFPGA(fmodule, base_addr, 
			   SHT_MZN::DCT::kAddrExtraPath, 1, 1);

    sleep(1);
    while(!(ReadModuleIn2ndryFPGA(fmodule, base_addr, SHT_MZN::TDC::kAddrStatus, 1) & SHT_MZN::TDC::kRegReadyLut)){
      std::cout << "#D waiting LUT ready" << std::endl;
    }// while

    if((int32_t)base_addr == BBP::kUpper){
      std::cout << "#D LUT is ready! (MIF-U)" << std::endl;
    }else{
      std::cout << "#D LUT is ready! (MIF-D)" << std::endl;
    }

    WriteModuleIn2ndryFPGA(fmodule, base_addr, 
			   SHT_MZN::DCT::kAddrExtraPath, 0, 1);
    WriteModuleIn2ndryFPGA(fmodule, base_addr, 
			   SHT_MZN::TDC::kAddrReqSwitch, 1, 1);
  }

};
