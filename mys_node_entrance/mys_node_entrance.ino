#define MY_RADIO_NRF24
#define MY_DEBUG
#define MY_RF24_CE_PIN 8
#define MY_NODE_ID 44

#include <Vcc.h>
#include <MySensors.h>
#include <SPI.h>

#define INTERRUPT 2
#define PIN_PRESENT 4
#define PIN_ABSENT 5
#define PIN_OPEN 6
#define PIN_LOCK 7
#define PIN_REED_DRIVER 14
#define OPEN 1
#define CLOSE 0
#define ABSENT 0
#define PRESENT 1
#define SENSOR_ID_PRESENT 99
#define SENSOR_ID_OPEN 101
#define SENSOR_ID_LOCK 102
#define SENSOR_ID_VCC 1


MyMessage msgPresent(SENSOR_ID_PRESENT, V_TRIPPED);
MyMessage msgOpen(SENSOR_ID_OPEN, V_TRIPPED);
MyMessage msgLock(SENSOR_ID_LOCK, V_TRIPPED);
MyMessage msgVcc(SENSOR_ID_VCC, V_VOLTAGE);

//uint8_t value = OPEN;
uint8_t v_open = CLOSE;
uint8_t v_open_old = CLOSE;
uint8_t v_lock = CLOSE;
uint8_t v_lock_old = CLOSE;
uint8_t v_present = 0;
uint8_t v_absent = 0;
uint8_t v_p = 1;

const float VccMin   = 0.0;           // Minimum expected Vcc level, in Volts.
const float VccMax   = 3.3;           // Maximum expected Vcc level, in Volts.
const float VccCorrection = 3.33/3.38;  // Measured Vcc by multimeter divided by reported Vcc
Vcc vcc(VccCorrection);
int vcc_counter = 0;

void setup() {
  pinMode(PIN_PRESENT, INPUT_PULLUP);
  pinMode(PIN_ABSENT, INPUT_PULLUP);
  pinMode(PIN_OPEN, INPUT_PULLUP);
  pinMode(PIN_LOCK, INPUT_PULLUP);
  pinMode(PIN_REED_DRIVER, INPUT_PULLUP);
}

void presentation()
{
    sendSketchInfo("Front Door Sensor", "0.1");

    present(SENSOR_ID_VCC, S_MULTIMETER);
    present(SENSOR_ID_PRESENT, S_DOOR);
    present(SENSOR_ID_OPEN, S_DOOR);
    present(SENSOR_ID_LOCK, S_DOOR);
}

void loop()
{
    //Serial.println("loop()");

    //value = value == OPEN ? CLOSE : OPEN;
    //send(msgPresent.set(value));

    handlePresents();
    handleReeds();  
    handleStatus();  
    
    //sleep(2000); FALLING
    sleep(digitalPinToInterrupt(INTERRUPT),FALLING,3000);
}

void handleStatus() {
    vcc_counter++;
    if(vcc_counter > 60) {
      float v = vcc.Read_Volts();
      send(msgVcc.set(v, 2));
      vcc_counter = 0;
    }
}

void handlePresents() {
  v_present = digitalRead(PIN_PRESENT);
  //Serial.print("present: ");
  //Serial.println(v_present);
  v_absent = digitalRead(PIN_ABSENT);
  //Serial.print("absent: ");
  //Serial.println(v_absent);

  if(v_present == 0) {
    if(v_p != PRESENT) {
      v_p = PRESENT;
      send(msgPresent.set(v_p));    
    }
  } 
  if(v_absent == 0) {
    if(v_p != ABSENT) {
      v_p = ABSENT;
      send(msgPresent.set(v_p));
    }
  }
}

void handleReeds() {
  digitalWrite(PIN_REED_DRIVER, LOW);
    pinMode(PIN_REED_DRIVER, OUTPUT);

    v_open = digitalRead(PIN_OPEN);
    if(v_open != v_open_old) {
      send(msgOpen.set(v_open));
      v_open_old = v_open;
    }
    v_lock = !digitalRead(PIN_LOCK);
    if(v_lock != v_lock_old) {
      send(msgLock.set(v_lock));
      v_lock_old = v_lock;
    }

    pinMode(PIN_REED_DRIVER, INPUT_PULLUP);
}

