/*
  Battery Monitor  
  This example code is in the public domain.
*/

#include <ArduinoBLE.h>

 // Bluetooth® Low Energy Battery Service
BLEService batteryService("180F");

// Bluetooth® Low Energy Battery Level Characteristic
BLEUnsignedCharCharacteristic batteryLevelChar("2A19",  // standard 16-bit characteristic UUID
    BLERead | BLENotify); // remote clients will be able to get notifications if this characteristic changes

int oldBatteryLevel = 0;  // last battery level reading from analog input
long previousMillis = 0;  // last time the battery level was checked, in ms

/*
  La primera línea del código debe incluir el archivo ArduinoBLE.h.
  Enseguida declararemos el Servicio de batería y las características del nivel de batería, 
  en esta parte se darán dos permisos: BLERead y BLENotify. 

  BLERead permitirá que los dispositivos centrales (teléfono móvil) lean datos del 
  dispositivo periférico (Arduino). Y BLENotify permite que los clientes remotos reciban 
  notificaciones si esta característica cambia. 

  Ahora vamos a saltar a la función de configuración.
*/

void setup() {
  Serial.begin(9600);    // initialize serial communication
  while (!Serial);

  pinMode(LED_BUILTIN, OUTPUT); // initialize the built-in LED pin to indicate when a central is connected

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }

  /*
    Aquí inicializará la comunicación en serie y BLE y esperará a que se abra el monitor en serie. 
    Establezca un nombre local para el dispositivo BLE. Este nombre aparecerá en los paquetes 
    publicitarios y puede ser utilizado por dispositivos remotos para identificar este dispositivo 
    BLE. 
  */
  BLE.setLocalName("BatteryMonitor");
  BLE.setAdvertisedService(batteryService); // add the service UUID
  batteryService.addCharacteristic(batteryLevelChar); // add the battery level characteristic
  BLE.addService(batteryService); // Add the battery service
  batteryLevelChar.writeValue(oldBatteryLevel); // set initial value for this characteristic

  /*
    Aquí agregaremos y estableceremos el valor para el UUID del servicio y la característica.
  */

  // start advertising
  BLE.advertise();

  Serial.println("Bluetooth® device active, waiting for connections...");
  /*
    Y aquí, comenzaremos a anunciar BLE. Comenzará a transmitir continuamente paquetes de publicidad BLE y será visible para los dispositivos centrales BLE remotos hasta que reciba una nueva conexión.
  */
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
    digitalWrite(LED_BUILTIN, HIGH);

    /*
    
    Y aquí, la función de bucle. Una vez que todo esté configurado y haya comenzado la publicidad, el dispositivo esperará a cualquier dispositivo central. Una vez conectado, mostrará la dirección MAC del dispositivo y encenderá el LED incorporado.

    */

    // check the battery level every 200ms
    // while the central is connected:
    while (central.connected()) {
      long currentMillis = millis();
      // if 200ms have passed, check the battery level:
      if (currentMillis - previousMillis >= 200) {
        previousMillis = currentMillis;
        updateBatteryLevel();
      }
      /*

        Ahora, comenzará a leer el voltaje analógico de A0, que será un valor entre 0 y 1023 y lo mapeará en el rango de 0 a 100. Imprimirá el nivel de la batería en el monitor serie y el valor se escribirá para las características de batteryLevelchar y espera 200 ms. Después de eso, todo el bucle se ejecutará nuevamente siempre que el dispositivo central esté conectado a este dispositivo periférico.

      */
    }
    // when the central disconnects, turn off the LED:
    digitalWrite(LED_BUILTIN, LOW);
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
    /*

      Una vez desconectado, se mostrará un mensaje en el dispositivo central y el LED se apagará.

    */
  }
}

void updateBatteryLevel() {
  /* Read the current voltage level on the A0 analog input pin.
     This is used here to simulate the charge level of a battery.
  */
  int battery = analogRead(A0);
  int batteryLevel = map(battery, 0, 1023, 0, 100);

  if (batteryLevel != oldBatteryLevel) {      // if the battery level has changed
    Serial.print("Battery Level % is now: "); // print it
    Serial.println(batteryLevel);
    batteryLevelChar.writeValue(batteryLevel);  // and update the battery level characteristic
    oldBatteryLevel = batteryLevel;           // save the level for next comparison
  }
}
