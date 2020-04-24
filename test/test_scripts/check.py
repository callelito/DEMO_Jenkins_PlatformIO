"""
Communication with arduino running.
int G = 0;
int Y = 1;
int R = 2;
int O = 11;

void setup() {
  Serial.begin(115200);
  pinMode(O, OUTPUT);
}

void loop() {
  if(Serial.available() > 0) {
    char data = Serial.read();
    switch( data ) {
      case 'G':
        Serial.println(analogRead(G));
      break;
      case 'Y':
        Serial.println(analogRead(Y));
      break;
      case 'R':
        Serial.println(analogRead(R));
      break;
      case 'H':
        analogWrite(O, 255);
        Serial.println("Ok");
      break;
      case 'M':
        analogWrite(O, 128);
        Serial.println("Ok");
      break;
      case 'L':
        analogWrite(O, 0);
        Serial.println("Ok");
      break;
      default:
        Serial.println("Err");
    }
  }                   
}
"""
import serial, time, sys
arduino = serial.Serial('/dev/ttyACM0', 115200, timeout=.1)
time.sleep(1)
arduino.reset_input_buffer()
arduino.reset_output_buffer()
arduino.readlines()
print ("Running Test")
print ("    Low Test")
arduino.write('L')
time.sleep(0.1)
assert (arduino.readline().rstrip() == "Ok")
time.sleep(0.1)
arduino.write('RGY')
R = int(arduino.readline())
G = int(arduino.readline())
Y = int(arduino.readline())
print ("        R: {}    G: {}    Y: {}".format(R, G, Y))
if G > Y and G > R:
	print ("        [PASSED]")
else:
	sys.exit(1)
print ("    Mid Test")
arduino.write('M')
time.sleep(0.1)
assert (arduino.readline().rstrip() == "Ok")
time.sleep(0.1)
arduino.write('RGY')
R = int(arduino.readline())
G = int(arduino.readline())
Y = int(arduino.readline())
print ("        R: {}    G: {}    Y: {}".format(R, G, Y))
if Y > G and Y > R:
	print ("        [PASSED]")
else:
	sys.exit(1)
print ("    High Test")
arduino.write('H')
time.sleep(0.1)
assert (arduino.readline().rstrip() == "Ok")
time.sleep(0.1)
arduino.write('RGY')
R = int(arduino.readline())
G = int(arduino.readline())
Y = int(arduino.readline())
print ("        R: {}    G: {}    Y: {}".format(R, G, Y))
if R > Y and R > G:
	print ("        [PASSED]")
else:
	sys.exit(1)

sys.exit(0)
