#ifndef SMART_BUTTON_H
#define SMART_BUTTON_H

#if ARDUINO >= 100
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif

// User can define his own timings
#ifndef SmartButton_debounce
#define SmartButton_debounce 10
#endif
#ifndef SmartButton_hold
#define SmartButton_hold 1000
#endif
#ifndef SmartButton_long
#define SmartButton_long 5000
#endif
#ifndef SmartButton_idle
#define SmartButton_idle 10000
#endif

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
    void DoAction(enum state st, enum input in);
    void ToPreClick(enum state st, enum input in);
    void ToIdle(enum state st, enum input in);
    void ToForcedIdle(enum state st, enum input in);
    void ToClick(enum state st, enum input in);
    void ToHold(enum state st, enum input in);
    void ToLongHold(enum state st, enum input in);
  public:
    SmartButton();
    SmartButton(int pin);
    ~SmartButton();
    void run();
    // Methods to redefine by user.
    inline virtual void onClick() {};       // On click.
    inline virtual void onHold() {};        // On hold.
    inline virtual void onLongHold() {};    // On long hold.
    inline virtual void onIdle() {};        // On timeout with too long key pressing.
    inline virtual void offClick() {};      // On depress after click.
    inline virtual void offHold() {};       // On depress after hold.
    inline virtual void offLongHold() {};   // On depress after long hold.
    inline virtual void offIdle() {};       // On depress after too long key pressing.
};

#endif
