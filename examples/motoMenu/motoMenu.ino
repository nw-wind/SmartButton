#include "SmartButton.h"

template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; }
#define endl "\n"

enum Mode {modeGauges=0, modeOnTheWay, modeSport, totalModes};

enum Button {Mode=0, Up, Down, OK};

class BtGeneral : SmartButton {
private:
  enum Button button;
public:
  BtGeneral(int pin) : SmartButton (pin) {}
  BtGeneral(int pin, enum Button mode) : SmartButton (pin) {button=mode;}
  void onClick() {
    bg.click(button);
  }
  void onHold() {
    bg.hold(button);
  }
  void onLongHold() {
    bg.longhold(button);
  }
}

class BtGauges {
  private:
    enum input {Idle=0, modeClick, okClick, okHold, okLongHold, upClick, downClick, speedOn, speedOff, totalInputs};
    enum state {dispNorm=0,dispSpeed, dispSport, dispMenu, totalStates};
    typedef void (BtGauges::*FSM)(enum state st, enum input in);
    enum state btState = Idle;
    enum input btInput = Release;
    FSM action[totalStates][totalINputs] = {
      {&BtGauges::toIdle,&BtGauges::toDispToggle,NULL,&BtGauges::toDispMenu,&BtGauges::toOdoDrop,&BtGauges::toOdoChange,&BtGauges::toOdoChange,&BtGauges::toDispGo,NULL},
      {&BtGauges::toIdle,NULL,NULL,NULL,NULL,NULL,NULL,NULL,&BtGauges::toDispStay},
      {NULL,&BtGauges::toDispToggle,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
      {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL}
    };

    void doAction(enum state st, enum input in) {      
      if (action[st][in] == NULL) {
        Serial.println("NULL action");
        return;
      }
      Serial.print("Do action "); Serial.print(btPin); Serial.print(" ");
      Serial.print(st); Serial.print(" "); Serial.println(in);
      (this->*(action[st][in]))(st, in);
    }

    void toIdle(enum state st, enum input in){Serial << "toIdle " << st << " " in << endl}
    void toDispToggle(enum state st, enum input in){Serial << "toDispToggle " << st << " " in << endl}
    void toDispMenu(enum state st, enum input in){Serial << "toDispMenu " << st << " " in << endl}
    void toOdoDrop(enum state st, enum input in){Serial << "toOdoDrop " << st << " " in << endl}
    void toOdoChange(enum state st, enum input in){Serial << "toOdoChange " << st << " " in << endl}
    void toDispGo(enum state st, enum input in){Serial << "toDispGo " << st << " " in << endl}
    void toDispStay(enum state st, enum input in){Serial << "toDispStay " << st << " " in << endl}
    //void toIdle(enum state st, enum input in){Serial << "Idle " << st << " " in << endl}
    //void toIdle(enum state st, enum input in){Serial << "Idle " << st << " " in << endl}

  public:
    void click(enum Button bt) {
      Serial << "Button " << bt << " clicked" << endl;
      switch(button) {
        case Mode:
          doAction(btState,modeClick);
          break;
        case Up:
          doAction(btState,upClick);
          break;
        case Down:
          doAction(btState,downClick);
          break;
        case OK:
          doAction(btState,okClick);
          break;
      }
    }
    void hold(enum Button bt) {
      Serial << "Button " << bt << " clicked" << endl;
    }
    void longhold(enum Button bt) {
      Serial << "Button " << bt << " clicked" << endl;
    }

    void speedOn() {
      doAction(btState,speedOn);
    }
    void speedOff() {
      doAction(btState,speedOff);
    }

};

btGeneral btMode(12,Mode);
btGeneral btUp(11,Up);
btGeneral btDown(10,Down);
btGeneral btOK(09,OK);

btGauges bg;

void setup() {
  Serial.begin(9600);
  Serial << "Ready" << endl;
}

unsigned long velo;

SmartDelay checkSpeed(1000000UL);
SmartDelay checkDisp(1100000UL);


void loop() {
  btMode.run();
  btUp.run();
  btDown.run();
  btOK.run();
  if (checkSpeed.Now()) {
    if (digitalRead(2)) bg.speedOn();
    else bg.speedOff();
  }
  if (checkDisp.Now()) {
    Serial << "Display" << endl;
  }
}
