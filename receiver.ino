#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
SoftwareSerial  BTSerial(2, 3); //RX TX
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

const int button = 13; //define button pin
int val = 0;
int oldVal = 0;
int backlight = 10; // define the backlight pin of LCD
//define item's nameLength and row number for printing purposes on the LCD screen
int nameLength = 0;
int row = 0;

// parallel arrays containing names of items and locations
String names[5] = {"chips", "mint", "nuts", "apple", "juice"};
int location[5] = {0, 0, 0, 0, 0}; //0 = out of fridge; 1 = in fridge
// array to translate ascii value, obtained via bluetooth, to decimal value 
int ascii[5] = {48, 49, 50, 51, 52};
int sensorValue;
boolean avail = BTSerial.available();

void setup() {
  BTSerial.begin(9600);
  pinMode(button, OUTPUT);
  pinMode(backlight, OUTPUT); // define the backlight pin as OUTPUT
  digitalWrite(backlight, HIGH); // set the backlight high
  lcd.begin(16, 2); // Print a message to the LCD.
  //printing welcome message to screen
  lcd.print("Welcome to IMF!");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Press me to see");
  delay(2000);
  lcd.setCursor(0, 1); //move cursor to next line
  lcd.println("what's in yo          ");
  delay(2000);
  lcd.clear();
  lcd.setCursor(5,1.5); //move cursor to middle of bottom line in screen
  lcd.print("FRIDGE!");
  delay(2000);
  lcd.clear();
  Serial.begin(9600); //begin serial communication
}

void loop() {
  val = digitalRead(button); //check if button pressed or not
  //Serial.println(val);
  //if button was just pressed right now
  if (val == HIGH && oldVal == LOW) {
//    Serial.println("in here");
    lcd.clear();
    // read information sent by master
    while (BTSerial.available()) {
      char index = BTSerial.read(); //reads index of items currently in fridge from master
      // iterate through array to match ascii value with index
      for (int i = 0; i < sizeof(names); i++) {
        if (ascii[i] == index) {
          // print name of item to LCD screen along the width of the screen.
          //if the list doesn't fit on one line, reset nameLength to 0 and set row = 1
          if (nameLength + names[i].length() > 15) {
            nameLength = 0;
            row = 1;
          }
          //move cursor to next line after the last print word
          lcd.setCursor(nameLength, row);
          //print item name followed by a space
          lcd.print(names[i] + " ");
          //update nameLength everytime an item's name is printed on the screen. the '+1' refers to the gap followed  by an item's name
          nameLength += names[i].length() + 1;
//          Serial.println(names[i]);
          location[i] = 1; //this means that the items getting printed is in the fridge
          // lcd.setCursor(0, 1);
          delay(500);
        }
      }
    }
    //reset row and nameLength
    row = 0;
    nameLength = 0;
//    Serial.println("setting old val to val");
  }
  oldVal = val; //update oldVal
  delay(500);
}
