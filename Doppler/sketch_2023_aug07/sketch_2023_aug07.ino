//#define NOLED
#ifndef NOLED
#include "build_top_bin.h" // set up the path to doppler_simple_io or custom firmware
#include <ICEClass.h>
ICEClass ice40;
#endif
//#define PIN_DAC0 39
//#define PIN_DAC1 39
#define BEAT 9
#define SQUARE 9
#define VCA 7
void setup() {
#ifndef NOLED
  ice40.upload(build_top_bin,build_top_bin_len); // Upload BitStream Firmware to FPGA -> see variant.h
  ice40.upload(); // Upload BitStream Firmware to FPGA -> see variant.h
  delay(100);
  //ice40.initSPI();  // start SPI runtime Link to FPGA
#endif
  // put your setup code here, to run once:
  //pinMode(PIN_DAC0,OUTPUT);
  //pinMode(PIN_DAC1,OUTPUT);
  //pinMode(SQUARE,OUTPUT);
  //dacInit();
}

void loop() {
}
