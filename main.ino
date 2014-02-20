void setup()
{
  //____________________________________SETUP_____________________________________
  //Lichtsensoren
  pinMode(light[1], INPUT);
  pinMode(light[2], INPUT);
  pinMode(light[3], INPUT);
  pinMode(light[4], INPUT);
  pinMode(light[5], INPUT);
  pinMode(light[6], INPUT);

#ifdef ULTRASONIC
  //Ultraschallsensoren
  pinMode(trigL, OUTPUT);
  pinMode(pwmL, INPUT);
  digitalWrite(trigL, HIGH);

  pinMode(trigR, OUTPUT);
  pinMode(pwmR, INPUT);
  digitalWrite(trigR, HIGH);
#endif

  //Motoren
  pinMode(M1, OUTPUT);  
  pinMode(M2, OUTPUT);

  //Drucksensoren
  pinMode(TOUCH1,INPUT); 
  digitalWrite(TOUCH1,HIGH);
  pinMode(TOUCH2,INPUT); 
  digitalWrite(TOUCH2,HIGH);

  //Display
#ifdef DISPLAY_OUT
  lcd.init();                    
  lcd.backlight();
  lcd.clear();
#endif

  lcd.print(VERSION);

  //Servos
  servoPos[SERVO1] = 0;
  servoPos[SERVO2] = 0;
  servoPos[SERVO3] = 0;

  //accelerometer
  accelSetup();
  Serial.begin(9600);
  
#ifdef DEBUG
  while(true) debug();
#else

  //___________________________________PROGRAM_______________________________________
  while(true)
  {
    if(!ramp())
    {
      motor(dspeed);
      
      //Hindernis oder Rettungsmanöver
      if(!digitalRead(TOUCH1) == 1 && !digitalRead(TOUCH2) == 1) //Vordere Drucksensoren
      {
        if(digitalRead(light[3]) == 0) //Mittlerer Lichtwert Schwarz
        {
          motor(0);
          obstacle(); //BUGGY                    
        }
        else
        {
          //Rettungsmanöver: Zurück auf die Linie
          motor(-dspeed);
          delay(700);
        }
      }

      //Suche oder Linie
#ifdef LINE_FOLLOWING
      if(WHITE)
      {
        search();
      }
      else
      {
        followLine();
      }
    }
    else if(ramp())
    {
      motor(250);
    }
#endif
  }
#endif
}




















