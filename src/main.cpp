#ifdef ARDUINO
#include <Arduino.h>

int GREEN_PIN = 11;
int RED_PIN = 9;
int YELLOW_PIN = 10;

int SENSOR_PIN = 1;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  Serial.begin(9600);
  while (!Serial) ;
}

void writeRGY(float red, float green, float yellow) {
  int linear_bright[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 13, 16, 18, 23, 26, 
    32, 36, 45, 50, 64, 74, 90, 105, 128, 145, 181, 205, 255
    };
  int ri = min(28*abs(red), 28);
  int gi = min(28*abs(green), 28);
  int yi = min(28*abs(yellow), 28);
  analogWrite(RED_PIN, linear_bright[ri]);
  analogWrite(GREEN_PIN, linear_bright[gi]);
  analogWrite(YELLOW_PIN, linear_bright[yi]);
}
int count = 0;
const float low_thrs = 1.;
const float mid_thrs = 2.7;
const float high_thrs = 3.5;
#define LOW_MID_DEL (mid_thrs-low_thrs)
#define MID_HIGH_DEL (high_thrs-mid_thrs)
// the loop function runs over and over again forever
void loop() {
  float v = analogRead(SENSOR_PIN)*5. / 1024;
  count = (count + 1) % 1000;
  if (count == 0) {
    Serial.print(v);
    Serial.println("v");
  }
  if (v < low_thrs) {
    writeRGY(0.,1.,0.);
  } else if (v < mid_thrs) {
    float dv = v - low_thrs;
    float g = (LOW_MID_DEL-dv)/LOW_MID_DEL;
    float y = dv/LOW_MID_DEL;
    writeRGY(0.,g,y);
  } else if (v < high_thrs) {
    float dv = v - mid_thrs;
    float y = (MID_HIGH_DEL-dv)/MID_HIGH_DEL;
    float r = dv/MID_HIGH_DEL;
    writeRGY(r,0.,y);
  } else {
    writeRGY(1.,0.,0.);
  }
  delay(1);
}
#else
int main(void) {
  return 0;
}
#endif
