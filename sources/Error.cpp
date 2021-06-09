#include "Error.h"

namespace az::json
{

Error::Error(const std::string& reason, int line /*= -1*/, int column /*= -1*/)
{
	context.line = line;
	context.column = column;
	context.reason = reason;
}

char const* Error::what() const noexcept
{
	return context.reason.c_str();
}

int Error::line() const
{
	return context.line;
}

int Error::column() const
{
	return context.column;
}

}