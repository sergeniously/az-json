#include <az/json/Reader.h>
#include <algorithm>
#include <istream>
#include <vector>
#include <cstring>

namespace az {
namespace json {

void Source::skipLexeme()
{
	for (auto letter : lexeme) {
		position.column++;
		if (letter == '\n') {
			position.column = 1;
			position.line++;
		}
	}
	lexeme.clear();
}

Source& Source::operator++()
{
	lexeme.push_back(getCharacter());
	skipCharacter();
	return *this;
}

class FileSource : public Source
{
	std::FILE* file;
	std::char_traits<char>::int_type character;

	void skipCharacter() override {
		character = std::fgetc(file);
	}
public:
	explicit FileSource(std::FILE* file)
		: file(file) 
	{
		character = std::fgetc(file);
	}
	std::char_traits<char>::int_type getCharacter() const override {
		return character;
	}
};

class StreamSource : public Source
{
	std::istream& stream;
	void skipCharacter() override {
		stream.get();
	}
public:
	StreamSource(std::istream& stream)
		: stream(stream)
	{}
	std::char_traits<char>::int_type getCharacter() const override {
		return stream.peek();
	}
};

class CStringSource : public Source
{
	const char* source = nullptr;
	void skipCharacter() override {
		source++;
	}
public:
	CStringSource(const char* source)
		: source(source)
	{}
	std::char_traits<char>::int_type getCharacter() const override {
		if (source && *source != 0) {
			return std::char_traits<char>::to_int_type(*source);
		}
		return std::char_traits<char>::eof();
	}
};

Reader& Reader::withNoThrows(bool v /*= true*/)
{
	options.no_throws = v;
	return *this;
}

Reader& Reader::strictly(bool v /*= true*/)
{
	options.strictly = v;
	return *this;
}

Reader::Reader(Value& root, const Options& options /*= {}*/)
	: root(root), options(options)
{
}

Reader::Token Reader::nextToken(Source& source) const
{
	enum class State {
		Begin,
		String,
		EscapedChar,
		UnicodeChar,
		NumberSign,
		NumberLiteral,
		NumberKeyword,
		Fraction,
		ExponentSign,
		ExponentHead,
		ExponentBody,
		Hexadecimal,
		Identifier,
		Comment,
		SingleComment,
		PluralComment,
		Whitespace
	} state = State::Begin;

	uint8_t unicode_size = 0;

	for (source.skipLexeme() ;; ++source) {
		auto character = source.getCharacter();

		switch (state) {
			case State::Begin:
				switch (character) {
					case std::char_traits<char>::eof():
						return Token::End;
					case ':':
						++source;
						return Token::Assignment;
					case ',':
						++source;
						return Token::Next;
					case '{':
						++source;
						return Token::ObjectBegin;
					case '}':
						++source;
						return Token::ObjectEnd;
					case '[':
						++source;
						return Token::ArrayBegin;
					case ']':
						++source;
						return Token::ArrayEnd;
					case '"':
					case '\'':
						state = State::String;
						break;
					case '-':
					case '+':
						state = State::NumberSign;
						break;
					case '.':
						state = State::Fraction;
						break;
					case '/':
						state = State::Comment;
						break;
					case '_':
						state = State::Identifier;
						break;
					case 0xEF:
					case 0xE2:
						state = State::Whitespace;
						break;
					default:
						if (isdigit(character)) {
							state = State::NumberLiteral;
						}
						else if (isalpha(character)) {
							state = State::Identifier;
						}
						else if (!isspace(character)) {
							return Token::Unknown;
						}
				}
				source.skipLexeme();
				break;

			case State::String: // "" or ''
				if (character == '\\') {
					state = State::EscapedChar;
				}
				else if (character == source.getLexeme().front()) {
					++source;
					return Token::String;
				}
				break;
			case State::EscapedChar:
				if (character == 'u') {
					state = State::UnicodeChar;
					unicode_size = 0;
				}
				else if (!strchr("\"\'\\/bfnrt\n", character)) {
					return Token::Unknown;
				}
				state = State::String;
				break;
			case State::UnicodeChar:
				if (!isxdigit(character)) {
					return Token::Unknown;
				}
				if (++unicode_size >= 4) {
					state = State::String;
				}
				break;
			case State::NumberSign:
				if (isdigit(character)) {
					state = State::NumberLiteral;
				}
				else if (isalpha(character)) {
					state = State::NumberKeyword;
				}
				else if (character == '.') {
					state = State::Fraction;
				}
				else {
					return Token::Unknown;
				}
				break;
			case State::NumberKeyword:
				if (!isalpha(character)) {
					return Token::Real;
				}
				break;
			case State::NumberLiteral:
				if (character == '.') {
					state = State::Fraction;
				}
				else if ((character == 'x' || character == 'X') && source.getLexeme().back() == '0') {
					state = State::Hexadecimal;
				}
				else if (character == 'e' || character == 'E') {
					state = State::ExponentSign;
				}
				else if (!isdigit(character)) {
					return Token::Integer;
				}
				break;
			case State::Fraction:
				if (character == 'e' || character == 'E') {
					state = State::ExponentSign;
				}
				else if (!isdigit(character)) {
					return Token::Real;
				}
				break;
			case State::ExponentSign:
				if (character == '+' || character == '-') {
					state = State::ExponentHead;
				}
				else if (isdigit(character)) {
					state = State::ExponentBody;
				}
				else {
					return Token::Unknown;
				}
				break;
			case State::ExponentHead:
				if (!isdigit(character)) {
					return Token::Unknown;
				}
				state = State::ExponentBody;
				break;
			case State::ExponentBody:
				if (!isdigit(character)) {
					return Token::Real;
				}
				break;
			case State::Hexadecimal:
				if (!isxdigit(character)) {
					return Token::Hex;
				}
				break;
			case State::Identifier:
				if (!isalnum(character) && character != '_') {
					return Token::Identifier;
				}
				break;
			case State::Comment:
				if (character == '/') {
					state = State::SingleComment;
				}
				else if (character == '*') {
					state = State::PluralComment;
				}
				else {
					return Token::Unknown;
				}
				break;
			case State::SingleComment:
				if (character == '\n') {
					state = State::Begin;
				}
				break;
			case State::PluralComment:
				if (character == '/' && source.getLexeme().back() == '*') {
					state = State::Begin;
				}
				break;
			case State::Whitespace: {
				static const std::vector<std::string> whitespaces = {
					{char(0xEF), char(0xBB), char(0xBF)}, // Byte Order Mark
					{char(0xE2), char(0x80), char(0xA8)}, // Line separator
					{char(0xE2), char(0x80), char(0xA9)}, // Paragraph
				};
				if (source.getLexeme().length() == 2) {
					auto sequence = source.getLexeme() + std::char_traits<char>::to_char_type(character);
					for (const auto& whitespace : whitespaces) {
						if (memcmp(whitespace.data(), sequence.data(), sequence.length()) == 0) {
							state = State::Begin;
							source.skipLexeme();
							break;
						}
					}
					if (state != State::Begin) {
						return Token::Unknown;
					}
				}
				break;
			}
			default:
				return Token::Unknown;
		}

	}
	return Token::End;
}

bool Reader::parseObject(Source& source, Value& value)
{
	value = Value(Value::Type::Object);
	auto token = nextToken(source);
	if (token == Token::ObjectEnd) {
		return true;
	}
	for (;; token = nextToken(source)) {
		std::string id;
		switch (token) {
			case Token::ObjectEnd:
				return true;
			case Token::Identifier:
				id = source.getLexeme();
				break;
			case Token::String:
				id = unescapeString(source.getLexeme());
				break;
			default:
				putError("identifier, string or } were expected", source);
				return false;
		}
		if (id.empty()) {
			putError("empty object name", source);
			return false;
		}
		if (nextToken(source) != Token::Assignment) {
			putError("assignment was expected", source);
			return false;
		}
		if (!parseValue(nextToken(source), source, value[id])) {
			putError("value was expected", source);
			return false;
		}
		token = nextToken(source);
		if (token != Token::Next) {
			break;
		}
	}
	if (token != Token::ObjectEnd) {
		putError("'}' or ',' were expected", source);
		return false;
	}
	return true;
}

bool Reader::parseArray(Source& source, Value& value)
{
	value = Value(Value::Type::Array);
	auto token = nextToken(source);
	if (token == Token::ArrayEnd) {
		return true;
	}
	for (;; token = nextToken(source)) {
		if (token == Token::ArrayEnd) {
			return true;
		}
		Value array_value;
		if (!parseValue(token, source, array_value)) {
			putError("value was expected", source);
			return false;
		}
		value.append(std::move(array_value));
		token = nextToken(source);
		if (token != Token::Next) {
			break;
		}
	}
	if (token != Token::ArrayEnd) {
		putError("']' or ',' were expected", source);
		return false;
	}
	return true;
}

bool Reader::parseValue(Token token, Source& source, Value& value)
{
	switch (token) {
		case Token::String:
			value = unescapeString(source.getLexeme());
			break;
		case Token::Integer:
		case Token::Hex: {
			const int base = (token == Token::Hex ? 16 : 10);
			value = int64_t(std::stoll(source.getLexeme(), nullptr, base));
			break;
		}
		case Token::Real:
			value = std::stod(source.getLexeme());
			break;
		case Token::Identifier: {
			const auto& lexeme = source.getLexeme();
			if (lexeme == "false") {
				value = false;
			}
			else if (lexeme == "true") {
				value = true;
			}
			else if (lexeme == "NaN" || lexeme == "Infinity") {
				value = strtod(lexeme.c_str(), nullptr);
			}
			else if (lexeme != "null") {
				value = lexeme;
			}
			break;
		}
		case Token::ArrayBegin:
			return parseArray(source, value);
		case Token::ObjectBegin:
			return parseObject(source, value);
		default:
			putError("value was expected", source);
			return false;
	}
	return true;
}

Reader& Reader::parse(Source& source)
{
	root.reset();
	errors.clear();
	if (parseValue(nextToken(source), source, root)) {
		if (options.strictly && nextToken(source) != Token::End) {
			putError("expected end of file", source);
		}
	}
	return *this;
}

Reader& Reader::parse(const char* text)
{
	CStringSource source(text);
	return parse(source);
}

Reader& Reader::parse(const std::string& text)
{
	IterableSource<std::string::const_iterator> source(text.begin(), text.end());
	return parse(source);
}

Reader& Reader::parse(std::istream& stream)
{
	StreamSource source(stream);
	return parse(source);
}

Reader& Reader::parse(std::FILE* file)
{
	FileSource source(file);
	return parse(source);
}

void Reader::putError(const std::string& reason, const Source& source)
{
	putError(reason, source.getPosition().line, source.getPosition().column);
}

void Reader::putError(const std::string& reason, int line, int column)
{
	errors.push_back(Error(reason, line, column));
	if (!options.no_throws) {
		throw errors.back();
	}
}

bool Reader::hasErrors() const
{
	return !errors.empty();
}

Error Reader::getLastError() const
{
	if (!errors.empty()) {
		return errors.back();
	}
	return Error();
}

std::string Reader::convertUnicode(uint32_t unicode)
{
	std::string utf8;
	utf8.reserve(4);

	// based on http://en.wikipedia.org/wiki/UTF-8

	if (unicode <= 0x7f) {
		utf8 += static_cast<char>(unicode);
	}
	else if (unicode <= 0x7FF) {
		utf8 += static_cast<char>(0xC0 | (0x1f & (unicode >> 6)));
		utf8 += static_cast<char>(0x80 | (0x3f & unicode));
	}
	else if (unicode <= 0xFFFF) {
		utf8 += static_cast<char>(0xE0 | (0xf & (unicode >> 12)));
		utf8 += static_cast<char>(0x80 | (0x3f & (unicode >> 6)));
		utf8 += static_cast<char>(0x80 | (0x3f & unicode));
	}
	else if (unicode <= 0x10FFFF) {
		utf8 += static_cast<char>(0xF0 | (0x7 & (unicode >> 18)));
		utf8 += static_cast<char>(0x80 | (0x3f & (unicode >> 12)));
		utf8 += static_cast<char>(0x80 | (0x3f & (unicode >> 6)));
		utf8 += static_cast<char>(0x80 | (0x3f & unicode));
	}
	return utf8;
}

std::string Reader::unescapeString(const std::string& string)
{
	std::string unescaped;
	unescaped.reserve(string.length());
	std::string unicode;

	enum class State {
		Regular, Escaped, Unicode
	} state = State::Regular;

	auto begin = std::next(string.begin());
	auto end = std::prev(string.end());

	for (auto letter = begin; letter != end; letter++) {
		switch (state) {
			case State::Regular:
				if (*letter == '\\') {
					state = State::Escaped;
				} else {
					unescaped += *letter;
				}
				break;
			case State::Escaped:
				switch (*letter) {
					case 'b':
						unescaped += '\b'; break;
					case 'f':
						unescaped += '\f'; break;
					case 'n':
						unescaped += '\n'; break;
					case 'r':
						unescaped += '\r'; break;
					case 't':
						unescaped += '\t'; break;
					case '\n':
						break; // skip an escaped new line
					case 'u':
						unicode.clear(); break;
					default:
						unescaped += *letter;
				}
				state = (*letter == 'u') ? State::Unicode : State::Regular;
				break;
			case State::Unicode:
				unicode += *letter;
				if (unicode.length() == 4) {
					unescaped += Reader::convertUnicode(std::stoul(unicode, nullptr, 16));
					state = State::Regular;
				}
				break;
			default:
				break;
		}
	}
	return unescaped;
}

Value parse(const std::string& text)
{
	Value json;
	Reader(json).withNoThrows().parse(text);
	return json;
}

Value parse(std::istream& stream)
{
	Value json;
	Reader(json).withNoThrows().parse(stream);
	return json;
}

Value parse(const char* text, std::size_t size)
{
	Value json;
	Reader(json).withNoThrows().parse(text, text + size);
	return json;
}

} /* namespace json */
} /* namespace az */
