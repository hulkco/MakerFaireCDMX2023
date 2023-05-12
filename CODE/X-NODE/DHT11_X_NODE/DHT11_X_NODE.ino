/*
  Example testing sketch for various DHT humidity/temperature sensors
  Written by ladyada, public domain
  Modify by Gustavo Reynaga for:  

  X-NODE MCU WIFI BLE (ESP32-WROOM-32)

  led -> IO4 ESP32 (POWER)


  Notas	          ESP32 |  Pin          Mikro BUS       Pin	    ESP32 Notas
  Analógico	      IO32  |  AN     1	    AN    PWM	  16	PWM  |	IO33    PWM
  Reset	          RST   |  RST    2	    RST	  INT	  15	INT	 |  IO26    Interrupción
  SPI Chip Select	IO2   |  CS	    3	    CS	  RX	  14	RX	 |  IO16    UART Receptor
  SPI Clock	      IO18  |  SCK    4	    SCK	  TX	  13	TX	 |  IO17      UART Transmisor
  SPI Data Out	  IO19  |  MISO   5	    MISO	SCL	  12	SCL	 |  IO22      I2C Clock
  SPI Data In	    IO23  |  MOSI   6	    MOSI	SDA	  11	SDA	 |  IO21      I2C Data
  Voltaje	        3V3	  |  3V3    7	    3.3V	5V	  10	5V	 |  5V      Voltaje
  GND	            GND	  |  GND    8	    GND	  GND	  9	  GND	 |  GND      GND
  LED (POWER)     IO4
  
*/


#include "DHT.h"

#define DHTPIN 21     // Digital pin connected to the DHT sensor

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));

  dht.begin();
}

void loop() {
  // Wait a few seconds between measurements.
  delay(5000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));
}
