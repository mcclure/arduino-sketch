//#define NO_ICE
//#define NO_AUDIO
#ifndef NO_ICE
#include "build_top_bin.h" // set up the path to doppler_simple_io or custom firmware
#include <ICEClass.h>
ICEClass ice40;
#endif
#define PIN_DAC0 39
//#define PIN_DAC1 39
#define BEAT 9
#define SQUARE 9
#define VCA 7

#define FPGA0 7
#define FPGA1 6
#define FPGA2 8
#define FPGA3 9

void setup() {
#ifndef NO_ICE
  ice40.upload(build_top_bin,build_top_bin_len); // Upload BitStream Firmware to FPGA -> see variant.h
  //ice40.upload(); // Upload BitStream Firmware to FPGA -> see variant.h
  delay(100);
  //ice40.initSPI();  // start SPI runtime Link to FPGA
#endif
  // put your setup code here, to run once:
#ifndef NO_AUDIO
  pinMode(PIN_DAC0,OUTPUT);
  //pinMode(PIN_DAC1,OUTPUT);
  //pinMode(SQUARE,OUTPUT);

  pinMode(FPGA0, INPUT);
  pinMode(FPGA1, INPUT);
  pinMode(FPGA2, INPUT);
  pinMode(FPGA3, INPUT);

  dacInit();
#endif
}

void loop() {
#ifndef NO_AUDIO
#define CHANNELS 4

/*
 * Generate audioframe deltas for four waveforms in a major chord.
 * This does not have to be correct, it only has to get us in some sort of ballpark
  perl -e '$semi = 2**(1/12);  $smax = ((1 << 16)/4); print("const uint16_t channel_max = $smax;\n"); $base = 48000/$smax*440; @note = map { $base * ($semi**$_) } (0,3,7,12); @ints = map { int($_) } @note; @strs = map { "$_" } @ints; $vals = join(", ", @strs); print("const uint16_t incr[CHANNELS] = {$vals};\n");'
*/
const uint16_t channel_max = 16384;
const uint16_t incr[CHANNELS] = {29, 34, 43, 58};

  const uint16_t half_channel_max = channel_max/2;
  
  static uint16_t osc[CHANNELS] = {half_channel_max,half_channel_max,half_channel_max,half_channel_max};
  bool on[CHANNELS] = {digitalRead(FPGA0), digitalRead(FPGA1), digitalRead(FPGA2), digitalRead(FPGA3)};

  uint16_t accum = 0;
  for(int c = 0; c < CHANNELS; c++) {
    if (on[c]) {
      osc[c] += incr[c];
      osc[c] %= channel_max;
    }
    uint16_t val = osc[c];
    accum += val;
  }
  accum /= 8; // CHANGE THIS LINE TO SET VOLUME
  dacWrite(accum,accum);
#endif
}
