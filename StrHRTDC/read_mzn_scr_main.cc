#include <iostream>
#include <iomanip>
#include <fstream>
#include <ios>
#include <cstdio>
#include <unistd.h>

#include "RegisterMap.hh"
#include "FPGAModule.hh"
#include "UDPRBCP.hh"
#include "BctBusBridgeFunc.hh"

// Main ___________________________________________________________________________
enum argIndex{kBin, kIp, kMzn, kFilePath, kApp};
using namespace LBUS;
using namespace LBUS::SHT_BASE;
int main(int argc, char* argv[])
{
  if(1 == argc){
    std::cout << "Usage\n";
    std::cout << "read_mzn_scr [IP address] [Mezzanine existence] [Output file path] [append (option)]" << std::endl;
    std::cout << " Description of mezzanine existence" << std::endl;
    std::cout << " - up   (mezzanine is attached on upper slot)" << std::endl;
    std::cout << " - low  (mezzanine is attached on lower slot)" << std::endl;

    return 0;
  }// usage
  
  // body ------------------------------------------------------
  const std::string kBoardIp   = argv[kIp];
  const std::string kMezzanine = argv[kMzn];
  const std::string kOutPath   = argv[kFilePath];
  std::ios_base::openmode ios_flag = std::ios::binary;
  if(argc == kApp+1) ios_flag = ios_flag | std::ios::app;

  uint32_t kBbpAddr = BBP::kUpper;
  if(kMezzanine == "up")  { kBbpAddr = BBP::kUpper; }
  if(kMezzanine == "low") { kBbpAddr = BBP::kLower; }

  std::ofstream ofs(kOutPath.c_str(), ios_flag);
  if(!ofs.is_open()){
    std::cerr << "#E: File cannot be not created (" << kFilePath << ")." << std::endl;
    return 0;
  }

  RBCP::UDPRBCP udp_rbcp(kBoardIp, RBCP::gUdpPort, RBCP::DebugMode::kNoDisp);
  HUL::FPGAModule fpga_module(udp_rbcp);

  const uint32_t kNumReadCh   = ReadModuleIn2ndryFPGA(fpga_module, kBbpAddr, SHT_MZN::SCR::kAddrNumCh, 1);
  const uint32_t kReadLength  = 4*kNumReadCh;

  std::cout << "#D: # of scaler channel: " << kNumReadCh << std::endl;
  std::cout << "#D: Read length is " << kReadLength << " bytes" << std::endl;

  if(0 == (ReadModuleIn2ndryFPGA(fpga_module, kBbpAddr, SHT_MZN::SCR::kAddrStatus, 1) & SHT_MZN::SCR::kIndexFifoEmpty)){
    // Previous data remains in FOFO
    // Probably, RBCP time out happended during FIFO readout
    std::cout << "#W: FIFO is not empty. RBCP time out?" << std::endl;
    WriteModuleIn2ndryFPGA(fpga_module, kBbpAddr, SHT_MZN::SCR::kAddrScrReset, SHT_MZN::SCR::kIndexFifoReset, 1);
    WriteModuleIn2ndryFPGA(fpga_module, kBbpAddr, SHT_MZN::SCR::kAddrScrReset, 0, 1);
  }

  ReadModuleIn2ndryFPGA(fpga_module, kBbpAddr, SHT_MZN::SCR::kAddrLatchCnt, 1);

  uint32_t fifo_data[kNumReadCh];
  const uint32_t kNumCycle = kNumReadCh;

  for(uint32_t i = 0; i<kNumCycle; ++i){
    uint32_t data = ReadModuleIn2ndryFPGA(fpga_module, kBbpAddr, SHT_MZN::SCR::kAddrReadFIFO, 4);
    fifo_data[i] = data;
  }

  ofs.write((char*)fifo_data, kNumReadCh*4);
  
  std::cout << "#D: Scaler is read " << std::endl;

  ofs.close();

  return 0;

}// main
