// Square pin test
#define SQUARE 9
void setup() {
    pinMode(SQUARE,OUTPUT);
}
void loop() {
    static int i = 0;
    digitalWrite(SQUARE, (i/800)%2 ? HIGH : LOW);
    i++;
    delay(1);
}
