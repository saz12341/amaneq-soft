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
  uint32_t reg_read;
  
  while (std::getline(file, board_ip)) {
    RBCP::UDPRBCP udp_rbcp(board_ip, RBCP::gUdpPort, RBCP::DebugMode::kNoDisp);
    HUL::FPGAModule fpga_module(udp_rbcp);
    
//    std::cout << "== module: " << board_ip << " ================" << std::endl;
    
    // get the phase selection status
    reg_read  = fpga_module.ReadModule(kAddrPhaseStatus,1);
    printf("%s \t: %02X\n",board_ip.c_str(),reg_read);
     
    // show messanges
    if((reg_read&kRegIsReady)==0) std::cout << board_ip << " is not ready." << std::endl;
    if((reg_read&kRegTimeout)!=0) std::cout << board_ip << " is timeout." << std::endl;
  }
  
  file.close();
  return 0;
  
}// main
