#include <SDL.h>
#include <SDL_mixer.h>
#include <unistd.h>

#define WAV_PATH "A4.wav"

// Our wave file
Mix_Chunk* wave = NULL;

int main(int argc, char* argv[]) {
    // Initialize SDL.
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
        return -1;

    // Initialize SDL_mixer
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 1, 4096) == -1)
        return -1;

    // Load our sound effect
    wave = Mix_LoadWAV(WAV_PATH);
    if (wave == NULL)
        return -1;

    int timer = 100;
    while (timer > 0) {
        Mix_PlayChannelTimed(-1, wave, 0, 1667);
        SDL_Delay(16.67);
        timer--;
        printf("%d\n", timer);
    }

    // clean up our resources
    Mix_FreeChunk(wave);

    // quit SDL_mixer
    Mix_CloseAudio();

    return 0;
}