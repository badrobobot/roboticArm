const int encoderPin = 2;
const int ledPin =  13; 

const int timeConst = 5;

volatile unsigned int cuenta = 0;
volatile int encoderState = 0; 
double rpms = 0; 

float steps = 0;

void setup() {
  // put your setup code here, to run once:

  // Start serial comunication
  Serial.begin(9600);
    
  //Configuracion del timmer2
  SREG = (SREG & 0b01111111); //Desabilitar interrupciones
  TIMSK2 = TIMSK2|0b00000001; //Habilita la interrupcion por desbordamiento
  TCCR2B = 0b00000111; //Configura preescala para que FT2 sea de 7812.5Hz
  //TCCR2B = 0b00000001; //Configura preescala para que FT2 sea de 8MHz
  SREG = (SREG & 0b01111111) | 0b10000000; //Habilitar interrupciones //Desabilitar interrupciones
}

void loop() {
  // put your main code here, to run repeatedly:
  sendToPC(&rpms);
  //Serial.println(rpms);
  //delay(100);
}


// Timer2 interrupt
ISR(TIMER2_OVF_vect){
  // calculate rpms 
  rpms = timeConst*steps; 
  rpms = rpms/100;
  // Calculate the chage in position
  steps=0;  
}


// Sent bytes over serial bus
void sendToPC(double* data)
{
  byte* byteData = (byte*)(data);
  Serial.write(byteData, 4);
}
