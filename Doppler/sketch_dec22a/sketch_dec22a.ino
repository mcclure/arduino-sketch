void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_DAC0,OUTPUT);
  pinMode(PIN_DAC1,OUTPUT);
  dacInit();
}

void loop() {
  // put your main code here, to run repeatedly:
  static uint16_t osc = 0; // Oscillator
  static uint16_t env = 0; // Envelope
  static uint16_t env2 = 0; // Envelope2
  static uint16_t w = 256;   // Wavelength
  static uint16_t w2 = 256;   // Wavelength-env2
  
  static uint16_t rnd = 0; // Random
  static uint16_t seed = 18131;  // Random seed
  static uint16_t seed2 = 89;  // Random seed2
  static int seq = 0;

  uint16_t lastEnv = env;
  env-=4;
  
  if (env > lastEnv) {
    rnd = (rnd + seed)*seed2;
    if (seq%4 < 3) {
      w *= 7; w /= 2;
    //} else if (seq%4 <3) {
    //  w *= 3; w /= 2;
    //} else {
      w /= ((rnd % 4)+2);
    }
    if (w > 2048 || w < 16 || seq%16==0) w = 400;
    seq++;
    w2 /= 2;
    if (w2 < 128)
      w2 = 2048*4;
  }
  osc+=w;
  env2 -= w2;
  dacWrite(osc,env);
}
