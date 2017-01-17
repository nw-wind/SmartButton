#include <SmartButton.h>

// SmartButton

SmartButton::SmartButton() {}
SmartButton::~SmartButton() {}
SmartButton::SmartButton(int pin) {
  btPin = pin;
  pinMode(pin, INPUT_PULLUP);
}
// Private
void SmartButton::DoAction(enum state st, enum input in) {
  /*
  static enum state ost=stDebug;
  static enum input oin=inDebug;
  if (ost!=st || oin!=in) {
    Serial.print("DO "); Serial.print(st); Serial.print(" "); Serial.println(in);
    ost=st; oin=in;
  }
  */
  switch (in) {
    case Release:
      //Serial.println("Release");
      btState=Idle;
      switch (st) {
        case Click: 
          offClick(); 
          break;
        case Hold: 
          offHold(); 
          break;
        case LongHold: 
          offLongHold(); 
          break;
        case ForcedIdle: 
          onIdle(); 
          break;
      }
      break;
    case WaitDebounce:
      switch (st) {
        case PreClick:
          //Serial.println("Click");
          btState=Click;
          onClick();
          break;
      }
      break;
    case WaitHold:
      switch (st) {
        case Click:
          //Serial.println("Hold");
          btState=Hold;
          onHold();
          break;
      }
      break;
    case WaitLongHold:
      switch (st) {
        case Hold:
          //Serial.println("Long");
          btState=LongHold;
          onLongHold();
          break;
      }
      break;
    case WaitIdle:
      switch (st) {
        case LongHold:
          //Serial.println("ForcedIdle");
          btState=ForcedIdle;
          break;
      }
      break;
    case Press:
      switch (st) {
        case Idle:
          //Serial.println("Press");
          pressTimeStamp=millis();
          btState=PreClick;
          break;
      }
      break;
  }
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

