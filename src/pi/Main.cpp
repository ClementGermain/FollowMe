#include <fstream>
#include <cstdio>
#include <unistd.h>
#include <csignal>
#include <execinfo.h>
#include <chrono>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include "ui/MainUI.hpp"
#include "car/LinkSTM32.hpp"
#include "car/Car.hpp"
#include "car/Camera.hpp"
#include "utils/Log.hpp"

using namespace std;
using namespace std::chrono;

void handler(int sig) {
	void *array[10];
	size_t size;

	// get void*'s for all entries on the stack
	size = backtrace(array, 10);

	// lolilol
	fprintf(stderr, "\nAh bah c'est bien Nils, super le segfault ! Génial, ");
	fprintf(stderr, "j'te remercie. Tout est foutu, c'est pas grave hein ! ");
	fprintf(stderr, "Oh, t'es vraiment qu'un sale petit con hein ! ");
	fprintf(stderr, "Putain mais c'est dingue !\n");
	// print out all the frames to stderr
	backtrace_symbols_fd(array, size, STDERR_FILENO);

	LogE << "SEGFAULT" << endl;
	// Save logs
	system_clock::time_point today = system_clock::now();
	std::time_t tt = system_clock::to_time_t ( today );
	
	string date(ctime(&tt));
	string dir("../../out/");
	mkdir(dir.c_str(), 0700);
	fstream file((dir+"Log "+date+"(after segfault).txt").c_str(), std::fstream::out);

	LogStream::Cursor cur(Log.getCursor(false));
	while(Log.hasPrevious(cur))
		file << Log.readPrevious(cur).formatedString() << endl;

	file.close();

	exit(1);
}

int main() {
	// Initializations
	signal(SIGSEGV, handler);   // install our segfault handler 
	LinkSTM32 link(100);
	Camera::init();

	// Main loop
	runUI();

	// Destroying
	Camera::destroy();
	return 0;
}

