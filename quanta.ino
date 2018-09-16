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


int escala[] = {396, 417, 528, 639, 741, 852, 963 };

//int escala[] = {100, 200, 300, 400, 500, 600, 700};

float amps[] = {0,0,0,0,0,0,0};


float pins[] = {0,0,0,0,0,0,0};

int count=1;

int frames = 0;

int sensorPin1 = 0;    // select the input pin for the potentiometer
int sensorPin2 = 1;    // select the input pin for the potentiometer

int sensorValue1 = 0;  // variable to store the value coming from the sensor
int sensorValue2 = 0;  // variable to store the value coming from the sensor


void setup() {
  AudioMemory(18);  
  Serial.begin(115200);  
  
  Serial.println("Begin!");

  pinMode(sensorPin1, INPUT);
  pinMode(sensorPin2, INPUT);  

  mixer1.gain(0, 0.43);
  mixer1.gain(1, 0.43);
  mixer1.gain(2, 0.43);
  mixer1.gain(3, 0.43);
  mixer2.gain(0, 0.43);
  mixer2.gain(1, 0.43);
  mixer2.gain(2, 0.43);
   
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.6);
  
  // Initialize processor and memory measurements
  int i;
  for(i =0; i < sizeof(escala)/sizeof(int); i++){
    //waves[i]->begin(amps[i] ,escala[i], WAVEFORM_SINE);   
    waves[i]->begin(0.0,escala[i],WAVEFORM_SINE); 
  }

  AudioProcessorUsageMaxReset();
  AudioMemoryUsageMaxReset();

  Serial.println("setup done");  
}

unsigned long last_time = millis();
unsigned long last_time2 = millis();
void loop() {
  sensorValue1 = analogRead(sensorPin1);
  sensorValue2 = analogRead(sensorPin2);

  float decayRate = (float)map(sensorValue2, 0, 1023, 1, 10000)/10000000;
   Serial.println(decayRate);
  int i;
  for(i =0; i < sizeof(escala)/sizeof(int); i++){               
    waves[i]->amplitude(amps[i]);      
    if(pins[i] > 0){
      pins[i] -= 0.01;
      amps[i] += 0.003;
    } else {
      if(amps[i] > 0.0){        
        amps[i] -= decayRate;
      }        
    }
  }   
  if(millis() - last_time2 >= 10){
    Serial.println("sensor1  : ");
    Serial.println(sensorValue1);
    Serial.println("sensor2  : ");
    Serial.println(sensorValue2);
    last_time2 = millis();
  }  
   
  if(millis() - last_time >= 50 + sensorValue1*2 + random(sensorValue1)){
    quanta(random(7));
    last_time = millis();
  }  
}


void quanta(int index){  
  int i = index;
  int a = 0;  
  if(pins[i] < 0.2){
    pins[i] = 1;
  }
  Serial.println(index);
  Serial.println(amps[index]);
}
