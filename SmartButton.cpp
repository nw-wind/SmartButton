

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
// Private
void SmartButton::DoAction(enum state st, enum input in) {
  if (action[st][in] == NULL) {
    //Serial.println("NULL action");
    return;
  }
  //Serial.print("Do action "); Serial.print(btPin); Serial.print(" ");
  //Serial.print(st); Serial.print(" "); Serial.println(in);
  (this->*(action[st][in]))(st, in);
}
void SmartButton::ToPreClick(enum state st, enum input in) {
  //Serial.println("Pre Click");
  pressTimeStamp = millis();
  btState = PreClick;
}
void SmartButton::ToIdle(enum state st, enum input in) {
  //Serial.print("IDLE "); Serial.print(btPin); Serial.print(" ");
  //Serial.print(st); Serial.print(" "); Serial.println(in);
  btState = Idle;
  switch (st) {
    case Click: offClick(); break;
    case Hold: offHold(); break;
    case LongHold: offLongHold(); break;
  }
}
void SmartButton::ToForcedIdle(enum state st, enum input in) {
  //Serial.print("Forced IDLE "); Serial.print(btPin); Serial.print(" ");
  //Serial.print(st); Serial.print(" "); Serial.println(in);
  btState = ForcedIdle;
}
void SmartButton::ToClick(enum state st, enum input in) {
  //Serial.println("Click");
  btState = Click;
  onClick();
}
void SmartButton::ToHold(enum state st, enum input in) {
  //Serial.println("Hold");
  btState = Hold;
  onHold();
}
void SmartButton::ToLongHold(enum state st, enum input in) {
  //Serial.println("LongHold");
  btState = LongHold;
  onLongHold();
}
// Public
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

