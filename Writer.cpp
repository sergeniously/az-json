#include "Writer.h"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <limits>
#include <cstring>

namespace az::json {

Writer& Writer::withIndentChar(char v)
{
	options.indent_char = v;
	return *this;
}

Writer& Writer::withIndentSize(uint32_t v)
{
	options.indent_size = v;
	return *this;
}

Writer& Writer::withLeftMargin(uint32_t v)
{
	options.left_margin = v;
	return *this;
}

Writer& Writer::withNewLine(const std::string& v)
{
	options.new_line = v;
	return *this;
}

Writer& Writer::withoutQuoting(bool v /*= true*/)
{
	options.quoting = !v;
	return *this;
}

Writer& Writer::pretty(bool v /*= true*/)
{
	options.pretty = v;
	return *this;
}

void Writer::write(const Value& value)
{
	int level = 0;
	writeIndentation(level);
	writeValue(value, level);
}

void Writer::writeValue(const Value& value, int level)
{
	switch (value.getType()) {
		case Value::Type::Null:
			stream << "null";
			break;
		case Value::Type::Bool:
			stream << (value.asBool() ? "true" : "false");
			break;
		case Value::Type::Integer: {
			if (value.isNegative()) {
				stream << '-';
			}
			stream << std::abs(value.asInteger());
			break;
		}
		case Value::Type::Real: {
			if (value.isNegative()) {
				stream << '-';
			}
			auto real = value.asReal();
			if (std::isinf(real)) {
				stream << "Infinity";
			} else if (std::isnan(real)) {
				stream << "NaN";
			} else {
				stream << std::abs(real);
			}
			break;
		}
		case Value::Type::String:
			stream << '"';
			escape(value.asString());
			stream << '"';
			break;
		case Value::Type::Array:
			stream << '[';
			if (!value.empty()) {
				auto count = value.size();
				for (const auto& array_value : value.getArray()) {
					writeNewLine();
					writeIndentation(level + 1);
					writeValue(array_value, level + 1);
					if (--count > 0) {
						stream << ',';
					}
				}
				writeNewLine();
				writeIndentation(level);
			}
			stream << ']';
			break;
		case Value::Type::Object:
			stream << '{';
			if (!value.empty()) {
				auto count = value.size();
				for (const auto& [identifier, object_value] : value.getObject()) {
					writeNewLine();
					writeIndentation(level + 1);
					writeIdentifier(identifier);
					writeValue(object_value, level + 1);
					if (--count > 0) {
						stream << ',';
					}
				}
				writeNewLine();
				writeIndentation(level);
			}
			stream << '}';
			break;
		default:
			stream << value.asString();
	}
}

void Writer::writeNewLine()
{
	if (options.pretty) {
		stream << options.new_line;
	}
}

void Writer::writeIndentation(int level)
{
	if (options.pretty) {
		stream << std::string(options.left_margin + (level * options.indent_size), options.indent_char);
	}
}

void Writer::writeIdentifier(const std::string& id)
{
	if (options.quoting || !isIdentifier(id)) {
		stream << '"';
		escape(id);
		stream << '"';
	} else {
		stream << id;
	}
	stream << ':';
	if (options.pretty) {
		stream << ' ';
	}
}

int Writer::convertUnicode(const char* begin, const char* end, uint32_t& unicode)
{
	int length = 1;
	const char* next = begin;
	while (next < end && (next - begin < length)) {
		auto byte = static_cast<uint8_t>(*next);
		if (length == 1) {
			if (byte < 0x80) {
				unicode = byte;
			}
			else if (byte < 0xE0) {
				unicode = byte & 0x1F;
				length = 2;
			}
			else if (byte < 0xF0) {
				unicode = byte & 0x0F;
				length = 3;
			}
			else if (byte < 0xF8) {
				unicode = byte & 0x07;
				length = 4;
			}
		}
		else {
			unicode = (unicode << 6) | (byte & 0x3F);
		}
		next++;
	}

	return int(next - begin);
}

void Writer::escape(const char* begin, const char* end)
{
	while (begin < end) {
		int length = 1;
		auto letter = *begin;
		switch (letter) {
			case '"':
			case '\'':
			case '\\':
			case '/':
				stream << '\\' << letter;
				break;
			case '\b':
				stream << "\\b";
				break;
			case '\f':
				stream << "\\f";
				break;
			case '\n':
				stream << "\\n";
				break;
			case '\r':
				stream << "\\r";
				break;
			case '\t':
				stream << "\\t";
				break;
			default: {
				uint32_t unicode = 0;
				length = convertUnicode(begin, end, unicode);
				if (unicode < 0x20 || unicode >= 0x80) {
					auto flags = stream.flags();
					stream << "\\u" << std::setfill('0') << std::setw(4) << std::hex << unicode;
					stream.flags(flags);
				}
				else {
					stream << letter;
				}
			}
		}
		begin += length;
	}
}

void Writer::escape(const std::string& string)
{
	escape(string.c_str(), string.c_str() + string.length());
}

void Writer::escape(const char* string)
{
	escape(string, string + strlen(string));
}

bool Writer::isIdentifier(const std::string& id)
{
	if (!id.empty() && (isalpha(id.front()) || id.front() == '_')) {
		return std::all_of(std::next(id.begin()), id.end(),
			[](auto letter) { return isalnum(letter) || letter == '_'; });
	}
	return false;
}

}
