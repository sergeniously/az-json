#pragma once
#include <list>
#include "Error.h"
#include "Value.h"

namespace az::json
{

class Source
{
	struct Position {
		int line = 1;
		int column = 1;
	} position;
	std::string lexeme;

	// moves to the next character
	virtual void skipCharacter() = 0;
public:
	virtual ~Source() = default;
	void skipLexeme(); // skips the current lexeme
	const std::string& getLexeme() const { return lexeme; }
	const Position& getPosition() const { return position; }
	Source& operator++();

	// returns the current character
	virtual std::char_traits<char>::int_type getCharacter() const = 0;
};

template<class Iterator>
class IterableSource : public Source
{
	Iterator first;
	Iterator last;

	void skipCharacter() override {
		first++;
	}
public:
	IterableSource(Iterator first, Iterator last)
		: first(first), last(last) {}

	std::char_traits<char>::int_type getCharacter() const override {
		if (first < last) {
			return std::char_traits<char>::to_int_type(*first);
		}
		return std::char_traits<char>::eof();
	}
};

class Reader
{
public:
	struct Options {
		// do not allow the source to contain excess data at the end
		bool strictly = false;
		// do not throw exceptions, just collect them
		bool no_throws = false;
		Options() {}
	};
	Reader(Value&, const Options& options = {});
	Reader& withNoThrows(bool = true);
	Reader& strictly(bool = true);

	Reader& parse(Source&);
	Reader& parse(const char*);
	Reader& parse(const std::string&);
	Reader& parse(std::istream&);
	Reader& parse(std::FILE*);

	template<class Iterator>
	Reader& parse(Iterator first, Iterator last) {
		IterableSource source(first, last);
		return parse(source);
	}

	bool hasErrors() const;
	Error getLastError() const;

	static std::string convertUnicode(uint32_t unicode);
	static std::string unescapeString(const std::string&);

private:
	enum class Token {
		Unknown,
		Identifier,
		Assignment,
		ObjectBegin,
		ObjectEnd,
		ArrayBegin,
		ArrayEnd,
		Next,
		String,
		Integer,
		Real,
		Hex,
		End
	};

	Token nextToken(Source&) const;
	bool parseArray(Source&, Value&);
	bool parseObject(Source&, Value&);
	bool parseValue(Token, Source&, Value&);
	void putError(const std::string&, int line = -1, int column = -1);
	void putError(const std::string&, const Source&);
private:
	Value& root;
	Options options;
	std::list<Error> errors;
};

Value parse(const char* text, std::size_t size);
Value parse(const std::string&);
Value parse(std::istream&);

}

inline az::json::Value operator""_json(const char* text, std::size_t size)
{
	return az::json::parse(text, size);
}
