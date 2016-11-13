#include <ostream>
#include <chrono>
#include "Log.hpp"

using namespace std;


// Global
LogStream Log;

const string headerError("E/");
const string headerWarning("W/");
const string headerDebug("D/");
const string headerInfo("I/");



LogStream::LogStream() : ostream(&buffer)
{
	
}


LogStream::Cursor LogStream::getCursor(bool fromBegin) {
	return !fromBegin ? buffer.lines.cend() : buffer.lines.cbegin();
}

bool LogStream::hasNext(Cursor const& cursor) {
	return cursor != buffer.lines.cend();
}
bool LogStream::hasPrevious(Cursor const& cursor) {
	return cursor != buffer.lines.cbegin();
}

LogLine const& LogStream::readNext(Cursor & cursor) {
	return *(cursor++);
}
LogLine const& LogStream::readPrevious(Cursor & cursor) {
	return *(--cursor);
}

ostream & LogStream::error() {
	return *this << headerError;
}
ostream & LogStream::warning() {
	return *this << headerWarning;
}
ostream & LogStream::info() {
	return *this << headerInfo;
}
ostream & LogStream::debug() {
	return *this << headerDebug;
}

LogLine::LogLine(std::string const& s) : str(s), date(chrono::system_clock::now())
{
	
}


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

string LogLine::coloredString(bool includeDate) const {
	long int t = chrono::duration_cast<chrono::milliseconds>(date.time_since_epoch()).count();
	char time[13];
	sprintf(time, "%2d:%02d:%02d.%03d", t/3600000%24, t/60000%60, t/1000%60, t%1000);

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
		res += "\e[0m-/";
	}

	if(includeDate) {
		res += time;
		res += '/';
	}
	res += str.substr(offset) + "\e[0m";

	return res;
}

template<typename Ch, typename Traits>
typename basic_seqbuf<Ch, Traits>::int_type
	basic_seqbuf<Ch, Traits>::overflow(typename basic_seqbuf<Ch, Traits>::int_type ch) {
	
	if(traits_type::eq_int_type(ch, traits_type::eof()))
		return traits_type::eof();

	if(ch == '\n') {
		if(!currentLine.empty()) {
			lines.emplace_back(currentLine);
			currentLine.clear();
		}
	}
	else {
		currentLine += ch;
	}
	return ch;
}
