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

  if(counter == 0)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(pulseIn(pwm, LOW) /50);
    counter = 1;
  }
  else
  {
    lcd.setCursor(0,1);
    lcd.print(pulseIn(pwm, LOW) /50);
    counter = 0;
  }  
  return pulseIn(pwm, LOW) /50;
}

int SENSOR_RAW_WERTE[46] =   { 666, 660, 620, 565, 521, 475, 437, 408, 381, 358, 337, 318, 305, 291, 276, 270, 255, 245, 236, 229, 221, 212, 207, 200, 196, 187, 176, 168, 159, 155, 152, 146, 142, 135, 134, 126, 122, 116, 114, 110, 105, 100,  98,  96,  95,    0 };
int SENSOR_MANIP_WERTE[46] = {  65,  70,  80,  90, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200, 210, 220, 230, 240, 250, 260, 270, 280, 290, 300, 320, 340, 360, 380, 400, 420, 440, 460, 480, 500, 530, 560, 590, 620, 650, 680, 710, 740, 770, 800, 1000 };
int Sharp(int sensor)
{
   sensor = analogRead(sensor);
   int idx=-1;
   for(int i=0; i<45; i++)
   {
      if((sensor<SENSOR_RAW_WERTE[i] && sensor>=SENSOR_RAW_WERTE[i+1]))
       { idx = i; break; }
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
  int highdspeed = dspeed;
  int lowdspeed = -dspeed;
  
  if(digitalRead(light[6]) == 0)
  {
    delay(400);
    motor(highdspeed, lowdspeed);
    delay(200);
    return;
  }
  
  if(digitalRead(light[2]) == 0 || digitalRead(light[4]) == 0)
  {
    while(digitalRead(light[2]) == 0 || digitalRead(light[3]) == 1) motor(lowdspeed, highdspeed);
    while(digitalRead(light[4]) == 0 || digitalRead(light[3]) == 1) motor(highdspeed, lowdspeed);
  }
  else if(digitalRead(light[1]) == 0 || digitalRead(light[5]) == 0)
  {
    while(digitalRead(light[1]) == 0 || digitalRead(light[3]) == 1) motor(lowdspeed, highdspeed);
    while(digitalRead(light[5]) == 0 || digitalRead(light[3]) == 1) motor(highdspeed, lowdspeed);
  }
  delay(10);
}

void search()
{
  int left[] =    {
    dspeed, -dspeed,-dspeed,dspeed,-dspeed,dspeed,dspeed,-dspeed, -dspeed,dspeed};
  int right[] = {
    dspeed,-dspeed,dspeed,dspeed,-dspeed, -dspeed,dspeed,-dspeed,dspeed,dspeed};
  int del[] = {
    800,  700,500,700,700,900,700,700,500,700};

  for(int i=0; i <= (sizeof(del) / sizeof(int))-1 && WHITE; i++)
  {
    motor(right[i], left[i], del[i]);
  }
}

void obstacle()
{
   motor(-200);
   delay(300);
   motor(0);
   if(ultraSonic(trigL,pwmL) < ultraSonic(trigR, pwmR))
   {
     motor(200, -200, 600);
     motor(200);
     delay(300);
     motor(-200, 200, 600);
     motor(200);
     delay(300);
     motor(-200, 200, 600);
     while(digitalRead(light[3]) == 1)
     {
       motor(200);
     }
   }
   else
   {
     motor(-200, 200, 600);
     motor(200);
     delay(300);
     motor(200, -200, 600); 
     motor(200);
     delay(300);
     motor(200, -200, 600); 
     while(digitalRead(light[3]) == 1)
     {
       motor(200);
     } 
   }
}

int cache = 0;
boolean ramp()
{
  if(accelReadz() <= -120 && cache == 0)
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

void print()
{
#ifdef DISPLAY_OUT
#else
  return;
#endif
}













