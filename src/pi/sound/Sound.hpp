#ifndef __SOUND__
#define __SOUND__

#include <string>


/**
 * Simple sound manager to play and stop music.
 * It uses the external program 'mpg123' (need to be installed)
 */
class Sound {
	public:
		//! Play a song identified by its path relative to the executabnle path.
		static int play(const std::string & filename ="../../res/music/random_music.mp3");
		//! Stop any currently played song.
		static int stop();
	private:
};

#endif
