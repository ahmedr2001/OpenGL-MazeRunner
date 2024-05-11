#include <unordered_map>
#include <string>
#include <vector>
#include "../../../vendor/miniaudio/miniaudio.h"
namespace our
{

    class AudioController
    {
    private:
        static AudioController *audioController;
        std::unordered_map<std::string, ma_sound*> sounds; // All audio tracks that the program can run
        std::vector<std::string> currentlyPlaying;          // All audios currently playing
        AudioController()
        {
            sounds["menu"] = new ma_sound();
            sounds["play-wall-e"] = new ma_sound();
            sounds["play-eve"] = new ma_sound();
            sounds["loss-wall-e"] = new ma_sound();
            sounds["loss-eve"] = new ma_sound();
            sounds["win"] = new ma_sound();
            // Initializing the audio engine
            ma_result result;
            ma_engine *pEngine = new ma_engine();

            result = ma_engine_init(NULL, pEngine);
            if (result == MA_SUCCESS)
            { // Succeeded to initialize the engine
                // Initializing the audio tracks
                ma_sound_init_from_file(pEngine, "assets/audio/menu.mp3", 0, NULL, NULL, sounds["menu"]);
                ma_sound_init_from_file(pEngine, "assets/audio/play-wall-e.mp3", 0, NULL, NULL, sounds["play-wall-e"]);
                ma_sound_init_from_file(pEngine, "assets/audio/play-eve.mp3", 0, NULL, NULL, sounds["play-eve"]);
                ma_sound_init_from_file(pEngine, "assets/audio/loss-wall-e.mp3", 0, NULL, NULL, sounds["loss-wall-e"]);
                ma_sound_init_from_file(pEngine, "assets/audio/loss-eve.mp3", 0, NULL, NULL, sounds["loss-eve"]);
                ma_sound_init_from_file(pEngine, "assets/audio/win.mp3", 0, NULL, NULL, sounds["win"]);
            }
        }

    public:
        AudioController(const AudioController &obj) = delete;
        static AudioController *getAudioController()
        {
            if (audioController == NULL)
            {
                audioController = new AudioController();
                return audioController;
            }
            else
            {
                return audioController;
            }
        }

        void play(std::string state, bool repeat, bool interrupt)
        {
            if (interrupt)
            {
                for (std::string playing : currentlyPlaying)
                {
                    ma_sound_stop(sounds[playing]);
                }
                currentlyPlaying.clear();
            }
            if (repeat)
            {
                ma_sound_set_looping(sounds[state], true);
            }
            else
            {
                ma_sound_set_looping(sounds[state], false);
            }
            ma_sound_seek_to_pcm_frame(sounds[state], 0); // rewind to start of audio
            ma_sound_start(sounds[state]);
            currentlyPlaying.push_back(state);
        }
    };

}
