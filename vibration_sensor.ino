#include <LiquidCrystal.h>

#define PERIOD 500 // period in micro secs (us)
#define VIB_ARR_SIZE 30 // array size for vibration data storage

// pin declarations
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
const int analog_Z = A0;
const int analog_Y = A1;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

float analogVibArrZ[VIB_ARR_SIZE];
float analogVibArrY[VIB_ARR_SIZE];

float z_max;
float z_min;
float y_max;
float y_min;

float vibAmplZ;
float vibAmplY;

float threshold = 70.0;

unsigned long last_us = 0L;

int counter = 0;

void setup() {
  Serial.begin(9600);
  lcd.begin(16,2);
  pinMode(6,OUTPUT);
  delay(2000);    // waits 2 secs before calibration

  lcd.setCursor(0,0);
  lcd.print("Get ready for");
  lcd.setCursor(0,1);
  lcd.print("calibration.");
  delay(1000);
  lcd.clear();

  float tf_calib_Z[10];
  float tf_calib_Y[10];
  float sum_Zup;
  float avg_Zup;
  float sum_Yup;
  float avg_Yup;
  float sum_Zdown;
  float avg_Zdown;
  float sum_Ydown;
  float avg_Ydown;


  //
  // Z calib
  //
  lcd.setCursor(0,0);
  lcd.print("Click button");
  lcd.setCursor(0,1);
  lcd.print("for up-Z");
  while (!digitalRead(6)) {
    delay(25);
  }
  delay(1000);
  for (int i = 0; i < 10; i++) {
    tf_calib_Z[i] = analogRead(analog_Z);
  } // for
  sum_Zup = 0;
  for (int i = 0; i < 10; i++) {
    sum_Zup += tf_calib_Z[i];
  } // for
  avg_Zup = sum_Zup / 10;
  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("Finished up-Z");
  lcd.setCursor(0,1);
  lcd.print("calibration");
  delay(1000);
  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("Click button");
  lcd.setCursor(0,1);
  lcd.print("for down-Z");
  while (!digitalRead(6)) {
    delay(25);
  }
  delay(1000);
  for (int i = 0; i < 10; i++) {
    tf_calib_Z[i] = analogRead(analog_Z);
  } // for
  sum_Zdown = 0;
  for (int i = 0; i < 10; i++) {
    sum_Zdown += tf_calib_Z[i];
  } // for
  avg_Zdown = sum_Zdown / 10;
  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("Finished down-Z");
  lcd.setCursor(0,1);
  lcd.print("calibration");
  delay(1000);
  lcd.clear();

  //
  // y calib
  //
  lcd.setCursor(0,0);
  lcd.print("Click button");
  lcd.setCursor(0,1);
  lcd.print("for up-Y");
  while (!digitalRead(6)) {
    delay(25);
  }
  delay(1000);
  for (int i = 0; i < 10; i++) {
    tf_calib_Y[i] = analogRead(analog_Y);
  } // for
  sum_Yup = 0;
  for (int i = 0; i < 10; i++) {
    sum_Yup += tf_calib_Y[i];
  } // for
  avg_Yup = sum_Yup / 10;
  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("Finished up-Y");
  lcd.setCursor(0,1);
  lcd.print("calibration");
  delay(1000);
  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("Click button");
  lcd.setCursor(0,1);
  lcd.print("for down-Y");
  while (!digitalRead(6)) {
    delay(25);
  }
  delay(1000);
  for (int i = 0; i < 10; i++) {
    tf_calib_Y[i] = analogRead(analog_Y);
  } // for
  sum_Ydown = 0;
  for (int i = 0; i < 10; i++) {
    sum_Ydown += tf_calib_Y[i];
  } // for
  avg_Ydown = sum_Ydown / 10;
  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("Finished down-Y");
  lcd.setCursor(0,1);
  lcd.print("calibration");
  delay(1000);
  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("Calibration");
  lcd.setCursor(0,1);
  lcd.print("complete.");
  delay(1000);
  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("Z:");
  lcd.setCursor(0,1);
  lcd.print("Y:");
} // setup

void loop() {
  counter = 0;
  while (counter < VIB_ARR_SIZE) {
    if (micros() - last_us > PERIOD) {
       last_us += PERIOD;
       sample();
    } // if
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

  printAmplZ(vibAmplZ);
  delay(140);
  printAmplY(vibAmplY);
  delay(140);

  // checking if vib is above threshold
  if ((vibAmplZ > threshold)||(vibAmplY > threshold)) {
    lcd.setCursor(8,0);
    lcd.print("WARNING!");
  } // if

} // loop

void sample() {
  analogVibArrZ[counter] = 0.1475 * (analogRead(analog_Z) - 416);
  analogVibArrY[counter] = 0.1432 * (analogRead(analog_Y) - 406) + 9.81;
} // sample

void printAmplZ(float ampl) {
  lcd.setCursor(2,0);
  lcd.print(ampl);
} // printAmplZ

void printAmplY(float ampl) {
  lcd.setCursor(2,1);
  lcd.print(ampl);
} // printAmplY
