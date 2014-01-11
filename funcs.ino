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
    if(counterLeft <links)
      analogWrite(E2, links);
  }

  detachInterrupt(ODOML);
  detachInterrupt(ODOMR);
}

void countLeft() {
  counterLeft++;
  //test
}
void countRight(){
  counterRight++;
}

int ultraSonic(int trig, int pwm)
{
  digitalWrite(trig, LOW);
  digitalWrite(trig, HIGH);

  if(oldcounter != counter)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(pulseIn(pwm, LOW) /50);
    oldcounter = counter;
  }
  else
  {
    lcd.setCursor(0,1);
    lcd.print(pulseIn(pwm, LOW) /50);
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

int moveServo(int servo,int start,int stops,int lenght = 50)
{          
  Servo myservo;  
  myservo.attach(servo); 
  if ( start < stops ) 
  {
    for(int pos =start ; pos <=stops; pos++)     
    {                                
      myservo.write(pos);            
      delay(lenght);                      
    }
  }
  else
  {
    for(int pos =start; pos >= stops; pos--)  
    {                                 
      myservo.write(pos);              
      delay(lenght);                    
    } 
  }
}

//============================================================================================PARTS===========================================================================================

void followLine()
{
  int highdspeed = dspeed;
  int lowdspeed = dspeed-30;

  if(digitalRead(light[2]) == 0 || digitalRead(light[4]) == 0)
  {
    if(digitalRead(light[2]) == 0) motor(highdspeed, -lowdspeed);
    if(digitalRead(light[4]) == 0) motor(-lowdspeed, highdspeed);
  }
  else if(digitalRead(light[1]) == 0 || digitalRead(light[5]) == 0)
  {
    while(digitalRead(light[1]) == 0 && digitalRead(light[3]) != 0) motor(highdspeed, -lowdspeed);
    while(digitalRead(light[5]) == 0 && digitalRead(light[3]) != 0)  motor(-lowdspeed, highdspeed);
  }
  delay(10);
}


void search()
{
  int left[] =    {
    dspeed, -dspeed,-dspeed,dspeed,-dspeed,dspeed,dspeed,-dspeed, -dspeed,dspeed    };
  int right[] = {
    dspeed,-dspeed,dspeed,dspeed,-dspeed, -dspeed,dspeed,-dspeed,dspeed,dspeed    };
  int del[] = {
    800,  700,400,700,700,900,700,700,500,700    };


  for(int i=0; i <= (sizeof(del) / sizeof(int))-1; i++)
  {
    motor(right[i], left[i], del[i]);
  }
}

void obstacle()
{
  /*
  OnFwd(BACKWARD, dspeed, dspeed);
  delay(500);
  while(ultraSonic(trigL,pwmL) > 10) OnFwd(RIGHT, dspeed, dspeed);
  while(Sharp(SHARP1) > 20) OnFwd(FORWARD, dspeed, dspeed);
  while(Sharp(SHARP1) < 20) OnFwd(LEFT, dspeed, dspeed);
  while(ultraSonic(trigL,pwmL) < 15) OnFwd(FORWARD, dspeed, dspeed);
  while(ultraSonic(trigL,pwmL) > 10) OnFwd(LEFT, dspeed, dspeed); 
  while(light[3] = 1) OnFwd(FORWARD, dspeed, dspeed);*/
}

void print()
{
#ifdef DISPLAY_OUT
#else
  return;
#endif
}










