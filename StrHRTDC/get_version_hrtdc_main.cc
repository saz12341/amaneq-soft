#include <iostream>
#include <iomanip>
#include <ios>
#include <cstdio>

#include "RegisterMap.hh"
#include "FPGAModule.hh"
#include "UDPRBCP.hh"
#include "BctBusBridgeFunc.hh"

enum argIndex{kBin, kIp, kMzn};
using namespace LBUS;
using namespace LBUS::SHT_BASE;
int main(int argc, char* argv[])
{
  if(1 == argc){
    std::cout << "Usage\n";
    std::cout << "get_version_hrtdc [IP address] [Mezzanine existence]" << std::endl;
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
  uint32_t bct_version = fpga_module.ReadModule(BCT::kAddrVersion, 4);
  uint32_t major_version = (bct_version >> 8) & 0xff;
  uint32_t minor_version = (bct_version) & 0xff;

  std::cout << "#D: AMANEQ firmware" << std::endl;
  std::cout << std::hex;
  std::cout << std::setfill('0') << std::right 
	    << "FW ID      : 0x" << std::setw(4) << ((bct_version >> 16) & 0xffff) << std::endl;
  std::cout << std::setfill(' ') << std::dec;  
  std::cout << "FW version : " << major_version << "."
	    << minor_version
	    << std::endl;

  std::cout << "\n#D: Mezzanine firmware" << std::endl;

  if(enslot_up){
    uint32_t bct_version = ReadModuleIn2ndryFPGA(fpga_module, BBP::kUpper,
						 SHT_MZN::BCT::kAddrVersion, 4);
    uint32_t major_version = (bct_version >> 8) & 0xff;
    uint32_t minor_version = (bct_version) & 0xff;

    std::cout << std::hex;
    std::cout << std::setfill('0') << std::right 
	      << "FW ID (MZN-U)     : 0x" << std::setw(4) << ((bct_version >> 16) & 0xffff) << std::endl;
    std::cout << std::setfill(' ') << std::dec;  
    std::cout << "FW version (MZN-U): " << major_version << "."
	      << minor_version
	      << std::endl;
  }

    if(enslot_low){
    uint32_t bct_version = ReadModuleIn2ndryFPGA(fpga_module, BBP::kLower,
						 SHT_MZN::BCT::kAddrVersion, 4);
    uint32_t major_version = (bct_version >> 8) & 0xff;
    uint32_t minor_version = (bct_version) & 0xff;

    std::cout << std::hex;
    std::cout << std::setfill('0') << std::right 
	      << "FW ID (MZN-D)     : 0x" << std::setw(4) << ((bct_version >> 16) & 0xffff) << std::endl;
    std::cout << std::setfill(' ') << std::dec;  
    std::cout << "FW version (MZN-D): " << major_version << "."
	      << minor_version
	      << std::endl;
  }

  return 0;

}// main
