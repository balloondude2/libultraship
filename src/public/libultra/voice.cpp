#include "libultraship/libultraship.h"
#include <SDL2/SDL.h>
#include <ratio>

// Establish a chrono duration for the N64 46.875MHz clock rate
// typedef std::ratio<3000, 64> n64ClockRatio;
// typedef std::ratio_divide<std::micro, n64ClockRatio> n64CycleRate;
// typedef std::chrono::duration<long long, n64CycleRate> n64CycleRateDuration;

extern "C" {
// uint8_t __osMaxControllers = MAXCONTROLLERS;

// int32_t osContInit(OSMesgQueue* mq, uint8_t* controllerBits, OSContStatus* status) {
//     *controllerBits = 0;
//     status->status |= 1;

//     if (SDL_Init(SDL_INIT_GAMECONTROLLER) != 0) {
//         SPDLOG_ERROR("Failed to initialize SDL game controllers ({})", SDL_GetError());
//         exit(EXIT_FAILURE);
//     }

//     std::string controllerDb = Ship::Context::LocateFileAcrossAppDirs("gamecontrollerdb.txt");
//     int mappingsAdded = SDL_GameControllerAddMappingsFromFile(controllerDb.c_str());
//     if (mappingsAdded >= 0) {
//         SPDLOG_INFO("Added SDL game controllers from \"{}\" ({})", controllerDb, mappingsAdded);
//     } else {
//         SPDLOG_ERROR("Failed add SDL game controller mappings from \"{}\" ({})", controllerDb, SDL_GetError());
//     }

//     Ship::Context::GetInstance()->GetControlDeck()->Init(controllerBits);

//     return 0;
// }

/**
 * File: voiceinit.c
 *
 * Initializes Voice Recognition System control structure and hardware
 */

// #include "ultra64.h"
// #include "PR/controller_voice.h"
// #include "PR/os_voice.h"
// #include "PR/controller.h"
// #include "macros.h"

// static u8 sCmds[] = {
//     0x1E, 0x6E, 0x08, 0x56, 0x03,
// };

int32_t osVoiceInit(OSMesgQueue* mq, OSVoiceHandle* hd, int channel) {
    LUSLOG_DEBUG("osVoiceInit, channel (ctrlr port): %x", channel);
    return 0;

//     s32 errorCode;
//     s32 i;
//     u8 status = 0;
//     u8 data[4];

//     hd->channel = channel;
//     hd->mq = mq;
//     hd->mode = VOICE_HANDLE_MODE_0;

//     errorCode = __osVoiceGetStatus(mq, channel, &status);
//     if (errorCode != 0) {
//         return errorCode;
//     }

//     if (__osContChannelReset(mq, channel) != 0) {
//         return CONT_ERR_CONTRFAIL;
//     }

//     for (i = 0; i < ARRAY_COUNT(sCmds); i++) {
//         errorCode = __osVoiceSetADConverter(mq, channel, sCmds[i]);
//         if (errorCode != 0) {
//             return errorCode;
//         }
//     }

//     errorCode = __osVoiceGetStatus(mq, channel, &status);
//     if (errorCode != 0) {
//         return errorCode;
//     }
//     if (status & 2) {
//         return CONT_ERR_VOICE_NO_RESPONSE;
//     }

//     /**
//      * data[0] = 0
//      * data[1] = 0
//      * data[2] = 1
//      * data[3] = 0
//      */
//     *(u32*)data = 0x100;
//     errorCode = __osVoiceContWrite4(mq, channel, 0, data);
//     if (errorCode != 0) {
//         return errorCode;
//     }

//     errorCode = __osVoiceCheckResult(hd, &status);
//     if (errorCode & 0xFF00) {
//         errorCode = CONT_ERR_INVALID;
//     }

//     return errorCode;
}

int32_t osVoiceSetWord(OSVoiceHandle* hd, u8* word) {
    // LUSLOG_DEBUG("osVoiceSetWord", NULL);
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
    LUSLOG_DEBUG("osVoiceGetReadData", NULL);
    // Hardcoded pictograph command
    // Implement voice recognition stuff to set this data. 
    result->warning = 0;
    result->answerNum = 0;
    result->voiceLevel = 1000;
    result->voiceRelLevel = 2500;
    result->answer[0] = 1; //VOICE_WORD_ID_CHEESE;
    result->distance[0] = 10;
    return 0;
}

int32_t osVoiceClearDictionary(OSVoiceHandle* hd, u8 numWords) {
    // LUSLOG_DEBUG("osVoiceClearDictionary", NULL);
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