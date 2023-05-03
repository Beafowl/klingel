#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include <string.h>
#include <vector>
#include <string>
#include "keys.cpp"

#define RST_PIN 22
#define SS_PIN 21

#define OPEN_LOCK_PIN 12
#define REJECTED_PIN 14
#define RELAY_PIN 5

void openDrain(byte pin, bool value) 
{ if(value)
    pinMode(pin, INPUT);
  else
    pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}

MFRC522 mfrc522(SS_PIN, RST_PIN);



void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  
  pinMode(OPEN_LOCK_PIN, OUTPUT);
  pinMode(REJECTED_PIN, OUTPUT);
  // setting it to input to have high impedance
  pinMode(RELAY_PIN, INPUT);
  digitalWrite(RELAY_PIN, LOW);
}

void loop() {
  
  // check if a card is present    

  if (!(mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial())) {
    return;
  } else {
    Serial.println("Karte gefunden");
    mfrc522.MIFARE_UnbrickUidSector(false);

      // read out content

      String content = "";

      for (byte b=0; b<mfrc522.uid.size; b++) {

        content.concat(String(mfrc522.uid.uidByte[b] < 0x10 ? " 0" : " "));
        content.concat(String(mfrc522.uid.uidByte[b], HEX));
      }

      bool foundKey = (std::find(Keys::keys.begin(), Keys::keys.end(), content) != Keys::keys.end());

      if (foundKey) {

        digitalWrite(OPEN_LOCK_PIN, HIGH);
        //pinMode(RELAY_PIN, OUTPUT);
        //digitalWrite(RELAY_PIN, LOW);
        pinMode(RELAY_PIN, OUTPUT);

        delay(200);
        digitalWrite(OPEN_LOCK_PIN, LOW);
        pinMode(RELAY_PIN, INPUT);
        return;
      }

      if (!foundKey) {

        digitalWrite(REJECTED_PIN, HIGH);
        delay(1000);
        digitalWrite(REJECTED_PIN, LOW);
        return;
      }
  }
}
