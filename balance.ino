#include <Wire.h>
#include <math.h>

int16_t Gyro_X,Gyro_Y,Gyro_Z,Acce_X,Acce_Y,Acce_Z;
double Ax, Ay, Az;

int incomingByte = 0;
bool motor1_cw = false;
bool motor1_ccw = false;
bool motor2_cw = false;
bool motor2_ccw = false;

#define fwd true
#define bck false

#define inf_lim 0.14
#define sup_lim 0.16

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  delay(250);

  Wire.beginTransmission(0x68);
  Wire.write(0x6b);
  Wire.write(0x00);
  Wire.endTransmission();

  pinMode(PA5, OUTPUT);
  pinMode(PA4, OUTPUT);
  pinMode(PC13, OUTPUT);
  pinMode(PC14, OUTPUT);
  pinMode(PC15, OUTPUT);

  //  PWM B1 Y B0
}
void loop() {
  // put your main code here, to run repeatedly:
/*
  Wire.beginTransmission(0x68);
  Wire.write(0x43);
  Wire.endTransmission();
  Wire.requestFrom(0x68,6);
  Gyro_X = Wire.read()<<8 | Wire.read();
  Gyro_Y = Wire.read()<<8 | Wire.read();
  Gyro_Z = Wire.read()<<8 | Wire.read();
*/
  // Serial.println(average_acce_z());

  if(average_acce_z() < sup_lim && average_acce_z() > inf_lim){
     digitalWrite(PC13, LOW); //STOP
  }else{
    digitalWrite(PC13, HIGH); //go

    if(average_acce_z() < inf_lim){
      //Serial.println("fwd");
      motor_a(fwd,100);
      motor_b(fwd,100);
    }
    if(average_acce_z() > sup_lim){
      //Serial.println("bck");
      motor_a(bck,100);
      motor_b(bck,100);
    }
    //Serial.println(average_acce_z());
      
  }


}

double average_acce_z(){
  double average = 0;
  double Ax_z;
  
  for(int i=0;i<10;i++){
    
  Wire.beginTransmission(0x68);
  Wire.write(0x3b);
  Wire.endTransmission();
  Wire.requestFrom(0x68,6);

  Acce_X = Wire.read()<<8 | Wire.read();
  Acce_Y = Wire.read()<<8 | Wire.read();
  Acce_Z = Wire.read()<<8 | Wire.read();

    Ax_z = Acce_Z/16384.0;

    // Serial.println(Ax_z);
  
    
    average = average + Ax_z;
  }
  
  average = average/10;

  return average;
}

void motor_a(bool direction, int speed){
  analogWrite(PB0,speed);    // Motor A
  if(direction){
      digitalWrite(PC14,HIGH);  // Motor A1 fwd
      digitalWrite(PC15,LOW);
  }else{
    digitalWrite(PC14,LOW);  // Motor A2 bck
    digitalWrite(PC15,HIGH);
  }
}

void motor_b(bool direction, int speed){
  analogWrite(PB1,speed);   // Motor B
  if(direction){
    digitalWrite(PA4,HIGH);  // Motor B1 fwd
    digitalWrite(PA5,LOW);
  }else{
    digitalWrite(PA4,LOW);  // Motor B2 bck
    digitalWrite(PA5,HIGH);
  }
}
