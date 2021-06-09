#pragma once
#include <string>
#include <exception>

namespace az::json
{

class Error : public std::exception
{
	struct Context {
		int line = -1;
		int column = -1;
		std::string reason;
	};
public:
	Error() = default;
	Error(const std::string& reason, int line = -1, int column = -1);
	char const* what() const noexcept override;
	int column() const;
	int line() const;

private:
	Context context;
};

}
