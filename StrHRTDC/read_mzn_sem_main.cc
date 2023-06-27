#include <iostream>
#include <cstdio>

#include "RegisterMap.hh"
#include "UDPRBCP.hh"
#include "FPGAModule.hh"
#include "BctBusBridgeFunc.hh"
#include "MznFuncs.hh"


enum argIndex{kBin, kIp, kMzn};
using namespace HUL;
using namespace LBUS::SHT_BASE;

int main(int argc, char* argv[])
{
  if(1 == argc){
    std::cout << "Usage\n";
    std::cout << "read_mzn_sem [IP address] [Mezzanine existence]" << std::endl;
    std::cout << " Description of mezzanine existence" << std::endl;
    std::cout << " - up   (mezzanine is attached on upper slot)" << std::endl;
    std::cout << " - low  (mezzanine is attached on lower slot)" << std::endl;
    std::cout << " - both (mezzanine is attached on both slot2)" << std::endl;
    return 0;
  }// usage
  
  // body ------------------------------------------------------
  std::string board_ip  = argv[kIp];
  std::string mezzanine = argv[kMzn];

  bool enslot_up  = false;
  bool enslot_low = false;
  if(mezzanine == "up")   { enslot_up  = true; }
  if(mezzanine == "low")  { enslot_low = true; }
  if(mezzanine == "both") { enslot_up  = true; enslot_low = true; }

  RBCP::UDPRBCP udp_rbcp(board_ip, RBCP::gUdpPort, RBCP::DebugMode::kNoDisp);
  HUL::FPGAModule fpga_module(udp_rbcp);

  std::cout << "==== Mezzanine upper slot ====" << std::endl;
  if(enslot_up) MZN::XilinxSEM::ReadStatus(fpga_module, BBP::kUpper);
  else          std::cout << "Slot is disabled." << std::endl;

  std::cout << std::endl;

  std::cout << "==== Mezzanine lower slot ====" << std::endl;
  if(enslot_low) MZN::XilinxSEM::ReadStatus(fpga_module, BBP::kLower);
  else           std::cout << "Slot is disabled." << std::endl;

  return 0;

}// main

