#include <LiquidCrystal_PCF8574.h>
#include <SmartDelay.h>

const int displayAddress = 0x27;
LiquidCrystal_PCF8574 lcd(displayAddress);

#include <stdarg.h>
#define PRINTF_BUF 21
void printAt(int c, int r, const char *s, ...) {
  char buf[PRINTF_BUF];
  va_list ap;
  va_start(ap, s);
  lcd.setCursor(c, r);
  vsnprintf(buf, sizeof(buf), s, ap);
  lcd.print(buf);
  va_end(ap);
}

// SmartButton

#define SmartButton_debounce 10
#define SmartButton_hold 1000
#define SmartButton_long 5000
#define SmartButton_idle 10000

class SmartButton {
  private:
    byte btPin;
    enum state {Idle = 0, PreClick, Click, Hold, LongHold, ForcedIdle, StatesNumber};
    enum input {Release = 0, WaitDebounce, WaitHold, WaitLongHold, WaitIdle, Press, InputsNumber};
    typedef void (SmartButton::*FSM)(enum state st, enum input in);
    enum state btState = Idle;
    enum input btInput = Release;
    unsigned long pressTimeStamp;
    FSM action[StatesNumber][InputsNumber] = {
      {NULL, NULL, NULL, NULL, NULL, &SmartButton::ToPreClick},
      {&SmartButton::ToIdle, &SmartButton::ToClick, NULL, NULL, NULL, NULL},
      {&SmartButton::ToIdle, NULL, &SmartButton::ToHold, NULL, NULL, NULL},
      {&SmartButton::ToIdle, NULL, NULL, &SmartButton::ToLongHold, NULL, NULL},
      {&SmartButton::ToIdle, NULL, NULL, NULL, &SmartButton::ToForcedIdle, NULL},
      {&SmartButton::ToIdle, NULL, NULL, NULL, NULL, NULL}
    };
    void DoAction(enum state st, enum input in) {
      if (action[st][in] == NULL) {
        //Serial.println("NULL action");
        return;
      }
      Serial.print("Do action "); Serial.print(btPin); Serial.print(" ");
      Serial.print(st); Serial.print(" "); Serial.println(in);
      (this->*(action[st][in]))(st, in);
    }
    void ToPreClick(enum state st, enum input in) {
      Serial.println("Pre Click");
      pressTimeStamp = millis();
      btState = PreClick;
    }
    void ToIdle(enum state st, enum input in) {
      Serial.print("IDLE "); Serial.print(btPin); Serial.print(" ");
      Serial.print(st); Serial.print(" "); Serial.println(in);
      btState = Idle;
      switch (st) {
        case Click: offClick(); break;
        case Hold: offHold(); break;
        case LongHold: offLongHold(); break;
      }
    }
    void ToForcedIdle(enum state st, enum input in) {
      Serial.print("Forced IDLE "); Serial.print(btPin); Serial.print(" ");
      Serial.print(st); Serial.print(" "); Serial.println(in);
      btState = ForcedIdle;
    }
    void ToClick(enum state st, enum input in) {
      Serial.println("Click");
      btState = Click;
      onClick();
    }
    void ToHold(enum state st, enum input in) {
      Serial.println("Hold");
      btState = Hold;
      onHold();
    }
    void ToLongHold(enum state st, enum input in) {
      Serial.println("LongHold");
      btState = LongHold;
      onLongHold();
    }
  public:
    SmartButton();
    SmartButton(int pin);
    ~SmartButton();
    void run();
    inline virtual void onClick() {};
    inline virtual void onHold() {};
    inline virtual void onLongHold() {};
    inline virtual void offClick() {};
    inline virtual void offHold() {};
    inline virtual void offLongHold() {};
};
SmartButton::SmartButton() {}
SmartButton::~SmartButton() {}
SmartButton::SmartButton(int pin) {
  btPin = pin;
  pinMode(pin, INPUT_PULLUP);
}
void SmartButton::run() {
  unsigned long mls = millis();
  //Serial.print("Run "); Serial.println(btState);
  if (!digitalRead(btPin)) {
    if (btState == Idle) {
      DoAction(btState, Press);
      return;
    }
    if (mls - pressTimeStamp > SmartButton_debounce) {
      DoAction(btState, WaitDebounce);
    }
    if (mls - pressTimeStamp > SmartButton_hold) {
      DoAction(btState, WaitHold);
    }
    if (mls - pressTimeStamp > SmartButton_long) {
      DoAction(btState, WaitLongHold);
    }
    if (mls - pressTimeStamp > SmartButton_idle) {
      DoAction(btState, WaitIdle);
    }
    return;
  } else {
    if (btState != Idle) {
      DoAction(btState, Release);
      return;
    }
  }
}

// /SmartButton

// Меню
byte menuCurrent = 0;
#define MENU_OFF 0
#define MENU_ON  1
byte menuMode = MENU_OFF;

class modeSmartButton: public SmartButton {
  public:
  modeSmartButton(int p) : SmartButton(p) {}
    virtual void onClick();
    virtual void offClick();
};

void modeSmartButton::onClick() {
  Serial.println("MODE ON CLICK");
  if (menuMode==MENU_OFF) {
    menuMode=MENU_ON;
    printAt(0,1,"MENU MODE");
  }
  else {
    menuMode=MENU_OFF;
    printAt(0,1,"         ");
  }
}
void modeSmartButton::offClick() {
  Serial.println("MODE OFF CLICK");
}

// Test object

#define DNORM   0
#define DBLACK  1
#define DWHITE  2

class Counter {
  private:
    unsigned int counter;
    byte col, row;
    byte format;
    SmartDelay del;
  public:
    Counter();
    ~Counter();
    Counter(unsigned int cnt, byte col, byte row, byte fmt, unsigned long p);
    unsigned int inc();
    unsigned int inc(unsigned int increment);
    unsigned int dec(unsigned int decrement);
    unsigned int get();
    unsigned int set(unsigned int newval);
    void display(byte mode);
    void run();
};
Counter::Counter() {};
Counter::~Counter() {};
Counter::Counter(unsigned int cnt, byte c, byte r, byte fmt, unsigned long p) {
  counter = cnt;
  col = c;
  row = r;
  format = fmt;
  del.Set(p);
}
unsigned int Counter::inc() {
  inc(1);
}
unsigned int Counter::inc(unsigned int i) {
  counter += i;
  if (counter >= 1000) counter = 0;
  return counter;
}
void Counter::display(byte mode) {
  char fmt[21];
  if (mode == DNORM) {
    sprintf(fmt, "%%%dd", format);
    printAt(col, row, fmt, counter);
  }
  if (mode == DWHITE) {
    sprintf(fmt, "%%%ds", format);
    printAt(col, row, fmt, " ");
  }
  if (mode == DBLACK) {
    for (int i = 0; i < format; i++) {
      fmt[i] = '\xFF';
    }
    fmt[format] = '\0';
    printAt(col, row, fmt);
  }
}
void Counter::run() {
  if (del.Now()) display(DNORM);
}

Counter c1(123, 0, 0, 3, 100000 * 15);
Counter c2(123, 4, 0, 3, 100000 * 11);
Counter c3(123, 8, 0, 3, 100000 * 12);
Counter c4(123, 12, 0, 3, 100000 * 13);

SmartDelay d1(100000 * 2);
SmartDelay d2(100000 * 27);
SmartDelay d3(100000 * 7);
SmartDelay d4(100000 * 15);

modeSmartButton btMode(6);
SmartButton btUp(7);
SmartButton btDn(8);
SmartButton btOk(9);

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setBacklight(200);
  lcd.noCursor();
  lcd.home();
  lcd.clear();
  printAt(0, 0, "%16s", "DISPLAY OK");
  delay(1000);
  lcd.clear();
  Serial.println("Ready");
}

void loop() {
  if (d1.Now()) c1.inc();
  if (d2.Now()) c2.inc();
  if (d3.Now()) c3.inc();
  if (d4.Now()) c4.inc();
  c1.run();
  c2.run();
  c3.run();
  c4.run();
  btMode.run();
  //  btUp.run();
  //  btDn.run();
  //  btOk.run();
  delay(100);
}
