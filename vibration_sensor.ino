#define PERIOD 500 // period in micro secs (us)
#define LED 4
#define VIB_ARR_SIZE 30 // array size for vibration data storage

int analog_Z = A0;
int analog_Y = A1;

int analogVibArrZ[VIB_ARR_SIZE];
int analogVibArrY[VIB_ARR_SIZE];

int z_max;
int z_min;
int y_max;
int y_min;

int vibAmplZ;
int vibAmplY;

int threshold = 70;

unsigned long last_us = 0L;

int counter = 0;

void setup() {
  Serial.begin(9600);
  pinMode(LED,OUTPUT);
  delay(2000);    // waits 2 secs before calibration
  /*
  int tf_calib_Z[10];
  int tf_calib_Y[10];
  for (int i = 0; i < 10; i++) {
    tf_calib_Z[i] = analogRead(analog_Z);
    tf_calib_Y[i] = analogRead(analog_Y);
  } // for

  int sum_Z = 0;
  int sum_Y = 0;
  for (int i = 0; i < 10; i++) {
    sum_Z += tf_calib_Z[i];
    sum_Y += tf_calib_Y[i]; 
  } // for

  int avg_Z = sum_Z / 10;
  int avg_Y = sum_Y / 10;

  */
  
  
} // setup

void loop() {
  counter = 0;
  while (counter < VIB_ARR_SIZE) {
    if (micros() - last_us > PERIOD) {
       last_us += PERIOD;
       sample();
    } // if
    
    // output on serial monitor
    Serial.print("Analog Signal Z-axis (in mV): ");
    Serial.println(analogVibArrZ[counter]);
    Serial.print("Analog Signal Y-axis (in mV): ");
    Serial.println(analogVibArrY[counter]);
    counter++;
  } // while
  
  z_max = analogVibArrZ[0];
  z_min = analogVibArrZ[0];
  y_max = analogVibArrY[0];
  y_min = analogVibArrY[0];

  // finding max
  for (int i = 1; i < VIB_ARR_SIZE; i++) {
    z_max = max(analogVibArrZ[i],z_max);
    y_max = max(analogVibArrY[i],y_max);
  } // for

  // finding min
  for (int i = 1; i < VIB_ARR_SIZE; i++) {
    z_min = min(analogVibArrZ[i],z_min);
    y_min = min(analogVibArrY[i],y_min);
  } // for

  // find abs of diff
  vibAmplZ = abs(z_max - z_min);
  vibAmplY = abs(y_max - y_min);

  // outputting vibAmpl
  for (int i = 0; i < 20; i++) {
    Serial.print("vibAmplZ: ");
    Serial.println(vibAmplZ);
    Serial.print("vibAmplY: ");
    Serial.println(vibAmplY);    
  } // for

  // checking if vib is above threshold
  if ((vibAmplZ || vibAmplY) > threshold) {
    Serial.print("Vibration Amplitude (Z): ");
    Serial.println(vibAmplZ);
    Serial.print("Vibration Amplitude (Y): ");
    Serial.println(vibAmplY);
    digitalWrite(LED,HIGH);
    delay(1000);      
    digitalWrite(LED,LOW);

  } // if

} // loop

void sample() {
  analogVibArrZ[counter] = analogRead(analog_Z);
  analogVibArrY[counter] = analogRead(analog_Y);
} // sample
