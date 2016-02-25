// bass_treble_int.cpp : 定义控制台应用程序的入口点。
//
#include "stdio.h"
#include "process_bass_treble.h"

#define APP_INPUT_BUFF_SIZE  (0x4000)
#define APP_OUTPUT_BUFF_SIZE (APP_INPUT_BUFF_SIZE*4)
#define OUT_SAMPLE_RATE     (48000)
#define IN_SAMPLE_RATE      (48000)
#define IN_SAMPLE_RATE_441      (44100)
#define IN_SAMPLE_RATE_96      (96000)
//UINT16 av_ifc_buff_in[APP_INPUT_BUFF_SIZE];//0x4000 0x2000
//UINT32 av_ifc_buff_out[APP_OUTPUT_BUFF_SIZE];

UINT16 audio_process_buff_left[APP_OUTPUT_BUFF_SIZE];
UINT16 audio_process_buff_right[APP_OUTPUT_BUFF_SIZE];

INT16 app_in_buff_left[APP_INPUT_BUFF_SIZE];//0x1000
INT16 app_in_buff_right[APP_INPUT_BUFF_SIZE];//0x1000


#define INV_F   44
#define TRAN_VAR 6
#define LEFT16 (16)
#define LEFT32 (32)
BassTrebleClass g_bass_treble;

int main(int argc, char* argv[])
{
	double  bass_db,treble_db;// -15~15
	FILE *fp_in,*fp_out,*fp_test;
    INT32 n;
    bass_db=0.0;
    treble_db=-11;
    fp_in  = fopen("48_1K_16bit.bin","rb");//48_1K_16bit.bin 44_1_1k_16bit.bin 96_1k_16bit.bin
	if (fp_in == NULL)
	{
		printf("aaaaa\n");
		return -1;
	}
	fp_out  = fopen("48k_16bit_out.bin","wb");//48_1K_16bit_out.bin 44_1k_16bit_out.bin 96_1k_16bit_out.bin
	if (fp_out == NULL)
	{	
		printf("ddddd\n");
		return -2;
	}
	printf("start NewTrackPass1 \n");
	InitBassTreble();
	NewTrackPass1(bass_db,treble_db, IN_SAMPLE_RATE);
	printf("treble_db %x\n",(int)treble_db);
	printf("start DoBassTreble \n");

    while(1)
	{
		n = fread(&app_in_buff_left,2,APP_INPUT_BUFF_SIZE,fp_in);
		//printf("read n = %d\n",n);
		if (n == 0)
		{
			printf("finished \n");
			break;
		}
		DoBassTreble(app_in_buff_left, n, audio_process_buff_left, AUDIO_LEFT_TYPE);
		fwrite(audio_process_buff_left,2,n,fp_out);
		
	}
	fclose(fp_in);
	fclose(fp_out);
	printf("end main \n");

	return 0;
}

