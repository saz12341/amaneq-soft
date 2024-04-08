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

    std::cout << "#D : Initialize DDR receiver" << std::endl;

    if(enslot_low){
      WriteModuleIn2ndryFPGA(fmodule, BBP::kLower,
			     SHT_MZN::DCT::kAddrTestMode, 1, 1 );
    }

    const int kMaxTraial = 5;
    
    if(enslot_up){
      bool is_aligned = false;
      
      for(int i = 0; i<kMaxTraial; ++i){
	uint32_t reg = enable_up | DCT::kRegTestModeU;
	fmodule.WriteModule(DCT::kAddrCtrlReg, reg);
	
	WriteModuleIn2ndryFPGA(fmodule, BBP::kUpper,
			       SHT_MZN::DCT::kAddrTestMode, 1, 1 );
	
	// Base
	fmodule.WriteModule(DCT::kAddrInitDDR, 0);
	sleep(1);

	uint32_t ret = fmodule.ReadModule(DCT::kAddrRcvStatus, 1);	
	if( ret & DCT::kRegBitAlignedU){
	  std::cout << "#D : DDR initialize succeeded (MZN-U)" << std::endl;
	  is_aligned = true;
	  break;
	}
	
	WriteModuleIn2ndryFPGA(fmodule, BBP::kUpper,
			       SHT_MZN::DCT::kAddrTestMode, 0, 1 );
      }

      if(!is_aligned){
	std::cout << "#E : Failed (MZN-U)" << std::endl;
      }

      WriteModuleIn2ndryFPGA(fmodule, BBP::kUpper,
      			     SHT_MZN::DCT::kAddrTestMode, 0, 1 );
      
    }// bit aligned ?

    if(enslot_low){
      bool is_aligned = false;
      
      for(int i = 0; i<kMaxTraial; ++i){
	uint32_t reg = enable_down | DCT::kRegTestModeD;
	fmodule.WriteModule(DCT::kAddrCtrlReg, reg);
	
	WriteModuleIn2ndryFPGA(fmodule, BBP::kLower,
			       SHT_MZN::DCT::kAddrTestMode, 1, 1 );
	
	// Base
	fmodule.WriteModule(DCT::kAddrInitDDR, 0);
	sleep(1);

	uint32_t ret = fmodule.ReadModule(DCT::kAddrRcvStatus, 1);	
	if( ret & DCT::kRegBitAlignedD){
	  std::cout << "#D : DDR initialize succeeded (MZN-D)" << std::endl;
	  is_aligned = true;
	  break;
	}

	WriteModuleIn2ndryFPGA(fmodule, BBP::kLower,
			       SHT_MZN::DCT::kAddrTestMode, 0, 1 );
      }

      if(!is_aligned){
	std::cout << "#E : Failed (MZN-D)" << std::endl;
      }

      WriteModuleIn2ndryFPGA(fmodule, BBP::kLower,
			     SHT_MZN::DCT::kAddrTestMode, 0, 1 );

    }// bit aligned ?
    

    uint32_t reg = enable_up | enable_down;
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
