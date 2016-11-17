#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sound.hpp"


//Sound::Sound(){}

void play_test(void){
	system("mpg123 -q  ../../res/music/random_music.mp3 &");
}

void stop_sound(void){
	system("pkill -f mpg123");
}
