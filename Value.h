#pragma once
#include <map>
#include <deque>
#include <string>
#include <initializer_list>
#include <ostream>

namespace az::json
{

class Value final
{
public:
	enum class Type : uint8_t {
		Null,
		Bool,
		Real,
		Integer,
		String,
		Array,
		Object
	};
	using Index = uint32_t;
	using String = std::string;
	using Array = std::deque<Value>;
	using Object = std::map<String,Value>;
	static const Value null;

	void reset(Type = Type::Null);

	Value() = default;
	Value(Type);
	Value(bool);
	Value(int);
	Value(int64_t);
	Value(uint64_t);
	Value(float);
	Value(double);
	Value(const char*);
	Value(std::string&&);
	Value(const std::string&);
	Value(const wchar_t*);
	Value(const std::wstring&);
	Value(const Array&);
	Value(const Object&);
	Value(std::initializer_list<Value>&&);
	Value(const Value&);
	Value(Value&&) noexcept;

	template<class Iterator>
	Value(Iterator begin, Iterator end) {
		reset(Type::Array);
		any.array_->assign(begin, end);
	}

	Value(std::nullptr_t) {}
	Value(void*) = delete;
	~Value();

	Value& operator=(const Value&);
	Value& operator=(Value&&) noexcept;

	void swap(Value&);
	bool empty() const;
	uint32_t size() const;
	Type getType() const;
	static const char* getTypeString(Type);
	const char* getTypeString() const;

	bool isNull() const;
	bool isBool() const;
	bool isInteger() const;
	bool isReal() const;
	bool isString() const;
	bool isArray() const;
	bool isObject() const;
	bool isNegative() const;

	std::wstring asWideString() const;
	std::string asEscapedString() const;
	std::string asString() const;
	int64_t asInteger() const;
	double asReal() const;
	bool asBool() const;

	operator int() const;
	operator int64_t() const;
	operator uint64_t() const;
	operator std::string() const;
	operator std::wstring() const;
	operator double() const;
	operator float() const;
	operator bool() const;

	Value& convert(Type);
	Value convert(Type) const;

	bool operator==(const Value&) const;
	bool operator!=(const Value&) const;
	bool operator<(const Value&) const;
	bool operator>(const Value&) const;
	bool operator<=(const Value&) const;
	bool operator>=(const Value&) const;

	Value& append(Value&&);
	Value& append(const Value&);

	Value& operator[](const char*);
	Value& operator[](const std::string&);
	Value& operator[](Index);
	Value& operator[](int);

	const Value& operator[](const char*) const;
	const Value& operator[](const std::string&) const;
	const Value& operator[](Index) const;
	const Value& operator[](int) const;

	Value get(const std::string& key, const Value& default_value = Value::null) const;

	template<class T>
	bool getTo(const std::string& key, T& target) const {
		if (has(key)) {
			target = T((*this)[key]);
			return true;
		}
		return false;
	}

	bool has(const std::string&) const;
	bool has(Index) const;

	std::string stringify() const;

	const Array& getArray() const;
	const Object& getObject() const;

private:
	Type type = Type::Null;

	union {
		bool bool_;
		int64_t integer_;
		double real_;
		String* string_;
		Array* array_;
		Object* object_;
	} any = {};
};

}

namespace std
{
ostream& operator<<(ostream& stream, const az::json::Value::Type&);
ostream& operator<<(ostream& stream, const az::json::Value&);
}
