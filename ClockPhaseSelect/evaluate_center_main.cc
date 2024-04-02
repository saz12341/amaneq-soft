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
  
  int num_module  = 0;
  
  std::string board_ip;
  uint32_t  reg_read;
  double    shift;
  double    reduced_shift;
  double    mean;
  double    max_reduced_shift;
  double    min_reduced_shift;
  while (std::getline(file, board_ip)) {
    RBCP::UDPRBCP udp_rbcp(board_ip, RBCP::gUdpPort, RBCP::DebugMode::kNoDisp);
    HUL::FPGAModule fpga_module(udp_rbcp);
    
    std::cout << "== module: " << board_ip << " ================" << std::endl;
    
    reg_read  = fpga_module.ReadModule(kAddrShift,4);
    printf("shift value (uint32_t): %08X\n",reg_read);
    
    shift     = (double)reg_read/(1<<29);
    printf("shift value: %f\n",shift);
    
    reduced_shift = reduce_shift(shift,mean);
    printf("reduced shift value: %f\n",reduced_shift);

    if(num_module==0){
      mean=reduced_shift;
      max_reduced_shift=reduced_shift;
      min_reduced_shift=reduced_shift;
    }

    if(reduced_shift>max_reduced_shift) max_reduced_shift=reduced_shift;
    if(reduced_shift<min_reduced_shift) min_reduced_shift=reduced_shift;

    num_module ++;
  }
  
  double    center = reduce_shift((max_reduced_shift+min_reduced_shift)/2,1);
  double    length = max_reduced_shift-min_reduced_shift;
  
  printf("== recommand acceptable shift range ==\n");
  printf("number_module: %d\n",num_module);
  printf("center: %f\n",center);
  printf("length: %f\n",length);

  if(length>1) printf("WARNING LENGTH!!!");
  
  printf("RECOMMEND CENTER:\n");
  printf("%08X\n",(uint32_t)(center*(1<<29)));
  printf("%08X\n",(uint32_t)((center+2)*(1<<29)));
  printf("%08X\n",(uint32_t)((center+4)*(1<<29)));
  printf("%08X\n",(uint32_t)((center+6)*(1<<29)));
  
  printf("RECOMMEND LENGTH (MIN):\n");
  printf("%08X\n",(uint32_t)(length*(1<<29)));
  
  printf("RECOMMEND LOCAL LENGTH (MIN):\n");
  printf("%08X\n",(uint32_t)((length/2)*(1<<29)));
  
  file.close();
  return 0;
  
}// main
