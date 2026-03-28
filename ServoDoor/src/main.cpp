#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define RedLed 6
#define BlueLed 7

// Create MFRC522 instance.
#define RST_PIN         9         
#define SS_PIN          10         
MFRC522 mfrc522(SS_PIN, RST_PIN); 

enum state {CorrectUid, WrongUid, DoorOpened};

Servo doorServo;

String UidContent();
void DoorStatus(state currentState);
void CloseDoor();
void OpenDoor();
void WaitForScannedCard();


state doorState;

long msStart = 0;
long msCurrent = 0;
long cardClock = 1000;
long doorClock = 3000;
unsigned long lastScanned;

bool cardIsOn = false;

void setup() {
	Serial.begin(9600);		
	while (!Serial);		
	SPI.begin();			// Init SPI bus

	pinMode(RedLed, OUTPUT);
	pinMode(BlueLed, OUTPUT);

  doorServo.attach(A1);
	CloseDoor();
	digitalWrite(RedLed, LOW);
	digitalWrite(BlueLed, LOW);

	mfrc522.PCD_Init();		// Init MFRC522
	mfrc522.PCD_DumpVersionToSerial();	// Show details of PCD - MFRC522 Card Reader details
	Serial.println(F("Hold card to the RFID reader"));
	Serial.println("");
}

void loop() {
  msStart = millis();

  WaitForScannedCard();
  
  if (cardIsOn == false && msStart - msCurrent > doorClock)
  {
    doorState = DoorOpened;
  }
  else if (cardIsOn){
    //comparing the contents
    if (UidContent().substring(1) == "01 02 03 04") {
      doorState = CorrectUid;
      msCurrent = msStart;
    }
    else {
      doorState = WrongUid;
    }
  }

  DoorStatus(doorState);
}
void WaitForScannedCard() {
  // Look for new cards
	if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()){
    lastScanned = msStart;
    if (cardIsOn == false){
      Serial.println("card detected");
      cardIsOn = true;
    }
    // Halt PICC
    mfrc522.PICC_HaltA();

  }
  else if (cardIsOn && (msStart - lastScanned > cardClock)){
    cardIsOn = false;
    Serial.println("card left");
  }


}

// To set the state of the door and the led lights
void DoorStatus(state currentState){
  switch (currentState)
  {
  case CorrectUid:
    OpenDoor();
    digitalWrite(RedLed, LOW);
	  digitalWrite(BlueLed, HIGH);
    break;

  case WrongUid:
    CloseDoor();
    digitalWrite(RedLed, HIGH);
	  digitalWrite(BlueLed, LOW);
    break;

  case DoorOpened:
    CloseDoor();
    digitalWrite(RedLed, LOW);
	  digitalWrite(BlueLed, LOW);
    break;
  
  default:
    break;
  }

};

String UidContent(){ //saving the contents of the uid, to be compair
  String Uid = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Uid.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    Uid.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  return Uid;
}

void CloseDoor(){doorServo.write(0);}  // To move servo to close door
void OpenDoor(){doorServo.write(110);} // To move servo to open door