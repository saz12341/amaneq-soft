#include <iostream>
#include <sstream>
#include <cstdio>

#include "RegisterMap.hh"
#include "UDPRBCP.hh"
#include "FPGAModule.hh"
#include "BctBusBridgeFunc.hh"

enum argIndex{kBin, kIp, kMzn, kMaskValUpper, kMaskValLower};
using namespace LBUS;
using namespace LBUS::SHT_BASE;

int main(int argc, char* argv[])
{
  if(1 == argc){
    std::cout << "Usage\n";
    std::cout << "set_tdcmask [IP address] [Mezzanine existence] [Mask value uppper mezzanine (Hex)] [Mask value lower mezzanine (Hex)]" << std::endl;
    std::cout << " Description of mezzanine existence" << std::endl;
    std::cout << " - up   (mezzanine is attached on upper slot)" << std::endl;
    std::cout << " - low  (mezzanine is attached on lower slot)" << std::endl;
    std::cout << " - both (mezzanine is attached on both slot2)" << std::endl;
    return 0;
  }// usage
  
  // body ------------------------------------------------------
  std::string board_ip     = argv[kIp];
  std::string mezzanine    = argv[kMzn];
  std::string mask_up_str  = argv[kMaskValUpper];
  std::string mask_low_str = argv[kMaskValLower];

  bool enslot_up  = false;
  bool enslot_low = false;
  if(mezzanine == "up")   { enslot_up  = true; }
  if(mezzanine == "low")  { enslot_low = true; }
  if(mezzanine == "both") { enslot_up  = true; enslot_low = true; }
    
  RBCP::UDPRBCP udp_rbcp(board_ip, RBCP::gUdpPort, RBCP::DebugMode::kNoDisp);
  HUL::FPGAModule fpga_module(udp_rbcp);

  std::istringstream iss_up(mask_up_str);
  std::istringstream iss_low(mask_low_str);
  uint32_t mask_up_val;
  uint32_t mask_low_val;
  iss_up  >> std::hex >> mask_up_val;
  iss_low >> std::hex >> mask_low_val;
  std::cout << "#D Set TDC mask to " << board_ip
	    << std::hex << ", 0x" << mask_up_val << ", 0x" << mask_low_val << std::endl;

  if(enslot_up){
    WriteModuleIn2ndryFPGA(fpga_module, BBP::kUpper,
			   SHT_MZN::TDC::kAddrTdcMask, mask_up_val, 4 );
  }

  if(enslot_low){
    WriteModuleIn2ndryFPGA(fpga_module, BBP::kLower,
			   SHT_MZN::TDC::kAddrTdcMask, mask_low_val, 4 );
  }

  return 0;
  
}// main
