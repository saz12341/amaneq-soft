#include <iostream>
#include <cstdio>

#include "RegisterMap.hh"
#include "UDPRBCP.hh"
#include "FPGAModule.hh"

using namespace HUL;
using namespace LBUS::CPS;

int main(int argc, char* argv[])
{
  if(argc != 3){
    std::cout << "Usage\n";
    std::cout << argv[0] << " [IP address] [length of acceptable shift range]" << std::endl;
    return 0;
  }// usage
  
  // body ------------------------------------------------------
  std::string board_ip = argv[1];

  RBCP::UDPRBCP udp_rbcp(board_ip, RBCP::gUdpPort, RBCP::DebugMode::kNoDisp);
  HUL::FPGAModule fpga_module(udp_rbcp);

  uint32_t reg_write = (uint32_t)strtol(argv[2],NULL,16);
  fpga_module.WriteModule(kAddrEepromLength,reg_write,4);

  return 0;
  
}// main
