#include <cstdlib>
#include <string>
#include <iostream>
#include "Sound.hpp"
#include "utils/Log.hpp"

using namespace std;


int Sound::play(const string & filename) {
	// stop any currenlty played song
	stop();
	LogI << "Playing song '" << filename << "'" << endl;
	string command("mpg123 -q ");
	command += filename + " &";
	return system(command.c_str());
}

int Sound::stop() {
	return system("pkill -f mpg123");
}
