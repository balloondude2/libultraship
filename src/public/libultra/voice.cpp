#include "libultraship/libultraship.h"
#include <SDL2/SDL.h>
#include <ratio>

// Establish a chrono duration for the N64 46.875MHz clock rate
// typedef std::ratio<3000, 64> n64ClockRatio;
// typedef std::ratio_divide<std::micro, n64ClockRatio> n64CycleRate;
// typedef std::chrono::duration<long long, n64CycleRate> n64CycleRateDuration;

extern "C" {

int32_t osVoiceInit(OSMesgQueue* mq, OSVoiceHandle* hd, int channel) {
    LUSLOG_DEBUG("osVoiceInit, channel (ctrlr port): %x", channel);
    // setupVosk();
    return 0;
}

int32_t osVoiceSetWord(OSVoiceHandle* hd, u8* word) {
    LUSLOG_DEBUG("osVoiceSetWord first byte: %x", *word);
    return 0;
}

int32_t osVoiceCheckWord(u8* word) {
    // LUSLOG_DEBUG("osVoiceCheckWord", NULL);
    return 0;
}

int32_t osVoiceStartReadData(OSVoiceHandle* hd) {
    // LUSLOG_DEBUG("osVoiceStartReadData", NULL);
    return 0;
}

int32_t osVoiceStopReadData(OSVoiceHandle* hd) {
    // LUSLOG_DEBUG("osVoiceStopReadData", NULL);
    return 0;
}

/*
typedef struct {
    / 0x00 / u16 warning;       // Warning
    / 0x02 / u16 answerNum;     // Candidate number (0~5)
    / 0x04 / u16 voiceLevel;    // Voice input level
    / 0x06 / u16 voiceRelLevel; // Relative voice level "voice_sn"
    / 0x08 / u16 voiceTime;     // Voice input time
    / 0x0A / u16 answer[5];     // Candidate word number
    / 0x14 / u16 distance[5];   // Distance value
} OSVoiceData; // size = 0x20
*/

// dist, ansNum,   warn,        voice, voiceRel
// (800, 2, VOICE_WARN_TOO_SMALL, 500, 2000);

int32_t osVoiceGetReadData(OSVoiceHandle* hd, OSVoiceData* result) {
    
    // Hardcoded pictograph command
    // Implement voice recognition stuff to set this data. 
    result->warning = 0;
    result->answerNum = 0;
    result->voiceLevel = 1000;
    result->voiceRelLevel = 2500;
    result->answer[0] = 1; //VOICE_WORD_ID_CHEESE;
    result->distance[0] = 10;
    LUSLOG_DEBUG("osVoiceGetReadData, top answer id: %x", result->answer[0]);
    return 0;
}

int32_t osVoiceClearDictionary(OSVoiceHandle* hd, u8 numWords) {
    LUSLOG_DEBUG("osVoiceClearDictionary, numWords: %d", numWords);
    return 0;
}

int32_t osVoiceMaskDictionary(OSVoiceHandle* hd, u8* maskPattern, int size) {
    // LUSLOG_DEBUG("osVoiceMaskDictionary", NULL);
    return 0;
}

int32_t osVoiceControlGain(OSVoiceHandle* hd, s32 analog, s32 digital) {
    // LUSLOG_DEBUG("osVoiceControlGain", NULL);
    return 0;
}

}