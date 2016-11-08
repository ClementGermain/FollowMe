#ifndef __COMMAND_LINE_HPP__
#define __COMMAND_LINE_HPP__

#include <string>
#include <istream>

typedef int (*MenuCallback)(istream & input, std::vector<int>, std::vector<std::string>);

class Menu {
	public:
		Menu(std::string const& name, int id, MenuCallback func, ...);
		~Menu();
		void addItem(std::string const&  name, int id);
		void addItem(std::string const&  name, MenuCallback func);
		int handleInput(istream & input, std::vector<int> & path = std::vector<int>(), std::vector<std::string> & pathName = std::vector<std::string>());
	private:
		std::vector<Menu*> items;
		const std::string name;
		MenuCallback callback;
		const int id;
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
