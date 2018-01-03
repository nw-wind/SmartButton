#include <SmartButton.h>

// SmartButton

SmartButton::SmartButton() {
  btState = state::Idle;
}
SmartButton::~SmartButton() {}
SmartButton::SmartButton(int pin) {
  btState = state::Idle;
  btPin = pin;
  pinMode(pin, INPUT_PULLUP);
}
// Private
void SmartButton::DoAction(input in) {
  switch (in) {
    case input::Release:
      //Serial.println("Release");
      btState=state::Idle;
      switch (btState) {
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
      switch (btState) {
        case state::PreClick:
          //Serial.println("Click");
          btState=state::Click;
          onClick();
          break;
      }
      break;
    case input::WaitHold:
      switch (btState) {
        case state::Click:
          //Serial.println("Hold");
          btState=state::Hold;
          onHold();
          break;
      }
      break;
    case input::WaitLongHold:
      switch (btState) {
        case state::Hold:
          //Serial.println("Long");
          btState=state::LongHold;
          onLongHold();
          break;
      }
      break;
    case input::WaitIdle:
      switch (btState) {
        case state::LongHold:
          //Serial.println("ForcedIdle");
          btState=state::ForcedIdle;
          break;
      }
      break;
    case input::Press:
      switch (btState) {
        case state::Idle:
          //Serial.println("Press");
          pressTimeStamp=0xFFFF & millis();
          btState=state::PreClick;
          break;
      }
      break;
  }
}

// Public
void SmartButton::run() {
  unsigned int mls = 0xFFFF & millis();
  if (!digitalRead(btPin))  DoAction(input::Press);
  else  DoAction(input::Release);
  if (mls - pressTimeStamp > SmartButton_debounce) DoAction(input::WaitDebounce);
  if (mls - pressTimeStamp > SmartButton_hold) DoAction(input::WaitHold);
  if (mls - pressTimeStamp > SmartButton_long) DoAction(input::WaitLongHold);
  if (mls - pressTimeStamp > SmartButton_idle) DoAction(input::WaitIdle);
}

