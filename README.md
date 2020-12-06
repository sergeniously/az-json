<h1 align="center">JSON5 & JSON C++ Manipulator</h1>

## Introducing
**az-json** is another user friendly and easy to use [JSON5](https://json5.org/) serialization & deserialization library for C++. 
It is also 100% compatible with the classic [JSON](https://www.json.org/)!
It is written using the features from the latest C++20 standard. So you probably won't be able to use this library in your old-fashioned project and could say that it is an disadvantage. But let's look to the future!

## How to use

### Constructing
The storage for json values is presented by az::json::Value class, which can contain one of az::json::Value::Type {Null, Bool, Integer, Real, Array, Object} values. So you can explicitly specify a value type during constructing:
```c++
az::json::Value json(az::json::Value::Type::Null); // makes null
az::json::Value json(az::json::Value::Type::Bool); // makes false
az::json::Value json(az::json::Value::Type::Integer); // makes 0
az::json::Value json(az::json::Value::Type::Real); // makes 0.0
az::json::Value json(az::json::Value::Type::String); // makes ""
az::json::Value json(az::json::Value::Type::Array); // makes []
az::json::Value json(az::json::Value::Type::Object); // makes {}
```
But there are another ways to construct json values from c++ standard types.
#### Null value
By default, az::json::Value is created as the null value. But you can explicitly express it by passing **nullptr** to its constructor:
```c++
az::json::Value json; // default value is null
az::json::Value json = nullptr; // construct null from c++ nullptr keyword
```
#### Boolean value
Since c++ has keywords for boolean types, it is very simple to create a json boolean value:
```c++
az::json::Value json = true; // create a true json value
az::json::Value json(false); // create a false json value
```
#### Integer value
**az-json** stores integer numbers as 64-bit signed value and you can create it either from int or int64_t c++ types:
```c++
az::json::Value json = 123; // create a json integer number from c++ int
az::json::Value json(int64_t(0x1234567890));
```
#### Real value
Real numbers are stored as c++ double-typed values and it is possible to construct them from float and double types:
```c++
az::json::Value json = -3.14; // create a json real number from c++ float
az::json::Value json(double(2.78));
```
#### String value
There are two sources to create a json string value: byte strings or wide strings:
```c++
az::json::Value json = "byte string";
az::json::Value json = std::string();
az::json::Value json = L"wide string";
az::json::Value json = std::wstring();
```
#### Array value

#### Object value
Assume you want to construct a JSON5/JSON object, something like that:
```js
{
	null: null,
	bool: true,
	integer: 2020,
	real: 3.14,
	string: "amazing",
	array: ['pizza', 0xC0FFEE, Infinity],
	object: {
		hour: 13,
		minute: 55,
		"day of week": "sunday",
	}
}
```
With **az-json** library you could simply do the following:
```c++
az::json::Value json = {
	{"null", nullptr},
	{"bool", true},
	{"integer", 2020},
	{"real", 3.14},
	{"string", "amazing"},
	{"array", {"pizza", 0xC0FFEE, Infinity}},
	{"object", {
		{"hour", 13},
		{"minute", 55},
		{"day of week", "Sunday"}
	}}
};
```
... or you still could do this in the classic way:
```c++
az::json::Value json; // make a @json null value
json["bool"] = true; // automatically transform the @json to object and add a boolean value with "bool" key
json["array"].append("pizza"); // create an array value in the @json object and add a string value to it
json["object"]["day of week"] = "Sunday"; // add an object inside the @json root object 
```