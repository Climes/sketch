void setup()
{
  //============================================================================================SETUP=============================================================================================
  //Lichtsensoren
  pinMode(light[1], INPUT);
  pinMode(light[2], INPUT);
  pinMode(light[3], INPUT);
  pinMode(light[4], INPUT);
  pinMode(light[5], INPUT);

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

  //Servos
  servoPos[SERVO1] = 0;
  servoPos[SERVO2] = 0;
  servoPos[SERVO3] = 0;
  //accelerometer
  accelSetup();
  
  lcd.print(VERSION);
  //======KALIBRIERUNG==========
  while(!digitalRead(TOUCH1) == 0 && !digitalRead(TOUCH2) == 0)
  {
  }
  lcd.clear();

  //Greenshield
  pinMode(LED,OUTPUT);        // SMD-LED
  pinMode(GR_RESET,OUTPUT);        // RESET-Ausgang
  digitalWrite(GR_RESET,LOW);

  for(int i=0; i<5; i++)
  {
    digitalWrite(LED,HIGH);
    delay(500-i*100);
    digitalWrite(LED,LOW);
    delay(100); 
  }
  digitalWrite(GR_RESET,HIGH);
  delay(1000);
  digitalWrite(GR_RESET,LOW);   

  lcd.clear();
  lcd.print("kalibriert");
  while(!digitalRead(TOUCH1) == 0 && !digitalRead(TOUCH2) == 0)
  {
  }
  lcd.clear();

  //============================
#ifdef DEBUG
  debug();
#else
  //attachInterrupt(4, Greenshield, FALLING);
  //delay(100);
  //===========================================================================================PROGRAM=============================================================================================
  while(true)
  {
    if(counter >= 100) 
    {
      counter = 0;
    }
    else 
      counter++;

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
    if(WHITE)
    {
      search();
    }
    else if(digitalRead(light[3] == 1))
    {
      followLine();
      //-->Implementierung Greenshield
    }
    /*if(!digitalRead(GR_INPUT) == 1)
     {
     if (digitalRead(light[1]) == 0 || digitalRead(light[2]) == 0 || digitalRead(light[4]) == 0 || digitalRead(light[5]) == 0) Greenshield();
     }*/
  }
#endif
}














