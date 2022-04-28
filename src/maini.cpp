
#if 0
//#include "SPH0645.h"
#include <I2S.h>
#include <SD.h>
#include "WAVEGENERATOR.cpp"
#include <bitset>
#include <iostream>
#include <string>


// const unsigned char createWavTemplateArray[] = {
//   0x52, 0x49, 0x46, 0x46, // Chunk ID (RIFF) 'R', 'I', 'F', 'F',
//   0x20, 0x20, 0x20, 0x20, // Chunk payload size  
//   0x57, 0x41, 0x56, 0x45, // RIFF resource format type 'W', 'A', 'V', 'E'

//   0x66, 0x6D, 0x74, 0x20, // Chunk ID (fmt)  'f' , 'm' , 't' , ' ' , 
//   0x10, 0x00, 0x00, 0x00, // Chunk payload size (0x14 = 20 bytes)
//   0x01, 0x00,       // Format Tag (IMA ADPCM)
//   0x01, 0x00,       // Channels (1)  
//   0x80, 0x3E, 0x00, 0x00, // Sample Rate, 0x3e80 = 16.0kHz
//   0x80, 0x3E, 0x00, 0x00, // Average Bytes Per Second
//   0x01, 0x00,       // Data Block Size (256 bytes) 
//   0x08, 0x00,       // ADPCM encoded bits per sample (4 bits)

//   0x64, 0x61, 0x74, 0x61, // Chunk ID (data) 'd' , 'a' , 't' , 'a'
//   0x20, 0x20, 0x20, 0x20, // Chunk payload size (calculate after rec!)
// };
const unsigned char createWavTemplateArray[] = {
  0x52, 0x49, 0x46, 0x46, // Chunk ID (RIFF) 'R', 'I', 'F', 'F',
  0x20, 0x20, 0x20, 0x20, // Chunk payload size  
  0x57, 0x41, 0x56, 0x45, // RIFF resource format type 'W', 'A', 'V', 'E'

  0x66, 0x6D, 0x74, 0x20, // Chunk ID (fmt)  'f' , 'm' , 't' , ' ' , 
  0x10, 0x00, 0x00, 0x00, // Chunk payload size 
  0x01, 0x00,       // Format Tag (IMA ADPCM)
  0x01, 0x00,       // Channels (1)  
  0x80, 0xBB, 0x00, 0x00, // Sample Rate, 0xBB80 = 48kHz
  0x00, 0x77, 0x01, 0x00, // Average Bytes Per Second // (sampling_rate*bitdepth)/8/ =00017700 /96000
  0x02, 0x00,       // Data Block Size (256 bytes) //bitdepth/8 * number of channels[2 bytes in length]
  0x10, 0x00,       // ADPCM encoded bits per sample (4 bits) //bitdepth

  0x64, 0x61, 0x74, 0x61, // Chunk ID (data) 'd' , 'a' , 't' , 'a'
  0x20, 0x20, 0x20, 0x20, // Chunk payload size (calculate after rec!)
};
 
int sdCs = D8;
File myFile;
File dataLogger;
char fileName[] = "ngori.WAV";
char datalogFile[]="audio_log.txt"; 
size_t recording_time=20000; //50 seconds
int sample;
typedef std::bitset<sizeof(sample)*8> bitset;
void setup() {
  // put your setup code here, to run once:
 

  Serial.begin(115200);
   // start I2S at 48 kHz with 16-bits per sample

 delay(1000);
  if (!I2S.begin(I2S_PHILIPS_MODE, 48000, 16)) {
    Serial.println("Failed to initialize I2S!");
    return;
  }
  SD.begin(sdCs) ? Serial.println("SD initialzation successfull") : Serial.println("SD initialization failed");
  // if (SD.exists(fileName))
  // {
  //   SD.remove(fileName);
  // }
  if (SD.exists(datalogFile))
  {
    SD.remove(datalogFile);
  }
  // (myFile = SD.open(fileName, FILE_WRITE)) ? Serial.println("SD opening successfull") : Serial.println("SD opening failed");
  (dataLogger = SD.open(datalogFile, FILE_WRITE)) ? Serial.println("SD opening dataLogger successfull") : Serial.println("SD opening dataLogger failed");

  // myFile.seek(0);
  
  // myFile.write(createWavTemplateArray, sizeof(createWavTemplateArray));

  // myFile.close();
  
  // (myFile = SD.open(fileName, FILE_WRITE)) ? Serial.println("SD opening successfull") : Serial.println("SD opening failed");

  //  Generate some waves, ramping up then down 25 times and write the data to the wav file
//   int rampVal = 25;
  
//   for(int g=0; g<12; g++){                  //Do this 12 times
//     for(int h=0; h<1000; h++){              //Do this 1000 Times
//       for(int i=1; i<254; i+=rampVal) {   //Count from 1 to 254, stepping up by rampVal
//         myFile.write(i);                //Write the ramping values to the wav file
//       } 
//       for(int i=254; i>10; i-=rampVal) {  //Ramp the values down again
//         myFile.write(i);                //Write the ramping values to the wav file
//       }
//     }

//     Serial.print("Wrote "); Serial.print(g); Serial.println(" of 5");
//     rampVal = rampVal + 20;    //Adjust the ramping value to create different tones
//   }
 while(millis()<recording_time){
     sample=I2S.read();
    // bitset bs(sample);
    // Serial.print("Size of sample: ");
    //  std::string str=bs.to_string();
    
     //Serial.print(reinterpret_cast<char *>(&sample));
    // for(int i=0;i<str.length();i++){
    //    Serial.print(str[i]);
    // }
    // Serial.println();
    //  myFile.write((char*)(&sample),2);
    //  myFile.write(reinterpret_cast<char *>(&sample),2);
    //  myFile.write(sample);
    // dataLogger.write((char*)&sample,4);
    // dataLogger.write("\n");
    Serial.println(sample);
 }
  dataLogger.close();

  // myFile.close();

  // (myFile = SD.open(fileName, FILE_WRITE)) ? Serial.println("SD opening successfull") : Serial.println("SD opening failed");
  
  // Serial.print("Type of file size: ");
  // Serial.println(sizeof(myFile.size()));
  //  unsigned long fSize = myFile.size() - 8;
  // //size_t fSize = myFile.size() - 8;

  // myFile.seek(4);
  // byte data[4] = {lowByte(fSize),highByte(fSize), fSize >> 16,fSize >> 24};
  // myFile.write(data, 4);
  // myFile.seek(40);
  // fSize = fSize - 36;
  // data[0] = lowByte(fSize); 
  // data[1] = highByte(fSize);
  // data[2] = fSize >> 16;
  // data[3] = fSize >> 24;
  // myFile.write((byte*)data,4);
  // myFile.close();

  Serial.println("Finished...");

}

void loop() {
  // put your main code here, to run repeatedly:

}
#endif