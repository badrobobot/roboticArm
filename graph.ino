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

 double rpms = 0; 
 double posA = 0;
 double posB = 0;
 float steps = 0;

 const int timeConst = 5;
 
void setup() {
  // put your setup code here, to run once:
  Wire.begin(); // Initiate the Wire library

  // Start serial comunication
  Serial.begin(9600);

  //Configuracion del timmer2
  SREG = (SREG & 0b01111111); //Desabilitar interrupciones
  TIMSK2 = TIMSK2|0b00000001; //Habilita la interrupcion por desbordamiento
  TCCR2B = 0b00000111; //Configura preescala para que FT2 sea de 7812.5Hz
  //TCCR2B = 0b00000001; //Configura preescala para que FT2 sea de 8MHz
  SREG = (SREG & 0b01111111) | 0b10000000; //Habilitar interrupciones //Desabilitar interrupciones
  
  // Set initial position as zero
  origin = testAngle();
}

void loop() {
  // put your main code here, to run repeatedly:

  //Print Angle Over Serial Bus
  Serial.println(testAngle());

  // Sent bytes over serial bus
  sendToPC(&rpms);
  //delay(1000);
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

// Sent bytes over serial bus
void sendToPC(double* data)
{
  byte* byteData = (byte*)(data);
  Serial.write(byteData, 4);
}

// Timer2 interrupt
ISR(TIMER2_OVF_vect){
  // Calculate the chage in position
  posB = readRawAngle();
  steps = posB- posA; 
  posA = posB;
  
  // calculate rpms 
  rpms = timeConst*steps; 
  rpms = rpms/100;
}

