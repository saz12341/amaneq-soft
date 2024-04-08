#include <iostream>
#include <iomanip>
#include <ios>
#include <cstdio>

#include "RegisterMap.hh"
#include "FPGAModule.hh"
#include "UDPRBCP.hh"
#include "BctBusBridgeFunc.hh"

enum argIndex{kBin, kIp, kMzn};
using namespace LBUS;
using namespace LBUS::SHT_BASE;
int main(int argc, char* argv[])
{
  if(1 == argc){
    std::cout << "Usage\n";
    std::cout << "reconfig_mzn [IP address] [Mezzanine existence]" << std::endl;
    std::cout << " Description of mezzanine existence" << std::endl;
    std::cout << " - up   (mezzanine is attached on upper slot)" << std::endl;
    std::cout << " - low  (mezzanine is attached on lower slot)" << std::endl;
    return 0;
  }// usage
  
  // body ------------------------------------------------------
  std::string board_ip  = argv[kIp];
  std::string mezzanine = argv[kMzn];

  bool enslot_up  = false;
  bool enslot_low = false;
  if(mezzanine == "up")   { enslot_up  = true; }
  if(mezzanine == "low")  { enslot_low = true; }

  RBCP::UDPRBCP udp_rbcp(board_ip, RBCP::gUdpPort, RBCP::DebugMode::kNoDisp);
  HUL::FPGAModule fpga_module(udp_rbcp);

  std::cout << "#D: Reconfig mezzanine FPGA (Please ignore RBCP time-out)" << std::endl;

  if(enslot_up){
    WriteModuleIn2ndryFPGA(fpga_module, BBP::kUpper,
			   SHT_MZN::BCT::kAddrReConfig, 0, 1);
  }

  if(enslot_low){
    WriteModuleIn2ndryFPGA(fpga_module, BBP::kLower,
			   SHT_MZN::BCT::kAddrReConfig, 0, 1);
  }

  return 0;

}// main
