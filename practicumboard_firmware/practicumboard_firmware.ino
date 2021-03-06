int xpin = PIN_PC0;                  // x-axis of the accelerometer
int ypin = PIN_PC2;                  // y-axis
int zpin = PIN_PC4;                  // z-axis (only on 3-axis models)  
int motor = PIN_PC1;
int switchPin = PIN_PD1;
int btstate = 3;
int prog_state = 0;
int countround = 0;
//run data on [0]
//walk data on [1]
float avg_value[2] = {0};
float time_value[2] = {0};
float runavg = 0;
float walkavg = 0;
int countsw = 1;
void setup()
{
    pinMode(motor,OUTPUT);
    pinMode(switchPin,INPUT_PULLUP); 
}
void loop()
{
    btstate = digitalRead(switchPin);
    if(btstate == LOW and prog_state == 0)
    {
        countsw += 1;
        wait_for_release();
    }

    // set first switch
    if(countsw==2 and prog_state == 0) //state 1 loop
    {
        prog_state = 1;        
    }
    else if(prog_state == 1 /*and runsuc*/){
      //run loop
      countsw = 1;
      //Serial.println("you are in state 1 >> run loop");
      avg();
      //re sw in the loop
      countsw = 1;
      prog_state = 2;
      
    }
    else if(prog_state == 2 ){
      //walk loop
      avg();
      countsw = 1;
      prog_state = 3;
    }
    else if(prog_state == 3){
      //stop counting and run motor loop
        for(int a=0;a<time_value[0];a++)
        {
          digitalWrite(motor,1);
          delay(100);
          digitalWrite(motor,0);
          delay((time_value[0]/(avg_value[0]*(time_value[0]/10))/1.2)*1000); //time : 1step
        }
        for(int b=0;b<time_value[1];b++)
        {
          digitalWrite(motor,1);
          delay(100);
          digitalWrite(motor,0);
          delay((time_value[1]/(avg_value[1]*(time_value[1]/10))/1.2)*1000); //time : 1step
        }
    }
}

void avg()
{
  float times = 0;
  float sum = 0;
  while(true)
  { 
     int x = analogRead(xpin);  //read from xpin
   
     int y = analogRead(ypin);  //read from ypin
   
    times += 0.7;
    float xt = ((float)x - 331.5)/65*9.8;
    float yt = ((float)y - 329.5)/68.5*9.8;
    //float zt = ((float)z - 340)/68*9.8;
    float vector = sqrt(pow(xt,2)+pow(yt,2));
    sum = sum + vector;
    btstate = digitalRead(switchPin);
    if(btstate==LOW)
    {
          countsw += 1;
          wait_for_release();
    }
    if(countsw == 2) 
    {
      float avg = sum/times;
      avg_value[countround] = avg;
      time_value[countround] = times;
      countround+=1;
      break;
    }
    delay(700);
  }
}

void wait_for_release()
{
    delay(10);
    while(digitalRead(switchPin)==LOW)
    {}
    delay(10);  
}

 //low = 0
 //high = 1 

