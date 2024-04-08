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

const double kToPs = 2000./2048.;

enum argIndex{kBin, kIp, kMzn, kAddrOffset};
using namespace LBUS;
using namespace LBUS::SHT_BASE;

int32_t trans_to_straight(uint32_t twos_compl)
{
  int32_t ret_val=0;
  if((twos_compl >> 15) == 1){
    // Negative val //
    ret_val = -(0xffff - (twos_compl & 0xffff) +1);
  }else{
    ret_val = static_cast<int32_t>(twos_compl);
  }

  return ret_val;
}

std::string trans_to_ipaddr(uint32_t reg)
{
  if(reg == 0){
    return "Not Connected";
  }else{
    std::ostringstream oss;
    for(int i = 3; i>=0; --i){
      oss << ((reg >> 8*i) & 0xff) << (i == 0? "" : ".");
    }

    return oss.str();
  }
}

int main(int argc, char* argv[])
{
  if(1 == argc){
    std::cout << "Usage\n";
    std::cout << "show_mzn_laccp [IP address] [Mezzanine existence] [Address offset (hex)(option)]" << std::endl;
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
    std::cout << " - LACCP" << std::endl;
    bdump(ReadModuleIn2ndryFPGA(fpga_module, BBP::kUpper, SHT_MZN::MUTIL::kAddrLaccpUp + addr_offset_val, 4));
    std::cout << std::dec << std::endl;

    Utility::PrintNormal("", "-- My offset --");
    uint32_t hbc_offset   = ReadModuleIn2ndryFPGA(fpga_module, BBP::kUpper,
						  SHT_MZN::MUTIL::kAddrHbcOffset + addr_offset_val, 2);
    uint32_t local_offset = ReadModuleIn2ndryFPGA(fpga_module, BBP::kUpper,
						  SHT_MZN::MUTIL::kAddrLocalFineOffset + addr_offset_val, 2);
    uint32_t laccp_offset = ReadModuleIn2ndryFPGA(fpga_module, BBP::kUpper,
						  SHT_MZN::MUTIL::kAddrLaccpFineOffset + addr_offset_val, 2);
    
    std::cout << "Heartbeat count offset: " << hbc_offset << "(" << hbc_offset*8 << " ns)" << std::endl;
    std::cout << "Local fine offset:      " << trans_to_straight(local_offset) << "(" << trans_to_straight(local_offset)*kToPs << " ps)" << std::endl;
    std::cout << "LACCP fine offset:      " << trans_to_straight(laccp_offset) << "(" << trans_to_straight(laccp_offset)*kToPs << " ps)" << std::endl;
    std::cout << std::endl;


    printf("Port, Partner IP address \n");
    for(uint32_t i = 0; i<num_mikumari; ++i){
      WriteModuleIn2ndryFPGA(fpga_module, BBP::kUpper, SHT_MZN::MUTIL::kAddrRegIndex + addr_offset_val, i, 1);
      uint32_t ip_addr = ReadModuleIn2ndryFPGA(fpga_module, BBP::kUpper, SHT_MZN::MUTIL::kAddrPartnerIpAddr + addr_offset_val, 4);
      std::cout << " " << i << ", " << trans_to_ipaddr(ip_addr) << std::endl;    
    }
  }

  if(enslot_low){
    uint32_t num_mikumari = ReadModuleIn2ndryFPGA(fpga_module, BBP::kLower, SHT_MZN::MUTIL::kAddrNumLinks + addr_offset_val, 1); 
    Utility::PrintNormal("", "-- Link Up status --");

    hddaq::BitDump bdump;
    std::cout << " - LACCP" << std::endl;
    bdump(ReadModuleIn2ndryFPGA(fpga_module, BBP::kLower, SHT_MZN::MUTIL::kAddrLaccpUp + addr_offset_val, 4));
    std::cout << std::dec << std::endl;

    Utility::PrintNormal("", "-- My offset --");
    uint32_t hbc_offset   = ReadModuleIn2ndryFPGA(fpga_module, BBP::kLower,
						  SHT_MZN::MUTIL::kAddrHbcOffset + addr_offset_val, 2);
    uint32_t local_offset = ReadModuleIn2ndryFPGA(fpga_module, BBP::kLower,
						  SHT_MZN::MUTIL::kAddrLocalFineOffset + addr_offset_val, 2);
    uint32_t laccp_offset = ReadModuleIn2ndryFPGA(fpga_module, BBP::kLower,
						  SHT_MZN::MUTIL::kAddrLaccpFineOffset + addr_offset_val, 2);
    
    std::cout << "Heartbeat count offset: " << hbc_offset << "(" << hbc_offset*8 << " ns)" << std::endl;
    std::cout << "Local fine offset:      " << trans_to_straight(local_offset) << "(" << trans_to_straight(local_offset)*kToPs << " ps)" << std::endl;
    std::cout << "LACCP fine offset:      " << trans_to_straight(laccp_offset) << "(" << trans_to_straight(laccp_offset)*kToPs << " ps)" << std::endl;
    std::cout << std::endl;


    printf("Port, Partner IP address \n");
    for(uint32_t i = 0; i<num_mikumari; ++i){
      WriteModuleIn2ndryFPGA(fpga_module, BBP::kLower, SHT_MZN::MUTIL::kAddrRegIndex + addr_offset_val, i, 1);
      uint32_t ip_addr = ReadModuleIn2ndryFPGA(fpga_module, BBP::kLower, SHT_MZN::MUTIL::kAddrPartnerIpAddr + addr_offset_val, 4);
      std::cout << " " << i << ", " << trans_to_ipaddr(ip_addr) << std::endl;    
    }
  }

  
  return 0;

}// main

