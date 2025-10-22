#include <Arduino.h>
#include <math.h>

// FSR402 ADC pins (ADC1)
const int fsrPins[] = {36, 39, 34, 35, 32, 33};
const int numSensors = 6;

// Sensor coordinates (x, y) in cm (updated to real positions)
float sensorCoords[][2] = {
  {3.7, 2.7},   // S1 - Lateral forefoot
  {7, 2.4},     // S2 - Medial forefoot
  {7, 6.2},     // S3 - Medial forefoot
  {16.5, 4.6},  // S4 - Midfoot
  {22, 2.8},    // S5 - Lateral heel
  {22, 6.4}     // S6 - Medial heel
};

// Calibration ranges (Layer 7, 0-76kg)
int rawMin[numSensors] = {0, 0, 0, 0, 0, 0}; // Raw at 0kg
int rawMax[numSensors] = {3259, 3259, 3259, 3259, 3259, 3259}; // Raw at 76kg

// Kalman Filter variables (for force in Newtons)
float x_est[numSensors] = {0};
float P[numSensors] = {100}; // Reduced from 500 for faster initial convergence
float Q = 5.0;              // Process noise (kept, can increase to 10 if needed)
float R = 25;               // Reduced from 25 to trust measurements more

// Read FSR with averaging
int readFSR(int pin) {
  long sum = 0;
  for (int i = 0; i < 10; i++) {
    sum += analogRead(pin);
    delayMicroseconds(100);
  }
  return sum / 10;
}

// Piecewise calibration to force (N) based on Layer 7
float getForce(int rawValue, int sensorIdx) {
  if (rawValue <= 0) return 0.0; // 0kg
  if (rawValue >= 3259) return 745.56; // Cap at 76kg
  if (rawValue <= 2925) return 0.0335 * rawValue; // 0-10kg
  else if (rawValue <= 2934) return 1.604 * (rawValue - 2934) + 196.2; // 10-20kg (adjusted)
  else if (rawValue <= 3056.33) return 1.604 * (rawValue - 2934) + 196.2; // 20-40kg
  else if (rawValue <= 3217.5) return 0.791 * (rawValue - 3056.33) + 392.4; // 40-53kg
  else return 5.437 * (rawValue - 3217.5) + 519.93; // 53-76kg
}

// Kalman Filter for force
float kalmanFilter(float measurement, int sensorIdx) {
  float x_pred = x_est[sensorIdx];
  float P_pred = P[sensorIdx] + Q;
  float K = P_pred / (P_pred + R);
  x_est[sensorIdx] = x_pred + K * (measurement - x_pred);
  P[sensorIdx] = (1 - K) * P_pred;
  return x_est[sensorIdx];
}

void setup() {
  Serial.begin(115200);
  Serial.println("RESET"); // Marker to indicate boot completion
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);
}

void loop() {
  float forces[numSensors];
  float smoothedForces[numSensors];

  // Read, calibrate, and smooth
  for (int i = 0; i < numSensors; i++) {
    int rawValue = readFSR(fsrPins[i]);
    forces[i] = getForce(rawValue, i); // 0-745.56N
    smoothedForces[i] = kalmanFilter(forces[i], i);
  }

  // Send data: "f1,f2,f3,f4,f5,f6\n" (forces in N)
  for (int i = 0; i < numSensors; i++) {
    Serial.print(smoothedForces[i], 1);
    if (i < numSensors - 1) Serial.print(",");
  }
  Serial.println();

  delay(10); // 100Hz sampling rate
}