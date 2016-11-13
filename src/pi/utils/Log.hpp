#ifndef __LOG_HPP__
#define __LOG_HPP__

#include <list>
#include <iostream>
#include <string>
#include <chrono>

class LogLine {
	public:
		LogLine(std::string const& s);
		std::string coloredString(bool includeDate=true) const;
		int getColor() const;

		std::string str;
		std::chrono::system_clock::time_point date;
};

template<typename Ch, typename Traits = std::char_traits<Ch> >
class basic_seqbuf : public std::basic_streambuf<Ch, Traits> {
	public:
		typedef std::basic_streambuf<Ch, Traits> base_type;
		typedef typename base_type::int_type int_type;
		typedef typename base_type::traits_type traits_type;

		virtual int_type overflow(int_type ch);
		std::list<LogLine> lines;

	private:
		std::string currentLine;
};

class LogStream : public std::ostream {
	public:
		LogStream();
		
		typedef typename std::list<LogLine>::const_iterator Cursor;

		Cursor getCursor(bool fromBegin = false);
		bool hasNext(Cursor const& cursor);
		bool hasPrevious(Cursor const& cursor);
		LogLine const& readNext(Cursor & cursor);
		LogLine const& readPrevious(Cursor & cursor);
		std::ostream & error();
		std::ostream & warning();
		std::ostream & debug();
		std::ostream & info();
	private:
		basic_seqbuf<char> buffer;

};

extern LogStream Log;
#define LogW Log.warning()
#define LogE Log.error()
#define LogI Log.info()
#define LogD Log.debug()


#endif

