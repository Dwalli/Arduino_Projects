#define redLED  4
#define orangeLED 3
#define greenLED 2
#define blueLED 8


#define button 7


unsigned long msStart = 0;
unsigned long MsPrevious = 0;


// repercent which LED is on green orange red or if the button is pressed
enum TrafficState {
    RED,
    ORANGE,
    GREEN,
    BLUE,
    BUTTEONPRESS
};


bool ButtonPressed = false;


TrafficState state = GREEN;


// The long of time the traffic light will wait
const int threeSeconds = 3000;
const int oneSeconds = 1000;


bool IsButtonPressed() {
    return digitalRead(button) == HIGH;
}






// Function to change the state of the LED when given true
void TurnLedOn(int Pin, bool State) {
    digitalWrite(Pin, State);
}




void setLight(TrafficState LEDState) {


    MsPrevious = msStart;
    state = LEDState;
    // when LEDstate change to ether Red orange or green it will determen which LED will turn on.
    switch (LEDState) {
    case GREEN:
        TurnLedOn(greenLED, true);
        TurnLedOn(redLED, false);
        TurnLedOn(orangeLED, false);
        TurnLedOn(blueLED, false);
        break;


    case ORANGE:
        TurnLedOn(greenLED, false);
        TurnLedOn(redLED, false);
        TurnLedOn(orangeLED, true);
        break;


    case RED:
        TurnLedOn(greenLED, false);
        TurnLedOn(redLED, true);
        TurnLedOn(orangeLED, false);
        break;
    case BLUE:
        TurnLedOn(blueLED, true);
        TurnLedOn(greenLED, false);
        TurnLedOn(redLED, true);
        TurnLedOn(orangeLED, false);
        break;
    }


}


void buttonSwitchLight() { // To turn on which LED on
    long elaps = msStart - MsPrevious;
    switch (state) {


    case GREEN:
        if (elaps >= oneSeconds) setLight(ORANGE);
        break;


    case ORANGE:
        if (elaps >= oneSeconds) {
            setLight(BLUE);
        }
        break;


    case BLUE:
        if (elaps >= threeSeconds) {
            setLight(GREEN);
            ButtonPressed = false;
        }
        break;


    case RED: // To make it loop back to green so ble LED can turn on
        if (elaps >= oneSeconds) setLight(GREEN);
        break;
    }


}


void switchLight() { // To turn on which LED on
    long elaps = msStart - MsPrevious;
    switch (state) {


    case GREEN:
        if (elaps >= oneSeconds) setLight(ORANGE);
        break;


    case ORANGE:
        if (elaps >= oneSeconds) setLight(RED);
        break;


    case RED:
        if (elaps >= oneSeconds) setLight(GREEN);
        break;
    }


}


void setup() {
    Serial.begin(9600);


    pinMode(redLED, OUTPUT);
    pinMode(orangeLED, OUTPUT);
    pinMode(greenLED, OUTPUT);
    pinMode(blueLED, OUTPUT);


    pinMode(button, INPUT);




    setLight(state); //set the fist LED on.


}


void loop() {
    msStart = millis();

    if (IsButtonPressed() && ButtonPressed == false) {
        ButtonPressed = true;
    }


    if (ButtonPressed == true) buttonSwitchLight();
    else switchLight();


}


