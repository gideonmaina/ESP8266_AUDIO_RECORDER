#include <iostream>
#include <cmath>
#include <fstream>
// #define SAMPLING_RATE 44100 /* Number of samples per second */
// const int bitdepth =16;
#define SAMPLING_RATE 1600 /* Number of samples per second */
const int bitdepth =8;
const float MAX_SAMPLE_AMPLITUDE=pow(2,bitdepth-1)-1;
class SineWaveGenerator{
   float frequency,amplitude, phase_angle=0.0f,SAMPLE_TIME= 1/SAMPLING_RATE, offset=0.0; 
   public:
   SineWaveGenerator(float freq,float amp):frequency(freq),amplitude(amp){
       offset= 2*M_PI*frequency/SAMPLING_RATE;
   };
   float process(){
        float sample=amplitude*sin(phase_angle);
        phase_angle += offset;
        return sample;
       }

};

