#include <iostream>
#include <cstdio>

#include "RegisterMap.hh"
#include "UDPRBCP.hh"
#include "FPGAModule.hh"
#include "BctBusBridgeFunc.hh"

enum argIndex{kBin, kIp, kMzn};
using namespace HUL;
using namespace LBUS::SHT_BASE;
void read_a_mzn(HUL::FPGAModule& fpga_module, uint32_t base_addr);

int main(int argc, char* argv[])
{
  if(1 == argc){
    std::cout << "Usage\n";
    std::cout << "read_mzn_xadc [IP address] [Mezzanine existence]" << std::endl;
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

  printf("==== Mezzanine upper slot ====\n");
  if(enslot_up) read_a_mzn(fpga_module, BBP::kUpper);
  else          std::cout << "Slot is disabled." << std::endl;

  printf("\n");

  printf("==== Mezzanine lower slot ====\n");
  if(enslot_low) read_a_mzn(fpga_module, BBP::kLower);
  else           std::cout << "Slot is disabled." << std::endl;

  return 0;

}// main

// read_a_mzn ____________________________________________________________________________
void
read_a_mzn(HUL::FPGAModule& fpga_module, uint32_t base_addr)
{
  using namespace LBUS;
  
  // XADC _____________________________________________________________
  WriteModuleIn2ndryFPGA(fpga_module, base_addr,
			 SHT_MZN::SDS::kAddrXadcDrpMode, SDS::kDrpReadMode, 1);

  // Read temperature monitor
  WriteModuleIn2ndryFPGA(fpga_module, base_addr,
			 SHT_MZN::SDS::kAddrXadcDrpAddr, SDS::kAddrDrpTemp, 1);
  WriteModuleIn2ndryFPGA(fpga_module, base_addr,
			 SHT_MZN::SDS::kAddrXadcExecute, 1, 1);
  uint32_t adc_temp = ReadModuleIn2ndryFPGA(fpga_module, base_addr,
					    SHT_MZN::SDS::kAddrXadcDrpDout, 2);

  // Read status
  uint32_t status = ReadModuleIn2ndryFPGA(fpga_module, base_addr,
					  SHT_MZN::SDS::kAddrSdsStatus, 1);
  
  // translate
  const uint32_t shift   = 4;
  //  const uint32_t mask    = 0xfff;
  const uint32_t max_adc = 0x1000;

  double temp = (adc_temp >> shift)*503.975/max_adc - 273.15; // 503.975: magic number

  printf("FPGA temp.  : %.2f C\n", temp);
  printf("\n");

  // Status
  printf("XADC status (0:false, 1:true)\n");
  printf(" - Over temp.   (Th. 125C)   : %d\n", (status & SDS::kXadcOverTemperature) >> 0);
  printf(" - Temp alarm   (Th. 85C)    : %d\n", (status & SDS::kXadcUserTempAlarm)   >> 1);

}
