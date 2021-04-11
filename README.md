<h1 align="center">JSON5 & JSON C++ Manipulator</h1>

## Introducing

**az-json** is another user friendly and easy to use [JSON5](https://json5.org/) serialization & deserialization library for C++. 
It is also 100% compatible with the classic [JSON](https://www.json.org/)!
It is written using the features from the latest C++20 standard. So you probably won't be able to use this library in your old-fashioned project and could say that it is an disadvantage. But let's look to the future!

## Constructing

The storage for JSON values is presented by az::json::Value class, which can contain one of az::json::Value::Type {Null, Bool, Integer, Real, Array, Object} values. 
So you can explicitly specify a value type during the constructing:
```c++
az::json::Value json(az::json::Value::Type::Null); // makes null
az::json::Value json(az::json::Value::Type::Bool); // makes false
az::json::Value json(az::json::Value::Type::Integer); // makes 0
az::json::Value json(az::json::Value::Type::Real); // makes 0.0
az::json::Value json(az::json::Value::Type::String); // makes ""
az::json::Value json(az::json::Value::Type::Array); // makes []
az::json::Value json(az::json::Value::Type::Object); // makes {}
```
But there are another ways to construct JSON values directly from c++ standard types.

### Null value
By default, az::json::Value is created as the null value. But you can explicitly express it by passing **nullptr** to its constructor:
```c++
az::json::Value json; // default value is null
az::json::Value json = nullptr; // construct null from c++ nullptr keyword
```
### Boolean value
Since c++ has keywords for boolean types, it is very simple to create a JSON boolean value:
```c++
az::json::Value json = true; // create a true json value
az::json::Value json(false); // create a false json value
```
### Integer value
**az-json** stores integer numbers as 64-bit signed value and you can create it either from int or int64_t c++ types:
```c++
az::json::Value json = 123; // create a json integer number from c++ int
az::json::Value json(int64_t(0x1234567890));
```
### Real value
Real numbers are stored as c++ double-typed values and it is possible to construct them from float and double types:
```c++
az::json::Value json = -3.14; // create a json real number from c++ float
az::json::Value json(double(2.78));
```
### String value
There are two sources to create a json string value: byte strings or wide strings.
> **Note**: Internally strings are stored as UTF-8 byte strings, so if you make a value from wide-string it will be converted to UTF-8 byte string.
```c++
az::json::Value json = "byte string";
az::json::Value json = std::string();
az::json::Value json = L"wide string";
az::json::Value json = std::wstring();
```
### Array value
Honestly, there are plenty of the different ways to cunstruct a JSON array. Generally, if you want to create an array from nothing you are supposed to use the initializer lists. It is also possible to construct a JSON array from c++ standard containers using their iterators.
```c++
az::json::Value json = {nullptr, 1, 2.0, "three"}; // make an array with 4 values

std::vector<int> vector = {1, 2, 3};
az::json::Value json(vector.begin(), vector.end()); // make an array with 3 integer numbers

az::json::Value json(az::json::Value::Array{}); // make an empty JSON array []
```
### Object value
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
json["array"].append({"pizza", 0xC0FFEE, INFINITY}); // create an array value in the @json object and add a few values to it
json["object"]["day of week"] = "Sunday"; // add an object inside the @json root object 
```
It is also possible to construct an empty object from empty initializer list surrounded with parentheses.
>**Note**: due to c++ specification it is unfortunately impossible to do so without parentheses (see the differences below).
```c++
az::json::Value json({}); // make an empty JSON object {}
az::json::Value json = {}; // make a JSON null value
```

## Serialization

The simplest way to serialize a JSON value is to call **az::json::Value::stringify(bool)** function which represents a value as a string in pretty (with indentations) or plain (without indentations) format that is controlled by a boolean argument.
```c++
az::json::Value json = {{"anything", 123}};
std::cout << json.stringify(true); // prints json in pretty format (by default)
std::cout << json.stringify(false); // prints json in plain format
```
But if you want to customize a result string of the JSON value in advanced ways, you can use **az::json::Writer** class which takes a reference to std::ostream as an output target and provides several options to control output format. All of them are passed into a constructor of the class.

Take a look at the provided options:
```c++
struct Options {
	bool pretty = false;
	bool quoting = true;
	char indent_char = ' ';
	uint32_t indent_size = 3;
	uint32_t left_margin = 0;
	std::string new_line = "\n";
};
```
Where:
- **pretty** option tells the Writer to use or not indentations and line breaking. If it is true the JSON value will be written in pretty format, otherwise - in plain format i.e. without any spaces between JSON elements.
- **quoting** option tells the Writer how to represent JSON keys in object values. If it's true the keys will always be surrounded with double quotes "". Otherwise if a key responds to a identifier restrictions it won't be surrounded with double quotes i.e. will be represented in JSON5 format.
- **indent char** option sets a character that will be used to make indentations between JSON values. By default it is a space character.
- **indent size** option sets a number of indentation characters per one level of hierarchy. By default it is a number 3.
- **left margin** option sets a number of indentation characters which will be additionally placed before every line of serialized JSON value.
- **new line** option sets a sequence of characters that will separate each line of pretty output format. By default it is a Linux line separator (\n). But it is possible to override it, for example, by a Windows line separator (\r\n).

Let's see how it could be used in the real code:
```c++
az::json::Writer::Options options;
options.pretty = true;
options.quoting = false;
options.indent_char = '\t';
options.indent_size = 1;
options.new_line = "\r\n";

az::json::Writer(std::cout, options).write({{"json", 5}});
```
Actually, there is no need to pass Options structure to a constructor of the Writer class. If so it will use options by default. Also it is not necessary at all to use Options structure to change any option, because there are corresponding class methods to set every option individually in following ways:
```c++
std::stringstream stream;
az::json::Writer(stream)
	.withoutQuoting()
	.withIndentChar('\t')
	.withIndentSize(1)
	.withNewLine("\r\n")
	.pretty().write({{"json",5}});
```

## Deserialization

To transform text to a JSON value **az::json::Reader** class is used. It is able to read and parse text from many sources such as raw strings, FILE pointers, standard input streams. A constructor of the class takes a reference to a target root **az::json::Value** which is supposed to contain all the parsed JSON tree. Also it optionally takes a structure that provides some options to control a parsing process.
```c++
struct Options {
	bool strictly = false;
	bool no_throws = false;
};
```
Where:
- **strictly** option (if true) tells the Reader not to allow the source to contain garbage data at the end. It means there will be an error if after main JSON is successfully parsed there has been left extra text which is not just some spaces. By default it is false.
- **no throws** option (if true) tells the Reader not to throw exceptions but collect them into internal error list which could be then retrieved by calling **getLastError** method. By default it is false.

```c++
az::json::Reader::Options options;
options.strictly = true;
options.no_throws = false;
az::json::Value root;
try {
	az::json::Reader(root, options).parse("{json:5}...");
catch (const az::json::Error& error) {
	std::cout << error.what() << error.line() << error.column();
}
```
Similar to the Writer the Reader class also has methods to set those options individually.
```c++
std::ifstream file();
az::json::Value json;
az::json::Reader(json)
	.withNoThrows().strictly().parse(file);
```
If **no throws** option is turned on exceptions will not be thrown so to know if there was an error or not it might require to manually check for errors by using special methods like hasErrors and getLastError.
```c++
FILE* file = fopen();
az::json::Value json;
az::json::Reader(json) reader;
if (reader.withNoThrows().parse(file).hasErrors()) {
	auto error = reader.getLastError();
}
fclose(file);
```