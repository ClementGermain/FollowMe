#ifndef __LOG_HPP__
#define __LOG_HPP__

#include <deque>
#include <iostream>
#include <string>
#include <chrono>
#include <mutex>

/** Examples of use:
 * 		LogE << "This is a fatal error" << endl;
 * 		LogW << "This is a simple warning" << endl;
 * 		LogI << "Hello world!" << endl;
 * 		LogD << "Message for debug only. ";
 * 		Log << "End of the previous line" << endl;
 **/

class LogLine {
	public:
		LogLine(std::string const& s);
		std::string coloredString(bool includeDate=true) const;
		std::string formatedString(bool includeDate=true) const;
		int getColor() const;

		std::string str;
		std::chrono::system_clock::time_point date;
};

/** Local class that binds a stream to a buffer **/
template<typename Ch, typename Traits = std::char_traits<Ch> >
class basic_seqbuf : public std::basic_streambuf<Ch, Traits> {
	public:
		typedef std::basic_streambuf<Ch, Traits> base_type;
		typedef typename base_type::int_type int_type;
		typedef typename base_type::traits_type traits_type;

		virtual int_type overflow(int_type ch);
		std::deque<LogLine> lines;

	private:
		std::string currentLine;
		std::mutex writingLock; 
};

class LogStream : public std::ostream {
	public:
		LogStream();
		
		typedef typename std::deque<LogLine>::const_iterator Cursor;

		/** Return a cursor that can be used to read the log from the begin or from the end **/
		Cursor getCursor(bool fromBegin = false);
		/** Indicate whether an other log line can be read AFTER with the given cursor **/
		bool hasNext(Cursor const& cursor);
		/** Indicate whether an other log line can be read BEFORE with the given cursor **/
		bool hasPrevious(Cursor const& cursor);
		/** Read the next log line, hasNext(cursor) must be checked before **/
		LogLine const& readNext(Cursor & cursor);
		/** Read the previous log line, hasPrevious(cursor) must be checked before **/
		LogLine const& readPrevious(Cursor & cursor);
		/** [Use macro LogE instead] Print the error header and return a stream to write in **/
		std::ostream & error();
		/** [Use macro LogW instead] Print the warning header and return a stream to write in **/
		std::ostream & warning();
		/** [Use macro LogD instead] Print the debug header and return a stream to write in **/
		std::ostream & debug();
		/** [Use macro LogI instead] Print the info header and return a stream to write in **/
		std::ostream & info();
	private:
		basic_seqbuf<char> buffer;
};

// Global variable, must be used to write logs
extern LogStream Log;
/** Macro to write in log with a tag
 * example: LogE << "This is a fatal error" << endl;
 **/
#define LogW Log.warning()
#define LogE Log.error()
#define LogI Log.info()
#define LogD Log.debug()


#endif

