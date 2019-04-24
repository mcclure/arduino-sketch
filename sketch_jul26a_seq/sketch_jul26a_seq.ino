#include <ctype.h>
#include <math.h>

#define PLAY 2
#define PITCH 3
#define FUNCS 26
#define STACKMAX 16
#define DEBUG 0

const char song[] = "\
(M 0 4 7) \
(N 0 3 7) \
(O 12 16 19) \
(R M M M M) \
(S N N N N) \
(T M M M O) \
R S R S T S R S \
";

typedef enum {
  S_SCAN = 0,
  S_WANTTAG,
  S_WANTRPAREN  
} State;

typedef struct {
  int sp;
  int stack[STACKMAX];
  int func[FUNCS];
  bool looped;
  State state;
} interp;

int next(interp *p) {
  bool isbuilding = false;
  int building = 0;
  while (1) {
    int i = p->stack[p->sp];
    char ch = song[i];
#if DEBUG
    Serial.print("Loop ");
    Serial.print(p->state);
    Serial.print("\n");
    Serial.print(p->sp);
    Serial.print(", ");
    Serial.print(i);
    Serial.print(", ");
    Serial.print(ch);
    Serial.print("\n");
#endif
    if (ch == '\0') {
      p->sp = 0; // In case of syntax error
      p->state = S_SCAN; // Ibid
      p->stack[p->sp] = 0;
      p->looped = true;
      if (isbuilding) // Mild code duplication :(
        return building;
      continue;
    } else switch (p->state) {
      case S_WANTTAG:
        if (isupper(ch))
          p->func[ch-'A'] = i+1;
        p->state = S_WANTRPAREN;
        break;

      case S_WANTRPAREN:
        if (ch == ')')
          p->state = S_SCAN;
        break;
      
      case S_SCAN:
        if (isdigit(ch)) {
          building *= 10;
          isbuilding = true;
          building += (ch - '0');
        } else {
#if DEBUG
          Serial.print("NOTDIGIT\n");
#endif
          if (isbuilding)
            return building;

          if (isupper(ch)) {
            p->sp++;
            p->stack[p->sp] = p->func[ch-'A'];
            continue;
          }

          switch (ch) {
            case '(':
              p->state = p->looped ? S_WANTRPAREN : S_WANTTAG;
              break;
            case ')':
              p->sp--;
              if (p->sp < 0)
                p->sp = 0;
              break;
            default:break;
          }
        }
        break;
    }
    p->stack[p->sp]++;
  }
}

static interp note;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PLAY, OUTPUT);
  pinMode(PITCH, OUTPUT);
#if DEBUG
  Serial.begin(9600);
#endif
}

void high() {
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(PLAY, HIGH);
}

void low() {
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(PLAY, LOW);
}

static int i = 0, p = 0;
const int rate = 300;
const int highTime = 80;

static int octave = 255/5;

void loop() {
#if DEBUG
  Serial.print("Booted\n");
#endif
  int n = next(&note);

#if DEBUG
  Serial.print("Out\n");
#endif

  if (true) {
    int p = octave*3 + (octave*n)/12;
    
    analogWrite(PITCH, p);
    high();
    delay(highTime);
    low();
    delay(rate-highTime);
  } else {
    delay(rate);
  }
  i++;
  p++;
}
