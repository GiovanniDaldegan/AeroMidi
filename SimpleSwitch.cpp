
// QUICK SOLUTION ALERT:
// BEFORE RUNNING ON ARDUINO IDE, FIX IT BACK TO <>
// #include <SimpleSwitch.h>
#include "SimpleSwitch.h"

SimpleSwitch::SimpleSwitch(void)
{
    // Initialize object with default settings...
    Define(2,MOMENTARY_ACTION,DEFAULT_CIRCUIT_TYPE,DEFAULT_DEBOUNCE_TIME,DEFAULT_NUMBER_OF_POSITIONS);
}

SimpleSwitch::SimpleSwitch(int pin, int actionType, int circuitType, int debounceTime, int numberOfPositions)
{
    // Initialize object with user provided settings...
    Define(pin,actionType,circuitType,debounceTime,numberOfPositions);
}

int SimpleSwitch::pin(void)
{
    return _pin;
}

void SimpleSwitch::Define(int pin, int actionType, int circuitType, int debounceTime, int numberOfPositions)
{
	_pin = pin;
	_actionType = actionType;
	_circuitType = circuitType;
	_numOfPositions = numberOfPositions;
	
	// Action defaults to Momentary
	if(_actionType < MOMENTARY_ACTION || _actionType > CIRCULAR_ACTION)
		_actionType = MOMENTARY_ACTION;
	
	// Circuit type defaults to closed
	if(_circuitType < DEFAULT_OPEN_PULL_DOWN || _circuitType > DEFAULT_CLOSED_PULL_UP)
		_circuitType = DEFAULT_CLOSED_PULL_UP;
		
	if(_circuitType == DEFAULT_OPEN_PULL_DOWN)
    {
        circuitOn = HIGH;
		circuitOff = LOW;
        pinMode(_pin, INPUT);
	}
	
	if(_circuitType == DEFAULT_CLOSED_PULL_DOWN)
	{
        circuitOn = LOW;
		circuitOff = HIGH;
        pinMode(_pin,INPUT);
	}
    
    if(_circuitType == DEFAULT_OPEN_PULL_UP)
    {
        circuitOn = LOW;
        circuitOff = HIGH;
        pinMode(_pin, INPUT_PULLUP);
    }
    
    if(_circuitType == DEFAULT_CLOSED_PULL_UP)
    {
        circuitOn = HIGH;
        circuitOff = LOW;
        pinMode(_pin, INPUT_PULLUP);
    }

	currState = circuitOff;        	// current state of foot switch
	lastState = circuitOff;    		// last state of the foot switch
	currReading = circuitOff;      	// current reading from foot switch pin (used for debouncing)
	lastReading = circuitOff;    	// last reading from foot switch pin (used for debouncing)
	lastDebounceTime = 0; 			// last time input pin changed (milliseconds)
	debounceDelay = debounceTime;  	// maximum debounce time  (milliseconds)
	currPosition = 0;				// start at first position
    stateChanged = false;
    was_toggled = false;
	changedPos = false;		// used to flag position changes in circular switches
	buttonState = false;
}

void SimpleSwitch::debounce(void)
{
	// check switch state...
     currReading = digitalRead(_pin);
    
    
    // FIX FOR DOUBLE HIT ON TOGGLE_ACTION (MAYBE CIRCULAR_ACTION TOO)
    // Test and see if it should stay...
    // The reason is that these states should only
    // be valid immediatly after a successful debounce
    // The way it is set right now, they only get reset if
    // the switch debounces again, which can lead to double
    // hits if calling code relies on wasToggled() or changedPos()
    was_toggled = false;
    changedPos = false; //(?) not sure about this one...
  
	  if (currReading != lastReading) 
	  {
		lastDebounceTime = millis();
	  }
	  
	  lastReading = currReading;
	  
	  if ((millis() - lastDebounceTime) > debounceDelay) 
	  {
	  		currState = currReading;
	
			switch(_actionType)
			{
				case MOMENTARY_ACTION:
				{
					if(currState == circuitOn)
                    {
						buttonState = true;			// THE BUTTON IS CURRENTLY PRESSED
                    }
					else
                    {
						buttonState = false;
                    }

                    if( currState != lastState )
                    {
                        stateChanged = true;
                    }
                   else
                   {
                       stateChanged = false;
                   }
                    
                    lastState = currState;
                    break;
				}
				
				case TOGGLE_ACTION:				// THE BUTTON WAS PRESSED AND WILL CHANGE STATE (ON/OFF)
				{
					if( (currState == circuitOn) && (lastState == circuitOff) )
					{ 
					  buttonState = !buttonState;
                      was_toggled = true;
					}
                    
                    if( ( currState == lastState ) ||
                       ( (currState == circuitOff) && (lastState == circuitOn) ))
                        was_toggled = false;

					lastState = currState;
					break;
				}
				
				case CIRCULAR_ACTION:		// THE BUTTON WAS PRESSED AND WILL SWITCH TO THE NEXT POSITION
				{
					if( (currState == circuitOn) && (lastState == circuitOff) )
					{ 
					   currPosition++;
					   if(currPosition >= _numOfPositions)
							currPosition = 0;
						buttonState = true;
						changedPos = true;
					}
					
					if( currState == lastState )
						changedPos = false;
					
					if( (currState == circuitOff) && (lastState == circuitOn) )
					{ 
						buttonState = false;
						changedPos = false;
					}
					
					lastState = currState;
					break;
				}
			}	
	  }
}

bool SimpleSwitch::on(void)
{
	// Button finished debouncing, so we can read it...
	return buttonState;
}

int SimpleSwitch::position(void)
{
	return currPosition;
}

bool SimpleSwitch::changedState(void)
{
    return stateChanged;
}

bool SimpleSwitch::wasToggled(void)
{
    return was_toggled;
}

bool SimpleSwitch::changedPosition(void)
{
	return changedPos;
}

void SimpleSwitch::setPosition(int p)
{
    if(p >= 0 && p < _numOfPositions)
        currPosition = p;
}

