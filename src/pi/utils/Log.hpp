#ifndef __LOG_HPP__
#define __LOG_HPP__

#include <deque>
#include <iostream>
#include <string>
#include <chrono>
#include <mutex>

/**
 * Small class to store a log line. Contains the text content and the date of the line.
 * The text starts with the flag identifier 'I/' (info), 'D/' (debug), 'W/' (warning) or 'E/' (error).
 * A color is associated with each one.
 */
class LogLine {
	public:
		/** Create a log line with the given string as text content and with current system date. **/
		LogLine(std::string const& s);
		/** Return a formated and bash-colored string of this log line. **/
		std::string coloredString(bool includeDate=true) const;
		/** Return a formated string of this log line. **/
		std::string formatedString(bool includeDate=true) const;
		/** Return the color of this line in 32-bit-long integer **/
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

/** This class inheri from std::ostream (standard output stream) and redirect the stream to
 * a internal buffer array.
 * The buffer can be read at any time.
 *
 * Examples of use:
 * 		LogE << "This is a fatal error" << endl;
 * 		LogW << "This is a simple warning" << endl;
 * 		LogI << "Hello world!" << endl;
 * 		LogD << "Message for debug only. ";
 * 		Log << "End of the previous line" << endl;
 **/
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

