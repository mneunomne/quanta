#include <Audio.h>
#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include <SerialFlash.h>
#include <Bounce.h>

AudioSynthWaveform       sine2;
AudioSynthWaveform       sine1;
AudioSynthWaveform       sine3;
AudioSynthWaveform       sine4;
AudioSynthWaveform       sine5;
AudioSynthWaveform       sine6;
AudioSynthWaveform       sine7;

AudioSynthWaveform *waves[7] = {
  &sine1, &sine2, &sine3, &sine4,
  &sine5, &sine6, &sine7
};

AudioMixer4              mixer2;   
AudioMixer4              mixer1;   

AudioOutputI2S           i2s1;     

AudioConnection          patchCord1(sine1, 0, mixer1, 0);
AudioConnection          patchCord4(sine2, 0, mixer2, 0);
AudioConnection          patchCord2(sine3, 0, mixer1, 1);
AudioConnection          patchCord3(sine4, 0, mixer2, 1);
AudioConnection          patchCord5(sine5, 0, mixer1, 2);
AudioConnection          patchCord6(sine6, 0, mixer2, 2);
AudioConnection          patchCord7(sine7, 0, mixer1, 3);

AudioConnection          patchCord8(mixer2, 0, i2s1, 1);
AudioConnection          patchCord9(mixer1, 0, i2s1, 0);

AudioControlSGTL5000     sgtl5000_1;     

boolean activated[] = {false, false, false, false, false, false, false}; 

int counter0 = 0;
int isActivated = 0;

int escala[] = {396, 417, 528, 639, 741, 852, 963 };
float amps[] = {0,0,0,0,0,0,0};
float pins[] = {0,0,0,0,0,0,0};
int counters[] = {0,0,0,0,0,0,0};
//pin_vals
int pirs[] = {0, 1, 2, 3, 4, 5, 6};
int led[] = {17, 18, 19, 20, 21, 22, 23};

int count=1;

int frames = 0;

int sensorPin1 = A0;    // select the input pin for the potentiometer
int sensorPin2 = A1;    // select the input pin for the potentiometer
int sensorPin3 = A2;  

int ledPin = 10;  

float sensorValue1 = 0;
float sensorValue2 = 0;
float sensorValue3 = 0;

void setup() {
  AudioMemory(18);  
  Serial.begin(115200);  
  Serial.println("Begin!");
  for(int i = 0; i < 7; i++){
    pinMode(pirs[i], INPUT);    
  }

  //for(int i = 0; i < 7; i++){
    //pinMode(led[i], OUTPUT);    
  //}

  pinMode(ledPin, OUTPUT);
  
  mixer1.gain(0, -0.43);
  mixer1.gain(1, 0.43);
  mixer1.gain(2, -0.43);
  mixer1.gain(3, 0.43);
  mixer2.gain(0, -0.43);
  mixer2.gain(1, 0.43);
  mixer2.gain(2, -0.43);
   
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.6);
  
  // Initialize processor and memory measurements
  int i;
  for(i =0; i < sizeof(escala)/sizeof(int); i++){
    //waves[i]->begin(amps[i] ,escala[i], WAVEFORM_SINE);   
    waves[i]->begin(0.0,escala[i]/2,WAVEFORM_SINE); 
    //waves[i]->pulseWidth(1.0);
  }
  Serial.println("setup done");  
  AudioProcessorUsageMaxReset();
  AudioMemoryUsageMaxReset(); 
  Serial.println("setup done");  
}

unsigned long last_time = millis();
unsigned long last_time2 = millis();
void loop() {
  
  sensorValue1 = analogRead(sensorPin1);
  sensorValue2 = analogRead(sensorPin2);
  sensorValue3 = analogRead(sensorPin3);
  
  float decayRate = (map(sensorValue2, 21, 1023, 0.00005, 0.01));


  
  
  int i;
  for(i =0; i < sizeof(escala)/sizeof(int); i++){               
    waves[i]->amplitude(amps[i]);   
   // waves[i]->phase(360);   
    if(pins[i] > 0){
      pins[i] -= 0.01;
      amps[i] += 0.05;
    } else {
      if(amps[i] > 0.0){        
        amps[i] -= decayRate;
      }
    }
  }   
  
  for(int i = 0; i < 7; i++){
    int v = digitalRead(pirs[i]);
    if(v == 1){ // activated
      if(!activated[i]){
         activated[i] = true;
         Serial.print(pirs[i]);
         Serial.print(" : ");
         Serial.println("Activated!!"); 
         quanta(i);
      }
    } else { // not activated
      if(activated[i]){
        activated[i] = false;
      }
    }
  }
}


void quanta(int index){
  int i = index;
  if(pins[i] < 0.2){
    pins[i] = 1;
  }
  Serial.print(millis());
  Serial.print(" : ");
  Serial.println(amps[i] );
  digitalWrite(ledPin, HIGH);
  delay(20);
  digitalWrite(ledPin, LOW);
  //Serial.println(amps[index]);
}
