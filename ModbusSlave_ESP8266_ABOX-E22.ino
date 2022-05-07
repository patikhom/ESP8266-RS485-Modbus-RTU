#include <Arduino.h>

#define LED_BUILTIN   D4
const int analogInPin = A0;  // ESP8266 Analog Pin ADC0 = A0

#include "ModbusRTU.h"          // https://github.com/emelianov/modbus-esp8266


//Modbus slave object
ModbusRTU mb;

//#include <SoftwareSerial.h>
//SoftwareSerial myModbusSlave;    // RX, TX
//SoftwareSerial myModbusSlave(14, 15);   // RX, TX
//SoftwareSerial mySerial(7,6);     // RX, TX


// COIL REGISTER
#define REG_COIL_RELAY_D7   0
#define REG_COIL_RELAY_D8   1

// DECREST INPUT REGISTER
#define REG_INPUT_D5      0
#define REG_INPUT_D6      1

// HOLDING REGISTER
#define REG_ANALOG_A0        0


void setup() {
  // put your setup code here, to run once:

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  
  pinMode(D5, INPUT_PULLUP);
  pinMode(D6, INPUT_PULLUP);
  
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);
  
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.begin(9600); // อัตรารับส่งข้อมูล

  
  mb.addCoil(REG_COIL_RELAY_D7);   // Coil Register 0
  mb.addCoil(REG_COIL_RELAY_D8);   // Coil Register 1
  
  mb.addIsts(REG_INPUT_D5);        // Input Register 0
  mb.addIsts(REG_INPUT_D6);        // Input Register 1
  
  mb.addHreg(REG_ANALOG_A0);       // Holding Register 0

  mb.begin(&Serial);      // ใช้การสื่อสารผ่าน Serial0 ของ ESP8266
  mb.setBaudrate(9600);   // อัตรารับส่งข้อมูล 9600 bps
  mb.slave(1);            // Slave ID // บอร์ด ID

}


void loop() {
  mb.task();
  
  mb.Ists(REG_INPUT_D5, digitalRead(D5));   // Input Register 0
  mb.Ists(REG_INPUT_D6, digitalRead(D6));   // Input Register 1
  
  digitalWrite(D7, mb.Coil(REG_COIL_RELAY_D7)); // Coil Register 0
  digitalWrite(D8, mb.Coil(REG_COIL_RELAY_D8)); // Coil Register 1

  int analogValue = analogRead(analogInPin);  // value read from the pot
  mb.Hreg(REG_ANALOG_A0, analogValue);        // Holding Register 0
}
