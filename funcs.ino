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

int Sharp(int pin)
{
  DistanceGP2Y0A21YK Dist;
  Dist.begin(pin);
  int distance = Dist.getDistanceCentimeter();
  if(distance == 0) return(0);
  return(distance);
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
  if((digitalRead(light[1]) == 0 || digitalRead(light[2]) == 0) && (digitalRead(light[4]) == 0 || digitalRead(light[5]) == 0))  //Ausnahme: kreuzung
  {
    intersection();
    return;
  }
  
  int highdspeed = dspeed;
  int lowdspeed = -(dspeed-10);
  
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

void intersection()
{
  lcd.clear();
  lcd.print("intersection"); //debug
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
     motor(200, -200, 500);
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

void print()
{
#ifdef DISPLAY_OUT
#else
  return;
#endif
}













