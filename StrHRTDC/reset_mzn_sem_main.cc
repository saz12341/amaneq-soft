#include <iostream>
#include <cstdio>

#include "RegisterMap.hh"
#include "UDPRBCP.hh"
#include "FPGAModule.hh"
#include "BctBusBridgeFunc.hh"
#include "MznFuncs.hh"

enum argIndex{kBin, kIp, kMzn};
using namespace LBUS;
using namespace LBUS::SHT_BASE;

int main(int argc, char* argv[])
{
  if(1 == argc){
    std::cout << "Usage\n";
    std::cout << "reset_mzn_sem [IP address]" << std::endl;
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

  std::cout << "==== Mezzanine upper slot ====" << std::endl;
  if(enslot_up){
    MZN::XilinxSEM::ResetSEMCore(fpga_module, BBP::kUpper);
  }else{
    std::cout << "Slot is disabled." << std::endl;
  }

  std::cout << std::endl;

  std::cout << "==== Mezzanine lower slot ====" << std::endl;
  if(enslot_low){
    MZN::XilinxSEM::ResetSEMCore(fpga_module, BBP::kLower);
  }else{
    std::cout << "Slot is disabled." << std::endl;
  }

  return 0;

}// main

// control sem in mzn ___________________________________________________________________
void
set_error_injection_address(HUL::FPGAModule& fpga_module, uint32_t base_addr,
			    uint32_t command,
			    uint32_t addr_linear_frame,
			    uint32_t addr_word,
			    uint32_t addr_bit
			    )
{
  // command              :  4bit
  // linear frame address : 17bit
  // word address         :  7bit
  // bit address          :  5bit
  const uint32_t mask_linear = 0x1ffff;
  const uint32_t mask_word   = 0x7f;
  const uint32_t mask_bit    = 0x1f;

  const uint32_t shift_word   = 5;
  const uint32_t shift_linear = 7 + shift_word;

  // For command (5th byte)
  const uint32_t mask_command  = 0xf;
  const uint32_t shift_command = 4;

  uint32_t reg_1st_4th = addr_bit & mask_bit;
  reg_1st_4th = reg_1st_4th | ((addr_word & mask_word) << shift_word);
  reg_1st_4th = reg_1st_4th | ((addr_linear_frame & mask_linear) << shift_linear);
  WriteModuleIn2ndryFPGA(fpga_module, base_addr,
			 SHT_MZN::SDS::kAddrSemErrAddr, reg_1st_4th, 4);

  uint32_t reg_5th = (command & mask_command) << shift_command;
  WriteModuleIn2ndryFPGA(fpga_module, base_addr,
			 SHT_MZN::SDS::kAddrSemErrAddr+4, reg_5th, 1);
}
