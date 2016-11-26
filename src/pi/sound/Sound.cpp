#include <cstdlib>
#include <string>
#include "Sound.hpp"

using namespace std;

int Sound::play(const string & filename) {
	// stop any currenlty played song
	stop();

	string command("mpg123 -q ");
	command += filename + " &";
	return system(command.c_str());
}

int Sound::stop() {
	return system("pkill -f mpg123");
}
