#ifndef __SOUND__
#define __SOUND__

#include <string>

class Sound {
	public:
		static int play(const std::string & filename ="../../res/music/random_music.mp3");
		static int stop();
	private:
};

#endif
