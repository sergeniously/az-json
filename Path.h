#pragma once
#include <list>
#include "Value.h"

namespace az::json
{
	class Path
	{
	public:
		Path(const std::string& path);
		const Value& resolve(const Value& root) const;
		Value& make(Value& root) const;
		std::string asString() const;
	private:
		struct Argument {
			enum class Type {
				None, Root, Key, Index
			};
			Type type = Type::None;
			Value::Index index = 0;
			std::string key;

			Argument() = default;
			Argument(Argument::Type type);
			Argument(const std::string& key);
			Argument(Value::Index index);
			static bool isKeySymbol(int symbol);
		};
		static std::list<Argument> parse(const std::string& path);
		std::list<Argument> arguments;
	};
}