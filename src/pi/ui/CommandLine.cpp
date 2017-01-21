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
	// Read the optional arguments
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

/**
 * Print all the words for this menu node.
 */
void Menu::printHelp(vector<string> & pathName) {
	cout << "#> ";
	for(string s : pathName)
		cout << s << " ";
	cout << ":" << endl;
	for(Menu * i : items)
		cout << "\t-" << i->name << endl;
}

int Menu::handleInput(istream & input, vector<int> & pathId, vector<string> & pathName) {
	string word;
	int pos = input.tellg();
	input >> word;

	if(!word.empty()) {
		for(Menu * i : items) {
			if(i->name.compare(word) == 0) {
				// Navigate recursively in menu
				pathId.push_back(i->id);
				pathName.push_back(i->name);
				return i->handleInput(input, pathId, pathName);
			}
		}

		if(word.compare("help") == 0) {
			// Display help
			printHelp(pathName);
			return 0;
		}
		else if(items.empty() && callback != NULL) {
			// this is a leaf menu, call the callback function
			input.seekg(pos);
			return callback(input, pathId, pathName);
		}
		else {
			// the given input is unknown in this menu
			printHelp(pathName);
			return 1;
		}
	}
	else {
		input.seekg(pos);
		if(items.empty() && callback != NULL)
			// this is a leaf menu, call the callback function
			return callback(input, pathId, pathName);
		else {
			// leaf or not leaf, that is the problem
			printHelp(pathName);
			return 1;
		}
	}
}

void Menu::print(int depth) {
	for(Menu * m : items) {
		for(int i=depth;--i>=0;)cout << "    ";
		cout << m->name << " " << m->id << " "<<m->callback<<endl;
		m->print(depth+1);
	}
}

CommandInterpreter::CommandInterpreter() :
	menu(NULL), end(true)
{
}

int CommandInterpreter::nextCommand() {
	string line;

	cout << "$> ";
	if(!getline(cin, line))
		return -1;

	stringstream stream(line);
	
	vector<int> i;
	vector<string> s;
	return menu->handleInput(stream, i, s);
}

int CommandInterpreter::readCommandLines() {
	end = false;
	int result = 0;

	cout << "UI for test v1.0\nType a command:" << endl;

	// Read command in loop until the end/error
	while(!end && (result = nextCommand()) >= 0);

	cout << "\nEnd." << endl;

	end = true;
	
	return result;
}

