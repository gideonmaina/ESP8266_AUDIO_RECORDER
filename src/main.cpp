#if 1
#include <Arduino.h>

#include "SPH0645.h"
#include <SD.h>
#include <SPI.h>

const unsigned char WAV_HEADER[] = {
    0x52, 0x49, 0x46, 0x46, // header_chunk_id 'R', 'I', 'F', 'F',
    0x20, 0x20, 0x20, 0x20, // header_chunk_data_size //rewritten after recording is done
    0x57, 0x41, 0x56, 0x45, // RIFF_type 'W', 'A', 'V', 'E'

    0x66, 0x6D, 0x74, 0x20, // format_chunk_id  'f' , 'm' , 't' , ' ' ,
    0x10, 0x00, 0x00, 0x00, // format_chunk_size
    0x01, 0x00,             // compression_code (PCM)
    0x01, 0x00,             // Number of Channels (1)
    0x80, 0xBB, 0x00, 0x00, // Sample Rate, 0xBB80 = 48kHz
    0x00, 0x77, 0x01, 0x00, // Average Bytes Per Second // (sampling_rate*bitdepth)/8/ =00017700 /96000
    0x02, 0x00,             // Data Block Size  //bitdepth/8 * number of channels[2 bytes in length]
    0x10, 0x00,             // bitdepth

    0x64, 0x61, 0x74, 0x61, // data_chunk_id 'd' , 'a' , 't' , 'a'
    0x20, 0x20, 0x20, 0x20, // data_chunk_size // calculated after writing to file
};

const int SD_CS = D8; // SD card chip select pin
File audioFile;
char filename[] = "/audio_rec.WAV";
size_t recording_time = 3000; // 3seconds_ish
volatile bool isRecording = false;

void setup()
{
  rx_buf_cnt = 0;

  pinMode(I2SI_WS, OUTPUT);
  pinMode(I2SI_BCK, OUTPUT);
  pinMode(I2SI_DATA, INPUT);

  WiFi.forceSleepBegin();
  delay(500);
  slc_init();
  i2s_init();

  Serial.begin(115200);
  if (SD.begin(SD_CS))
  {
    Serial.println("Initializing SD ....");
    if (SD.exists(filename))
      SD.remove(filename);

    if (audioFile = SD.open(filename, FILE_WRITE))
    { 
     
      audioFile.seek(0);
      audioFile.write(WAV_HEADER, sizeof(WAV_HEADER));
      audioFile.close();
    }
    else
    {
      Serial.println("Failed to open file");
      return;
    }

    Serial.println("Recording starting...");
    isRecording = true;
    slc_init();
    i2s_init();
  }
  else
  {
    Serial.println("Failed to initialize SD");
  }
}

void loop()
{
#if 1

  int32_t value;
  // char withScale[256];
  if (rx_buf_flag && isRecording)
  {
    // Serial.print("rx_buf_flag:\t");
    // Serial.println(rx_buf_flag);

    if (audioFile = SD.open(filename, FILE_WRITE)) // FAILS HERE
    { 
      Serial.println("Writing buffer contents to SD");
      for (int x = 0; x < SLC_BUF_LEN; x++)
      {

        if (i2s_slc_buf_pntr[rx_buf_idx][x] > 0)
        {
          // Serial.print("\ti2s_slc_buf_pntr[rx_buf_idx][");
          // Serial.print(x);
          // Serial.print("]:\t");
          value = convert(i2s_slc_buf_pntr[rx_buf_idx][x]);
          audioFile.write(value);
          // sprintf(withScale, "-1 %f 1", (float)value / 4096.0f);
          // Serial.println(withScale);
        }
      }
      audioFile.close();
      
    }
    else
    {
      Serial.println("SD failed to open to write buffer");
      
    }

    rx_buf_flag = false;
  }

  if ((millis() > recording_time)&&isRecording)
  {
      Serial.println("Finished recording");
      isRecording = false;
      if (audioFile = SD.open(filename, FILE_WRITE))
      {
        Serial.println("SD preparing cleanup");
        unsigned long file_size = audioFile.size() - 8;
        audioFile.seek(4);
        byte data[4] = {lowByte(file_size), highByte(file_size), file_size >> 16, file_size >> 24};
        audioFile.write(data, 4);
        audioFile.seek(40);
        file_size = file_size - 36;
        data[0] = lowByte(file_size);
        data[1] = highByte(file_size);
        data[2] = file_size >> 16;
        data[3] = file_size >> 24;
        audioFile.write((byte *)data, 4);
        audioFile.close();
        Serial.println("Finished saving to SD");
      }
    else
    {
      Serial.println("SD preparing cleaning up fail");
    }
  }

#endif
  // ESP.wdtFeed();
}

#endif

/*
ESP8266 Timers
Software WDT = 3.2 seconds (cannot be changed)
Hardware WDT = 8.2 seconds (cannot be changed)

*/