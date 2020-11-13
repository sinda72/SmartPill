#include <LiquidCrystal.h>
#include <Servo.h> 
#include <TimeLib.h>
#include <time.h>


#include "SoftwareSerial.h"
SoftwareSerial mySerial(10, 11);
# define Start_Byte 0x7E
# define Version_Byte 0xFF
# define Command_Length 0x06
# define End_Byte 0xEF
# define Acknowledge 0x00 //Returns info with command 0x41 [0x01: info, 0x00: no info]
# define ACTIVATED LOW


int led0 = 7;
int buttonStop = 6;
boolean isPlaying = false;
boolean bLedOn = false;
boolean bSwOn = false;



const int rs = 12, en = 9, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
int servoPin = 8;
int sem=0;
Servo servo; 
int angle = 0; // servo position in degrees 

//사용자 입력 변수
int hourr = 2;
int minu = 30;

//현재시간
int nhour=0;
int nminu=0;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  Serial.begin(9600);
  digitalWrite(led0, LOW);
  pinMode(buttonStop, INPUT);
  digitalWrite(buttonStop,HIGH);
  
  playFirst();
  isPlaying = false;
  
  pinMode(led0, OUTPUT);

  //현재시간
  setTime(2,30,0,1,11,11); 
  //adjustTime(adjustment);
  
  servo.attach(servoPin); 

  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print(" TODAY is 06/24");
}



  void playFirst()
  {
    execute_CMD(0x3F, 0, 0);
    delay(500);
    setVolume(20);
    delay(500);
    execute_CMD(0x11,0,1); 
    delay(500);
  }
  
  void pause()
  {
    execute_CMD(0x0E,0,0);
    delay(500);
  }
  
  void play()
  {
    execute_CMD(0x0D,0,1); 
    delay(500);
  }
  
  void playNext()
  {
    execute_CMD(0x01,0,1);
    delay(500);
  }
  
  void playPrevious()
  {
    execute_CMD(0x02,0,1);
    delay(500);
  }
  
  void setVolume(int volume)
  {
    execute_CMD(0x06, 0, volume); // Set the volume (0x00~0x30)
    delay(2000);
}

void execute_CMD(byte CMD, byte Par1, byte Par2)
// Excecute the command and parameters
{
// Calculate the checksum (2 bytes)
word checksum = -(Version_Byte + Command_Length + CMD + Acknowledge + Par1 + Par2);
// Build the command line
byte Command_line[10] = { Start_Byte, Version_Byte, Command_Length, CMD, Acknowledge,
Par1, Par2, highByte(checksum), lowByte(checksum), End_Byte};
//Send the command line to the module
for (byte k=0; k<10; k++)
  {
  mySerial.write( Command_line[k]);
 }
}

void moto(){
  for(angle = 90; angle > 0; angle--) 
  { 
    servo.write(angle); 
    delay(5); 
  } 
  // now scan back from 180 to 0 degrees
  for(angle = 0; angle < 90; angle++) 
  { 
    servo.write(angle); 
    delay(5);
  } 
 }

void loop() {   
  
  lcd.setCursor(5, 1);
  lcd.print(hourr);
  lcd.setCursor(7, 1);
  lcd.print(":");
  lcd.setCursor(8, 1);
  lcd.print(minu);

  nhour = hour();
  nminu= minute();

   if (digitalRead(buttonStop) == ACTIVATED)
  {    
  if(bSwOn == false) {
      bSwOn = true;
      if(bLedOn == false){
        digitalWrite(led0, LOW);
        bLedOn = true;
        } else {
          digitalWrite(led0, HIGH);
          bLedOn = false;
          }
        }
    
    if(isPlaying)
    {
      pause();
      isPlaying = false;
    }else
    {
      isPlaying = true;
      play();
    }
  }
 else {
        bSwOn = false;
      } 
  
  if(hourr== nhour){
    if(minu==nminu){
      if(sem==0){
        moto();
        digitalWrite(led0, HIGH);
        playFirst();
        isPlaying = true;
        Serial.println("start\n");
        sem=1;
      }        
    }
  }
  
}
