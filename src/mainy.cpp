#if 0
#include <Arduino.h>

//#include "SPH0645.h"
#include <SD.h>
// #include "Wav.h"
// #include "wavefile.cpp"
// #include <LittleFS.h>
#include "WAVEGENERATOR.cpp"
/* Main -----------------------------------------------------------------------*/


#define CHIP_SELECT D8
const char filename[] = "/record_test.wav";
int counter=0;
File audio_file;
float sample;
int intSample;
short shortSample;
int sigsteen=16;
short one=1;
int header_chunk=44136;
int data_size=44100;
int byte_rate=((data_size*bitdepth)/8);
short block_align=bitdepth/8;
short bit_depth=16;
int sampling_duration=5; //5 seconds 
void
setup()
{

  Serial.begin(115200);
  if (!SD.begin(CHIP_SELECT)) Serial.println("SD begin failed");
  while(!SD.begin(CHIP_SELECT)){
    Serial.print(".");
    delay(500);
  }

  SD.remove(filename);

  audio_file=SD.open(filename, FILE_WRITE);

  if (!audio_file) return; 
SineWaveGenerator sinewave(440,0.5);

    audio_file.write("RIFF",4);
    audio_file.write(reinterpret_cast<char *>(&header_chunk),2);
    audio_file.write("WAVE",4);
    audio_file.write("fmt ",4);
    audio_file.write(reinterpret_cast<char *>(&sigsteen),4);
    audio_file.write(reinterpret_cast<char *>(&one),2);
    audio_file.write(reinterpret_cast<char *>(&one),2);
    audio_file.write(reinterpret_cast<char *>(&data_size),4);
    audio_file.write(reinterpret_cast<char *>(&byte_rate),4);
    audio_file.write(reinterpret_cast<char *>(&block_align),2);
    audio_file.write(reinterpret_cast<char *>(&bit_depth),2);
    audio_file.write("data",4);
    audio_file.write(reinterpret_cast<char *>(44100),4);



  for (int i=0; i<SAMPLING_RATE*sampling_duration; i++){
    sample=sinewave.process();
    //intSample=static_cast<int>(sample*MAX_SAMPLE_AMPLITUDE);
    shortSample=static_cast<short>(sample*MAX_SAMPLE_AMPLITUDE);
     audio_file.write((char*)(&shortSample),2);
}

audio_file.close();



}

void
loop()
{
  
}
#endif