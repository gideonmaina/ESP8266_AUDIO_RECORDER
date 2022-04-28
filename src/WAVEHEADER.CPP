#include <iostream>
#include <cmath>
#include <fstream>
#include "WAVEGENERATOR.cpp"

class WAVEHEADER{
    //Header Chunk
    char header_chunk_id[4]={'R','I','F','F',}; //4 bytes
    /*
    4 bytes, filesize - 8 bytes 
    To be rewritten when audio data has been recorded unless the size of data is determined beforehand
    */
    char  header_chunk_data_size[4]={'-','-','-','-'}; 
    char RIFF_Type[4]={'W','A','V','E',}; //4 bytes


    //Format chunk
    char  format_chunk_id[4]={'f','m','t',' '}; //4 bytes , contains a trailing space
    int format_chunk_size=0x10; //= 16 , 4 bytes in length
    short compression_code=0x01; //PCM, 2 bytes in length /audio format
    short num_channels=0x01; // MONO, 2 bytes in length
    int sampling_rate=44100; // 0x0000AC44 in hexadecimal ,4 bytes in length
    int byte_rate=sampling_rate*bitdepth/8; // sample_rate*bytes_per_sample*number of channels , bytes_per_sample=bit_depth/8 ,[4 bytes in length] 
    short  block_align=2; //number of channels * bytes per sample OR bitdepth/8 * number of channels[2 bytes in length]
    short bits_per_sample=16; //bitdepth or bits per sample


    //Data chunk
    char data_chunk_id[4]={'d','a','t','a'}; //4 bytes in length
    /* actual size of audio data in bytes = Number of samples * sample byte size * number of channels. 
       To be rewritten when audio data has been recorded unless the size of data is determined beforehand
     */
    char data_chunk_size[4]={'-','-','-','-'}; 
                           


    public:
    //Constructors
    WAVEHEADER(std::ofstream &audio_file){
    writeHeader(audio_file);
    };
    
    // WAVEHEADER(){};
    
    int preAudioDataPosition=0;
    int postAudioDataPosition=0;
    int data_size;

    void writeHeader(std::ofstream &audio_file){
    // audio_file<<header_chunk_id;
    // audio_file<<header_chunk_data_size;
    // audio_file<<RIFF_Type;
    // audio_file<<format_chunk_id;
    // audio_file<<format_chunk_size;
    // audio_file<<compression_code;
    // audio_file<<num_channels;
    // audio_file<<sampling_rate;
    // audio_file<<byte_rate;
    // audio_file<<block_align;
    // audio_file<<bits_per_sample;
    //  audio_file<<data_chunk_id;
    // audio_file<<data_chunk_size;


    audio_file<<"RIFF";
    audio_file<<"----";
    audio_file<<"WAVE";
    audio_file<<"fmt ";
    writeToFile(audio_file,16,4);
    writeToFile(audio_file,1,2);
    writeToFile(audio_file,1,2);
    writeToFile(audio_file,sampling_rate,4);
    writeToFile(audio_file,sampling_rate*bitdepth/8,4);
    writeToFile(audio_file,bitdepth/8,2);
    writeToFile(audio_file, bitdepth, 2); // Bit depth
    audio_file<<"data"; 
    audio_file<<"----";


    preAudioDataPosition=audio_file.tellp();

    };
    void reWriteHeaderMetadata(std::ofstream &audio_file){
        std::cout<<preAudioDataPosition<<std::endl;
        postAudioDataPosition=audio_file.tellp();
        data_size=postAudioDataPosition-preAudioDataPosition;

        //rewrite data_chuck_size
        audio_file.seekp(preAudioDataPosition-4);
        std::cout<<"Audio Data Size: "<<data_size<<std::endl;
        // audio_file<<data_size;
        writeToFile(audio_file,data_size,4);

        //File size -8 bytes
        short file_offset=8;// basicaly any number that can occupy 8 bytes of data
        int header_chunk_data_size=postAudioDataPosition-file_offset;
        //seek 4 bytes from the begining of the file which is the start of header_chunk_data_size
        audio_file.seekp(4,std::ios::beg); 
        // audio_file<<header_chunk_data_size;
        writeToFile(audio_file, postAudioDataPosition - 8, 4);
         std::cout<<"Header Chunk Data Size: "<<(postAudioDataPosition - 8)<<std::endl;
    };

    void writeToFile(std::ofstream &file, int value, int size) {
    file.write(reinterpret_cast<const char*> (&value), size);
}
};


int main(){
int sampling_duration=2; //5 seconds 
SineWaveGenerator sinewave(440,0.5);
std::ofstream audio_stream;
audio_stream.open("audio_test.wav",std::ios::binary);
// audio_stream.open("audio_test");
WAVEHEADER wavmetadata(audio_stream);
// WAVEHEADER wavmetadata;

  for (int i=0; i<SAMPLING_RATE*sampling_duration; i++){
    float sample=sinewave.process();
    int intSample=static_cast<int>(sample*MAX_SAMPLE_AMPLITUDE);
    short shortSample=static_cast<short>(sample*MAX_SAMPLE_AMPLITUDE);
    // audio_stream<<intSample<<std::endl;
    // audio_stream.write(reinterpret_cast<char *>(&intSample),2);
  //  wavmetadata.writeToFile(audio_stream, intSample, 2);
   audio_stream.write((char*)(&shortSample),2);
}
// int postAudioPosition = audio_stream.tellp();
// audio_stream.seekp(preAudioPosition - 4);
//     wavmetadata.writeToFile(audio_stream, postAudioPosition - preAudioPosition, 4);

//     audio_stream.seekp(4, std::ios::beg);
//     wavmetadata.writeToFile(audio_stream, postAudioPosition - 8, 4);
wavmetadata.reWriteHeaderMetadata(audio_stream);
audio_stream.close();



}