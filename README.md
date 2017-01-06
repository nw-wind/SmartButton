# SmartButton

Extendable class to handle buttons and make own behavior. 

Required: SmartDelay (https://github.com/nw-wind/SmartDelay)

The idea is to make a class which you can not use as is but you should extend with your own functionality.

The base class SmartButton has private methods only. All public are virtual, empty and should be overloaded to use.

    // Methods to redefine by user.
    inline virtual void onClick() {};       // On click.
    inline virtual void onHold() {};        // On hold.
    inline virtual void onLongHold() {};    // On long hold.
    inline virtual void onIdle() {};        // On timeout with too long key pressing.
    inline virtual void offClick() {};      // On depress after click.
    inline virtual void offHold() {};       // On depress after hold.
    inline virtual void offLongHold() {};   // On depress after long hold.
    inline virtual void offIdle() {};       // On depress after too long key pressing.
    
These methods are "events" called by the base class. You can overload methods you need only. For example, to take only clicks just define your method onClick() and put your code there.

You can redefine timings also.

    // Debounce time where the button has been pressed but is not Click event.
    #define SmartButton_debounce 10
    // The hold time after pressing the button we have Hold event.
    #define SmartButton_hold 1000
    // The long hold time for LongHold event.
    #define SmartButton_long 5000
    // The idle time after that the key is pressed too long time and become have no value as an error.
    #define SmartButton_idle 10000
    
Please dont forget to create the constructor of the new class by this template:

    class toggleSmartButton: public SmartButton {
      public:
        toggleSmartButton(int p) : SmartButton(p) {}  // The constructor. I hope I will make a template for it soon.
        virtual void onClick();		// Methods to redefine are here.
    };
    
    // Make the object. We have a simple button connected to the digital pin 12.
    toggleSmartButton bt(12);
    
    byte myToggle=0;
    
    // This method toggles variable myToggle byt click.
    void modeSmartButton::onClick() {
    	if (myToggle) {
	      Serial.println("ON");
	    } else {
	      Serial.println("OFF");
	    }
      myToggle=!myToggle;
    }
    
Now please put anywhere in the main loop the run() call. Note! You should not use delay() function nowhere in your code because it blocks the execution of the programm. Use SmartDelay (https://github.com/nw-wind/SmartDelay) instead. 
    
    void loop() {
      bt.run(); // Just put into loop().
    }
    
I'm sorry, now the digital port set as INPUT_PULLUP by default. Be sure you switch your button to the ground clicking.
    
Todo:
- Double click.
- Triple click.
- Pass pin modes in constructor.
- Add begin(pin,mode) method.
