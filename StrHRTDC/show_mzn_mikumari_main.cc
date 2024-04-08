#include <iostream>
#include <cstdio>
#include <ios>
#include <iomanip>
#include <sstream>

#include "RegisterMap.hh"
#include "UDPRBCP.hh"
#include "FPGAModule.hh"
#include "BitDump.hh"
#include "Utility.hh"
#include "BctBusBridgeFunc.hh"

enum argIndex{kBin, kIp, kMzn, kAddrOffset};
using namespace LBUS;
using namespace LBUS::SHT_BASE;

int main(int argc, char* argv[])
{
  if(1 == argc){
    std::cout << "Usage\n";
    std::cout << "show_mzn_mikumari [IP address] [Mezzanine existence] [Address offset (hex)(option)]" << std::endl;
    std::cout << " Description of mezzanine existence" << std::endl;
    std::cout << " - up   (mezzanine is attached on upper slot)" << std::endl;
    std::cout << " - low  (mezzanine is attached on lower slot)" << std::endl;
    std::cout << " - both (mezzanine is attached on both slot2)" << std::endl;

    return 0;
  }// usage
  
  // body ------------------------------------------------------
  std::string board_ip     = argv[kIp];
  std::string mezzanine    = argv[kMzn];
  std::string addr_offset  = "0x0000";
  if(argc == kAddrOffset+1){
    addr_offset  = argv[kAddrOffset];
  }

  bool enslot_up  = false;
  bool enslot_low = false;
  if(mezzanine == "up")   { enslot_up  = true; }
  if(mezzanine == "low")  { enslot_low = true; }
  if(mezzanine == "both") { enslot_up  = true; enslot_low = true; }

  RBCP::UDPRBCP udp_rbcp(board_ip, RBCP::gUdpPort, RBCP::DebugMode::kNoDisp);
  HUL::FPGAModule fpga_module(udp_rbcp);

  std::istringstream iss_addr(addr_offset);
  uint32_t addr_offset_val;
  iss_addr >> std::hex >> addr_offset_val;

  if(enslot_up){
    uint32_t num_mikumari = ReadModuleIn2ndryFPGA(fpga_module, BBP::kUpper, SHT_MZN::MUTIL::kAddrNumLinks + addr_offset_val, 1); 
    Utility::PrintNormal("", "-- Link Up status --");

    hddaq::BitDump bdump;
    std::cout << " - CBT" << std::endl;
    bdump(ReadModuleIn2ndryFPGA(fpga_module, BBP::kUpper,
				SHT_MZN::MUTIL::kAddrCbtLaneUp + addr_offset_val, 4));
    std::cout << " - MIKUMARI" << std::endl;
    bdump(ReadModuleIn2ndryFPGA(fpga_module, BBP::kUpper,
				SHT_MZN::MUTIL::kAddrMikumariUp + addr_offset_val, 4));
    std::cout << " - LACCP" << std::endl;
    bdump(ReadModuleIn2ndryFPGA(fpga_module, BBP::kUpper,
				SHT_MZN::MUTIL::kAddrLaccpUp + addr_offset_val, 4));

    std::cout << std::dec << std::endl;

    printf("Port, IdelayTap, Bitslip Number \n");
    for(uint32_t i = 0; i<num_mikumari; ++i){
      WriteModuleIn2ndryFPGA(fpga_module, BBP::kUpper,
			     SHT_MZN::MUTIL::kAddrRegIndex + addr_offset_val, i, 1);
      uint32_t idelay_tap =  ReadModuleIn2ndryFPGA(fpga_module, BBP::kUpper,
						   SHT_MZN::MUTIL::kAddrCbtTapValueIn + addr_offset_val, 1);
      uint32_t bitslip_num = ReadModuleIn2ndryFPGA(fpga_module, BBP::kUpper,
						   SHT_MZN::MUTIL::kAddrCbtBitSlipIn + addr_offset_val, 1);
      printf(" %2d, %8d, %8d \n", i, idelay_tap, bitslip_num);
    }
  }

  if(enslot_low){
    uint32_t num_mikumari = ReadModuleIn2ndryFPGA(fpga_module, BBP::kLower, SHT_MZN::MUTIL::kAddrNumLinks + addr_offset_val, 1); 
    Utility::PrintNormal("", "-- Link Up status --");

    hddaq::BitDump bdump;
    std::cout << " - CBT" << std::endl;
    bdump(ReadModuleIn2ndryFPGA(fpga_module, BBP::kLower,
				SHT_MZN::MUTIL::kAddrCbtLaneUp + addr_offset_val, 4));
    std::cout << " - MIKUMARI" << std::endl;
    bdump(ReadModuleIn2ndryFPGA(fpga_module, BBP::kLower,
				SHT_MZN::MUTIL::kAddrMikumariUp + addr_offset_val, 4));
    std::cout << " - LACCP" << std::endl;
    bdump(ReadModuleIn2ndryFPGA(fpga_module, BBP::kLower,
				SHT_MZN::MUTIL::kAddrLaccpUp + addr_offset_val, 4));

    std::cout << std::dec << std::endl;

    printf("Port, IdelayTap, Bitslip Number \n");
    for(uint32_t i = 0; i<num_mikumari; ++i){
      WriteModuleIn2ndryFPGA(fpga_module, BBP::kLower,
			     SHT_MZN::MUTIL::kAddrRegIndex + addr_offset_val, i, 1);
      uint32_t idelay_tap =  ReadModuleIn2ndryFPGA(fpga_module, BBP::kLower,
						   SHT_MZN::MUTIL::kAddrCbtTapValueIn + addr_offset_val, 1);
      uint32_t bitslip_num = ReadModuleIn2ndryFPGA(fpga_module, BBP::kLower,
						   SHT_MZN::MUTIL::kAddrCbtBitSlipIn + addr_offset_val, 1);
      printf(" %2d, %8d, %8d \n", i, idelay_tap, bitslip_num);
    }
  }


  return 0;

}// main

