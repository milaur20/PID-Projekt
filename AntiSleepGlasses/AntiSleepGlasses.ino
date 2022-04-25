#include <Wire.h>
#include <WireIMXRT.h>
#include <WireKinetis.h>
#include <SparkFun_APDS9960.h>

// Global Variables
SparkFun_APDS9960 apds = SparkFun_APDS9960();
uint8_t proximity_data = 0;

boolean eyeIsClosed = false;
int Sinput = A1;
int piezo = 2;
int buzz = 3;
int sleeping_counter;
int sleeping_counter_thres1 = 10;
int sleeping_counter_thres2 = 20; 
int sleeping_counter_thres3 = 30;
bool sleeping_stage1 = false;
bool sleeping_stage2 = false;
bool sleeping_stage3 = false;

String eye_is_closed = " + Øjet er lukket";
String eye_is_open = " + Øjet er åben";

void setup() {
  pinMode(Sinput, INPUT);  /// here it is sensors pin  so we are defining here that this pin is input pin
  pinMode(piezo, OUTPUT);
  pinMode(buzz, OUTPUT);
  // Initialize Serial port
  Serial.begin(9600);
  Serial.println();
  Serial.println(F("------------------------------------"));
  Serial.println(F("SparkFun APDS-9960 - ProximitySensor"));
  Serial.println(F("------------------------------------"));
  
  // Initialize APDS-9960 (configure I2C and initial values)
  if ( apds.init() ) {
    Serial.println(F("APDS-9960 initialization complete"));
  } else {
    Serial.println(F("Something went wrong during APDS-9960 init!"));
  }
  
  // Adjust the Proximity sensor gain
  if ( !apds.setProximityGain(PGAIN_2X) ) {
    Serial.println(F("Something went wrong trying to set PGAIN"));
  }
  
  // Start running the APDS-9960 proximity sensor (no interrupts)
  if ( apds.enableProximitySensor(false) ) {
    Serial.println(F("Proximity sensor is now running"));
  } else {
    Serial.println(F("Something went wrong during sensor init!"));
  }
}

void loop() {
  Serial.println(sleeping_counter);
  SampleData();
  IsEyeClosed();
  //CheckSleepingStage();
  Output();
  //printLort();
}
void SampleData() {

  if(digitalRead(Sinput)==LOW){
       sleeping_counter++; //COUNT UP!!!  
     }
     else
      {
        if(digitalRead(Sinput)== HIGH){
                 sleeping_counter = 0;
           }
      }
// Read the proximity value
  if ( !apds.readProximity(proximity_data) ) {
    Serial.println("Error reading proximity value");
  } else {
    Serial.print("Proximity: ");
    Serial.print(proximity_data);
    if(digitalRead(Sinput)==LOW)  {
      Serial.println(eye_is_closed);
     }
     else
      {
        if(digitalRead(Sinput)== HIGH){
                 Serial.println(eye_is_open);
           }
      }

  }
  if(sleeping_counter >= sleeping_counter_thres1) {
    sleeping_stage1 = true;
  }
  if(sleeping_counter >= sleeping_counter_thres2)  {
    sleeping_stage2 = true;
    Serial.println("PRUTSKID");
  }
  if(sleeping_counter >= sleeping_counter_thres3)  {
    sleeping_stage3 = true;
  }
  if(sleeping_counter <sleeping_counter_thres1) {
    sleeping_stage1 = false;
    sleeping_stage2 = false;
    sleeping_stage3 = false;
  }
  // Wait 250 ms before next reading
  delay(250);
}
void IsEyeClosed()  {
  if(proximity_data > 200 && proximity_data < 230 ) {
    Serial.println("1");
  }
    if(proximity_data > 230 && proximity_data < 255) {
      Serial.println("2");
    }
      if(proximity_data >= 255) {
      Serial.println("3");
      }
    }
void Output(){
  if(sleeping_stage1 == true){
    //digitalWrite(piezo, HIGH);
    Serial.println("PIEZO KØRER!!!!");
  }
  if(sleeping_stage2 == true){
    tone(buzz, 1000);
    Serial.println("NU KØRER BUZZER");
  }
  else  {
    noTone(buzz);
  }
}
void printLort()  {
  if(eyeIsClosed) {
    Serial.println("1");
  }
  if(eyeIsClosed) {
    Serial.println("2");
  }
  if(eyeIsClosed) {
    Serial.println("3");
  }
}
