// MPU-9250 RAW I2C -> ASCII SERIAL STREAM

#include <Wire.h>

#define MPU_ADDR 0x68

// Registers
#define PWR_MGMT_1     0x6B
#define ACCEL_CONFIG   0x1C
#define GYRO_CONFIG    0x1B
#define CONFIG         0x1A
#define SMPLRT_DIV     0x19
#define ACCEL_XOUT_H   0x3B

void writeReg(uint8_t reg, uint8_t val) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(reg);
  Wire.write(val);
  Wire.endTransmission();
}

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Wire.begin();
  Wire.setClock(400000);   

  // ---- MPU9250 CONFIGURATION ----
  writeReg(PWR_MGMT_1, 0x00);     // Wake up
  delay(100);

  writeReg(CONFIG, 0x03);         // DLPF ≈ 44 Hz
  writeReg(SMPLRT_DIV, 0x04);     // 1 kHz / (1+4) = 200 Hz

  writeReg(ACCEL_CONFIG, 0x00);   // ±2g (16384 LSB/g)
  writeReg(GYRO_CONFIG,  0x00);   // ±250 dps (131 LSB/dps)
}

void loop() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(ACCEL_XOUT_H);
  Wire.endTransmission(false);

  Wire.requestFrom(MPU_ADDR, 14, true);

  int16_t ax = (Wire.read() << 8) | Wire.read();
  int16_t ay = (Wire.read() << 8) | Wire.read();
  int16_t az = (Wire.read() << 8) | Wire.read();

  Wire.read(); Wire.read(); // temperature (ignored)
  
  int16_t gx = (Wire.read() << 8) | Wire.read();
  int16_t gy = (Wire.read() << 8) | Wire.read();
  int16_t gz = (Wire.read() << 8) | Wire.read();

  // ---- SCALING TO PHYSICAL UNITS ----
  float ax_g = ax / 16384.0f;
  float ay_g = ay / 16384.0f;
  float az_g = az / 16384.0f;

  float gx_dps = gx / 131.0f;
  float gy_dps = gy / 131.0f;
  float gz_dps = gz / 131.0f;

  // ---- CSV LINE ----
  Serial.print(ax_g, 6); Serial.print(',');
  Serial.print(ay_g, 6); Serial.print(',');
  Serial.print(az_g, 6); Serial.print(',');
  Serial.print(gx_dps, 6); Serial.print(',');
  Serial.print(gy_dps, 6); Serial.print(',');
  Serial.println(gz_dps, 6);

  delay(10);  // 100 Hz output
}
