#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

void chat_sound() {
    PlaySound(TEXT("sound\\chat.wav"), NULL, SND_FILENAME | SND_ASYNC);
}

void click_sound() {
    PlaySound(TEXT("sound\\click.wav"), NULL, SND_FILENAME | SND_ASYNC);
}