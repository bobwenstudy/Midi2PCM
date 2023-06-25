#include "wav_help.h"


typedef struct 
{
    // RIFF�� (RIFF-Chunk)
    char  chunk_id[4];          // 4 �ֽ�, RIFF ��־
    int   chunk_size;           // 4 �ֽ�, ��chunk��С
    char  format[4];            // 4 �ֽ�, WAVE ��־

    // ��ʽ���� (Format-Chunk) 16 �ֽ�
    char  subchunk1_id[4];      // 4 �ֽ�, fmt ��־
    int   subchunk1_size;       // 4 �ֽ�, subchunk ��С
    short audio_format;         // 2 �ֽ�, ��Ƶ��ʽ��� 
    short num_channels;         // 2 �ֽ�, ������
    int   sample_rate;          // 4 �ֽ�, ������
    int   byte_rate;            // 4 �ֽ�, λ��, �������� 
    short block_align;          // 2 �ֽ�, һ���������������ݿ��С, ���ݿ���� 
    short bits_per_sample;      // 2 �ֽ�, һ������ռ�� bit ��

    // char  chDATA[4];       // ���ݱ�Ƿ���data �� ��ѡ
    // int   dwDATALen;       // �������ݵĳ��ȣ����ļ�����С42һ�㡣����Ǽ�����Ƶ����ʱ���Ĺؼ�����~

    // ���ӿ�(Fact-Chunk) ��ǰ��ƫ�Ƶ�ַ��Ҫ����ʵ�����ݱ��

    // ���ݿ�(Data-Chunk) ��ǰ��ƫ�Ƶ�ַ��Ҫ����ʵ�����ݱ��
    char  subchunk2_id[4];       // 4 �ֽ�, data ��־
    int   subchunk2_size;        // subchunk ��С

} wavfile_header_t;

/* wav �ļ�ͷĬ������ */
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
    uint32_t data_cnt;           /* �ۼ�д���֡������ */
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

    /* ����ͷ��Ϣ */
    h->subchunk2_size = audio.data_cnt;

    /* ������¼����ܴ�С */
    h->chunk_size = (4 + (8 + h->subchunk1_size) + (8 + h->subchunk2_size));
    
    /* ����ͷ��Ϣ */
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

    /* ��д��̶���С���ļ�ͷռλ�� */
    fwrite(&audio.header,  sizeof(wavfile_header_t), 1, audio.fp);
    
    /* ������Ϣ */
    audio.data_cnt = 0;
}