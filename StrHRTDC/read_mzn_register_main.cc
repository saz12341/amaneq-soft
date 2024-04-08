#include <iostream>
#include <cstdio>
#include <ios>
#include <iomanip>
#include <sstream>

#include "RegisterMap.hh"
#include "FPGAModule.hh"
#include "UDPRBCP.hh"
#include "BctBusBridgeFunc.hh"

enum argIndex{kBin, kIp, kMzn, kAddr, kNBytes};
using namespace LBUS;
using namespace LBUS::SHT_BASE;

int main(int argc, char* argv[])
{
  if(1 == argc){
    std::cout << "Usage\n";
    std::cout << "read_mzn_register [IP address] [Mezzanine existence] [Address (hex)] [Num bytes]" << std::endl;
    std::cout << " Description of mezzanine existence" << std::endl;
    std::cout << " - up   (mezzanine is attached on upper slot)" << std::endl;
    std::cout << " - low  (mezzanine is attached on lower slot)" << std::endl;
    std::cout << " - both (mezzanine is attached on both slot2)" << std::endl;
    return 0;
  }// usage
  
  // body ------------------------------------------------------
  std::string board_ip     = argv[kIp];
  std::string mezzanine    = argv[kMzn];
  std::string rbcp_address = argv[kAddr];
  std::string num_bytes    = argv[kNBytes];

  bool enslot_up  = false;
  bool enslot_low = false;
  if(mezzanine == "up")   { enslot_up  = true; }
  if(mezzanine == "low")  { enslot_low = true; }
  if(mezzanine == "both") { enslot_up  = true; enslot_low = true; }

  RBCP::UDPRBCP udp_rbcp(board_ip, RBCP::gUdpPort, RBCP::DebugMode::kNoDisp);
  HUL::FPGAModule fpga_module(udp_rbcp);

  std::istringstream iss_addr(rbcp_address);
  std::istringstream iss_nbytes(num_bytes);
  uint32_t address_val;
  uint32_t nbytes_val;
  iss_addr   >> std::hex >> address_val;
  iss_nbytes >> nbytes_val;

  if(enslot_up){
    uint32_t reg = ReadModuleIn2ndryFPGA(fpga_module, BBP::kUpper, address_val, nbytes_val);
    std::cout << "#D: Read register (MZN-U): " << reg << " (0x" << std::hex << reg << ")" << std::endl;
  }

  if(enslot_low){
    uint32_t reg = ReadModuleIn2ndryFPGA(fpga_module, BBP::kLower, address_val, nbytes_val);
    std::cout << "#D: Read register (MZN-L): " << reg << " (0x" << std::hex << reg << ")" << std::endl;
  }

  
  return 0;

}// main

