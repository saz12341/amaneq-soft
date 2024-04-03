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
  if(argc != 4){
    std::cout << "Usage\n";
    std::cout << argv[0] << " [File of IP address list] [center of acceptable shift range] [length of acceptable shift range]" << std::endl;
    return 0;
  }// usage
  
  // body ------------------------------------------------------
  std::ifstream file(argv[1]);
  
  if (not file.is_open()) {
    std::cout << argv[1]  << " can not be opened." << std::endl;
    return 0;
  }
  
  uint32_t u_global_center  = (uint32_t)strtol(argv[2],NULL,16);
  uint32_t u_local_length   = (uint32_t)strtol(argv[3],NULL,16);
  double   global_center    = (double)u_global_center/(1<<29);
  
  std::string board_ip;
  uint32_t reg_read;
  double   shift;
  
  double   local_center;
  uint32_t u_local_center;
  
  while (std::getline(file, board_ip)) {
    RBCP::UDPRBCP udp_rbcp(board_ip, RBCP::gUdpPort, RBCP::DebugMode::kNoDisp);
    HUL::FPGAModule fpga_module(udp_rbcp);
    
    std::cout << "== module: " << board_ip << " ================" << std::endl;
    
    reg_read  = fpga_module.ReadModule(kAddrShift,4);
    printf("shift value (uint32_t): %08X\n",reg_read);
    
    shift     = (double)reg_read/(1<<29);
    printf("shift value: %f\n",shift);
    
    local_center  = reduce_shift(shift,global_center);
    printf("local_center: %f\n",local_center);
  
    u_local_center=(uint32_t)(local_center*(1<<29));
    printf("local_center (uint32_t): %08X\n",u_local_center);
  
    // set tha acceptable shift value range
    fpga_module.WriteModule(kAddrEepromCenter,u_local_center,4);
    fpga_module.WriteModule(kAddrEepromLength,u_local_length,4);
    
    // ensure tha acceptable shift value range is written into EEPROM
    while(u_local_center!=fpga_module.ReadModule(kAddrEepromCenter,4)){}
    while(u_local_length!=fpga_module.ReadModule(kAddrEepromLength,4)){}
    
    // reset CECE62002 clock
    fpga_module.WriteModule(kAddrPhaseOperate,0);
  }
  
  file.close();
  return 0;
  
}// main
