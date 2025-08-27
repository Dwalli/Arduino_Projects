int LedRed = 2;
int LedOrange = 3;
int LedGreen = 4;

unsigned long msStart = 0;
unsigned long redMsPrevious = 0;
unsigned long OrangeMsPrevious = 0;
unsigned long GreenMsPrevious = 0;

unsigned long greenStay = 2000;
unsigned long redStay = 2000;
unsigned long orangeSwitch = 1000;

bool redOnOrOff = false;
bool greenOnOrOff = false;
bool orangeOnOrOff = false;

int ROB = 1;


// To turn on a light by it's reperented colour
void greenLight(bool isOn = false) {
    if (isOn == false) digitalWrite(LedGreen, LOW);
    else digitalWrite(LedGreen, HIGH);
}
void redLight(bool isOn = false) {
    if (isOn == false) digitalWrite(LedRed, LOW);
    else digitalWrite(LedRed, HIGH);

}
void orangeLight(bool isOn = false) {
    if (isOn == false) digitalWrite(LedOrange, LOW);
    else digitalWrite(LedOrange, HIGH);
}

// when ther is in put it should preform this cycle
void defualtCaycle() {

    if (msStart - redMsPrevious > redStay && greenOnOrOff == false) {
        redMsPrevious = msStart;
        OrangeMsPrevious = msStart;
        GreenMsPrevious = msStart;
        redOnOrOff = false;
        orangeOnOrOff = true;
        greenOnOrOff = true;
        Serial.println("R");
        redLight(redOnOrOff);
        orangeLight(orangeOnOrOff);
    }
    if (msStart - OrangeMsPrevious > orangeSwitch && redOnOrOff == false) {
        redMsPrevious = msStart;
        OrangeMsPrevious = msStart;
        GreenMsPrevious = msStart;
        orangeOnOrOff = false;
        redOnOrOff = true;
        Serial.println("O");
        orangeLight(orangeOnOrOff);
        greenLight(greenOnOrOff);
    }
    if (msStart - GreenMsPrevious > greenStay && orangeOnOrOff == false) {
        redMsPrevious = msStart;
        OrangeMsPrevious = msStart;
        GreenMsPrevious = msStart;
        greenOnOrOff = false;
        Serial.println("G");
        greenLight(greenOnOrOff);
        redLight(redOnOrOff);
    }

}



void setup() {
    Serial.begin(9600);
    pinMode(LedRed, OUTPUT);
    pinMode(LedOrange, OUTPUT);
    pinMode(LedGreen, OUTPUT);

    redOnOrOff = true;
    redLight(redOnOrOff);
}

void loop() {
    msStart = millis();
    defualtCaycle();
}

