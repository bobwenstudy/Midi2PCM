#include "stdio.h"

#define TSF_IMPLEMENTATION
#include "tsf.h"

#define TML_IMPLEMENTATION
#include "tml.h"

#include "wav_help.h"

// Holds the global instance pointer
static tsf* g_TinySoundFont;

// Holds global MIDI playback state
static double g_Msec;               //current playback time
static tml_message* g_MidiMessage;  //next message to be played

int g_sample_rate;
int g_bits_per_sample;
int g_num_channels;


void process_midi_data(uint8_t *stream, int len)
{
	int work_len = 0;
	//Number of samples to process
	int SampleBlock, SampleCount = (len / (g_num_channels * (g_bits_per_sample/8))); //2 output channels
	for (SampleBlock = TSF_RENDER_EFFECTSAMPLEBLOCK; SampleCount; SampleCount -= SampleBlock)
	{
		//We progress the MIDI playback and then process TSF_RENDER_EFFECTSAMPLEBLOCK samples at once
		if (SampleBlock > SampleCount) SampleBlock = SampleCount;

		//Loop through all MIDI messages which need to be played up until the current playback time
		for (g_Msec += SampleBlock * (1000.0 / g_sample_rate); g_MidiMessage && g_Msec >= g_MidiMessage->time; g_MidiMessage = g_MidiMessage->next)
		{
			switch (g_MidiMessage->type)
			{
				case TML_PROGRAM_CHANGE: //channel program (preset) change (special handling for 10th MIDI channel with drums)
					tsf_channel_set_presetnumber(g_TinySoundFont, g_MidiMessage->channel, g_MidiMessage->program, (g_MidiMessage->channel == 9));
					break;
				case TML_NOTE_ON: //play a note
					tsf_channel_note_on(g_TinySoundFont, g_MidiMessage->channel, g_MidiMessage->key, g_MidiMessage->velocity / 127.0f);
					break;
				case TML_NOTE_OFF: //stop a note
					tsf_channel_note_off(g_TinySoundFont, g_MidiMessage->channel, g_MidiMessage->key);
					break;
				case TML_PITCH_BEND: //pitch wheel modification
					tsf_channel_set_pitchwheel(g_TinySoundFont, g_MidiMessage->channel, g_MidiMessage->pitch_bend);
					break;
				case TML_CONTROL_CHANGE: //MIDI controller messages
					tsf_channel_midi_control(g_TinySoundFont, g_MidiMessage->channel, g_MidiMessage->control, g_MidiMessage->control_value);
					break;
			}
		}

		// Render the block of audio samples in short format
		tsf_render_short(g_TinySoundFont, (short*)stream, SampleBlock, 0);

		work_len = (SampleBlock * ((g_num_channels * (g_bits_per_sample/8))));
		wav_data_write(stream, work_len);
		stream += work_len;
	}
}

int main(int argc, char *argv[])
{
	tml_message* TinyMidiLoader = NULL;

	int tsf_mode = TSF_MONO;

	g_sample_rate = 44100;
	g_bits_per_sample = 16;
	g_num_channels = tsf_mode == TSF_MONO? 1: 2;

	wav_init("test.wav", g_sample_rate, g_bits_per_sample, g_num_channels);

	// Load the midi from a file
	TinyMidiLoader = tml_load_filename("test.mid");
	if (!TinyMidiLoader)
	{
		fprintf(stderr, "Could not load MIDI file\n");
		return 1;
	}

	//Set up the global MidiMessage pointer to the first MIDI message
	g_MidiMessage = TinyMidiLoader;

	// Load the SoundFont from a file
	g_TinySoundFont = tsf_load_filename("test.sf2");
	if (!g_TinySoundFont)
	{
		fprintf(stderr, "Could not load SoundFont\n");
		return 1;
	}

	//Initialize preset on special 10th MIDI channel to use percussion sound bank (128) if available
	tsf_channel_set_bank_preset(g_TinySoundFont, 9, 128, 0);

	// Set the SoundFont rendering output mode
	tsf_set_output(g_TinySoundFont, tsf_mode, g_sample_rate, 0.0f);

	uint8_t data_buf[1024];
	int data_buf_len = sizeof(data_buf);

	while(g_MidiMessage) {
		process_midi_data(data_buf, data_buf_len);
	}

	wav_stop();

	// We could call tsf_close(g_TinySoundFont) and tml_free(TinyMidiLoader)
	// here to free the memory and resources but we just let the OS clean up
	// because the process ends here.
	return 0;
}
