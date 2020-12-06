#include "Value.h"
#include <sstream>
#include <iomanip>
#include <cmath>
#include <cstring>
#include <limits>
#include <algorithm>
#include "Writer.h"
#include "Reader.h"

namespace az::json
{
const Value Value::null;

Value::Value(Type type)
{
	reset(type);
}

Value::Value(bool val)
{
	reset(Type::Bool);
	any.bool_ = val;
}

Value::Value(int val)
{
	reset(Type::Integer);
	any.integer_ = val;
}

Value::Value(int64_t val)
{
	reset(Type::Integer);
	any.integer_ = val;
}

Value::Value(uint64_t val)
	: Value(int64_t(val))
{
}

Value::Value(float val)
{
	reset(Type::Real);
	any.real_ = val;
}

Value::Value(double val)
{
	reset(Type::Real);
	any.real_ = val;
}

Value::Value(const wchar_t* wide_string)
{
	reset(Type::String);
	for (const wchar_t* wide_char = wide_string; *wide_char != 0; wide_char++) {
		any.string_->append(Reader::convertUnicode(*wide_char));
	}
}

Value::Value(const std::wstring& wide_string)
	: Value(wide_string.c_str())
{
}

Value::Value(const char* string)
{
	reset(Type::String);
	any.string_->assign(string);
}

Value::Value(std::string&& string)
{
	reset(Type::String);
	any.string_->assign(string);
}

Value::Value(const std::string& string)
{
	reset(Type::String);
	any.string_->assign(string);
}

Value::Value(const Array& arr)
{
	reset(Type::Array);
	any.array_->assign(arr.begin(), arr.end());
}

Value::Value(const Object& obj)
{
	reset(Type::Object);
	*any.object_ = obj;
}

Value::Value(std::initializer_list<Value> list)
{
	bool is_object = std::all_of(list.begin(), list.end(), [](const Value& value) {
		return value.isArray() && value.size() == 2 && value[0].isString();
	});
	if (!is_object) {
		reset(Type::Array);
		any.array_->assign(
			std::make_move_iterator(list.begin()),
			std::make_move_iterator(list.end())
		);
	} else {
		reset(Type::Object);
		for (auto& pair : list) {
			any.object_->emplace(std::move(*pair[0].any.string_), std::move(pair[1]));
		}
	}
}

Value::Value(const Value& other)
{
	*this = other;
}

Value::Value(Value&& other) noexcept
{
	*this = other;
}

Value& Value::operator=(const Value& other)
{
	reset(other.type);
	switch (type) {
		case Type::String:
			*any.string_ = *other.any.string_;
			break;
		case Type::Array:
			*any.array_ = *other.any.array_;
			break;
		case Type::Object:
			*any.object_ = *other.any.object_;
			break;
		default:
			any = other.any;
	}
	return *this;
}

Value& Value::operator=(Value&& other) noexcept
{
	reset();
	swap(other);
	return *this;
}

Value::~Value()
{
	reset();
}

void Value::swap(Value& other)
{
	std::swap(type, other.type);
	std::swap(any, other.any);
}

void Value::reset(Type type /*= Type::Null*/)
{
	switch (this->type) {
		case Type::String:
			delete any.string_;
			break;
		case Type::Array:
			delete any.array_;
			break;
		case Type::Object:
			delete any.object_;
			break;
		default:
			break;
	}
	any = {};

	switch (type) {
		case Type::String:
			any.string_ = new String();
			break;
		case Type::Array:
			any.array_ = new Array();
			break;
		case Type::Object:
			any.object_ = new Object();
			break;
		default:
			break;
	}
	this->type = type;
}

bool Value::empty() const
{
	switch (type) {
		case Type::String:
			return any.string_->empty();
		case Type::Array:
			return any.array_->empty();
		case Type::Object:
			return any.object_->empty();
		default:
			return type == Type::Null;
	}
}

Value::Type Value::getType() const
{
	return type;
}

const char* Value::getTypeString(Value::Type type)
{
	switch (type) {
		case Type::Null:
			return "null";
		case Type::Bool:
			return "bool";
		case Type::Real:
			return "real";
		case Type::Integer:
			return "integer";
		case Type::String:
			return "string";
		case Type::Array:
			return "array";
		case Type::Object:
			return "object";
		default:
			return "unknown";
	}
}

const char* Value::getTypeString() const
{
	return getTypeString(type);
}

std::wstring Value::asWideString() const
{
	std::wstring wide_string;
	auto byte_string = asString();
	const char* begin = byte_string.c_str();
	const char* end = begin + byte_string.size();
	while (begin < end) {
		uint32_t unicode = 0;
		begin += Writer::convertUnicode(begin, end, unicode);
		wide_string += wchar_t(unicode);
	}
	return wide_string;
}

std::string Value::asString() const
{
	switch (type) {
		case Type::String:
			return *any.string_;
		case Type::Array:
		case Type::Object:
			// throw Error();
			return {};
		default: {
			return stringify();
		}
	}
}

int64_t Value::asInteger() const
{
	switch (type) {
		case Type::Bool:
			return any.bool_;
		case Type::Integer:
			return any.integer_;
		case Type::Real:
			return int64_t(any.real_);
		case Type::String:
			return std::strtoll(any.string_->c_str(), nullptr, 10);
		default:
			// throw Error();
			return 0;
	}
}

double Value::asReal() const
{
	switch (type) {
		case Type::Bool:
			return double(any.bool_);
		case Type::Integer:
			return double(any.integer_);
		case Type::Real:
			return any.real_;
		case Type::String:
			return std::strtod(any.string_->c_str(), nullptr);
		default:
			// throw Error();
			return 0;
	}
}

bool Value::asBool() const
{
	switch (type) {
		case Type::Bool:
			return any.bool_;
		case Type::Integer:
			return any.integer_ != 0;
		case Type::Real:
			return std::isnan(any.real_) || any.real_ == 0 ?
				false : true;
		case Type::String:
			if (!any.string_->empty()) {
				auto compare = [](char left, char right) {
					return std::tolower(left) == right;
				};
				return std::equal(any.string_->begin(), any.string_->end(), std::begin("false"), compare) ?
					false : true;
			}
			return false;
		default:
			// throw Error();
			return false;
	}
}

Value::operator int() const
{
	return int(asInteger());
}

Value::operator int64_t() const
{
	return asInteger();
}

Value::operator uint64_t() const
{
	return asInteger();
}

Value::operator std::string() const
{
	return asString();
}

Value::operator std::wstring() const
{
	return asWideString();
}

Value::operator double() const
{
	return asReal();
}

Value::operator float() const
{
	return float(asReal());
}

Value::operator bool() const
{
	return asBool();
}

bool Value::operator==(const Value& other) const
{
	if (type != other.type) {
		return false;
	}
	switch (type) {
		case Type::Null:
			return true;
		case Type::Bool:
			return any.bool_ == other.any.bool_;
		case Type::Integer:
			return any.integer_ == other.any.integer_;
		case Type::Real:
			return any.real_ == other.any.real_;
		case Type::String:
			return *any.string_ == *other.any.string_;
		case Type::Array:
			return *any.array_ == *other.any.array_;
		case Type::Object:
			return *any.object_ == *other.any.object_;
		default:
			// throw Error
			return false;
	}
}

bool Value::operator!=(const Value& other) const
{
	return !(*this == other);
}

bool Value::operator<(const Value& other) const
{
	if (type != other.type) {
		return type < other.type;
	}
	switch (type) {
		case Type::Null:
			return false;
		case Type::Bool:
			return any.bool_ < other.any.bool_;
		case Type::Integer:
			return any.integer_ < other.any.integer_;
		case Type::Real:
			return any.real_ < other.any.real_;
		case Type::String:
			return *any.string_ < *other.any.string_;
		case Type::Array:
			return *any.array_ < *other.any.array_;
		case Type::Object:
			return *any.object_ < *other.any.object_;
		default:
			// throw Error
			return false;
	}
}

bool Value::operator>(const Value& other) const
{
	return other < *this;
}

bool Value::operator<=(const Value& other) const
{
	return !(other < *this);
}

bool Value::operator>=(const Value& other) const
{
	return !(*this < other);
}

Value& Value::operator[](const char* key)
{
	return (*this)[std::string(key)];
}

Value& Value::operator[](const std::string& key)
{
	if (!isObject()) {
		reset(Type::Object);
	}
	return (*any.object_)[key];
}

const Value& Value::operator[](const char* key) const
{
	return (*this)[std::string(key)];
}

const Value& Value::operator[](const std::string& key) const
{
	if (has(key)) {
		return any.object_->at(key);
	}
	return null;
}

Value Value::get(const std::string& key, const Value& default_value) const
{
	if (has(key)) {
		return (*this)[key];
	}
	return default_value;
}

Value& Value::operator[](Index index)
{
	if (!isArray()) {
		reset(Type::Array);
	}
	if (any.array_->size() <= index) {
		any.array_->resize(index + 1);
	}
	return any.array_->at(index);
}

Value& Value::operator[](int index)
{
	return (*this)[Index(index)];
}

const Value& Value::operator[](Index index) const
{
	if (has(index)) {
		return any.array_->at(index);
	}
	return null;
}

const Value& Value::operator[](int index) const
{
	return (*this)[Index(index)];
}

Value& Value::append(Value&& other)
{
	if (!isArray()) {
		reset(Type::Array);
	}
	any.array_->push_back(other);
	return any.array_->back();
}

Value& Value::append(const Value& other)
{
	if (!isArray()) {
		reset(Type::Array);
	}
	any.array_->push_back(other);
	return any.array_->back();
}

std::string Value::stringify() const
{
	std::stringstream stream;
	Writer(stream).pretty().write(*this);
	return stream.str();
}

bool Value::isNull() const
{
	return type == Type::Null;
}

bool Value::isBool() const
{
	return type == Type::Bool;
}

bool Value::isInteger() const
{
	return type == Type::Integer;
}

bool Value::isReal() const
{
	return type == Type::Real;
}

bool Value::isString() const
{
	return type == Type::String;
}

bool Value::isArray() const
{
	return type == Type::Array;
}

bool Value::isObject() const
{
	return type == Type::Object;
}

bool Value::isNegative() const
{
	switch (type) {
		case Type::Real:
			return std::signbit(any.real_);
		case Type::Integer:
			return any.integer_ < 0;
		default:
			return false;
	}
}

uint32_t Value::size() const
{
	switch (type) {
		case Type::Bool:
			return sizeof(any.bool_);
		case Type::Integer:
			return sizeof(any.integer_);
		case Type::Real:
			return sizeof(any.real_);
		case Type::String:
			return any.string_->size();
		case Type::Array:
			return any.array_->size();
		case Type::Object:
			return any.object_->size();
		default:
			return 0;
	}
}

bool Value::has(const std::string& name) const
{
	return isObject() ? any.object_->contains(name) : false;
}

bool Value::has(Index index) const
{
	return isArray() ? index < any.array_->size() : false;
}

const Value::Array& Value::getArray() const
{
	if (isArray()) {
		return *any.array_;
	}
	throw Error("value is not an array");
}

const Value::Object& Value::getObject() const
{
	if (isObject()) {
		return *any.object_;
	}
	throw Error("value is not an object");
}

Value::Iterator Value::begin() const
{
	if (isArray()) {
		return Iterator(any.array_->begin());
	}
	if (isObject()) {
		return Iterator(any.object_->begin());
	}
	if (isNull()) {
		return Iterator();
	}
	throw std::runtime_error("bad type");
}

Value::Iterator Value::end() const
{
	if (isArray()) {
		return Iterator(any.array_->end());
	}
	if (isObject()) {
		return Iterator(any.object_->end());
	}
	if (isNull()) {
		return Iterator();
	}
	throw std::runtime_error("bad type");
}

Value& Value::convert(Type type)
{
	if (this->type != type) {
		switch (type) {
			case Type::Bool:
				*this = asBool();
				break;
			case Type::Integer:
				*this = asInteger();
				break;
			case Type::Real:
				*this = asReal();
				break;
			case Type::String:
				*this = asString();
				break;
			case Type::Array: {
				Value old_value;
				swap(old_value);
				if (old_value.type == Type::Object) {
					for (auto& kv : *old_value.any.object_) {
						this->append(std::move(kv.second));
					}
				} else {
					this->append(std::move(old_value));
				}
				break;
			}
			case Type::Object: {
				Value old_value;
				swap(old_value);
				if (old_value.type == Type::Array) {
					for (Index id = 0; id < old_value.size(); id++) {
						(*this)["id" + std::to_string(id)].swap(old_value[id]);
					}
				} else {
					(*this)[getTypeString(old_value.type)].swap(old_value);
				}
				break;
			}
			default:
				reset();
		}
	}
	return *this;
}

Value Value::convert(Type type) const
{
	return Value(*this).convert(type);
}

}

namespace std
{

ostream& operator<<(ostream& stream, const az::json::Value::Type& type)
{
	stream << az::json::Value::getTypeString(type);
	return stream;
}

ostream& operator<<(ostream& stream, const az::json::Value& value)
{
	az::json::Writer(stream).pretty().write(value);
	return stream;
}

}
