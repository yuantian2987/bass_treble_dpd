
#include "process_bass_treble.h"
#include "process_bass_treble_table.h"
#define INV_F   44
#define TRAN_VAR 6
#define LEFT16 (16)
#define LEFT32 (32)
BassTrebleClass g_bass_treble;

/*for get pcm data*/
INT64  gNum = 1;
INT16 bassCount = 0;
INT16 trebleCount = 0;


///static inline INT16 DoFilterLeft16(INT16 in,AUDIO_TRACK_TYPE tracktype)
static INT16 DoFilterLeft16(INT16 in,AUDIO_TRACK_TYPE tracktype)
{
    INT64 out = 0,temp;
    INT64 data_hi;
    UINT32 data_lo;
    INT32 tempIn = 0;

    if(tracktype == AUDIO_LEFT_TYPE) {
#if 0
        asm volatile ("mult %0,%1"::"r"(g_bass_treble.b0Bass),"r"(in));
        asm volatile ("madd %0,%1"::"r"(g_bass_treble.b1Bass),"r"(g_bass_treble.Lxn1Bass));
        asm volatile ("madd %0,%1"::"r"(g_bass_treble.b2Bass),"r"(g_bass_treble.Lxn2Bass));
        asm volatile ("msub %0,%1"::"r"(g_bass_treble.a1Bass),"r"(g_bass_treble.Lyn1Bass));
        asm volatile ("msub %0,%1"::"r"(g_bass_treble.a2Bass),"r"(g_bass_treble.Lyn2Bass));
        asm volatile ("mfhi %0":"=r"(data_hi));
        asm volatile ("mflo %0":"=r"(data_lo));
        temp = (INT64)((data_hi<<32)|data_lo);
#endif
		temp = (INT64)g_bass_treble.b0Bass*in + (INT64)g_bass_treble.b1Bass*g_bass_treble.Lxn1Bass
											  + (INT64)g_bass_treble.b2Bass*g_bass_treble.Lxn2Bass
											  - (INT64)g_bass_treble.a1Bass*g_bass_treble.Lyn1Bass
											  - (INT64)g_bass_treble.a2Bass*g_bass_treble.Lyn2Bass;
        out = (temp*g_bass_treble.a0BassInv)>>(INV_F -TRAN_VAR);
        g_bass_treble.Lxn2Bass = g_bass_treble.Lxn1Bass;
        g_bass_treble.Lxn1Bass = in;
        g_bass_treble.Lyn2Bass = g_bass_treble.Lyn1Bass;
        g_bass_treble.Lyn1Bass = out;

        // Treble filter
        tempIn = out;
#if 0
        asm volatile ("mult %0,%1"::"r"(g_bass_treble.b0Treble),"r"(tempIn));
        asm volatile ("madd %0,%1"::"r"(g_bass_treble.b1Treble),"r"(g_bass_treble.Lxn1Treble));
        asm volatile ("madd %0,%1"::"r"(g_bass_treble.b2Treble),"r"(g_bass_treble.Lxn2Treble));
        asm volatile ("msub %0,%1"::"r"(g_bass_treble.a1Treble),"r"(g_bass_treble.Lyn1Treble));
        asm volatile ("msub %0,%1"::"r"(g_bass_treble.a2Treble),"r"(g_bass_treble.Lyn2Treble));
        asm volatile ("mfhi %0":"=r"(data_hi));
        asm volatile ("mflo %0":"=r"(data_lo));
        temp = (INT64)((data_hi<<32)|data_lo);
#endif
		temp = (INT64)g_bass_treble.b0Treble*tempIn + (INT64)g_bass_treble.b1Treble*g_bass_treble.Lxn1Treble
													+ (INT64)g_bass_treble.b2Treble*g_bass_treble.Lxn2Treble
													- (INT64)g_bass_treble.a1Treble*g_bass_treble.Lyn1Treble
													- (INT64)g_bass_treble.a2Treble*g_bass_treble.Lyn2Treble;
        out = (temp*g_bass_treble.a0TrebleInv)>>(INV_F -TRAN_VAR);

        g_bass_treble.Lxn2Treble = g_bass_treble.Lxn1Treble;
        g_bass_treble.Lxn1Treble = tempIn;
        g_bass_treble.Lyn2Treble = g_bass_treble.Lyn1Treble;
        g_bass_treble.Lyn1Treble = out;
    } else {
#if 0
        asm volatile ("mult %0,%1"::"r"(g_bass_treble.b0Bass),"r"(in));
        asm volatile ("madd %0,%1"::"r"(g_bass_treble.b1Bass),"r"(g_bass_treble.Rxn1Bass));
        asm volatile ("madd %0,%1"::"r"(g_bass_treble.b2Bass),"r"(g_bass_treble.Rxn2Bass));
        asm volatile ("msub %0,%1"::"r"(g_bass_treble.a1Bass),"r"(g_bass_treble.Ryn1Bass));
        asm volatile ("msub %0,%1"::"r"(g_bass_treble.a2Bass),"r"(g_bass_treble.Ryn2Bass));
        asm volatile ("mfhi %0":"=r"(data_hi));
        asm volatile ("mflo %0":"=r"(data_lo));
        out = (INT64)((data_hi<<32)|data_lo);
#endif
		out = (INT64)g_bass_treble.b0Bass*in + (INT64)g_bass_treble.b1Bass*g_bass_treble.Rxn1Bass
											  + (INT64)g_bass_treble.b2Bass*g_bass_treble.Rxn2Bass
											  - (INT64)g_bass_treble.a1Bass*g_bass_treble.Ryn1Bass
											  - (INT64)g_bass_treble.a2Bass*g_bass_treble.Ryn2Bass;
        //out /=g_bass_treble.a0Bass;
        out = (out*g_bass_treble.a0BassInv)>>(INV_F -TRAN_VAR);

        g_bass_treble.Rxn2Bass = g_bass_treble.Rxn1Bass;
        g_bass_treble.Rxn1Bass = in;
        g_bass_treble.Ryn2Bass = g_bass_treble.Ryn1Bass;
        g_bass_treble.Ryn1Bass = out;
        // Treble filter
        tempIn = out;
#if 0
        asm volatile ("mult %0,%1"::"r"(g_bass_treble.b0Treble),"r"(tempIn));
        asm volatile ("madd %0,%1"::"r"(g_bass_treble.b1Treble),"r"(g_bass_treble.Rxn1Treble));
        asm volatile ("madd %0,%1"::"r"(g_bass_treble.b2Treble),"r"(g_bass_treble.Rxn2Treble));
        asm volatile ("msub %0,%1"::"r"(g_bass_treble.a1Treble),"r"(g_bass_treble.Ryn1Treble));
        asm volatile ("msub %0,%1"::"r"(g_bass_treble.a2Treble),"r"(g_bass_treble.Ryn2Treble));
        asm volatile ("mfhi %0":"=r"(data_hi));
        asm volatile ("mflo %0":"=r"(data_lo));
        out = (INT64)((data_hi<<32)|data_lo);
#endif
		out = (INT64)g_bass_treble.b0Treble*tempIn + (INT64)g_bass_treble.b1Treble*g_bass_treble.Rxn1Treble
											       + (INT64)g_bass_treble.b2Treble*g_bass_treble.Rxn2Treble
											       - (INT64)g_bass_treble.a1Treble*g_bass_treble.Ryn1Treble
											       - (INT64)g_bass_treble.a2Treble*g_bass_treble.Ryn2Treble;
        //out /=g_bass_treble.a0Treble;
        out = (out*g_bass_treble.a0TrebleInv)>>(INV_F -TRAN_VAR);

        g_bass_treble.Rxn2Treble = g_bass_treble.Rxn1Treble;
        g_bass_treble.Rxn1Treble = tempIn;
        g_bass_treble.Ryn2Treble = g_bass_treble.Ryn1Treble;
        g_bass_treble.Ryn1Treble = out;
    }
	///out>>=6;
    out>>=(6+1);//right 1 is in order to prevent overflow
    if(out > 0x7FFF)
        out = 0x7FFF;
    else if(out < -0x8000)
        out = -0x8000;

    return (INT16)out;

}

///static inline INT16 DoFilterUltraSound(INT16 in, AUDIO_TRACK_TYPE tracktype)
static INT16 DoFilterUltraSound(INT16 in, AUDIO_TRACK_TYPE tracktype)
{
#if 0
    INT64 out = 0,temp;
    INT64 data_hi;
    UINT32 data_lo;
    INT32 tempIn = 0;

    if(tracktype == AUDIO_LEFT_TYPE) {
        asm volatile ("mult %0,%1"::"r"(g_bass_treble.b0UltraBass),"r"(in));
        asm volatile ("madd %0,%1"::"r"(g_bass_treble.b1UltraBass),"r"(g_bass_treble.Lxn1UltraBass));
        asm volatile ("madd %0,%1"::"r"(g_bass_treble.b2UltraBass),"r"(g_bass_treble.Lxn2UltraBass));
        asm volatile ("msub %0,%1"::"r"(g_bass_treble.a1UltraBass),"r"(g_bass_treble.Lyn1UltraBass));
        asm volatile ("msub %0,%1"::"r"(g_bass_treble.a2UltraBass),"r"(g_bass_treble.Lyn2UltraBass));
        asm volatile ("mfhi %0":"=r"(data_hi));
        asm volatile ("mflo %0":"=r"(data_lo));
        temp = (INT64)((data_hi<<32)|data_lo);
        out = (temp*g_bass_treble.a0UltraBassInv)>>(INV_F -TRAN_VAR);
        g_bass_treble.Lxn2UltraBass = g_bass_treble.Lxn1UltraBass;
        g_bass_treble.Lxn1UltraBass = in;
        g_bass_treble.Lyn2UltraBass = g_bass_treble.Lyn1UltraBass;
        g_bass_treble.Lyn1UltraBass = out;

        // Treble filter
        tempIn = out;
        asm volatile ("mult %0,%1"::"r"(g_bass_treble.b0UltraTreble),"r"(tempIn));
        asm volatile ("madd %0,%1"::"r"(g_bass_treble.b1UltraTreble),"r"(g_bass_treble.Lxn1UltraTreble));
        asm volatile ("madd %0,%1"::"r"(g_bass_treble.b2UltraTreble),"r"(g_bass_treble.Lxn2UltraTreble));
        asm volatile ("msub %0,%1"::"r"(g_bass_treble.a1UltraTreble),"r"(g_bass_treble.Lyn1UltraTreble));
        asm volatile ("msub %0,%1"::"r"(g_bass_treble.a2UltraTreble),"r"(g_bass_treble.Lyn2UltraTreble));
        asm volatile ("mfhi %0":"=r"(data_hi));
        asm volatile ("mflo %0":"=r"(data_lo));
        temp = (INT64)((data_hi<<32)|data_lo);
        out = (temp*g_bass_treble.a0UltraTrebleInv)>>(INV_F -TRAN_VAR);

        g_bass_treble.Lxn2UltraTreble = g_bass_treble.Lxn1UltraTreble;
        g_bass_treble.Lxn1UltraTreble = tempIn;
        g_bass_treble.Lyn2UltraTreble = g_bass_treble.Lyn1UltraTreble;
        g_bass_treble.Lyn1UltraTreble = out;
    } else {
        asm volatile ("mult %0,%1"::"r"(g_bass_treble.b0UltraBass),"r"(in));
        asm volatile ("madd %0,%1"::"r"(g_bass_treble.b1UltraBass),"r"(g_bass_treble.Rxn1UltraBass));
        asm volatile ("madd %0,%1"::"r"(g_bass_treble.b2UltraBass),"r"(g_bass_treble.Rxn2UltraBass));
        asm volatile ("msub %0,%1"::"r"(g_bass_treble.a1UltraBass),"r"(g_bass_treble.Ryn1UltraBass));
        asm volatile ("msub %0,%1"::"r"(g_bass_treble.a2UltraBass),"r"(g_bass_treble.Ryn2UltraBass));
        asm volatile ("mfhi %0":"=r"(data_hi));
        asm volatile ("mflo %0":"=r"(data_lo));
        out = (INT64)((data_hi<<32)|data_lo);
        //out /=g_bass_treble.a0Bass;
        out = (out*g_bass_treble.a0UltraBassInv)>>(INV_F -TRAN_VAR);

        g_bass_treble.Rxn2UltraBass = g_bass_treble.Rxn1UltraBass;
        g_bass_treble.Rxn1UltraBass = in;
        g_bass_treble.Ryn2UltraBass = g_bass_treble.Ryn1UltraBass;
        g_bass_treble.Ryn1UltraBass = out;
        // Treble filter
        tempIn = out;
        asm volatile ("mult %0,%1"::"r"(g_bass_treble.b0UltraTreble),"r"(tempIn));
        asm volatile ("madd %0,%1"::"r"(g_bass_treble.b1UltraTreble),"r"(g_bass_treble.Rxn1UltraTreble));
        asm volatile ("madd %0,%1"::"r"(g_bass_treble.b2UltraTreble),"r"(g_bass_treble.Rxn2UltraTreble));
        asm volatile ("msub %0,%1"::"r"(g_bass_treble.a1UltraTreble),"r"(g_bass_treble.Ryn1UltraTreble));
        asm volatile ("msub %0,%1"::"r"(g_bass_treble.a2UltraTreble),"r"(g_bass_treble.Ryn2UltraTreble));
        asm volatile ("mfhi %0":"=r"(data_hi));
        asm volatile ("mflo %0":"=r"(data_lo));
        out = (INT64)((data_hi<<32)|data_lo);
        //out /=g_bass_treble.a0Treble;
        out = (out*g_bass_treble.a0UltraTrebleInv)>>(INV_F -TRAN_VAR);

        g_bass_treble.Rxn2UltraTreble = g_bass_treble.Rxn1UltraTreble;
        g_bass_treble.Rxn1UltraTreble = tempIn;
        g_bass_treble.Ryn2UltraTreble = g_bass_treble.Ryn1UltraTreble;
        g_bass_treble.Ryn1UltraTreble = out;
    }

    return (INT16)(out>>6);
#endif
}

UINT8 ProcessPass1(INT16 *databufferIn, INT32 len, INT16 *databufferOut,AUDIO_TRACK_TYPE tracktype)
{
    INT64 i =0;

    for ( i = 0; i < len; i++) {

        databufferOut[i] = DoFilterLeft16((INT16)databufferIn[i],tracktype);

    }
    return TRUE;
}

UINT8 ProcessPass2(INT16 *databufferIn, INT32 len, INT16 *databufferOut,AUDIO_TRACK_TYPE tracktype)
{
    INT64 i =0;

    for ( i = 0; i < len; i++) {
        databufferOut[i] = DoFilterUltraSound((INT16)databufferIn[i],tracktype);
    }
    return TRUE;
}

static UINT8  CoefficentsLeft16 (DOUBLE hz, FLOAT slope, DOUBLE gain, AUDIO_TYPE type)
{
    //DOUBLE w = 0.0;
    INT64 a=0;
    INT64 b=0;
    INT64 tempcosvalue = 0;
    INT64 tempsinvalue = 0;
    INT32 gainIndex;

    gainIndex = 10*(gain + 25);

    if (type == AUDIO_BASS_TYPE) {
#if USE_GAIN_ADD
        gainIndex = 10*(gain  + 25-9);
#endif
        a = gain_a[gainIndex];
        b = gain_b[gainIndex];
        tempcosvalue = t_basscos[AUDIO_FREQ_48K];
        tempsinvalue = t_basssin[AUDIO_FREQ_48K];
        g_bass_treble.b0Bass = (a * ((((a<<LEFT16) + (gNum<<LEFT32)) - (a - (gNum<<LEFT16)) * tempcosvalue + b * tempsinvalue)>>LEFT16))>>12; //left shift 20
        g_bass_treble.b1Bass = (2 * a * ((((a<<LEFT16) - (gNum<<LEFT32)) - (a + (gNum<<LEFT16)) * tempcosvalue)>>LEFT16))>>12;//left shift 20
        g_bass_treble.b2Bass = (a * ((((a<<LEFT16) + (gNum<<LEFT32)) - (a - (gNum<<LEFT16)) * tempcosvalue - b * tempsinvalue)>>LEFT16))>>12; //left shift 20
        g_bass_treble.a0Bass = (((a<<LEFT16) + (gNum<<LEFT32)) + (a - (gNum<<LEFT16)) * tempcosvalue + b * tempsinvalue)>>12;//left shitf 20
        g_bass_treble.a0BassInv = (((((INT64)1)<<INV_F))/g_bass_treble.a0Bass);//left44-20=24
        g_bass_treble.a1Bass = (-2 * (((a<<LEFT16) - (gNum<<LEFT32)) + (a + (gNum<<LEFT16)) * tempcosvalue))>>12;//left shift 20
        g_bass_treble.a2Bass = (((a<<LEFT16) + (gNum<<LEFT32)) + (a - (gNum<<LEFT16)) * tempcosvalue - b * tempsinvalue)>>12;//left shift 20
    } else if(type == AUDIO_TREBLE_TYPE) { //assumed trebleType
#if USE_GAIN_ADD
        gainIndex = 10*(gain + 25);
#endif
        a = gain_a[gainIndex];
        b = gain_b[gainIndex];
        tempcosvalue = t_treblecos[AUDIO_FREQ_48K];
        tempsinvalue = t_treblesin[AUDIO_FREQ_48K];
        g_bass_treble.b0Treble =(a* ((((a<<LEFT16) + (gNum<<LEFT32)) + (a - (gNum<<LEFT16)) * tempcosvalue + b * tempsinvalue)>>LEFT16))>>12;//left shift 20
        g_bass_treble.b1Treble = (-2 * a * ((((a<<LEFT16) - (gNum<<LEFT32)) + (a + (gNum<<LEFT16)) * tempcosvalue)>>LEFT16))>>12;//left shift 20
        g_bass_treble.b2Treble = (a * ((((a<<LEFT16) + (gNum<<LEFT32)) + (a - (gNum<<LEFT16)) * tempcosvalue - b * tempsinvalue)>>LEFT16))>>12;//left shift 20
        g_bass_treble.a0Treble = (((a<<LEFT16) + (gNum<<LEFT32)) - (a - (gNum<<LEFT16)) * tempcosvalue + b * tempsinvalue)>>12;// left shift 20
        g_bass_treble.a0TrebleInv = (((((INT64)1)<<INV_F) )/g_bass_treble.a0Treble);
        g_bass_treble.a1Treble = (2 * (((a<<LEFT16) - (gNum<<LEFT32)) - (a + (gNum<<LEFT16)) * tempcosvalue))>>12;//left shift 20
        g_bass_treble.a2Treble = (((a<<LEFT16) + (gNum<<LEFT32)) - (a - (gNum<<LEFT16)) *tempcosvalue - b * tempsinvalue)>>12;//left shift 20
    } else if(type == AUDIO_ULTRABASS_TYPE) {
        a = gain_a[gainIndex];
        b = gain_b[gainIndex];
        tempcosvalue = t_ultraBasscos[AUDIO_FREQ_48K];
        tempsinvalue = t_ultraBasssin[AUDIO_FREQ_48K];
        g_bass_treble.b0UltraBass =(a * ((((a<<LEFT16) + (gNum<<LEFT32)) - (a - (gNum<<LEFT16)) * tempcosvalue + b * tempsinvalue)>>LEFT16))>>12; //left shift 20
        g_bass_treble.b1UltraBass = (2 * a * ((((a<<LEFT16) - (gNum<<LEFT32)) - (a + (gNum<<LEFT16)) * tempcosvalue)>>LEFT16))>>12;//left shift 20
        g_bass_treble.b2UltraBass = (a * ((((a<<LEFT16) + (gNum<<LEFT32)) - (a - (gNum<<LEFT16)) * tempcosvalue - b * tempsinvalue)>>LEFT16))>>12; //left shift 20
        g_bass_treble.a0UltraBass = (((a<<LEFT16) + (gNum<<LEFT32)) + (a - (gNum<<LEFT16)) * tempcosvalue + b * tempsinvalue)>>12;//left shitf 20
        g_bass_treble.a0UltraBassInv = ((((INT64)1)<<INV_F) )/g_bass_treble.a0UltraBass;
        g_bass_treble.a1UltraBass =  (-2 * (((a<<LEFT16) - (gNum<<LEFT32)) + (a + (gNum<<LEFT16)) * tempcosvalue))>>12;//left shift 20
        g_bass_treble.a2UltraBass = (((a<<LEFT16) + (gNum<<LEFT32)) + (a - (gNum<<LEFT16)) * tempcosvalue - b * tempsinvalue)>>12;//left shift 20
    } else if(type == AUDIO_ULTRATREBLE_TYPE) {
        a = gain_a[gainIndex];
        b = gain_b[gainIndex];
        tempcosvalue = t_ultraTreblecos[AUDIO_FREQ_48K];
        tempsinvalue = t_ultraTreblesin[AUDIO_FREQ_48K];
        g_bass_treble.b0UltraTreble = (a* ((((a<<LEFT16) + (gNum<<LEFT32)) + (a - (gNum<<LEFT16)) * tempcosvalue + b * tempsinvalue)>>LEFT16))>>12;//left shift 20
        g_bass_treble.b1UltraTreble = (-2 * a * ((((a<<LEFT16) - (gNum<<LEFT32)) + (a + (gNum<<LEFT16)) * tempcosvalue)>>LEFT16))>>12;//left shift 20
        g_bass_treble.b2UltraTreble = (a * ((((a<<LEFT16) + (gNum<<LEFT32)) + (a - (gNum<<LEFT16)) * tempcosvalue - b * tempsinvalue)>>LEFT16))>>12;//left shift 20
        g_bass_treble.a0UltraTreble = (((a<<LEFT16) + (gNum<<LEFT32)) - (a - (gNum<<LEFT16)) * tempcosvalue + b * tempsinvalue)>>12;// left shift 20
        g_bass_treble.a0UltraTrebleInv = ((((INT64)1)<<INV_F) )/g_bass_treble.a0UltraTreble;
        g_bass_treble.a1UltraTreble = (2 * (((a<<LEFT16) - (gNum<<LEFT32)) - (a + (gNum<<LEFT16)) * tempcosvalue))>>12;//left shift 20
        g_bass_treble.a2UltraTreble = (((a<<LEFT16) + (gNum<<LEFT32)) - (a - (gNum<<LEFT16)) *tempcosvalue - b * tempsinvalue)>>12;//left shift 20
    }
    return TRUE;
}
static UINT8 TransVar(void)
{
    g_bass_treble.b0Bass = ( g_bass_treble.b0Bass>>0);
    g_bass_treble.b1Bass = (g_bass_treble.b1Bass>>0);
    g_bass_treble.b2Bass = (g_bass_treble.b2Bass>>0);
    g_bass_treble.a0Bass =  (g_bass_treble.a0Bass>>TRAN_VAR);
    g_bass_treble.a1Bass = (g_bass_treble.a1Bass>>TRAN_VAR);
    g_bass_treble.a2Bass = (g_bass_treble.a2Bass>>TRAN_VAR);
    g_bass_treble.b0Treble  =  (g_bass_treble.b0Treble>>TRAN_VAR);
    g_bass_treble.b1Treble = (g_bass_treble.b1Treble>>TRAN_VAR) ;
    g_bass_treble.b2Treble = (g_bass_treble.b2Treble>>TRAN_VAR) ;
    g_bass_treble.a0Treble = (g_bass_treble.a0Treble>>TRAN_VAR);
    g_bass_treble.a1Treble = (g_bass_treble.a1Treble>>TRAN_VAR);
    g_bass_treble.a2Treble =  (g_bass_treble.a2Treble>>TRAN_VAR);
#if USE_ULTRA_SOUND
    g_bass_treble.b0UltraBass =  (g_bass_treble.b0UltraBass>>0);
    g_bass_treble.b1UltraBass = (g_bass_treble.b1UltraBass>>0) ;
    g_bass_treble.b2UltraBass = (g_bass_treble.b2UltraBass>>0) ;
    g_bass_treble.a0UltraBass = (g_bass_treble.a0UltraBass>>TRAN_VAR);
    g_bass_treble.a1UltraBass = (g_bass_treble.a1UltraBass>>TRAN_VAR);
    g_bass_treble.a2UltraBass =  (g_bass_treble.a2UltraBass>>TRAN_VAR);
    g_bass_treble.b0UltraTreble =  (g_bass_treble.b0UltraTreble>>TRAN_VAR);
    g_bass_treble.b1UltraTreble = (g_bass_treble.b1UltraTreble>>TRAN_VAR) ;
    g_bass_treble.b2UltraTreble = (g_bass_treble.b2UltraTreble>>TRAN_VAR) ;
    g_bass_treble.a0UltraTreble = (g_bass_treble.a0UltraTreble>>TRAN_VAR);
    g_bass_treble.a1UltraTreble = (g_bass_treble.a1UltraTreble>>TRAN_VAR);
    g_bass_treble.a2UltraTreble =  (g_bass_treble.a2UltraTreble>>TRAN_VAR);
#endif
}

static UINT8  TransferParameters(/*Shuttle & shuttle*/DOUBLE bless_db,DOUBLE treble_db,  DOUBLE ultraBass_db, DOUBLE ultraTreble_db, UINT64 m_currate)
{
    g_bass_treble.dB_bass = bless_db;//bless_db;
    g_bass_treble.dB_treble = treble_db;//treble_db;
    g_bass_treble.dB_ultraBass = ultraBass_db;//ultrasound;
    g_bass_treble.dB_ultraTreble = ultraTreble_db;//ultrasound;
    g_bass_treble.mPreGain = 1;   // Unity gain
    return TRUE;
}

UINT8 NewTrackPass1(DOUBLE  bless_db,DOUBLE treble_db , UINT64 sampleRate)
{
    const FLOAT slope = 0.4f;   // same slope for both filters
    const DOUBLE hzBass = 250.0f;
    const DOUBLE hzTreble = 4000.0f;
    const DOUBLE hzUltraBass = 20.0f;
    const DOUBLE hzUltraTreble = 20000.0f;
    TransferParameters(bless_db, treble_db, 2, 3, sampleRate);
// Compute coefficents of the low shelf biquand IIR filter
    CoefficentsLeft16(hzBass, slope, g_bass_treble.dB_bass, AUDIO_BASS_TYPE);

// Compute coefficents of the high shelf biquand IIR filter
    CoefficentsLeft16(hzTreble, slope,  g_bass_treble.dB_treble, AUDIO_TREBLE_TYPE);
#if USE_ULTRA_SOUND
// Compute coefficents of the high shelf biquand IIR filter
    CoefficentsLeft16(hzUltraBass, slope,  g_bass_treble.dB_ultraBass, AUDIO_ULTRABASS_TYPE);
// Compute coefficents of the high shelf biquand IIR filter
    CoefficentsLeft16(hzUltraTreble, slope,  g_bass_treble.dB_ultraTreble, AUDIO_ULTRATREBLE_TYPE);
#endif
    TransVar();
    return TRUE;
}

void InitBassTreble(void)
{
    // restore saved preferences
    g_bass_treble.dB_bass = 0.0;
    g_bass_treble.dB_treble = 0.0;
    g_bass_treble.dB_ultraBass = 0.0;
    g_bass_treble.dB_ultraTreble = 0.0;
    g_bass_treble.mPreGain = 1;// Validate data
    //(re)initialise filter parameters
    g_bass_treble.Lxn1Bass=g_bass_treble.Lxn2Bass=g_bass_treble.Lyn1Bass=g_bass_treble.Lyn2Bass=0;
    g_bass_treble.Lxn1UltraBass=g_bass_treble.Lxn2UltraBass=g_bass_treble.Lyn1UltraBass=g_bass_treble.Lyn2UltraBass=0;
    g_bass_treble.Lxn1Treble=g_bass_treble.Lxn2Treble=g_bass_treble.Lyn1Treble=g_bass_treble.Lyn2Treble=0;
    g_bass_treble.Lxn1UltraTreble=g_bass_treble.Lxn2UltraTreble=g_bass_treble.Lyn1UltraTreble=g_bass_treble.Lyn2UltraTreble=0;
    g_bass_treble.Rxn1Bass=g_bass_treble.Rxn2Bass=g_bass_treble.Ryn1Bass=g_bass_treble.Ryn2Bass=0;
    g_bass_treble.Rxn1Treble=g_bass_treble.Rxn2Treble=g_bass_treble.Ryn1Treble=g_bass_treble.Ryn2Treble=0;
    g_bass_treble.Rxn1UltraTreble=g_bass_treble.Rxn2UltraTreble=g_bass_treble.Ryn1UltraTreble=g_bass_treble.Ryn2UltraTreble=0;

    //NewTrackPass1(bless_db, treble_db,  sampleRate);
}


void DoBassTreble(INT16 *databufferIn, INT32 len, INT16 *databufferOut,AUDIO_TRACK_TYPE tracktype)
{
#if USE_ULTRA_SOUND
    ProcessPass2(databufferIn,len,databufferIn,tracktype);// add 2dB at 20Hz and 3dB at 20kHz
#endif
    ProcessPass1(databufferIn,len,databufferOut,tracktype);
}

