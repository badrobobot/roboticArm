 #include <Wire.h> 

 byte _address = 0x36;
 
 #define  RAWANGLE    0x0C
 #define  ANGLE       0x0E
 #define  STATUS      0x0B  
 #define  AGC         0x1A
 #define  MAGNITUDE   0x1B  

 float deg; 
 int   origin;
 int   angle;
 
void setup() {
  // put your setup code here, to run once:
  Wire.begin(); // Initiate the Wire library
  Serial.begin(9600);

  // 
  origin = testAngle();
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.print("Angle: ");
  Serial.print(testAngle());
  Serial.print(",");
  delay(1000);
}

int testAngle(){
  Wire.beginTransmission(_address); // Begin transmission to the Sensor 
  Wire.write(RAWANGLE); //Ask the particular registers for data
  Wire.endTransmission(false); // Ends the transmission and transmits the data from the two registers
  Wire.requestFrom(_address,2); // Request the transmitted two bytes from the two registers
  if(Wire.available()<=2) {  // 
        int A = Wire.read() & 0x0F;
        int B = Wire.read();
        //float rawAngle = (A<<8) | B;
        angle = (((A<<8) | B) - origin + 4095) % 4095;
   }
  return angle;
}

float readRawAngle(){
  Wire.beginTransmission(_address); // Begin transmission to the Sensor 
  Wire.write(RAWANGLE); //Ask the particular registers for data
  Wire.endTransmission(false); // Ends the transmission and transmits the data from the two registers
  Wire.requestFrom(_address,2); // Request the transmitted two bytes from the two registers
  if(Wire.available()<=2) {  // 
        int A = Wire.read() & 0x0F;
        int B = Wire.read();
        //float rawAngle = (A<<8) | B;
        angle = (((A<<8) | B) - origin + 4095) % 4095;
        //deg = (float)angle/4095.0*360.0;
        //rad = (float)angle/4095.0*2*PI;
   }
  return angle;
}

int readAngle(){
  Wire.beginTransmission(_address); // Begin transmission to the Sensor 
  Wire.write(ANGLE); //Ask the particular registers for data
  Wire.endTransmission(false); // Ends the transmission and transmits the data from the two registers
  Wire.requestFrom(_address,2); // Request the transmitted two bytes from the two registers
  if(Wire.available()<=2) {  // 
        int A = Wire.read() & 0x0F;
        int B = Wire.read();
        //float rawAngle = (A<<8) | B;
        angle = ((A<<8) | B);
        //deg = (float)angle/4095.0*360.0;
        //rad = (float)angle/4095.0*2*PI;
   }
  return angle;
}

