#ifndef ULTRA64_VOICE_H
#define ULTRA64_VOICE_H

// #include "ultratypes.h"
#include "message.h"

typedef enum OsVoiceHandleMode {
    /* 0 */ VOICE_HANDLE_MODE_0,
    /* 1 */ VOICE_HANDLE_MODE_1,
    /* 2 */ VOICE_HANDLE_MODE_2,
    /* 3 */ VOICE_HANDLE_MODE_3,
    /* 4 */ VOICE_HANDLE_MODE_4
} OsVoiceHandleMode;

typedef struct {
    /* 0x0 */ OSMesgQueue* mq;
    /* 0x4 */ s32 channel; // Controller port
    /* 0x8 */ OsVoiceHandleMode mode;
    /* 0xC */ u8 status;
} OSVoiceHandle; // size = 0x10

typedef struct {
    /* 0x00 */ u16 warning;       // Warning
    /* 0x02 */ u16 answerNum;     // Candidate number (0~5)
    /* 0x04 */ u16 voiceLevel;    // Voice input level
    /* 0x06 */ u16 voiceRelLevel; // Relative voice level "voice_sn"
    /* 0x08 */ u16 voiceTime;     // Voice input time
    /* 0x0A */ u16 answer[5];     // Candidate word number
    /* 0x14 */ u16 distance[5];   // Distance value
} OSVoiceData; // size = 0x20

#define VOICE_STATUS_READY 0 /* stop/end */
#define VOICE_STATUS_START 1 /* Voice Undetected (no voice input) */
#define VOICE_STATUS_CANCEL 3 /* Cancel (cancel extraneous noise) */
#define VOICE_STATUS_BUSY 5 /* Detected/Detecting (voice being input, recognition processing under way) */
#define VOICE_STATUS_END 7 /* End recognition processing (enable execution of Get Recognition Results command) */

#define VOICE_WARN_TOO_SMALL 0x400 /* Voice level is too low (100 < Voice Level < 150) */
#define VOICE_WARN_TOO_LARGE 0x800 /* Voice level is too high (Voice Level > 3500) */
#define VOICE_WARN_NOT_FIT 0x4000 /* No words match recognition word (No. 1 Candidate Distance Value > 1600) */
#define VOICE_WARN_TOO_NOISY 0x8000 /* Too much ambient noise (Relative Voice Level =< 400) */

typedef struct {
    /* 0x000 */ u16 words[20][15]; // 20 words, each with up to 15 syllables
    /* 0x258 */ u8 numWords;
} OSVoiceDictionary; // size = 0x25C

typedef struct {
    /* 0x00 */ OSVoiceDictionary* dict;
    /* 0x04 */ s8 mode;
    /* 0x08 */ OSVoiceData* data;
    /* 0x0C */ u16 distance;
    /* 0x0E */ u16 answerNum;
    /* 0x10 */ u16 warning;
    /* 0x12 */ u16 voiceLevel;
    /* 0x14 */ u16 voiceRelLevel;
} OSVoiceUnk; // size = 0x18

#ifdef __cplusplus
extern "C" {
#endif
int32_t osVoiceInit(OSMesgQueue* mq, OSVoiceHandle* hd, int channel);
int32_t osVoiceSetWord(OSVoiceHandle* hd, u8* word);
int32_t osVoiceCheckWord(u8* word);
int32_t osVoiceStartReadData(OSVoiceHandle* hd);
int32_t osVoiceStopReadData(OSVoiceHandle* hd);
int32_t osVoiceGetReadData(OSVoiceHandle* hd, OSVoiceData* result);
int32_t osVoiceClearDictionary(OSVoiceHandle* hd, u8 numWords);
int32_t osVoiceMaskDictionary(OSVoiceHandle* hd, u8* maskPattern, int size);
int32_t osVoiceControlGain(OSVoiceHandle* hd, int32_t analog, int32_t digital);
#ifdef __cplusplus
}
#endif
#endif
