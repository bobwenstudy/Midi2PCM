
#ifndef _WAV_HELP_
#define _WAV_HELP_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdio.h"
#include "stddef.h"
#include "stdint.h"

void wav_data_write(uint8_t *dat, int len);
void wav_stop(void);
void wav_init(char* const file_path, int sample_rate, int bits_per_sample, int num_channels);

#ifdef __cplusplus
}
#endif

#endif