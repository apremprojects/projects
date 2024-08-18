#include <Wire.h>
#include <LPS.h>
#include <LSM6.h>
#include <LIS3MDL.h>

LPS ps;
LSM6 imu;
LIS3MDL mag;

enum Mode { TRANSMIT, STOP };

Mode mode;
char line[2048];
unsigned long int prev_time;
unsigned long int heart_beat_time;

const byte PICO_I2C_SDA = 16;
const byte PICO_I2C_SCL = 17;

char read() {
  char ch = '\n';
  if (Serial.available()) {
    ch = Serial.read();
  }
  return ch;
}

void read_line(char* line) {
  char ch;
  int length = 0;
  while ((ch = read()) != '\n') {
    if (ch == '\r') {
      continue;
    }
    line[length++] = ch; 
  }
  line[length] = '\0';
}

void setup() {
  Serial.begin(115200);
  while (!Serial);
  prev_time = millis();

  Wire.setSDA(PICO_I2C_SDA);
  Wire.setSCL(PICO_I2C_SCL);
  
  Wire.begin();
  if (!imu.init()) {
    Serial.print("IMU initialization failed\n");
  }
  
  imu.enableDefault();

  // Set the gyro full scale to 1000 dps because the default
  // value is too low, and leave the other settings the same.
  imu.writeReg(LSM6::CTRL2_G, 0b10001000);

  // Set the accelerometer full scale to 16 g because the default
  // value is too low, and leave the other settings the same.
  imu.writeReg(LSM6::CTRL1_XL, 0b10000100);

  if (!mag.init()) {
    // Failed to detect the LIS3MDL.
    Serial.println(F("Failed to detect the LIS3MDL."));

  }
  mag.enableDefault();
  
  if (!ps.init()) {
    Serial.println("Failed to autodetect pressure sensor!");
  }
  ps.enableDefault();
  
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.println("Starting...");

  mode = STOP;
  prev_time = millis();
}

//void setup1() {
// pinMode(LED_BUILTIN, OUTPUT);
//  delay(5000);
//}

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

  unsigned long curr_time = millis();
  const int diff_time = static_cast<int>(curr_time - prev_time);
  if (diff_time < 25) {
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
    const float temp = ps.readTemperatureF();
    const float press = ps.readPressureInchesHg();
    const float alt = ps.pressureToAltitudeFeet(press);
    sprintf(line, "s: %d, %d, %d, %d, %d, %d, %d, %d, %d, %f, %f, %f, %lu\r\n", 
      imu.a.x, imu.a.y, imu.a.z, 
      imu.g.x, imu.g.y, imu.g.z, 
      mag.m.x, mag.m.y, mag.m.z, 
      press, alt, temp, curr_time);
    Serial.print(line);
  }
}

//void loop1() {
//}
