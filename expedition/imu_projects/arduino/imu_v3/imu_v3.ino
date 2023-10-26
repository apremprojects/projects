#include <Wire.h>
#include <LSM6.h>
#include <LIS3MDL.h>

LSM6 imu;
LIS3MDL mag;

enum Mode { TRANSMIT, STOP };

Mode mode;
char report[1024];
unsigned long int prev_time;
unsigned long int heart_beat_time;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Wire.begin();

  while (!imu.init()) {
      Serial.println(F("Failed to detect the LSM6."));
      delay(100);
  }

  imu.enableDefault();

  // Set the gyro full scale to 1000 dps because the default
  // value is too low, and leave the other settings the same.
  imu.writeReg(LSM6::CTRL2_G, 0b10001000);

  // Set the accelerometer full scale to 16 g because the default
  // value is too low, and leave the other settings the same.
  imu.writeReg(LSM6::CTRL1_XL, 0b10000100);

  while (!mag.init()) {
      // Failed to detect the LIS3MDL.
      Serial.println(F("Failed to detect the LIS3MDL."));
      delay(100);

  }
  mag.enableDefault();

  Serial.println("Starting...");

  mode = STOP;
  prev_time = millis();
}

void loop() {
  if (Serial.available()) {
    char cmd = Serial.read();
    switch (cmd) {
      case 's':
        mode = TRANSMIT;
        heart_beat_time = millis();
        break;
    }
  }

  if (mode == STOP) {
    return;
  }

  const unsigned long int curr_time = millis();
  const unsigned int elapsed = curr_time - prev_time;
  if (elapsed < 25) {
    return;
  }
  prev_time = curr_time;
  if ((curr_time - heart_beat_time) > 1000) {
    mode = STOP;
    return;
  }

  if (mode == TRANSMIT) {
    imu.read();
    mag.read();
    sprintf(report, "s: %d, %d, %d, %d, %d, %d, %d, %d, %d, %lu\r\n", 
      imu.a.x, imu.a.y, imu.a.z, 
      imu.g.x, imu.g.y, imu.g.z, 
      mag.m.x, mag.m.y, mag.m.z, 
      curr_time);
    Serial.print(report);
  }
}
