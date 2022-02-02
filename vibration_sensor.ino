#define PERIOD 500 // period in micro secs (us)
#define LED 4

int analog_Z = A0;
int analog_Y = A1;

int vibArrSize = 200;
int analogVibArr[vibArrSize];

int z_max;
int z_min;
int y_max;
int y_min;

int vibAmpl; // used for intemediate calculation of vib

int threshold = 70;

unsigned long last_us = 0L;

void setup() {
  Serial.begin(9600);
  pinMode(LED,OUTPUT);

} // setup

void loop() {

  if (micros() - last_us > PERIOD) {
       last_us += PERIOD;
       sample();
  } // if

  z_max = analogVibArr[0];
  z_min = analogVibArr[0];

  // finding max
  for (int i = 1; i < 200; i++) {
    z_max = max(analogVibArr[i],z_max);
  } // for

  // finding min
  for (int i = 1; i < 200; i++) {
    z_min = min(analogVibArr[i],z_min);
  } // for

  // find abs of diff
  vibAmpl = abs(z_max - z_min);

  // checking if vib is above threshold
  if (vibAmpl > threshold) {
    Serial.print("Vibration Amplitude: ");
    Serial.println(vibAmpl);
    digitalWrite(LED,HIGH);
    delay(1000);      // wait 1 sec
    digitalWrite(LED,LOW);

  }

} // loop

void sample() {
  for (int i = 0; i < 200; i++) {
    analogVibArr[i] = analogRead(analog_Z);
    Serial.print("Analog Signal (in mV): ");
    Serial.println(analogVibArr[i]);
  } // for
} // sample
