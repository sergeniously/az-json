#pragma once
#include <map>
#include <deque>
#include <string>
#include <initializer_list>
#include <ostream>
#include <iterator>

namespace az::json
{

class Iterator;

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

	void assign(bool);
	void assign(int);
	void assign(int64_t);
	void assign(uint64_t);
	void assign(float);
	void assign(double);
	void assign(const char*);
	void assign(std::string&&);
	void assign(const std::string&);
	void assign(const wchar_t*);
	void assign(const std::wstring&);
	void assign(const Array&);
	void assign(const Object&);
	void assign(Value&&) noexcept;
	void assign(const Value&);

	Value(Type type = Type::Null) {
		reset(type);
	}

	Value(std::nullptr_t) {}
	
	template<class T>
	Value(T&& value) {
		assign(value);
	}

	Value(Value&& other) noexcept {
		assign(other);
	}

	Value(const Value& other) {
		assign(other);
	}

	Value(std::initializer_list<Value>);

	template<class Iterator>
	Value(Iterator begin, Iterator end) {
		reset(Type::Array);
		any.array_->assign(begin, end);
	}

	Value(void*) = delete;

	~Value() {
		reset();
	}

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

	bool asBool() const;
	int64_t asInteger() const;
	double asReal() const;
	std::string asString() const;
	std::wstring asWideString() const;

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
	void append(std::initializer_list<Value>);

	Value& operator[](const char*);
	Value& operator[](const std::string&);
	Value& operator[](Index);
	Value& operator[](int);

	const Value& operator[](const char*) const;
	const Value& operator[](const std::string&) const;
	const Value& operator[](Index) const;
	const Value& operator[](int) const;

	Value get(const std::string& key, const Value& default_value = Value::null) const;
	Value get(Index, const Value& default_value = Value::null) const;

	bool has(const std::string&) const;
	bool has(Index) const;

	std::string stringify(bool pretty = true) const;

	const Array& getArray() const;
	const Object& getObject() const;

	Iterator begin() const;
	Iterator end() const;

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

class Iterator {
	Value::Type type = Value::Type::Null;
	Value::Array::const_iterator arr_iter;
	Value::Object::const_iterator obj_iter;
public:
	using value_type = Value;
	using pointer = const Value*;
	using reference = const Value&;
	using iterator_category = std::random_access_iterator_tag;
	using difference_type = std::ptrdiff_t;

	Iterator() = default;
	Iterator(const Iterator&) = default;
	Iterator(Value::Array::const_iterator iter) {
		type = Value::Type::Array;
		arr_iter = iter;
	}
	Iterator(Value::Object::const_iterator iter) {
		type = Value::Type::Object;
		obj_iter = iter;
	}
	bool isArray() const {
		return type == Value::Type::Array;
	}
	bool isObject() const {
		return type == Value::Type::Object;
	}
	Iterator::reference value() const {
		if (isArray()) {
			return *arr_iter;
		}
		if (isObject()) {
			return obj_iter->second;
		}
		return Value::null;
	}
	std::string key() const {
		if (isObject()) {
			return obj_iter->first;
		}
		return {};
	}
	Iterator& operator++() {
		if (isArray()) {
			arr_iter++;
		} else if (isObject()) {
			obj_iter++;
		}
		return *this;
	}
	Iterator operator++(int) {
		Iterator iter = *this;
		++(*this);
		return iter;
	}
	bool operator==(const Iterator& other) const {
		return type == other.type &&
			arr_iter == other.arr_iter &&
			obj_iter == other.obj_iter;
	}
	bool operator!=(const Iterator& other) const {
		return !(*this == other);
	}
	Iterator::reference operator*() const {
		return value();
	}
};

}

namespace std
{
ostream& operator<<(ostream& stream, const az::json::Value::Type&);
ostream& operator<<(ostream& stream, const az::json::Value&);
}
