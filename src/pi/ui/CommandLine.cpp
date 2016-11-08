#include <iostream>
#include <string>
#include <sstream>
#include <istream>
#include <cstdarg>
#include "CommandLine.hpp"

using namespace std;

/* 
 * $> motor pwm back 40
 * $> motor back pwm 100
 * $> motor all stop
 * $> motor front pwm 20 time 2000
 * $> camera preview
 * $> camera capture
 * $> keyboard
 * $> save sensor all
 * $> save all
 * $> exit
 **/

Menu::Menu(string const& name, int id, MenuCallback func, ...) :
	name(name), id(id), callback(func)
{
	va_list args;
	va_start(args, func);

	Menu * sub;
	while((sub = va_arg(args, Menu*)) != NULL) {
		items.push_back(sub);
	}

	va_end(args);
}

Menu::~Menu() {
	for(Menu * i : items)
		delete i;
}

int Menu::handleInput(istream & input, vector<int> & pathId, vector<string> & pathName) {
	string word;
	int pos = input.tellg();
	input >> word;

	if(!word.empty()) {
		for(Menu * i : items) {
			if(i->name.compare(word) == 0) {
				pathId.push_back(i->id);
				pathName.push_back(i->name);
				return i->handleInput(input, pathId, pathName);
			}
		}

		if(word.compare("help") == 0) {
			cout << "# ";
			for(string s : pathName)
				cout << s << " ";
			cout << ":" << endl;
			for(Menu * i : items)
				cout << "\t-" << i->name << endl;

			return 0;
		}
		else if(items.empty() && callback != NULL) {
			input.seekg(pos);
			return callback(input, pathId, pathName);
		}
		else {
			return -1;
		}
	}
	else {
		input.seekg(pos);
		if(items.empty() && callback != NULL)
			return callback(input, pathId, pathName);
		else
			return -1;
	}
}

int CommandInterpreter::nextCommand() {
	string line;
	getline(cin, line);
	stringstream stream(line);
	vector<int> i;
	vector<string> s;
	return menu->handleInput(stream, i, s);
}

void CommandInterpreter::finish() {
	end = true;
}

int CommandInterpreter::readCommandLines() {
	end = false;
	int result = 0;
	while(!end && (result = nextCommand()) >= 0);
	return result;
}

