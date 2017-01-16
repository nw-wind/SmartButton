#include "SmartButton.h"

enum mode {modeGauges=0, modeOnTheWay, modeSport, totalModes};

class BtGauges : public SmartButton {
  private:
    enum input {Idle=0, modeClick, okClick, okHold, okLongHold, upClick, downClick, speedOn, totalInputs};
    enum state {dispNorm=0,dispSpeed,dispSport,totalStates};
    typedef void (BtGauges::*FSM)(enum state st, enum input in);
    enum state btState = Idle;
    enum input btInput = Release;
    FSM action[totalStates][totalINputs] = {
      {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
      {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
      {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL}
    };
};