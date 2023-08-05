#include <az/json/Reader.h>
#include <az/json/Writer.h>
#include <fstream>

int main(int argc, const char **argv)
{
	if (argc != 3) {
		std::cerr << "Invalid arguments" << std::endl;
		return -1;
	}

	az::json::Value json;
	az::json::Reader reader(json);
	reader.withNoThrows();

	if (std::string(argv[1]) == "-file") {
		std::ifstream file(argv[2]);
		reader.parse(file);
	} else if (std::string(argv[1]) == "-text") {
		reader.parse(argv[2]);
	} else {
		std::cerr << "Invalid option: " << argv[1] << std::endl;
	}

	if (reader.hasErrors()) {
		auto error = reader.getLastError();
		std::cerr << "Error: " << error.what() << 
			" at [" << error.line() << ":" << error.column() << "]" << std::endl;
	} else {
		std::cout << json << std::endl;
	}
	return 0;
}
