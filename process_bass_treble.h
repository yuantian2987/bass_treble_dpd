
#ifndef _AUDIOPROCESS_H
#define _AUDIOPROCESS_H
///#include "cs_types.h"
#define TRUE (1)
#define FALSE (0)
#ifndef M_PI
#define	M_PI		3.14159265358979323846  /* pi */
#endif
typedef  unsigned long long  UINT64; 
typedef  long long  INT64; 
typedef  double  DOUBLE;
typedef  float  FLOAT;
typedef  unsigned char    UINT8;
typedef  unsigned char    uint8;
typedef  unsigned int       UINT32;
typedef  unsigned int       uint32;
typedef  unsigned short    UINT16;
typedef  unsigned short    uint16;
typedef  short            INT16;
typedef  short            int16;
typedef  unsigned char   uchar;
typedef int   INT32;

#define USE_ULTRA_SOUND (0)
#define USE_GAIN_ADD (1)

typedef enum
{
    AUDIO_BASS_TYPE = 0,
    AUDIO_TREBLE_TYPE,
    AUDIO_ULTRABASS_TYPE,
    AUDIO_ULTRATREBLE_TYPE,
    AUDIO_TYPE_MAX,
}AUDIO_TYPE;

typedef enum
{
    AUDIO_LEFT_TYPE = 0,
    AUDIO_RIGHT_TYPE,
    AUDIO_TRACK_TYPE_MAX,
}AUDIO_TRACK_TYPE;


typedef struct _BassTrebleClass
{
    UINT64 mCurRate;
    INT32    mCurChannel;
    INT32    mPass;
    UINT8   mSecondPassDisabled;
    //BassTreble
    INT32 Lxn1Bass, Lxn2Bass, b0Bass, b1Bass, b2Bass, Rxn1Bass, Rxn2Bass, 
    Lxn1Treble, Lxn2Treble, b0Treble, b1Treble, b2Treble, Rxn1Treble, Rxn2Treble, 
    Lxn1UltraBass, Lxn2UltraBass, b0UltraBass, b1UltraBass, b2UltraBass, Rxn1UltraBass, Rxn2UltraBass,
    Lxn1UltraTreble, Lxn2UltraTreble, b0UltraTreble, b1UltraTreble, b2UltraTreble, Rxn1UltraTreble, Rxn2UltraTreble;
    INT64 Lyn1Bass, Lyn2Bass, a0Bass, a1Bass, a2Bass ,a0BassInv, Ryn1Bass, Ryn2Bass;
    INT64 Lyn1UltraBass, Lyn2UltraBass, a0UltraBass, a1UltraBass, a2UltraBass, a0UltraBassInv, Ryn1UltraBass, Ryn2UltraBass;
    // High shelf
    INT64 Lyn1Treble, Lyn2Treble, a0Treble, a1Treble, a2Treble, a0TrebleInv, Ryn1Treble, Ryn2Treble;
    INT64 Lyn1UltraTreble, Lyn2UltraTreble, a0UltraTreble, a1UltraTreble, a2UltraTreble, a0UltraTrebleInv, Ryn1UltraTreble, Ryn2UltraTreble;

    DOUBLE dB_bass, dB_treble, dB_ultraBass, dB_ultraTreble, dB_level;
    UINT8   mbNormalize;
    UINT8 mPreGain;	
} BassTrebleClass;

void InitBassTreble(void);

void DoBassTreble(INT16 *databuffer, INT32 len, INT16 *databufferOut,AUDIO_TRACK_TYPE tracktype);

UINT8 NewTrackPass1(DOUBLE  bless_db,DOUBLE treble_db , UINT64 sampleRate);


#endif

