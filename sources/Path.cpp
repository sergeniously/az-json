#include <az/json/Path.h>
#include <algorithm>

namespace az {
namespace json {

Path::Argument::Argument(const std::string& key)
	: type(Type::Key), key(key)
{
}

Path::Argument::Argument(Argument::Type type)
	: type(type)
{
}

Path::Argument::Argument(Value::Index index)
	: type(Type::Index), index(index)
{
}

bool Path::Argument::isKeySymbol(int symbol)
{
	return symbol == '_' || symbol == '-' || (!ispunct(symbol) && !isspace(symbol));
}

Path::Path(const std::string& path)
	: arguments(parse(path))
{
}

std::list<Path::Argument> Path::parse(const std::string& path)
{
	std::list<Argument> arguments;
	enum class State {
		Start, DotOrBracket, Dot, Bracket, Quote, Key, Index, QuottedKey
	} state = State::Start;
	auto offset = path.begin();
	for (auto current = path.begin(); current != path.end(); current++) {
		switch (state) {
		case State::Start:
			if (*current == '.') {
				state = State::Dot;
			}
			else if (*current == '[') {
				state = State::Bracket;
			}
			else if (*current == '"' || *current == '\'') {
				state = State::Quote;
			}
			else if (Argument::isKeySymbol(*current)) {
				state = State::Key;
			}
			else {
				return {};
			}
			break;
		case State::DotOrBracket:
			if (*current == '.') {
				state = State::Dot;
			}
			else if (*current == '[') {
				state = State::Bracket;
			}
			else {
				return {};
			}
			break;
		case State::Dot:
			if (*current == '"' || *current == '\'') {
				state = State::Quote;
			}
			else if (Argument::isKeySymbol(*current)) {
				offset = current;
				state = State::Key;
			}
			else {
				return {};
			}
			break;
		case State::Bracket:
			if (isdigit(*current)) {
				offset = current;
				state = State::Index;
			}
			else {
				return {};
			}
			break;
		case State::Quote:
			offset = current - 1;
			if (*current != *offset) {
				state = State::QuottedKey;
			}
			else {
				return {};
			}
			break;

		case State::Key:
			if (*current == '.') {
				arguments.push_back(std::string(offset, current));
				state = State::Dot;
			}
			else if (*current == '[') {
				arguments.push_back(std::string(offset, current));
				state = State::Bracket;
			}
			else if (!Argument::isKeySymbol(*current)) {
				return {};
			}
			break;
		case State::Index:
			if (*current == ']') {
				arguments.push_back(std::stoul(std::string(offset, current)));
				state = State::DotOrBracket;
			}
			else if (!isdigit(*current)) {
				return {};
			}
			break;
		case State::QuottedKey:
			if (*current == *offset) {
				arguments.push_back(std::string(offset + 1, current));
				state = State::DotOrBracket;
			}
			break;
		default:
			return {};
		}
	}
	if (state == State::Key) {
		arguments.push_back(std::string(offset, path.end()));
	}
	else if (state == State::Dot && arguments.empty()) {
		arguments.push_back(Argument::Type::Root);
	}
	return arguments;
}

const Value& Path::resolve(const Value& root) const
{
	if (arguments.empty()) {
		return Value::null;
	}
	if (arguments.front().type == Argument::Type::Root) {
		return root;
	}

	const Value* node = &root;
	for (const auto& argument : arguments) {
		if (argument.type == Argument::Type::Key) {
			if (!node->isObject() || !node->has(argument.key.c_str())) {
				// Error: unable to resolve path (object value expected at position...)
				return Value::null;
			}
			node = &((*node)[argument.key]);
		}
		else if (argument.type == Argument::Type::Index) {
			if (!node->isArray() || !node->has(argument.index)) {
				// Error: unable to resolve path (array value expected at position...)
				return Value::null;
			}
			node = &((*node)[argument.index]);
		}
		else {
			return Value::null;
		}
	}
	return *node;
}

Value& Path::make(Value& root) const
{
	Value* node = &root;
	for (const auto& argument : arguments) {
		switch (argument.type) {
			case Path::Argument::Type::Key:
				node = &((*node)[argument.key]);
				break;
			case Path::Argument::Type::Index:
				node = &((*node)[argument.index]);
				break;
			default:
				break;
		}
	}
	return *node;
}

std::string Path::asString() const
{
	std::string output;
	for (const auto& argument : arguments) {
		switch (argument.type) {
			case Path::Argument::Type::Key:
				output += '.';
				if (std::all_of(argument.key.begin(), argument.key.end(), Path::Argument::isKeySymbol)) {
					output += argument.key;
				}
				else {
					output += '\'';
					output += argument.key;
					output += '\'';
				}
				break;
			case Path::Argument::Type::Index:
				output += '[';
				output += std::to_string(argument.index);
				output += ']';
				break;
			default:
				output += '.';
		}
	}
	return output;
}

} /* namespace json */
} /* namespace az */
