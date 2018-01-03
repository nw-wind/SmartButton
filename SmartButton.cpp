#include <SmartButton.h>

// SmartButton

SmartButton::SmartButton() {}
SmartButton::~SmartButton() {}
SmartButton::SmartButton(int pin) {
  btPin = pin;
  pinMode(pin, INPUT_PULLUP);
}
// Private
void SmartButton::DoAction(enum input in) {
  /*
  static enum state ost=stDebug;
  static enum input oin=inDebug;
  if (ost!=st || oin!=in) {
    Serial.print("DO "); Serial.print(st); Serial.print(" "); Serial.println(in);
    ost=st; oin=in;
  }
  */
  enum state st=btState;
  switch (in) {
    case input::Release:
      //Serial.println("Release");
      btState=state::Idle;
      switch (st) {
        case state::Click: 
          offClick(); 
          break;
        case state::Hold: 
          offHold(); 
          break;
        case state::LongHold: 
          offLongHold(); 
          break;
        case state::ForcedIdle: 
          onIdle(); 
          break;
      }
      break;
    case input::WaitDebounce:
      switch (st) {
        case state::PreClick:
          //Serial.println("Click");
          btState=state::Click;
          onClick();
          break;
      }
      break;
    case input::WaitHold:
      switch (st) {
        case state::Click:
          //Serial.println("Hold");
          btState=state::Hold;
          onHold();
          break;
      }
      break;
    case input::WaitLongHold:
      switch (st) {
        case state::Hold:
          //Serial.println("Long");
          btState=state::LongHold;
          onLongHold();
          break;
      }
      break;
    case input::WaitIdle:
      switch (st) {
        case state::LongHold:
          //Serial.println("ForcedIdle");
          btState=state::ForcedIdle;
          break;
      }
      break;
    case input::Press:
      switch (st) {
        case state::Idle:
          //Serial.println("Press");
          pressTimeStamp=millis();
          btState=state::PreClick;
          break;
      }
      break;
  }
}

// Public
void SmartButton::run() {
  unsigned long mls = millis();
  //Serial.print("Run "); Serial.println(btState);
  if (!digitalRead(btPin))  DoAction(input::Press);
  else  DoAction(input::Release);
  if (mls - pressTimeStamp > SmartButton_debounce) DoAction(input::WaitDebounce);
  if (mls - pressTimeStamp > SmartButton_hold) DoAction(input::WaitHold);
  if (mls - pressTimeStamp > SmartButton_long) DoAction(input::WaitLongHold);
  if (mls - pressTimeStamp > SmartButton_idle) DoAction(input::WaitIdle);
}

