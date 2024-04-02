#include <iostream>
#include <cstdio>

#include "RegisterMap.hh"
#include "UDPRBCP.hh"
#include "FPGAModule.hh"

using namespace HUL;
using namespace LBUS::CPS;

int main(int argc, char* argv[])
{
  if(argc != 2){
    std::cout << "Usage\n";
    std::cout << argv[0] << " [IP address]" << std::endl;
    return 0;
  }// usage
  
  // body ------------------------------------------------------
  std::string board_ip = argv[1];

  RBCP::UDPRBCP udp_rbcp(board_ip, RBCP::gUdpPort, RBCP::DebugMode::kNoDisp);
  HUL::FPGAModule fpga_module(udp_rbcp);

  uint32_t reg_read;
  reg_read  = fpga_module.ReadModule(kAddrPhaseStatus,1);
  printf("%02X\n",reg_read);

  return 0;
  
}// main
