
#include "vosk_api.h"
#include <spdlog/spdlog.h>


static VoskModel *model;
static VoskRecognizer *recognizer;

typedef VoskModel* (*ptr_vosk_model_new)(const char *model_path);
typedef void (*ptr_vosk_model_free)(VoskModel *model);
typedef VoskRecognizer* (*ptr_vosk_recognizer_new_grm)(VoskModel *model, float sample_rate, const char *grammar);
typedef void (*ptr_vosk_recognizer_free)(VoskRecognizer *recognizer);
typedef int (*ptr_vosk_recognizer_accept_waveform)(VoskRecognizer *recognizer, const char *data, int length);
typedef const char *(*ptr_vosk_recognizer_final_result)(VoskRecognizer *recognizer);
typedef void (*ptr_vosk_set_log_level)(int log_level);
typedef void (*ptr_vosk_recognizer_set_max_alternatives)(VoskRecognizer *recognizer, int max_alternatives);

ptr_vosk_recognizer_accept_waveform VoskAcceptWaveform;
ptr_vosk_recognizer_final_result VoskFinalResult;
ptr_vosk_model_new VoskNewModel;
ptr_vosk_recognizer_new_grm VoskNewRecognizer;
ptr_vosk_model_free VoskFreeModel;
ptr_vosk_recognizer_free VoskFreeRecognizer;
ptr_vosk_set_log_level VoskSetLogLevel;
ptr_vosk_recognizer_set_max_alternatives VoskSetAlternatives;

void voskVoiceSetWord(uint16_t length, uint16_t *word, uint8_t lang) {
    QByteArray word_array;
    uint8_t *bytes = (uint8_t*)word;
    for (int i = 0; i < (length*2); i+=2)
    {
        word_array.append(bytes[i+1]);
        word_array.append(bytes[i]);
    }

    QString hex = word_array.toHex();
    QJsonValue value = vruwordsobject.value(hex.toUpper());
    if (value == QJsonValue::Undefined)
    {
        auto toUtf8 = QStringDecoder(QStringDecoder::Utf8);
        QString encoded_string = toUtf8(word_array);
        if (toUtf8.hasError())
        {
            if (lang == 0 /* English */)
                DebugMessage(M64MSG_ERROR, "Unknown word: %s", hex.toUpper().toUtf8().constData());
            else /* Japanese or demo */
                DebugMessage(M64MSG_ERROR, "Unknown Japanese word: %s", hex.toUpper().toUtf8().constData());
        }
        else
        {
            words.append(encoded_string.toLower());
            word_indexes.append(word_list_count);
        }
    }
    else
    {
        QJsonArray value_list = value.toArray();
        for (int i = 0; i < value_list.size(); ++i)
        {
            words.append(value_list.at(i).toString().toLower());
            word_indexes.append(word_list_count);
            DebugMessage(M64MSG_VERBOSE, "word loaded: %s, index: %d", words.last().toUtf8().constData(), word_indexes.last());
        }
    }

    ++word_list_count;
    if (word_list_count == word_list_length)
    {
        words.append("[unk]");
        word_indexes.append(word_list_count);
        QJsonArray array = QJsonArray::fromStringList(words);
        QJsonDocument doc;
        doc.setArray(array);
        if (recognizer)
            VoskFreeRecognizer(recognizer);
        recognizer = VoskNewRecognizer(model, (float)hardware_spec->freq, doc.toJson().constData());
        VoskSetAlternatives(recognizer, 3);
    }
}

Uint32 StopTalking(Uint32, void *)
{
    l_TalkingState = 0;
    timer_id = 0;
    return(0);
}

EXPORT void CALL SetMicState(int state)
{
    if (timer_id)
    {
        SDL_RemoveTimer(timer_id);
        timer_id = 0;
    }
    l_TalkingState = state;

    if (state)
    {
        timer_id = SDL_AddTimer(2000, StopTalking, NULL);
        SDL_ClearQueuedAudio(audio_dev);
        SDL_PauseAudioDevice(audio_dev, 0);
    }
    else
        SDL_PauseAudioDevice(audio_dev, 1);
}

void voskVoiceClearDictionary(uint8_t length) {
    word_list_count = 0;
    word_list_length = length;
    words.clear();
    word_indexes.clear();
    if (recognizer)
        VoskFreeRecognizer(recognizer);
    DebugMessage(M64MSG_VERBOSE, "word list cleared");
    recognizer = nullptr;
}

// EXPORT void CALL SetVRUWordMask(uint8_t, uint8_t *)
// {
// }

static bool compare(QString &s1, QString &s2)
{
    return s1.size() > s2.size();
}

void voskVoiceGetReadData(uint16_t *error_flags, uint16_t *num_results, uint16_t *mic_level, uint16_t *voice_level, uint16_t *voice_length, uint16_t *matches) {
    SDL_PauseAudioDevice(audio_dev, 1);
    uint16_t match[5] = {0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF};
    *error_flags = 0;
    QByteArray data(SDL_GetQueuedAudioSize(audio_dev), 0);
    uint32_t audio_length = SDL_DequeueAudio(audio_dev, data.data(), SDL_GetQueuedAudioSize(audio_dev));
    VoskAcceptWaveform(recognizer, data.constData(), audio_length);
    QJsonDocument doc = QJsonDocument::fromJson(VoskFinalResult(recognizer));
    QJsonObject object = doc.object();
    QStringList found;
    QStringList alternatives;
    QString textResult;
    QJsonArray alternatives_array = object.value("alternatives").toArray();
    for (int i = 0; i < alternatives_array.size(); ++i)
    {
        textResult = alternatives_array.at(i).toObject().value("text").toString();
        if (!textResult.isEmpty())
            alternatives.append(textResult);
    }
    for (int i = 0; i < words.size() - 1 /* -1 to skip the [unk] entry */; ++i)
    {
        if (!found.contains(words.at(i)) && alternatives.filter(words.at(i)).size() > 0)
        {
            /* found a match */
            found.append(words.at(i));
            if (found.size() == 5)
                break;
        }
    }
    /* Use the longest match */
    std::sort(found.begin(), found.end(), compare);
    for (int i = 0; i < found.size(); ++i)
    {
        match[i] = word_indexes.at(words.indexOf(found.at(i)));
        // DebugMessage(M64MSG_INFO, "match %d: %s (%u)", i, found.at(i).toUtf8().constData(), match[i]);
    }
    if (found.size() == 0 && alternatives.size() > 0)
    {
        /* heard something but it didn't match anything */
        *error_flags = 0x4000;
        match[0] = 0; // we match index 0, but mark the error flag saying we are really not sure
        found.append("0");
        // DebugMessage(M64MSG_INFO, "heard a noise, but no word match");
    }

    *num_results = found.size();
    /* The N64 programming manual states what range these values should be within */
    *mic_level = 0xBB8;
    *voice_level = 0xBB8;
    *voice_length = 0x8004;
    matches[0] = match[0];
    matches[1] = *error_flags ? 0x700 : 0;
    matches[2] = match[1];
    matches[3] = (match[1] == 0x7FFF) ? 0 : 0x100;
    matches[4] = match[2];
    matches[5] = (match[2] == 0x7FFF) ? 0 : 0x200;
    matches[6] = match[3];
    matches[7] = (match[3] == 0x7FFF) ? 0 : 0x300;
    matches[8] = match[4];
    matches[9] = (match[4] == 0x7FFF) ? 0 : 0x400;
}

static int setupVosk()
{
    if (voskLib)
        return 0;
    voskLib = new QLibrary((QDir(QCoreApplication::applicationDirPath()).filePath("vosk")));
    VoskAcceptWaveform = (ptr_vosk_recognizer_accept_waveform) voskLib->resolve("vosk_recognizer_accept_waveform");
    VoskFinalResult = (ptr_vosk_recognizer_final_result) voskLib->resolve("vosk_recognizer_final_result");
    VoskNewModel = (ptr_vosk_model_new) voskLib->resolve("vosk_model_new");
    VoskNewRecognizer = (ptr_vosk_recognizer_new_grm) voskLib->resolve("vosk_recognizer_new_grm");
    VoskFreeModel = (ptr_vosk_model_free) voskLib->resolve("vosk_model_free");
    VoskFreeRecognizer = (ptr_vosk_recognizer_free) voskLib->resolve("vosk_recognizer_free");
    VoskSetLogLevel = (ptr_vosk_set_log_level) voskLib->resolve("vosk_set_log_level");
    VoskSetAlternatives = (ptr_vosk_recognizer_set_max_alternatives) voskLib->resolve("vosk_recognizer_set_max_alternatives");

    VoskSetLogLevel(-1);

    QJsonDocument vruwordjson = QJsonDocument::fromJson(vruwords.toUtf8());
    vruwordsobject = vruwordjson.object();

    l_TalkingState = 0;
    SDL_AudioSpec *desired, *obtained;
    if(hardware_spec != NULL) free(hardware_spec);
    desired = (SDL_AudioSpec*)malloc(sizeof(SDL_AudioSpec));
    obtained = (SDL_AudioSpec*)malloc(sizeof(SDL_AudioSpec));
    desired->freq = 44100;
    desired->format = AUDIO_S16SYS;
    desired->channels = 1;
    desired->samples = 1024;
    desired->callback = NULL;
    desired->userdata = NULL;
    audio_dev = SDL_OpenAudioDevice(NULL, 1, desired, obtained, SDL_AUDIO_ALLOW_SAMPLES_CHANGE | SDL_AUDIO_ALLOW_FREQUENCY_CHANGE);
    free(desired);
    hardware_spec = obtained;
    recognizer = nullptr;
    model = nullptr;
    timer_id = 0;
    if (QFile::exists(QDir(ConfigGetUserDataPath()).filePath("vosk-model-small-en-us-0.15/conf/mfcc.conf")))
    {
        model = VoskNewModel(QDir(ConfigGetUserDataPath()).filePath("vosk-model-small-en-us-0.15").toUtf8().constData());
        return 1;
    }
    delete voskLib;
    voskLib = NULL;
    return 0;
}