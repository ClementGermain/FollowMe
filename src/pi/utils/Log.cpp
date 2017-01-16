#include <ostream>
#include <chrono>
#include "Log.hpp"

using namespace std;
using namespace std::chrono;


// Global variable, must be used to write logs
LogStream Log;

// Local constant string tags for log types
const string headerError("E/");		// Error
const string headerWarning("W/");	// Warning
const string headerDebug("D/");		// Debug
const string headerInfo("I/");		// Info
const string headerDefault("-/");	// Undefined


/** LogStream constructor **/
LogStream::LogStream() : ostream(&buffer)
{
	
}

/** Return a cursor that can be used to read the log from the begin or from the end **/
LogStream::Cursor LogStream::getCursor(bool fromBegin) {
	return !fromBegin ? buffer.lines.cend() : buffer.lines.cbegin();
}

/** Indicate whether an other log line can be read AFTER with the given cursor **/
bool LogStream::hasNext(Cursor const& cursor) {
	return cursor != buffer.lines.cend();
}

/** Indicate whether an other log line can be read BEFORE with the given cursor **/
bool LogStream::hasPrevious(Cursor const& cursor) {
	return cursor != buffer.lines.cbegin();
}

/** Read the next log line, hasNext(cursor) must be checked before **/
LogLine const& LogStream::readNext(Cursor & cursor) {
	return *(cursor++);
}

/** Read the previous log line, hasPrevious(cursor) must be checked before **/
LogLine const& LogStream::readPrevious(Cursor & cursor) {
	return *(--cursor);
}

/** Print the error header and return a stream to write in **/
ostream & LogStream::error() {
	return *this << headerError;
}

/** Print the warning header and return a stream to write in **/
ostream & LogStream::warning() {
	return *this << headerWarning;
}

/** Print the info header and return a stream to write in **/
ostream & LogStream::info() {
	return *this << headerInfo;
}

/** Print the debug header and return a stream to write in **/
ostream & LogStream::debug() {
	return *this << headerDebug;
}

/** Constructor for a new log line **/
LogLine::LogLine(std::string const& s) : str(s), date(chrono::system_clock::now())
{
	
}

/** Return the log line color as an integer (0xRRGGBBAA) **/
int LogLine::getColor() const {
	if(str.find(headerError) == 0)
		return 0xff0000ff;
	else if(str.find(headerWarning) == 0)
		return 0xeeaa00ff;
	else if(str.find(headerDebug) == 0)
		return 0xdd00ddff;
	else if(str.find(headerInfo) == 0)
		return 0x0000ddff;
	else
		return 0xddddddff;

}

/** Return the log line as a formated string with Bash color codes **/
string LogLine::coloredString(bool includeDate) const {
	// Convert date to string with format "HH:MM::SS.mmm"
	char time[13];
	int ms = duration_cast<milliseconds>(date.time_since_epoch()).count() % 1000;
	time_t rawtime = system_clock::to_time_t(date);
	size_t len = strftime(time, 9, "%T", localtime(&rawtime));
	sprintf(time+len, ".%03d", ms);

	// Define color and insert date between the tag and the content
	string res;
	int offset = 2;
	if(str.find(headerError) == 0)
		res += "\e[1;91m"+headerError;
	else if(str.find(headerWarning) == 0)
		res += "\e[1;93m"+headerWarning;
	else if(str.find(headerDebug) == 0)
		res += "\e[0;95m"+headerDebug;
	else if(str.find(headerInfo) == 0)
		res += "\e[0;94m"+headerInfo;
	else {
		offset = 0;
		res += "\e[0m";
		res += headerDefault;
	}

	if(includeDate) {
		res += time;
		res += '/';
	}
	res += str.substr(offset) + "\e[0m";

	return res;
}

/** Return the log line as a formated string **/
string LogLine::formatedString(bool includeDate) const {
	// Convert date to string with format "HH:MM::SS.mmm"
	char time[13];
	int ms = duration_cast<milliseconds>(date.time_since_epoch()).count() % 1000;
	time_t rawtime = system_clock::to_time_t(date);
	size_t len = strftime(time, 9, "%T", localtime(&rawtime));
	sprintf(time+len, ".%03d", ms);

	// Insert date between the tag and the content
	string res;
	int offset = 2;
	if(str.find(headerError) == 0)
		res += headerError;
	else if(str.find(headerWarning) == 0)
		res += headerWarning;
	else if(str.find(headerDebug) == 0)
		res += headerDebug;
	else if(str.find(headerInfo) == 0)
		res += headerInfo;
	else {
		offset = 0;
		res += headerDefault;
	}

	if(includeDate) {
		res += time;
		res += '/';
	}
	res += str.substr(offset);

	return res;
}

/** This method receives each character written in the LogStream and saves them **/
template<typename Ch, typename Traits>
typename basic_seqbuf<Ch, Traits>::int_type
	basic_seqbuf<Ch, Traits>::overflow(typename basic_seqbuf<Ch, Traits>::int_type ch) {
	
	if(traits_type::eq_int_type(ch, traits_type::eof()))
		return traits_type::eof();

	writingLock.lock();

	if(ch == '\n') {
		// Here, the end of a line is reached
		// We ignore empty lines
		if(!currentLine.empty()) {
			// Add the current line to the array
			lines.emplace_back(currentLine);
			currentLine.clear();
		}
	}
	else {
		// append the character to the current line
		currentLine += ch;
	}

	writingLock.unlock();

	return ch;
}
