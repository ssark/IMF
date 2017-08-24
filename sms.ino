#include <SPI.h>
#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <Temboo.h>
#include "TembooAccount.h" // File containing Temboo account information
// used to send SMS
#include <Wire.h> // For serial communication with another arduino

byte ethernetMACAddress[] = ETHERNET_SHIELD_MAC;
EthernetClient client;

int numRuns = 1;   // Execution count, so this doesn't run forever
int maxRuns = 1;   // Maximum number of times the Choreo should be executed
int x;

// parallel arrays with names and locations
String names[5] = {"chips", "mint", "nuts", "apple", "juice"};
int location[5] = {1, 1, 1, 1, 1};


void setup() {
  Wire.begin(8);                
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output

  while (!Serial);

  if (Ethernet.begin(ethernetMACAddress) == 0) {
    //Serial.println("FAIL");
    while (true);
  }
  //Serial.println("OK");
  //delay(1000);

  //Serial.println("Setup complete.\n");
}


// function that executes whenever data is received from master
// this function is registered as an event in setup()
void receiveEvent(int howMany) {

  // Serial communication
  while (1 < Wire.available()) { 
  }

  // read in index and location
  x = Wire.read();    
  Serial.println(x);
  // y = Wire.read();
  // location[x] = y; 
  // Serial.println(y);

  // check location
  if (numRuns <= maxRuns) {
    // delay(60000);
    // if (location[x] == 0) {
    TembooChoreo SendSMSChoreo(client);

    // Invoke the Temboo client
    SendSMSChoreo.begin();

    // Set Temboo account credentials
    SendSMSChoreo.setAccountName(TEMBOO_ACCOUNT);
    SendSMSChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    SendSMSChoreo.setAppKey(TEMBOO_APP_KEY);

    // Set Choreo inputs
    String AuthTokenValue = "d558044c31f041db628941ac481d838c";
    SendSMSChoreo.addInput("AuthToken", AuthTokenValue);
    String BodyValue = "Reminder: Buy more " + names[x] + "!";
    SendSMSChoreo.addInput("Body", BodyValue);
    String ToValue = "+12153754459";
    SendSMSChoreo.addInput("To", ToValue);
    String AccountSIDValue = "ACfd625c80670237064ee9dae1fc445844";
    SendSMSChoreo.addInput("AccountSID", AccountSIDValue);
    String FromValue = "+13158025038";
    SendSMSChoreo.addInput("From", FromValue);

    // Identify the Choreo to run
    SendSMSChoreo.setChoreo("/Library/Twilio/SMSMessages/SendSMS");

    // Run the Choreo; when results are available, print them to serial
    SendSMSChoreo.run();

    while (SendSMSChoreo.available()) {
      char c = SendSMSChoreo.read();
      //Serial.print(c);
    }
    SendSMSChoreo.close();
  //}
  }
}

void loop() {
}