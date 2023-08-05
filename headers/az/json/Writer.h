#pragma once
#include <iostream>
#include "Value.h"

namespace az {
namespace json {

class Writer
{
public:
	struct Options {
		bool pretty = false;
		bool quoting = true;
		char indent_char = ' ';
		uint32_t indent_size = 3;
		uint32_t left_margin = 0;
		std::string new_line = "\n";
		Options() {}
	};
	Writer(std::ostream& stream, const Options& options = {})
		: stream(stream), options(options) {}

	Writer& pretty(bool = true);
	Writer& withIndentChar(char);
	Writer& withIndentSize(uint32_t);
	Writer& withLeftMargin(uint32_t);
	Writer& withNewLine(const std::string&);
	Writer& withoutQuoting(bool = true);

	void write(const Value&);
	void escape(const char*);
	void escape(const std::string&);
	void escape(const char* begin, const char* end);
	static int convertUnicode(const char* begin, const char* end, uint32_t& unicode);
	static bool isIdentifier(const std::string&);

private:
	void writeNewLine();
	void writeIndentation(int level);
	void writeIdentifier(const std::string&);
	void writeValue(const Value&, int level);

private:
	std::ostream& stream;
	Options options;
};

} /* namespace json */
} /* namespace az */
