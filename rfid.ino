#include <Wire.h>
#include <SoftwareSerial.h>
SoftwareSerial RFID(2, 3);
SoftwareSerial  BTSerial(5, 6); //RX TX

// 3 parallel arrays
String names[5] = {"chips", "mint", "nuts", "apple", "juice"};
String items[5] = {"6A0049C6D8","6A0049C655", "6A0049EFC4", "6F007F61BC", "6A003E3389"};
int location[5] = {1, 1, 1, 1, 0};

boolean notSent;
int notFirst = 0;

void setup() {
  BTSerial.begin(9600); // bluetooth communication
  RFID.begin(9600); // rfid sensor
  Wire.begin(); // serial communication between arduinos
  Serial.begin(9600);
}

byte x = 0;

void loop() {

  // send fridges contents right away
  while (notFirst == 0) {
    firstBluetooth();
    }

  // read RFID tag
  char tagString[11];
  int index = 0;
  String tagnum = "";
  String tagnum2 = "";

  while (RFID.available() > 0) {

    int readByte = RFID.read(); //read next available byte

    if (index <= 10) {
      tagString[index] = readByte;
      index ++;
    }

  }

  tagnum = tagString;
  tagnum2 = tagnum.substring(1, 11);
  if (tagnum2 != "") {
    Serial.print(tagnum2 + " ");
    changeLocation(tagnum2); // update location of item
    resetReader();
  }

  clearTag(tagString); //Clear the char of all value
  resetReader();

  bluetooth(); // start Bluetooth communication

  search(); // check if item is in fridge via serial monitor

}

void resetReader() {
  delay(150);
}

void clearTag(char one[]) {
  // clear the char array by filling with null 
  // Will think same tag has been read otherwise
  for (int i = 0; i <= strlen(one); i++) {
    one[i] = 0;
  }
}

// updates location of fridge
void changeLocation(String tag) {
  for (int j = 0; j < sizeof(location); j++) {
    if (items[j] == tag) {
      if (location[j] == 1) {
        location[j] = 0;
        Serial.println(names[j] + " out of fridge " + "(" + location[j] + ")");
        notSent = true;

        // Serial communication
        Wire.beginTransmission(8); // transmit to other arduino
        Wire.write(j);
        Wire.write(0);// sends one byte
        Serial.println("sent success");
        delay(500);
        Wire.endTransmission(); // stop transmitting
        delay(500);
      }
      else {
        location[j] = 1;
        Serial.println(names[j] + " in fridge " + "(" + location[j] + ")");
        notSent = true;
        // Serial communication - if item is put back into fridge 
        Wire.beginTransmission(8); // transmit to device #8
        Wire.write(j); 
        Wire.write(1); // sends one byte
        Serial.println("sent success");
        delay(500);
        Wire.endTransmission();    // stop transmitting
        delay(500);
      }
    }
  }
}

void bluetooth() {
  if (notSent) {
    for (int j = 0; j < sizeof(names); j++) {
      if (location[j] == 1) {
        BTSerial.print(j);
        Serial.println(j);
      }
    }
    notSent = false;
  }
}

// display contents of fridge without location change
void firstBluetooth() {
  delay(1000);
  if (notFirst == 0) {
    for (int j = 0; j < sizeof(names); j++) {
      if (location[j] == 1) {
        BTSerial.print(j);
        Serial.println(j);
      }
    }
    notFirst = 1;
  }
}

// search for item to see if it's in the fridge 
// via serial monitor
void search() {
  String item;
  if (Serial.available()) {
    item = Serial.readString();

    for (int j = 0; j < sizeof(names); j++) {
      if (item == names[j]) {
        if (location[j] == 1) {
          Serial.println(item + " is in fridge");
        }
        else Serial.println(item + " is not in fridge");
      }
    }
  }
}