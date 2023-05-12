/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

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
  
*/



int led = 21;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(led, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(led, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(1000);                      // wait for a second
  digitalWrite(led, LOW);   // turn the LED off by making the voltage LOW
  delay(1000);                      // wait for a second
}
