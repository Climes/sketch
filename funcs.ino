void motor(int rechts, int links=1,int del=0)
{
  if(links == 1) links = rechts;
  boolean richtr = HIGH;
  boolean richtl = HIGH;
  if(rechts < 0)
  {
    richtr = LOW;
    rechts = rechts * rechts / rechts;
  }
  if(links < 0)
  {
    richtl = LOW;
    links = links * links / links;
  }
  digitalWrite(M1, richtr);
  digitalWrite(M2, richtl);
  analogWrite(E1, rechts);
  analogWrite(E2, links);
  delay(10);
  if(del >0)delay(del);
}

void turn(int rechts, int links=1)
{
  if(links == 1) links = rechts;
  boolean richtr = HIGH;
  boolean richtl = HIGH;
  if(rechts < 0)
  {
    richtr = LOW;
    rechts = rechts * rechts / rechts;
  }
  if(links < 0)
  {
    richtl = LOW;
    links = links * links / links;
  }
  counterLeft = 0;
  counterRight = 0;
  attachInterrupt(ODOML, countLeft, FALLING);
  attachInterrupt(ODOMR, countRight, FALLING);

  while(counterLeft <links || counterRight <rechts)
  {
    digitalWrite(M1, richtr);
    digitalWrite(M2, richtl);

    if(counterRight <rechts)
      analogWrite(E1, rechts);
    else if(links != rechts)
      analogWrite(E1, 0);

    if(counterLeft <links)
      analogWrite(E2, links);
    else if(links != rechts)
      analogWrite(E2, 0);
  }

  detachInterrupt(ODOML);
  detachInterrupt(ODOMR);
}

void countLeft() {
  counterLeft++;
}
void countRight(){
  counterRight++;
}

int ultraSonic(int trig, int pwm)
{
  digitalWrite(trig, LOW);
  digitalWrite(trig, HIGH);
  return pulseIn(pwm, LOW) /50;
}

int SENSOR_RAW_WERTE[46] =   { 
  666, 660, 620, 565, 521, 475, 437, 408, 381, 358, 337, 318, 305, 291, 276, 270, 255, 245, 236, 229, 221, 212, 207, 200, 196, 187, 176, 168, 159, 155, 152, 146, 142, 135, 134, 126, 122, 116, 114, 110, 105, 100,  98,  96,  95,    0 };
int SENSOR_MANIP_WERTE[46] = {  
  65,  70,  80,  90, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200, 210, 220, 230, 240, 250, 260, 270, 280, 290, 300, 320, 340, 360, 380, 400, 420, 440, 460, 480, 500, 530, 560, 590, 620, 650, 680, 710, 740, 770, 800, 1000 };
int Sharp(int sensor)
{
  sensor = analogRead(sensor);
  int idx=-1;
  for(int i=0; i<45; i++)
  {
    if((sensor<SENSOR_RAW_WERTE[i] && sensor>=SENSOR_RAW_WERTE[i+1]))
    { 
      idx = i; 
      break; 
    }
  }
#define MO (SENSOR_MANIP_WERTE[idx+1])
#define MU (SENSOR_MANIP_WERTE[idx])
#define RO (SENSOR_RAW_WERTE[idx+1])
#define RU (SENSOR_RAW_WERTE[idx])
  return MO - ((sensor-RO)*(MO-MU)/(RU-RO)) - 40;
#undef MO
#undef MU
#undef RO
#undef RU
}

//-----------------------------Wire.h-additions-----------------------------------

//Writes val to address register on device
void writeTo(int device, byte address, byte val) {
  Wire.beginTransmission(device); //start transmission to device
  Wire.write(address);        // send register address
  Wire.write(val);        // send value to write
  Wire.endTransmission(); //end transmission
}

//reads num bytes starting from address register on device in to buff array
void readFrom(int device, byte address, int num, byte buff[]) {
  Wire.beginTransmission(device); //start transmission to device
  Wire.write(address);        //sends address to read from
  Wire.endTransmission(); //end transmission

    Wire.beginTransmission(device); //start transmission to device
  Wire.requestFrom(device, num);    // request 6 bytes from device

  int i = 0;
  while(Wire.available())    //device may send less than requested (abnormal)
  {
    buff[i] = Wire.read(); // receive a byte
    i++;
  }
  Wire.endTransmission(); //end transmission
}
//---------------------------------------------------------------------------------------

void accelSetup()
{
  Wire.begin();
  writeTo(DEVICE, 0x2D, 0);     
  writeTo(DEVICE, 0x2D, 16);
  writeTo(DEVICE, 0x2D, 8);
}

int accelRead(int d)
{
  int regAddress = 0x32;    //first axis-acceleration-data register on the ADXL345
  int x, y, z;

  readFrom(DEVICE, regAddress, TO_READ, buff); //read the acceleration data from the ADXL345

  //each axis reading comes in 10 bit resolution, ie 2 bytes.  Least Significat Byte first!!
  //thus we are converting both bytes in to one int
  x = (((int)buff[1]) << 8) | buff[0];  
  y = (((int)buff[3])<< 8) | buff[2];
  z = (((int)buff[5]) << 8) | buff[4]; 

  //It appears that delay is needed in order not to clog the port
  delay(50);

  switch(d)
  {
  case 1:
    return(x);
  case 2:
    return(y);
  case 3:
    return(z);
  }
}

int accelReadx(){ 
  return(accelRead(1));
}
int accelReady() {
  return(accelRead(2));
}
int accelReadz(){ 
  return(accelRead(3));
}

int moveServo(int servo,int pos,int lenght = 50)
{          
  Servo myservo;  
  myservo.attach(servo); 
  if (servoPos[servo] < pos) 
  {
    for(servoPos[servo] ; servoPos[servo] <=pos; servoPos[servo]++)     
    {                                
      myservo.write(servoPos[servo]);            
      delay(lenght);                      
    }
  }
  else
  {
    for(servoPos[servo]; servoPos[servo] >=pos; servoPos[servo]--)  
    {                                 
      myservo.write(servoPos[servo]);              
      delay(lenght);                    
    } 
  }
}
//____________________________________PARTS_____________________________________
void followLine()
{
  int highdspeed = dspeed + (190-dspeed);
  int lowdspeed  = -(dspeed + (190-dspeed));

  if(digitalRead(light[6]) == 0)
  {
    if(accelReadz()<= -120) ;
    else
    {
      motor(highdspeed, lowdspeed);
      delay(500);
      motor(0);
      delay(100);
      return;
    }
  }

  if((digitalRead(light[2]) == 0 || digitalRead(light[4]) == 0)  && digitalRead(light[3]) == 1)
  {
    if(digitalRead(light[2]) == 0) while(digitalRead(light[3]) == 1 && digitalRead(light[4]) == 1 && !(WHITE) && digitalRead(light[6]) == 1) motor(lowdspeed, highdspeed);
    if(digitalRead(light[4]) == 0) while(digitalRead(light[3]) == 1 && digitalRead(light[2]) == 1 && !(WHITE) && digitalRead(light[6]) == 1) motor(highdspeed, lowdspeed);
  }
  if(digitalRead(light[1]) == 0 || digitalRead(light[5]) == 0)
  {
    if(digitalRead(light[1]) == 0) while(digitalRead(light[4]) == 1 && digitalRead(light[6]) == 1) motor(-dspeed, dspeed);
    if(digitalRead(light[5]) == 0) while(digitalRead(light[2]) == 1 && digitalRead(light[6]) == 1) motor(highdspeed, lowdspeed);
  }

  delay(10);
}

void search()
{
  int left[] =    {
    dspeed, -dspeed,-dspeed,dspeed,-dspeed,dspeed,dspeed,-dspeed, -dspeed,dspeed      };
  int right[] = {
    dspeed,-dspeed,dspeed,dspeed,-dspeed, -dspeed,dspeed,-dspeed,dspeed,dspeed      };
  int del[] = {
    80,  70,50,70,70,90,70,70,50,70      };

  for(int i=0; (i <= (sizeof(del) / sizeof(int))-1) && (WHITE) && rampup() == false && !digitalRead(TOUCH1) == 0 && !digitalRead(TOUCH2) == 0; i++)
  {
    if(mil())
    {
      zCache = accelReadz();
    }
    for(int a=0; a <= del[i] && (WHITE) && !digitalRead(TOUCH1) == 0 && !digitalRead(TOUCH2) == 0 && rampup() == false; a++)
    { 
      motor(right[i], left[i]);
      delay(1);
    }
    zCache = 999;
  }
}

void obstacle()
{
  motor(-200);
  delay(300);
  motor(0);
  if(Sharp(SHARP1) > Sharp(SHARP2))
  {
    motor(200, -200, 800);
    motor(200);
    delay(500);
    motor(-200, 200, 800);
    motor(200);
    delay(500);
    motor(-200, 200, 500);
    while(digitalRead(light[3]) == 1)
    {
      motor(200);
    }
  }
  else
  {
    motor(-200, 200, 800);
    motor(200);
    delay(500);
    motor(200, -200, 800); 
    motor(200);
    delay(500);
    motor(200, -200, 800); 
    while(digitalRead(light[3]) == 1)
    {
      motor(200);
    } 
  }
}

int cache = 0;
long oldmillis = 0;

boolean mil()
{
  if(millis()>(oldmillis + 200))
  {
    oldmillis = millis();
    return true;
  }
  else
  {
    return false;
  }
}

boolean rampup()
{
  if(zCache<= -120 && cache == 0 && zCache != 999)
  {
    cache = 1;
    return true;
  }
  if(cache == 1)
  {
    if(accelReadz() >= -60)
    {
      cache = 0;
      return false;
    }
    return true;
  }
  else
  {
    return false;
  }

}

int cache2 = 0;
boolean rampdown()
{
  if(zCache >= 50 && cache2 == 0 && zCache != 999)
  {
    cache2 = 1;
    return true;
  }
  if(cache2 == 1)
  {
    if(accelReadz() <= -20)
    {
      cache2 = 0;
      return false;
    }
    return true;
  }
  else
  {
    return false;
  }

}

void print()
{
#ifdef DISPLAY_OUT
#else
  return;
#endif
}













