#include <SmartButton.h>

// SmartButton

#define SmartButton_debounce 10
#define SmartButton_hold 1000
#define SmartButton_long 5000
#define SmartButton_idle 10000

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
    case WaitIdle: onIdle(); break;
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

