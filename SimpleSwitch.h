#ifndef SIMPLE_SWITCH_H_
#define SIMPLE_SWITCH_H_


#include "Arduino.h"

// =============================================================================
// SimpleSwitch
// V. 1.0
// 2014 Carlos Eduardo Mello
//
// V. 1.1
// 2017 Carlos Eduardo Mello
// =============================================================================
// This class manages all house keeping (tracking state, debouncing, etc.) for
// a switch to be used in an Arduino project. It was originaly meant for use with
// Footswitches in my own projects, but can be applied to any simple electro-mechanical 
// momentary switch. The class implements software-side toggle action, so we can use 
// simple momentary switches as on/off or even multi state "circular" switches.

// Each instance of the SimpleSwitch class controls a single physical switch,
// which should be connected to one of the digital i/o pins.The switch can 
// be of 3 types: Momentary, Toggle or Circular as explained bellow:

// =============================================================================
// USING SimpleSwitch
// =============================================================================

// In order to use SimpleSwitch, you just have to include this library in your sketch and
// declare a global instance of the class. The class constructor requires that you provide a 
// pin number, an action type, a circuit type and a debounce time for the switch. For  
// circular-action switches, the constructor requires one extra argument: the number of toggling 
// positions, which can be any number from 2 to 127. 

// SimpleSwitch <var_name>(<pin_number>,<action_type>,<circuit_type>,<debounce_time>,<num_of_positions>);

// After that you add a call to the debounce() method at the beginning of your scketch. 
// This call is mandatory in order to keep the switch working properly. The class automatically 
// accounts for the declared action type and circuit type when handling actions at debounce time.
// After debounce(), you typically call on(). If on() returns true, it means:

// - a momentary switch is currently turned ON
// - a toggle switch has been toggled ON
// - a circular switch has moved to the next position

// Here are a couple of examples of SimpleSwitch in use:
// Example 1 shows how to use a momentary switch.
// Example 2 shows a circular switch in action.
// (for more information read the section bellow the examples)

// EXAMPLE 1:

/*
#include <SimpleSwitch.h>

// ...

 SimpleSwitch button(4,MOMENTARY_ACTION,DEFAULT_OPEN_PULL_UP,0);

// ...

 void loop()
 {
	button.debounce();
	if(button.on())
	{
		// do your stuff here...
	}
	else
	{
		// or here...			
	}
}
 	
*/

// EXAMPLE 2:

/*
 #include <SimpleSwitch.h>
 
 // ...
 
 SimpleSwitch button(12,CIRCULAR_ACTION,DEFAULT_OPEN_PULL_UP,30,3);

 // ...

 void loop()
 {
	button.debounce()

	if(button.changedPosition())
	{
		//do something...
		switch(button.position())
		{
			case 0:
			//  here...
			break;

			case 1:
			// or here...
			break;

			case 2:
			// or here...
			break;
		}
	}

*/

// CONSTRUCTOR PARAMETERS:

// === Pin Number ===
//
// This parameter is used to inform SimpleSwitch which i/o pin the
// the button is connected to on the Arduino. This pin will be defined as an
// INPUT pin. (The other terminal on the button is normally connected to the Vcc
// pin of the board in use so that when closed it will apply the appropriate
// logic level to the refered pin. It is also a good idea to add a pull down resistor (10KΩ)
// between the input pin and ground, to avoid noise when the pin is unconnected)
// If the pin number is not define (the user employs the default constructor), this
// field is set to the DEFAULT_PIN_NUMBER.
const int DEFAULT_PIN_NUMBER = 2;

// === Switch Type Constants ===
//
// These constants define the action type for the switch:

// Momentary switches only maintain their states while being pressed. The
// on/off state of the switch is taken directly from switch condition, accounting
// for OPEN/CLOSED initial states (see Circuit Types bellow)
const int MOMENTARY_ACTION = 1;
// Toggle switches alternate between ON and OFF states when they are actuated. These
// states are held until another toggle occurs.
const int TOGGLE_ACTION = 2;
// Circular switches change to one of its possible states ("positions") each time their are toggled.
// Upon reaching the last possible state they return to the initial one. Hence their name.
const int CIRCULAR_ACTION = 3;

const int DEFAULT_ACTION_TYPE = MOMENTARY_ACTION;

// === Circuit Type Constants ===
//
// These constants define wether the switch is considered "ON" with a closed or open circuit.
// Footswitches such as the Edirol DP2 (Roland) use reverse action, meaning 
// their circuit is closed by default and open when pressed. Other commonly used
// pedals behave the opposite way. When declaring an instance of this class, user code
// needs to choose the type of circuit which fits the switch that will be used.
// The class accepts the choice of circuit type defined by the user at construction time.
// If the constructor receives an invalid value, it defaults to DEFAULT_CLOSED_PULL_UP.
// It is entirely up to the user to chose a circuit type that is compatible with
// the hardware in use...

// DEFAULT_OPEN and DEFAULT_CLOSE refer to the way the button operates: DEFAULT_OPEN buttons
// have their terminals normally separated and connected when the button is pushed.
// DEFAULT_CLOSED buttons work the opposite way, separating the terminals when pushed.
// PULL_UP and PULL_DOWN refer to the way the input pins are wired. To use the PULL_DOWN
// options, add a 10K resistor connecting the pin to GND. For PULL_UP the internal pullup
// resistor will be used, so there is no need for external resistors. Either way,
// the library will call pinMode() on the pin, so you don't have to.

const int DEFAULT_OPEN_PULL_DOWN = 0;
const int DEFAULT_CLOSED_PULL_DOWN = 1;
const int DEFAULT_OPEN_PULL_UP = 2;
const int DEFAULT_CLOSED_PULL_UP = 3;
const int DEFAULT_CIRCUIT_TYPE = DEFAULT_OPEN_PULL_UP;

// === Debounce Time ===

// The debounce time is the time required for an electro/mechanical switch to stop bouncing
// on/off after the circuit is closed opened. This parameter is taken as a millisecond value
// and typically should be set around 10 to 40 ms. The user should test with a medium value
// (say 20 ms) and modify it if the switch doesn't respond as expected. Caution: if the
// debounce time is too long, there may be interaction problems with the switch and other
// time-dependent code. If it is too short, there may not be enough time for the button to debounce
// and it may behave erratically. Obs.: this class's implementation does not rely on delay(), all debouncing code is done with software timers.

const int DEFAULT_DEBOUNCE_TIME = 20    ; //miliseconds

// === Number of Positions ===

// This paramenter is only applied to circular action switches. Other switch types simply
// ignore this value. The numberOfPositions parameter defines how many "positions" a circular switch
// can assume. This is typically used to implement things like multiple "modes" of operation
// in a sketch, which can be toggled through a button connected to one of the i/o pins. Each position
//  or state in the circular switch is identified by an integer which ranges from 0 (zero) to N-1, where 
// N = numberOfPositions. So, for instance, a switch with 4 states would have 0, 1, 2 and 3 as its
// positions. 

// User code must inform this parameter when choosing CIRCULAR_ACTION as action type
// The number provided must be a positive integer and greater than 2. If an invalid value 
// is provided here, the button's behavior will default to TOGGLE_ACTION (see above).

const int DEFAULT_NUMBER_OF_POSITIONS = 1;

// METHODS

// === debounce ===

// debounce() handles all internal logic for the button, including debouncing code and the various
// specific switch behaviours, depending on the type of switch. debounce() does not return anything,
// but it must be called once in the sketches's loop(), before any other calls can be made to instances
// of this class. After debounce(), one should normally call on(), position() or changedPosition().
// Read bellow to learn how to use these functions.

// === on ===

// on() determines if the button was actuated. As there are three types of switches there are also
// three possible behaviors for this call:

// MOMENTARY_ACTION: if the switch was configured for momentary action, on() returns true while the 
// button is pressed and false otherwise.

// TOGGLE_ACTION: if the switch was configured for toggle action, on() returns true after the first 
// press of the button and false upon a second press, and so on, alternating betwen ON and OFF.

// CIRCULAR_ACTION: circular switches act like momentary switches as far as on() is concerned, but on
// top of this basic behaviour, they can be used to choose between various options by calling position() 
// and changedPosition(). 

// == position ==

// position() returns the current position choice for a switch. It can be used to make the sketch
// "navigate" through various states using a simple switch. While the class does all the house keeping
// to keep track of these states, user code can use them to implement things like modes of operation
// or similar functionality in a sketch. This function call is only used by the circular switch type.
// position() can be called at any time in a sketch and it will allways return the current-most position
// seleted by the switch. If the user wants to fetch this information only once, right after the 
// change in position occurs, position() must be used in conjunction with changedPosition();

// === changedState ===

// changedState() returns true if the switch has just changed its state. At other times it allways returns false.*

// === wasToggled ===

// wasToggled() returns true if the switch has just toggled its state. At other times it allways returns false.*

// === changedPosition ===

// changedPosition() returns true if the switch has just changed its internal position. At other
// times it allways returns false.*

// OBS.: In all the descriptions above, the word "just" means that the call was made right after the
// button finished debouncing to a different state, as in the same turn of the main loop.

class SimpleSwitch
{
	// ================================================= [PRIVATE]
	private: 
	int _pin;
	int _actionType;
	int _circuitType;
	int _numOfPositions;
	int circuitOn;
	int circuitOff;
	int currState;        			// current state of the switch
	int lastState;    				// last state of the switch
	bool buttonState;				// toggled state of the switch (ON or OFF)
	int currPosition;				// current position of circular switch
    bool stateChanged;              // remembers if switch was "just" switched on or off
    bool was_toggled;               // remembers if switch was "just" toggled on or off
	bool changedPos;                // remembers if switch has "just" changed position
	int currReading;      			// current reading from foot switch pin (used for debouncing)
	int lastReading;    			// last reading from foot switch pin (used for debouncing)
	unsigned long lastDebounceTime; 			// last time input pin changed (milliseconds)
	unsigned long debounceDelay;  			// maximum debounce time  (milliseconds)
	
	
	// ================================================= [PUBLIC]
	public:
    SimpleSwitch(void);
	SimpleSwitch(int pin, int actionType, int circuitType, int debounceTime, int numberOfPositions);
    void Define(int pin, int actionType, int circuitType, int debounceTime, int numberOfPositions);
	void debounce(void);
	bool on(void);
	int position(void);
    bool changedState(void);
    bool wasToggled(void);
	bool changedPosition(void);
    void setPosition(int p);
    int pin(void);
};

#endif


