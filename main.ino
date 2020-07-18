//Pin definitions
#define RELAY_STATE 2
#define AMP_SIGNAL 3

//Solution 1 --> Threshold levels
int reading;
int lowerLimit = 230;
int higherLimit = 660;

//Solution 2 --> 2 claps required
bool clap1=false;
bool clap2=false;

//Solution 3 --> Interval requirement
int clapShortInterval= 500;
int clapLongInterval = 1000;

//Solution 4 --> Silence requirements
int silenceShortInterval= 200;
int silenceLongInterval= 500;

//Timers
unsigned long checkClap;
unsigned long currentTimer;

void setup(){ 
  digitalWrite(RELAY_STATE,LOW);
  pinMode(RELAY_STATE,OUTPUT); 
}

void loop() {  
  checkReading();
  triggerRelay();
}

void switchRelay(){
  if(digitalRead(RELAY_STATE)==HIGH){
    digitalWrite(RELAY_STATE,LOW);
 }
  else{ 
    digitalWrite(RELAY_STATE,HIGH);
  }
}

void checkReading(){
  reading = analogRead(AMP_SIGNAL);
  Serial.println(reading);
}

void triggerRelay(){
  if ((reading > higherLimit || reading < lowerLimit) && clap1==false && clap2==false){
    clap1=true;
    checkClap=millis();
}

//Clap 1 is heard --> check for Clap 2
  while(clap1==true){
    currentTimer=millis();
    if (currentTimer-checkClap>=clapShortInterval && currentTimer-checkClap<= clapLongInterval){
      checkReading();
        if ((reading > higherLimit || reading < lowerLimit)&& clap2==false){
          clap2=true;
          clap1=false;
          checkClap=millis();
        }
    }
  //If Clap 2 is not heard within interval, reset the system
  else if (currentTimer-checkClap>clapLongInterval){
    clap1=false;
  }
}


//Clap 2 is heard --> check for silence
while(clap2==true){
  currentTimer=millis();
  if (currentTimer-checkClap>=silenceShortInterval){
      checkReading();
      if (currentTimer-checkClap>=silenceLongInterval){
        switchRelay();
        clap2=false;
        delay(1500); //avoids picking up relay switching (clicking) noise
    }
      //If there is noise after the silence interval, reset the system
      else if(reading > higherLimit || reading < lowerLimit && currentTimer-checkClap<silenceLongInterval ){
        clap2=false;
      }
    }
  }
}
