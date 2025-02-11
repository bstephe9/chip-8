#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>

void get_audio_device_info() {
    int n = SDL_GetNumAudioDevices(0);
    const char *name = SDL_GetAudioDeviceName(0, 0);

    SDL_AudioSpec spec = {0};
    SDL_GetAudioDeviceSpec(0, 0, &spec);

    printf("Num: %d\n", n);
    printf("Name: %s\n", name);
}

void my_audio_callback(void *userdata, Uint8 *stream, int len) {}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    SDL_AudioSpec desired = {0};
    desired.freq = 22050;
    desired.format = AUDIO_S16LSB;
    desired.channels = 0;
    desired.samples = 8192;
    desired.callback = my_audio_callback;
    desired.userdata = NULL;

    SDL_AudioSpec obtained = {0};
    SDL_AudioDeviceID id = SDL_OpenAudioDevice(NULL, 0, &desired, &obtained,
                                               SDL_AUDIO_ALLOW_ANY_CHANGE);
    if (id == 0) {
        fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_PauseAudioDevice(id, 0);

    return 0;
}