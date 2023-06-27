#include <iostream>
#include <iomanip>
#include <ios>
#include <cstdio>

#include "RegisterMap.hh"
#include "FPGAModule.hh"
#include "UDPRBCP.hh"

enum argIndex{kBin, kIp};

using namespace HUL;
using namespace LBUS::SLT;
int main(int argc, char* argv[])
{
  if(1 == argc){
    std::cout << "Usage\n";
    std::cout << "activate_localstc [IP address]" << std::endl;
    return 0;
  }// usage
  
  // body ------------------------------------------------------
  std::string board_ip = argv[kIp];

  RBCP::UDPRBCP udp_rbcp(board_ip, RBCP::gUdpPort, RBCP::DebugMode::kNoDisp);
  HUL::FPGAModule fpga_module(udp_rbcp);

  std::cout << "#D: Set DAQ run status => IDLE" << std::endl;  
  fpga_module.WriteModule(LSTC::kAddrRunStatus,  1);

  std::cout << "#D: Reset gate number" << std::endl;
  fpga_module.WriteModule(LSTC::kAddrGateNumber, 0);

  std::cout << "#D: Heartbeat counter soft reset" << std::endl;
  fpga_module.WriteModule(LSTC::kAddrHbcReset,   0);

  std::cout << "#D: Set auto gate number increment" << std::endl;
  fpga_module.WriteModule(LSTC::kAddrAutoUpdate, 1);


  std::cout << "#D: Set DAQ run status => RUNNING" << std::endl;  
  fpga_module.WriteModule(LSTC::kAddrRunStatus,  1);
  
  return 0;

}// main
