void debug()
{
}
int log_i = 0;
String logEvent[999] = {};
void addLog(String string)
{
  logEvent[log_i] = getTime() + string;
  log_i++;
}

String getTime()
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

  return(mins2+ ":" + secs2);
}


