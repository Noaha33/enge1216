
//temp1 is the variable where we will input the bluetooth 
//all bluetooth code is there but not included due to issues we were having

#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 8
//#include "SoftwareSerial.h"
//SoftwareSerial mySerial(2, 3);

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

#define P1A 10 // define pin 10as for P1A
#define P2A 11 // define pin 11 as for P2A
#define EN12 9 // define pin 9 as for 1,2EN enable


//void initBluetooth() {
  //mySerial.begin(9600);
  //Serial.begin(9600);
//} 

//void updateSerial() {
  //if(mySerial.available())
    //Serial.write(mySerial.read());

  //if(Serial.available())
    //mySerial.write(Serial.read());
//}

//global vars

float Celsius = 0;
float Fahrenheit = 0;
float MeanTemp = 0;
int i = 0;
float NewFahrenheit = 0;
int roundFahrenheit = 0;

int newTemp = 0; 
int pin=13;
int count = 0;
int motor = 0;
int temp1 = 68;

void setup() {
  pinMode(pin,OUTPUT);
  sensors.begin();
  Serial.begin(9600);
  //initBluetooth();
  Serial.begin(9600);// setup Serial Monitor to display information
  pinMode(P1A, OUTPUT);// define pin as OUTPUT for P1A
  pinMode(P2A, OUTPUT);// define pin as OUTPUT for P2A
  pinMode(EN12, OUTPUT);// define pin as OUTPUT for 1,2EN
  motor = 0; 
  
}

void loop(){

  motor = motor;

  i = 0;
  NewFahrenheit = 0;
  for (i = 0; i < 45; i++) //repeats loop 45 times once every second to get good average data
  {
    sensors.requestTemperatures();
    Celsius = sensors.getTempCByIndex(0);
    Fahrenheit = sensors.toFahrenheit(Celsius);

    NewFahrenheit = (NewFahrenheit + Fahrenheit); // adds each farenheit to a sum to find average later
    Serial.println(Fahrenheit,4); // prints non additive farenheit to ensure its working
    delay(1000); // delay in each reading
    
  }
  MeanTemp = (NewFahrenheit / (45)); // finds mean temp
  Serial.print(MeanTemp,2); // prints mean temp to ensure it is working correctly
  Serial.println(" F");
  newTemp = (MeanTemp-temp1);
  Serial.println(newTemp);
   //dummy var that is supposed to reprsent the 
 
  // all of these turn the motor for a certain amount of time to turn the knob on fan and motor is used so that we
  // can turn it from low to high becuase that will take a different distance than off to high ect..,
  if ((newTemp>5) && (motor == 0)) { //off - high 
    L293D('L',255,1);
    delay(950); 
    L293D('L',255,0);
    motor = 1;
  }
  else if ((newTemp<=5) && (-3<=newTemp) && (motor==0)) { //off - low
    L293D('L',255,1);
    delay(4000);
    L293D('L',255,0);
    motor = 2;
  }
  else if ((newTemp<=5) && (-3<=newTemp) && (motor==1)) { //high - low
    L293D('L',255,1);
    delay(3000);
    L293D('L',255,0);
    motor = 2;
    
  }
  else if ((newTemp>5) && (motor==2)) { //low - high
    L293D('R',255,1);
    delay(3000);
    L293D('R',255,0);
    motor = 1;
  }
  else if ((newTemp<-3) && (motor==1)) { //high - off
    L293D('R',255,1);
    delay(950);
    L293D('R',255,0);
    motor = 0;
  }
  else if ((newTemp<-3) && (motor==2)) { //low - off
    L293D('R',255,1);
    delay(4000);
    L293D('R',255,0);
    motor = 0;
  }
  else if ((newTemp<-3) && (motor==0)) { //off - off
    L293D('R',0,0);
    motor = 0;
  }
  else if ((newTemp>5) && (motor==1)) { //high - high
    L293D('R',0,0);
    motor = 1;
  }
  else if ((newTemp<=5) && (-3<=newTemp) && (motor==2)) { //low-low
    L293D('R',0,0);
    motor = 2;
  }
  delay(1800000); //30 minute delay between tests 
  Serial.println(motor);
  //Need to add delay here for the loop so that we can set how many times it tests temp ect
}


void L293D(char dir,int spd, int en) // This code reverses motor speeds and sets Left and Right as varibles
{
  if(dir =='L')
  {
    if(en ==0){
       Serial.println("Motor Stopped");// Shows motor stopped so I can make sure code is wokring
    }else{
       Serial.print(" Rotating CW: "); // L = clockwise
       Serial.println(spd);// Lets me see speed of motor        
    }
    digitalWrite(EN12 ,en);// Enable 1A and 2A changes polarity
    analogWrite(P1A,spd);
    digitalWrite(P2A,LOW);    
     
    digitalWrite(EN12 ,en);// Disable 1A and 2A changes polarity
    digitalWrite(P1A,LOW);
    analogWrite(P2A,spd);
  }
}

