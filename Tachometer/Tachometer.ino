// ===============================
// AUTHOR     :       Bach Tran
// CREATE DATE     :  5/18/2022
// PURPOSE     :      Arduino based Tachometer
// SPECIAL NOTES:     Made for Mountain Drone Team
// ===============================
// Change History:
//
//==================================

const byte interruptPin = 2; //Photoresistor pin
const int blades = 1; // number of blades
volatile int count = 0; //counting variable
unsigned long start_time; //start time of count 0 to calculate RPM
volatile float RPM = 0; // RPM
unsigned long elapsed_time; // Time between each count

void setup() {
  //Set interrupt pin mode
  pinMode(interruptPin, INPUT_PULLUP);
  
  //Turn on laser
  pinMode(12,OUTPUT); 
  digitalWrite(12,HIGH);

  //Set interrupt
  attachInterrupt(digitalPinToInterrupt(interruptPin), counting, FALLING);

  //Serial communication
  Serial.begin(9600);
}

void loop() {
  //Calculate RPM based on the 35 count moving average.
  if(count > 35*blades){
    elapsed_time = micros()-start_time;
    RPM = (float)count/(elapsed_time)*1000000*60/blades;
    count = 0;
  }
  
  //Printout RPM 10 times per second
  Serial.print("\tRPM: ");
  Serial.println(RPM);
  delay(100);
  
}

void counting(){
  // Reset count if there is no new count for 5 seconds
  // Or begin counting from 0
  if((micros() - start_time) > 5000000 || count == 0){
    count = 1;
    start_time = micros();
  }else{
    count++;
  }
}
