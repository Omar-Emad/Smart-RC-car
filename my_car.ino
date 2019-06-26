int dir1a = 40; // direction for motor A (pin 1) blue 
int speedb = 3;  // black
int dir2a = 41; // direction for motor A (pin 2) purple
int speeda = 2;  //green
int dir1b = 42; // direction for motor B (pin 3) grey
int dir2b = 43; // direction for motor B (pin 4) white
char val;
int sped;

//============================================================================
#include<LiquidCrystal.h>
#include <Servo.h>
#include <NewPing.h> //sonar.ping() / US_ROUNDTRIP_CM
LiquidCrystal lcd(7,8,9,10,11,12);
int curDist=0;
Servo myservo;
int piezo = A0;
int echo=30;
int trig=31;
int ldr=A1;
int smoke=A3;
int temp=A8;
int sound=4;
int ir=A4;
int right_line=24;
int left_line=A15;
int flame=6;
int phi=36;
int buzz = 50;
int led = 51;
int servo=5;
NewPing sonar(trig, echo,300 );




//========= setup ===================================================================
 
void setup ()
{
  Serial.begin(9600);
  lcd.begin(16,2);
  myservo.attach(5);
  pinMode(dir1a, OUTPUT);
  pinMode(dir2a, OUTPUT);
  pinMode(dir1b, OUTPUT);
  pinMode(dir2b, OUTPUT);
  pinMode(speeda, OUTPUT);
  pinMode(speedb, OUTPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo,INPUT);
  pinMode(left_line,INPUT);
  pinMode(phi,INPUT);
  pinMode(led,OUTPUT);
  pinMode(buzz,OUTPUT);
  pinMode(temp,INPUT);
  pinMode(sound,INPUT);
  pinMode(flame,INPUT);
    
}
//============= main==================================================================

void loop()
{

 myservo.write(90);
  curDist = distance();
  if (curDist < 30&&curDist!=0) 
    stope();
    back(4);
    delay(500);
   


//==================== temp =================================================

 float temp_reading=analogRead(temp);
 float tempC=((temp_reading*5.0)/1024.0)*100;
 if(tempC>60) 
  {      
     digitalWrite(buzz, HIGH);
     digitalWrite(led, HIGH);
     delay(800);
     digitalWrite(buzz, LOW);
     digitalWrite(led, LOW);
     delay(600);
  }
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("Temperature in C :");
 lcd.setCursor(4,1);
 lcd.print(tempC);  lcd.print(" ");  lcd.print((char)223);  lcd.print("C");
 delay(1000);


 //================= smoke awy =================================
 int smk=analogRead(smoke);
 if (smk >= 460)
 {
  forward(5);
  delay(1000);
  stope();
 }
 
 
 //================= LDR =========================================
 int LDR=analogRead(ldr);
 if(LDR <= 500)
 digitalWrite(led,HIGH);
 else
 digitalWrite(led,LOW);

//================= flame =========================================

int fire = digitalRead(flame);
if(fire == 1)
{
      digitalWrite(buzz,HIGH);
      digitalWrite(led,HIGH);
      delay(1000);
      digitalWrite(buzz,LOW);
      digitalWrite(led,LOW);
      delay(200);
    
  }
  
//================= sound =========================================
int snd = digitalRead(sound);
if(snd == 1)
{
  left(9);
  delay(2000);
  right(9);
  delay(2000);
  stope();
  }
 
//================= vibiration ====================================
  
  int piz=analogRead (piezo);
  if (piz >=190)
  {
    stope();
    for (int i=0;i<2;i++)
    {
      digitalWrite(buzz,HIGH);
      digitalWrite(led,HIGH);
      delay(2000);
      digitalWrite(buzz,LOW);
      digitalWrite(led,LOW);
      delay(500);
    }
    piz=0;
  }

//================ infrared =================================================

  if(analogRead(ir) >= 500)
  {
    back(5);
    delay(500);
    stope();
  }
  
//================= phi ===================================================  
 
  if(!digitalRead(phi))
  {
    stope ();
    for(int i=0;i<5&&!digitalRead(phi);i++)
    {
      digitalWrite(led,HIGH);
      digitalWrite(buzz,HIGH);
      delay(200);
      digitalWrite(buzz,LOW);
      digitalWrite(led,LOW);
      delay(200);
    }
    digitalWrite(buzz,LOW);
    digitalWrite(led,LOW);
    delay(10);  
  }

//================ loop ============================================

  if(Serial.available()&&digitalRead(phi))
  {
     val=Serial.read();
     Serial.println(val);
     if (val>=48&&val<=57)
     sped=(int)val - 48;
     
     if (val=='F')
     forward(sped);
     else if (val=='I')
     forward_right(sped);
     else if (val=='G')
     forward_left(sped);
     else if (val=='B')
     back(sped);
     else if (val=='J')
     back_right(sped);
     else if (val=='H')
     back_left(sped);
     else if (val=='R')
     right(sped);
     else if (val=='L')
     left(sped);
     else if (val=='S')
     stope();
     else if (val=='V')
     ultrasonic();
     else if (val=='X')
     line();
     else if (val=='U')
     ultrasonic_2();

  }                                                                                                                                                                                                             
}
//============== functions ==========================================================


//================= line follower ====================================================
void line()
{
  while(1)
  {   
    if(!(digitalRead(right_line))&&!(analogRead(left_line)<400))
    forward(5);
    else if(!(digitalRead(right_line))&&(analogRead(left_line)<400))
        right(9);
    else if((digitalRead(right_line))&&(analogRead(left_line)>400))
        left(9);
    if(Serial.read()=='x')
    break;
  }
}



//============= distance ===================================================

int distance() 
{   
  unsigned int uS = sonar.ping();
  int cm = uS/US_ROUNDTRIP_CM;
  return cm;
}
//============ ulreasonic 1 ==================================================
void ultrasonic ()
{
  while (1)
  { 
  curDist = distance();  
  if (curDist < 40 && curDist>2) 
  {
    stope();   
    myservo.write(30); 
    delay(500); 
    int rightDistance = distance();
    delay(500);
    myservo.write(150);
    delay(500); 
    int leftDistance = distance();
    delay(500); 
    myservo.write(90); 
    if (leftDistance>=rightDistance||rightDistance<2) 
    {
        left(9);
        delay(500);
        stope();
    }
    else if (rightDistance>leftDistance||rightDistance<2)
    {
        right(9);
        delay(500);
        stope();
    }
  }
  else 
  forward(3); 
  if(Serial.read()=='v')
   {
    break;
   }
  }
}

//=============ultrasonic 2 ==========================================
void ultrasonic_2 ()
{ 
  while(1)
  {
    curDist = distance();
    if (curDist<20&&curDist>10)
      stope();
    else if(curDist>20)
    forward(4);
    else if (curDist<10)
    back(4);
    if(Serial.read()=='u')
      break;
  }
}


//======================= direction ===========================================================

void forward (int x)
{
  int y=map(x,0,9,0,255);
  digitalWrite (dir1a,HIGH);
  digitalWrite (dir2a,LOW);
  digitalWrite (dir1b,HIGH);
  digitalWrite (dir2b,LOW);
  analogWrite (speeda,y);
  analogWrite (speedb,y);
}

void back(int x)
{
  int y=map(x,0,9,0,255);
  digitalWrite (dir1a,LOW);
  digitalWrite (dir2a,HIGH);
  digitalWrite (dir1b,LOW);
  digitalWrite (dir2b,HIGH);
  analogWrite (speeda,y);
  analogWrite (speedb,y);
}

void right (int x)
{
  int y=map(x,0,9,0,255);
  digitalWrite (dir1a,HIGH);
  digitalWrite (dir2a,LOW);
  digitalWrite (dir1b,LOW);
  digitalWrite (dir2b,HIGH);
  analogWrite (speeda,y);
  analogWrite (speedb,y);
}
void left (int x)
{
  int y=map(x,0,9,0,255);
  digitalWrite (dir1a,LOW);
  digitalWrite (dir2a,HIGH);
  digitalWrite (dir1b,HIGH);
  digitalWrite (dir2b,LOW);
  analogWrite (speeda,y);
  analogWrite (speedb,y);
}
void stope ()
{
  digitalWrite (dir1a,LOW);
  digitalWrite (dir2a,LOW);
  digitalWrite (dir1b,LOW);
  digitalWrite (dir2b,LOW);
  analogWrite (speeda,0);
  analogWrite (speedb,0);
}

void forward_right (int x)
{
  int y=map(x,0,9,0,255);
  digitalWrite (dir1a,HIGH);
  digitalWrite (dir2a,LOW);
  digitalWrite (dir1b,HIGH);
  digitalWrite (dir2b,LOW);
  analogWrite (speeda,y);
  analogWrite (speedb,0);
}
void forward_left (int x)
{
  int y=map(x,0,9,0,255);
  digitalWrite (dir1a,HIGH);
  digitalWrite (dir2a,LOW);
  digitalWrite (dir1b,HIGH);
  digitalWrite (dir2b,LOW);
  analogWrite (speeda,0);
  analogWrite (speedb,y);
}

void back_left (int x)
{
  int y=map(x,0,9,0,255);
  digitalWrite (dir1a,LOW);
  digitalWrite (dir2a,HIGH);
  digitalWrite (dir1b,LOW);
  digitalWrite (dir2b,HIGH);
  analogWrite (speeda,0);
  analogWrite (speedb,y);
}


void back_right (int x)
{
  int y=map(x,0,9,0,255);
  digitalWrite (dir1a,LOW);
  digitalWrite (dir2a,HIGH);
  digitalWrite (dir1b,LOW);
  digitalWrite (dir2b,HIGH);
  analogWrite (speeda,y);
  analogWrite (speedb,0);
}

