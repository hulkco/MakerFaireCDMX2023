/*
  Battery Monitor

  This example creates a Bluetooth® Low Energy peripheral with the standard battery service and
  level characteristic. The A0 pin is used to calculate the battery level.
  
  This example code is in the public domain.

  Written by Gustavo Reynaga @gsreynaga, public domain

  For:  X-NODE MCU WIFI BLE (ESP32-WROOM-32)

  Notas	          ESP32 |  Pin        Mikro BUS       Pin	 |  ESP32   Notas
  Analógico	      IO32  |  AN     1	  AN    PWM	  16	PWM  |	IO33    PWM
  Reset	          RST   |  RST    2	  RST	  INT	  15	INT	 |  IO26    Interrupción
  SPI Chip Select	IO2   |  CS	    3	  CS	  RX	  14	RX	 |  IO16    UART Receptor
  SPI Clock	      IO18  |  SCK    4	  SCK	  TX	  13	TX	 |  IO17    UART Transmisor
  SPI Data Out	  IO19  |  MISO   5	  MISO	SCL	  12	SCL	 |  IO22    I2C Clock
  SPI Data In	    IO23  |  MOSI   6	  MOSI	SDA	  11	SDA	 |  IO21    I2C Data
  Voltaje	        3V3	  |  3V3    7	  3.3V	5V	  10	5V	 |  5V      Voltaje
  GND	            GND	  |  GND    8	  GND	  GND	  9	  GND	 |  GND     GND
  LED (POWER)     IO4

  ESP32 ADC works in a similar way like Arduino only difference here 
  is it has 12 bit ADC. So, the ESP32 board maps the analog voltage 
  values ranging from 0 to 4095 in digital discrete values.  

*/

#include <ArduinoBLE.h>

 // Bluetooth® Low Energy Battery Service
BLEService batteryService("180F");

// Bluetooth® Low Energy Battery Level Characteristic
BLEUnsignedCharCharacteristic batteryLevelChar("2A19",  // standard 16-bit characteristic UUID
    BLERead | BLENotify); // remote clients will be able to get notifications if this characteristic changes

int oldBatteryLevel = 0;  // last battery level reading from analog input
long previousMillis = 0;  // last time the battery level was checked, in ms

const int ledPin = 4; // pin to use for the LED

void setup() {
  Serial.begin(9600);    // initialize serial communication
  while (!Serial);

  pinMode(ledPin, OUTPUT); // initialize the built-in LED pin to indicate when a central is connected

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }

  /* Set a local name for the Bluetooth® Low Energy device
     This name will appear in advertising packets
     and can be used by remote devices to identify this Bluetooth® Low Energy device
     The name can be changed but maybe be truncated based on space left in advertisement packet
  */
  BLE.setLocalName("BatteryMonitor");
  BLE.setAdvertisedService(batteryService); // add the service UUID
  batteryService.addCharacteristic(batteryLevelChar); // add the battery level characteristic
  BLE.addService(batteryService); // Add the battery service
  batteryLevelChar.writeValue(oldBatteryLevel); // set initial value for this characteristic

  /* Start advertising Bluetooth® Low Energy.  It will start continuously transmitting Bluetooth® Low Energy
     advertising packets and will be visible to remote Bluetooth® Low Energy central devices
     until it receives a new connection */

  // start advertising
  BLE.advertise();

  Serial.println("Bluetooth® device active, waiting for connections...");
}

void loop() {
  // wait for a Bluetooth® Low Energy central
  BLEDevice central = BLE.central();

  // if a central is connected to the peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's BT address:
    Serial.println(central.address());
    // turn on the LED to indicate the connection:
    digitalWrite(ledPin, HIGH);

    // check the battery level every 200ms
    // while the central is connected:
    while (central.connected()) {
      long currentMillis = millis();
      // if 200ms have passed, check the battery level:
      if (currentMillis - previousMillis >= 200) {
        previousMillis = currentMillis;
        updateBatteryLevel();
      }
    }
    // when the central disconnects, turn off the LED:
    digitalWrite(ledPin, LOW);
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }
}

void updateBatteryLevel() {
  /* Read the current voltage level on the analog input pin.
     This is used here to simulate the charge level of a battery.
  */
  int battery = analogRead(32);
  int batteryLevel = map(battery, 0, 4095, 0, 100);

  /*
    ESP32 ADC works in a similar way like Arduino only difference here 
    is it has 12 bit ADC. So, the ESP32 board maps the analog voltage 
    values ranging from 0 to 4095 in digital discrete values.  
  */

  if (batteryLevel != oldBatteryLevel) {      // if the battery level has changed
    Serial.print("Battery Level % is now: "); // print it
    Serial.println(batteryLevel);
    batteryLevelChar.writeValue(batteryLevel);  // and update the battery level characteristic
    oldBatteryLevel = batteryLevel;           // save the level for next comparison
  }
}
