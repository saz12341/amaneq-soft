#include <iostream>
#include <sstream>
#include <cstdio>

#include "RegisterMap.hh"
#include "UDPRBCP.hh"
#include "FPGAModule.hh"

enum argIndex{kBin, kIp, kMaskValMainU, kMaskValMainD, kMaskValMznU, kMaskValMznD, kMaskValEx};
using namespace HUL;
using namespace LBUS::SLT;

int main(int argc, char* argv[])
{
  if(1 == argc){
    std::cout << "Usage\n";
    std::cout << "set_tdcmask [IP address] [Mask value Main-U (Hex)] [Mask value Main-D (Hex)] [Mask value MZN-U (Hex)] [Mask value MZN-D (Hex)] [kMaskEx]"
	      << std::endl;
    return 0;
  }// usage
  
  // body ------------------------------------------------------
  std::string board_ip        = argv[kIp];
  std::string mask_mainu_str  = argv[kMaskValMainU];
  std::string mask_maind_str  = argv[kMaskValMainD];
  std::string mask_mznu_str   = argv[kMaskValMznU];
  std::string mask_mznd_str   = argv[kMaskValMznD];
  std::string mask_ex_str     = argv[kMaskValEx];
  
    
  RBCP::UDPRBCP udp_rbcp(board_ip, RBCP::gUdpPort, RBCP::DebugMode::kNoDisp);
  HUL::FPGAModule fpga_module(udp_rbcp);

  std::istringstream iss_mainu(mask_mainu_str);
  std::istringstream iss_maind(mask_maind_str);
  std::istringstream iss_mznu(mask_mznu_str);
  std::istringstream iss_mznd(mask_mznd_str);
  std::istringstream iss_ex(mask_ex_str);
  uint32_t mask_mainu_val;
  uint32_t mask_maind_val;
  uint32_t mask_mznu_val;
  uint32_t mask_mznd_val;
  uint32_t mask_ex_val;
  iss_mainu >> std::hex >> mask_mainu_val;
  iss_maind >> std::hex >> mask_maind_val;
  iss_mznu  >> std::hex >> mask_mznu_val;
  iss_mznd  >> std::hex >> mask_mznd_val;
  iss_ex    >> std::hex >> mask_ex_val;

  std::cout << "#D Set TDC mask to " << board_ip
	    << std::hex
	    << ", 0x" << mask_mainu_val
	    << ", 0x" << mask_maind_val
	    << ", 0x" << mask_mznu_val
	    << ", 0x" << mask_mznd_val
    	    << ", 0x" << mask_ex_val
	    << std::endl;


  fpga_module.WriteModule(TDC::kTdcMaskMainU, mask_mainu_val, 4);
  fpga_module.WriteModule(TDC::kTdcMaskMainD, mask_maind_val, 4);
  fpga_module.WriteModule(TDC::kTdcMaskMznU,  mask_mznu_val, 4);
  fpga_module.WriteModule(TDC::kTdcMaskMznD,  mask_mznd_val, 4);
  fpga_module.WriteModule(TDC::kTdcMaskEx,    mask_ex_val,   4);

  return 0;
  
}// main
