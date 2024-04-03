#include <iostream>
#include <cstdio>

#include <fstream>
#include <cmath>

#include "RegisterMap.hh"
#include "UDPRBCP.hh"
#include "FPGAModule.hh"

using namespace HUL;
using namespace LBUS::CPS;

double reduce_shift(double shift, double mean){
  return fmod(fmod(shift-(mean-1.),2)+2.,2)+mean-1.;
}

int main(int argc, char* argv[])
{
  if(argc != 2){
    std::cout << "Usage\n";
    std::cout << argv[0] << " [File of IP address list]" << std::endl;
    return 0;
  }// usage
  
  // body ------------------------------------------------------
  std::ifstream file(argv[1]);
  
  if (not file.is_open()) {
    std::cout << argv[1]  << " can not be opened." << std::endl;
    return 0;
  }
  
  std::string board_ip;
  
  while (std::getline(file, board_ip)) {
    RBCP::UDPRBCP udp_rbcp(board_ip, RBCP::gUdpPort, RBCP::DebugMode::kNoDisp);
    HUL::FPGAModule fpga_module(udp_rbcp);
    
//    std::cout << "== module: " << board_ip << " ================" << std::endl;
    
    // reset CECE62002 clock
    fpga_module.WriteModule(kAddrPhaseOperate,0);
  }
  
  file.close();
  return 0;
  
}// main
