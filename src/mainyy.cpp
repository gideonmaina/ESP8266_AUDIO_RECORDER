#if 0
#include "Arduino.h"
#include <FS.h>
#include "Wav.h"
#include "I2S.h"
#include <SD.h>
#include "SPH0645.h"


//comment the first line and uncomment the second if you use MAX9814
//#define I2S_MODE I2S_MODE_RX
// #define I2S_MODE I2S_MODE_ADC_BUILT_IN

#define CHIP_SELECT D8

const int record_time = 10;  // second
const char filename[] = "/sound.wav";

const int headerSize = 44;
const int waveDataSize = record_time * 88000;
const int numCommunicationData = 8000;
const int numPartWavData = numCommunicationData/4;
byte header[headerSize];
char communicationData[numCommunicationData];
char partWavData[numPartWavData];
File file;

void setup() {
  rx_buf_cnt = 0;
  // uint32_t value;
  pinMode(I2SI_WS, OUTPUT);
  pinMode(I2SI_BCK, OUTPUT);
  pinMode(I2SI_DATA, INPUT);
    
  
  Serial.begin(115200);
  if (!SD.begin(CHIP_SELECT)) Serial.println("SD begin failed");
  while(!SD.begin(CHIP_SELECT)){
    Serial.print(".");
    delay(500);
  }


  CreateWavHeader(header, waveDataSize);

  SD.remove(filename);

  file = SD.open(filename, FILE_WRITE);

  if (!file) return;
  file.write(header, headerSize);
  // I2S_Init(I2S_MODE, I2S_BITS_PER_SAMPLE_32BIT);
  i2s_init();

  for (int j = 0; j < waveDataSize/numPartWavData; ++j) {
    // I2S_Read(communicationData, numCommunicationData);
    //  Serial.println(I2S.read(communicationData, numCommunicationData));
     Serial.println((char)I2S.read());
    

    // for (int i = 0; i < numCommunicationData/8; ++i) {
    //   partWavData[2*i] = communicationData[8*i + 2];
    //   partWavData[2*i + 1] = communicationData[8*i + 3];
    //   Serial.println(partWavData[2*i]); 
    // }
    // value = convert(i2s_slc_buf_pntr[rx_buf_idx][j]);
    
    // file.write((const byte*)partWavData, numPartWavData);

    
  }
  file.close();
  Serial.println("finish");
}

void loop() {
}

#endif