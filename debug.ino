//GEDANKENSCHROTT ;D
void debug()
{

}
String *log_arr = new String[999];
int log_i = 0;

void addLog(String string)
{




  log_i++;
}

int getTime(String &time)
{
  int secs = millis()/1000;
  int mins= 0;
  while(secs>=60)
  {
    if(secs>=60)
    {
      secs -= 60;
      mins++;
    }
  }

  String secs2;
  String mins2 = String(mins);
  if(secs <=9) 
  {
    secs2= "0" + String(secs);
  }
  else
  {
    secs2 = String(secs);
  }

  time = "WORKS!";
}

