#include <emscripten/bind.h>
#include <emscripten.h>
#include <lame/lame.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <iostream>
#include <fstream>

#define MAX_ENCODE_SAMLES       65536 * 8
#define BUFFER_SIZE             (MAX_ENCODE_SAMLES * 1.25 + 7200)

typedef struct {
  lame_global_flags *lame;
} encoder_t;

typedef enum
{
  VBR_METHOD_NONE     =  0,
  VBR_METHOD_DEFAULT  =  1,
  VBR_METHOD_ABR      =  2
} vbrMethod_e;

int encoder_destroy(encoder_t *enc) {
  if (!enc) return -1;
  if (enc->lame) free(enc->lame);
  free(enc);
  return 0;
}

extern "C" EMSCRIPTEN_KEEPALIVE
ssize_t encode(int rate, int channels, int bitrate, vbrMethod_e vbrMethod, int nsamples) {
  
  encoder_t *enc;
  enc->lame = lame_init();
  if (!enc->lame) {
    encoder_destroy(enc);
    return -1;
  }
  lame_set_in_samplerate(enc->lame, rate);
  lame_set_num_channels(enc->lame, channels);

  switch (vbrMethod) {
    case VBR_METHOD_NONE:
        lame_set_VBR(enc->lame, vbr_off);
        break;
    case VBR_METHOD_ABR:
        lame_set_VBR(enc->lame, vbr_abr);
        break;
    case VBR_METHOD_DEFAULT:
    default:
        lame_set_VBR(enc->lame, vbr_default); 
        break;
  };

  lame_set_brate(enc->lame, bitrate);

  ssize_t ret;
  ret = lame_init_params(enc->lame);
  if (ret == -1) {
    encoder_destroy(enc);
    return -2;
  }
  
  printf("nsamples: %d\n", nsamples);

  int m_outOffset = 0;
  int chunkStart = 0;
  unsigned char* mp3buf = (unsigned char*)malloc(1.25 * nsamples + 7200);    
  
  std::ifstream isL ("audioBufferL", std::ifstream::binary);
  std::ifstream isR ("audioBufferR", std::ifstream::binary);
  while (chunkStart < nsamples) {
    if (!isL) return -3;
    int chunkEnd = std::min(chunkStart + MAX_ENCODE_SAMLES, nsamples);
    int chunkLength = chunkEnd - chunkStart;
    int out_bytes = 0;
    
    isL.seekg (0, isL.cur);
    int length = isL.tellg();

    float* pcm_l = new float [chunkLength];
    isL.read ((char*)pcm_l, chunkLength * sizeof(float));
    int pcm_r_length = 0;
    if (channels == 2 && isR)
      pcm_r_length = chunkLength;
    
    float* pcm_r = new float [pcm_r_length];
    if (isR)
      isR.read ((char*)pcm_r, chunkLength * sizeof(float));
    
    out_bytes = lame_encode_buffer_ieee_float(enc->lame, pcm_l, pcm_r, 
                                              chunkLength, 
                                              mp3buf + m_outOffset, BUFFER_SIZE);
    delete[] pcm_l;
    delete[] pcm_r;
    if (out_bytes < 0)
      return -4;
    chunkStart = chunkEnd;
    m_outOffset += out_bytes;

    EM_ASM({
      const progress = $0;
      const elm = document.getElementsByClassName('progress-bar').item(0);
      if (elm){
          const pcg = Math.round(progress * 100);
          elm.setAttribute('aria-valuenow',pcg);
          elm.setAttribute('style','width:'+Number(pcg)+'%');
          elm.setAttribute('class','progress-bar progress-bar-striped progress-bar-animated');
          elm.innerText = Number(pcg)+'%';
      }
    }, float(chunkEnd * sizeof(float)) / float(nsamples * 4));
    emscripten_sleep(1);
  }
  if (isL) {
    isL.close();
    std::remove("audioBufferL");
  }
  if (isR) {
    isR.close();
    std::remove("audioBufferR");
  }
  
  FILE *file;
  printf("Write File\n");
  file = fopen("output.mp3", "w");
  fwrite(mp3buf, sizeof mp3buf[0], m_outOffset, file);
  fclose(file);

  free( mp3buf );
  printf("Complete\n");

  return 1;
}

EMSCRIPTEN_BINDINGS() {
  emscripten::function("encode", &encode);
}