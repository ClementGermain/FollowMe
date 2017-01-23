#ifndef __COMMAND_LINE_HPP__
#define __COMMAND_LINE_HPP__

#include <vector>
#include <string>
#include <istream>

/**
 * Prototype of callback called when a known command is typed.
 */
typedef int (*MenuCallback)(std::istream & input, std::vector<int>, std::vector<std::string>);

/**
 * Hierarchical menu that describes the command recognized by the CommandInterpreter.
 */
class Menu {
	public:
		/**
		 * A menu has a name and an ID (facultative, can be always 0), a callbak (facultative for non terminal menu, can be NULL), and a list of sub-menu ending by NULL.
		 */
		Menu(std::string const& name, int id, MenuCallback func, ...);
		~Menu();
		/** Read the next word from input and go deeper in the menu, run callback, or print help. */
		int handleInput(std::istream & input, std::vector<int> & pathId, std::vector<std::string> & pathName);
		void print(int depth = 0);
		void printHelp(std::vector<std::string> & pathName);
	private:
		std::vector<Menu*> items;
		const std::string name;
		const int id;
		MenuCallback callback;
};

/**
 * The command interpreter reads the standard input stream and execute recognized commands.
 */
class CommandInterpreter {
	public:
		CommandInterpreter();
		void setMenu(Menu * m) { menu = m; }
		int readCommandLines();
		int nextCommand();
	private:
		Menu * menu;
		bool end;
};

#endif
