template<class T> inline Print &operator <<(Print &obj, T arg) {
  obj.print(arg);
  return obj;
}
#define endl "\n"

#include <SmartButton.h>


#define MENU_OFF 0
#define MENU_ON  1
byte menuMode = MENU_OFF;

// New class based on Smartbutton
class modeSmartButton: public SmartButton {
  public:
    modeSmartButton(int p) : SmartButton(p) {}
    modeSmartButton(int p, int m) : SmartButton(p,m) {}
    virtual void begin(int p, int m) {
      SmartButton::begin(p, m);
    }
    virtual void onClick();		// Methods to redefine
    virtual void offClick();	// You will use only these 2 methods
};

// The action on the button click. Toggle "menu mode".
void modeSmartButton::onClick() {
  Serial.println("Key pressed.");
  if (menuMode == MENU_OFF) {
    menuMode = MENU_ON;
    Serial.println("Menu mode on.");
  } else {
    menuMode = MENU_OFF;
    Serial.println("Menu mode off.");
  }
}

// The action on the button release after click. Nothing.
void modeSmartButton::offClick() {
  Serial.println("Key depressed.");
}

// The object. Pin 6 used as input.
modeSmartButton btMode(6,INPUT_PULLUP);

void setup() {
  Serial.begin(115200);
  Serial.println("Ready");
  //btMode.begin(6, INPUT_PULLUP);
}

void loop() {
  btMode.run(); // Just put into loop().
  //delay(100);
}
