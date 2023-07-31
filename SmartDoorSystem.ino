/* An phim tren dieu kien:
    0 la che do dong cua co bao dong
    1 la che do mo cua tu dong
    2 la che do mo cua thu cong:
      Phim mui ten xuong la dong cua
      Phim mui ten len la mo cua */    

#include <IRremote.h>
#include <LiquidCrystal.h>
#include <Servo.h>

#define trigPin 10
#define echoPin 9
#define buzzer 8
#define irPin 7
#define servoPin 6

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
Servo door;
int pos=180;
bool war, open;
long duration;
float distanceInch;
int timer;

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(irPin);
  lcd.begin(16,2);
  door.attach(servoPin);
  door.write(pos);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzer, OUTPUT);
}
void loop() {
  if (IrReceiver.decode()) {
    Serial.println(IrReceiver.decodedIRData.command);    
    switch(IrReceiver.decodedIRData.command){
      case 22:
        lcd.clear();
        lcdClose(); 
        doorClose();
        break;
      case 12:
        autoMode();   
        break ;
      case 24:
        notAutoMode();
        break;                                    
    }
    IrReceiver.resume();
  }
}

void doorOpen(){
  while(1==1){  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  distanceInch = duration * 0.0133 /2;

  if(distanceInch<10)
  {
    servoOpen();
  } else {
    delay(1000);
    servoClose();
  }
    
  if (IrReceiver.decode()) {      
    if (IrReceiver.decodedIRData.command==22){
      lcd.clear();
      doorClose();
      break;        
    } else if(IrReceiver.decodedIRData.command==24) {
      notAutoMode();
      break;
    }
    IrReceiver.resume();
    }    
  }
}

void notAuto(){
  while(1==1){
    if (IrReceiver.decode()) {
Serial.println(IrReceiver.decodedIRData.command);      
      switch(IrReceiver.decodedIRData.command){
        case 9:
          servoOpen();
          break;
        case 7:
          servoClose();
          break;
      }
      IrReceiver.resume();
    }
    if (IrReceiver.decode()) {      
      if (IrReceiver.decodedIRData.command==22){
        lcd.clear();
        doorClose();
        break;        
      } else if (IrReceiver.decodedIRData.command==12){
        autoMode();
        break;
    }
    IrReceiver.resume();
    }
  } 
}


void doorClose(){
  if(open){
    servoClose();
  }  
  while(1==1){  
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);

    distanceInch = duration * 0.0133 /2;
    Serial.println(distanceInch);
    if(distanceInch<7){
      if(war==false) lcd.clear();
      war=true;
      lcdWarning();
      digitalWrite(buzzer, HIGH);
      delay(50);
      digitalWrite(buzzer, LOW);
      timer = distanceInch * 20;
      delay(timer);
    } else {
      if(war) lcd.clear();       
      lcdClose();
      war=false;
    }
      
    if (IrReceiver.decode()) {      
      if (IrReceiver.decodedIRData.command==12){
        autoMode();
        break;        
      } else if (IrReceiver.decodedIRData.command==24) {
        notAutoMode();
        break;        
      }
      IrReceiver.resume();
    }    
  }
}

void lcdClose(){     
  lcd.setCursor(5, 0);
  lcd.print("SORRY");
  lcd.setCursor(2, 1);
  lcd.print("WE'RE CLOSED");
}

void lcdOpen(){
  lcd.setCursor(4, 0);
  lcd.print("WELCOME");
  lcd.setCursor(3, 1);
  lcd.print("WE'RE OPEN");
}

void lcdWarning(){ 
  lcd.setCursor(4, 0);
  lcd.print("WARNING");
  lcd.setCursor(3, 1);
  lcd.print("STAY AWAY");
}

void servoOpen(){
  pos=0;
	door.write(pos);  
	delay(500);
  open=true;
}

void servoClose(){
	pos=180;  
  door.write(pos);  
  delay(500);
  open=false;  
}

void autoMode(){
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("AUTO MODE: ON");
  delay(700);
  lcd.clear();
  lcdOpen();
  doorOpen();
}

void notAutoMode(){
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("AUTO MODE: OFF");
  delay(700);
  lcd.clear();
  lcdOpen();
  notAuto();
}

  