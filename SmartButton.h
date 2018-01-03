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
    enum class state : byte  {Idle, PreClick, Click, Hold, LongHold, ForcedIdle};
    enum class input : byte {Release, WaitDebounce, WaitHold, WaitLongHold, WaitIdle, Press};
    state btState;
    unsigned int pressTimeStamp;

  private:
    void DoAction(input in);
    
  public:
    SmartButton();
    SmartButton(int pin);
    SmartButton(int pin, int mode) {btPin=pin; pinMode(pin,mode);}
    ~SmartButton();
    void begin(int p, int m) {btPin=p; pinMode(p,m);}
    void run();
    
  // Methods to redefine by user.
  public:
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
