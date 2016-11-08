#ifndef __COMMAND_LINE_HPP__
#define __COMMAND_LINE_HPP__

#include <vector>
#include <string>
#include <istream>

typedef int (*MenuCallback)(std::istream & input, std::vector<int>, std::vector<std::string>);

class Menu {
	public:
		Menu(std::string const& name, int id, MenuCallback func, ...);
		~Menu();
		int handleInput(std::istream & input, std::vector<int> & path, std::vector<std::string> & pathName);
		void print(int depth = 0);
	private:
		std::vector<Menu*> items;
		const std::string name;
		const int id;
		MenuCallback callback;
};

class CommandInterpreter {
	public:
		void setMenu(Menu * m) { menu = m; }
		int readCommandLines();
		int nextCommand();
		void finish();
	private:
		Menu * menu;
		bool end;
};

#endif
