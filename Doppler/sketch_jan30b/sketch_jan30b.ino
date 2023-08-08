#define VCA 7
void setup() {
  // put your setup code here, to run once:
  pinMode(VCA,OUTPUT);
}

void loop() {
  static int i = 0;
  analogWrite(VCA, i++);
}
