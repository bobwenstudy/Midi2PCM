#include "wav_help.h"


typedef struct 
{
    // RIFF块 (RIFF-Chunk)
    char  chunk_id[4];          // 4 字节, RIFF 标志
    int   chunk_size;           // 4 字节, 总chunk大小
    char  format[4];            // 4 字节, WAVE 标志

    // 格式化块 (Format-Chunk) 16 字节
    char  subchunk1_id[4];      // 4 字节, fmt 标志
    int   subchunk1_size;       // 4 字节, subchunk 大小
    short audio_format;         // 2 字节, 音频格式类别 
    short num_channels;         // 2 字节, 声道数
    int   sample_rate;          // 4 字节, 采样率
    int   byte_rate;            // 4 字节, 位速, 传输速率 
    short block_align;          // 2 字节, 一个采样多声道数据块大小, 数据块对齐 
    short bits_per_sample;      // 2 字节, 一个采样占的 bit 数

    // char  chDATA[4];       // 数据标记符＂data ＂ 可选
    // int   dwDATALen;       // 语音数据的长度，比文件长度小42一般。这个是计算音频播放时长的关键参数~

    // 附加块(Fact-Chunk) 当前块偏移地址需要根据实际数据变更

    // 数据块(Data-Chunk) 当前块偏移地址需要根据实际数据变更
    char  subchunk2_id[4];       // 4 字节, data 标志
    int   subchunk2_size;        // subchunk 大小

} wavfile_header_t;

/* wav 文件头默认配置 */
#define SUBCHUNK1_SIZE         (16)
#define AUDIO_FORMAT           (1)      /* PCM */
#define SUBCHUNK2_SIZE         (0)
#define CHUNK_SIZE             (4 + (8 + SUBCHUNK1_SIZE) + (8 + SUBCHUNK2_SIZE))
#define SAMPLE_RATE            (48000)
#define BITS_PER_SAMPLE        (32)
#define NUM_CHANNELS           (1)

#define BYTE_RATE              (SAMPLE_RATE * NUM_CHANNELS * BITS_PER_SAMPLE / 8)
#define BLOCK_ALIGN            (NUM_CHANNELS * BITS_PER_SAMPLE / 8)

static const wavfile_header_t wav_header_default = {
    .chunk_id         = "RIFF",
    .chunk_size       = CHUNK_SIZE,
    .format           = "WAVE",

    .subchunk1_id     = "fmt ",
    .subchunk1_size   = SUBCHUNK1_SIZE,
    .audio_format     = AUDIO_FORMAT,
    .num_channels     = NUM_CHANNELS,
    .sample_rate      = SAMPLE_RATE,
    .byte_rate        = BYTE_RATE,
    .block_align      = BLOCK_ALIGN,
    .bits_per_sample  = BITS_PER_SAMPLE,

    .subchunk2_id     = "data",
    .subchunk2_size   = SUBCHUNK2_SIZE,
};

enum {
    RECORD_AUDIO_FORMAT_48KHz_32BIT,
    RECORD_AUDIO_FORMAT_24KHz_16BIT,
    RECORD_AUDIO_FORMAT_12KHz_16BIT,
};
typedef int record_audio_format_t;

#define BUF_LEN 480
float buf[BUF_LEN];

typedef struct 
{
    wavfile_header_t header;
    FILE *fp;
    uint32_t data_cnt;           /* 累计写入的帧数据量 */
}record_audio_t;

record_audio_t audio;





void wav_data_write(uint8_t *dat, int len)
{   
    fwrite(dat, len, 1, audio.fp);
    audio.data_cnt += len;
}


void wav_stop(void)
{
    wavfile_header_t *h = &audio.header;

    /* 更新头信息 */
    h->subchunk2_size = audio.data_cnt;

    /* 最后重新计算总大小 */
    h->chunk_size = (4 + (8 + h->subchunk1_size) + (8 + h->subchunk2_size));
    
    /* 更新头信息 */
    fseek(audio.fp, 0, SEEK_SET);
    int n = fwrite(&audio.header,  sizeof(wavfile_header_t), 1, audio.fp);
    fclose(audio.fp);
}

void wav_init(char* const file_path, int sample_rate, int bits_per_sample, int num_channels)
{
    audio.fp = fopen(file_path, "wb");
    audio.header = wav_header_default;
	
    wavfile_header_t *h = &audio.header;

	h->sample_rate = sample_rate;
	h->bits_per_sample = bits_per_sample;
	h->num_channels = num_channels;

    h->byte_rate = (h->sample_rate * h->num_channels * h->bits_per_sample / 8);
    h->block_align = (h->num_channels * h->bits_per_sample / 8);

    /* 先写入固定大小的文件头占位置 */
    fwrite(&audio.header,  sizeof(wavfile_header_t), 1, audio.fp);
    
    /* 配置信息 */
    audio.data_cnt = 0;
}