#include <iostream>
#include <iomanip>
#include <ios>
#include <cstdio>

#include "RegisterMap.hh"
#include "FPGAModule.hh"
#include "UDPRBCP.hh"
#include "BctBusBridgeFunc.hh"
#include "HrTdcFuncs.hh"

enum argIndex{kBin, kIp, kMzn};
using namespace LBUS;
using namespace LBUS::SHT_BASE;
int main(int argc, char* argv[])
{
  if(1 == argc){
    std::cout << "Usage\n";
    std::cout << "initialize [IP address] [Mezzanine exisitence]" << std::endl;
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
  fpga_module.WriteModule(BCT::kAddrReset, 0);

  if(enslot_up){
    uint32_t reg = fpga_module.WriteModule(DCT::kAddrCtrlReg, 1);
    fpga_module.WriteModule(DCT::kAddrCtrlReg, reg | DCT::kRegFRstU);
    fpga_module.WriteModule(DCT::kAddrCtrlReg, reg);
  }

  if(enslot_low){
    uint32_t reg = fpga_module.WriteModule(DCT::kAddrCtrlReg, 1);
    fpga_module.WriteModule(DCT::kAddrCtrlReg, reg | DCT::kRegFRstD);
    fpga_module.WriteModule(DCT::kAddrCtrlReg, reg);
  }

  
  HUL::DAQ::DdrInitialize(fpga_module, enslot_up, enslot_low);
  if(enslot_up)   HUL::DAQ::CalibLUT(fpga_module, BBP::kUpper);
  if(enslot_low)  HUL::DAQ::CalibLUT(fpga_module, BBP::kLower);

  return 0;

}// main
