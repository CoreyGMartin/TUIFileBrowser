#include <Windows.h>
#include <string>
#include <sstream>

/* Console error exception class. */
class Win32ConsoleError {
public:
	using id_type = decltype(GetLastError());
	using file_type = char const *;
	using string_type = std::string;
private:
	id_type code_;
	int	line_;
	file_type file_;
public:
	Win32ConsoleError(int line, file_type file) : code_(GetLastError()), line_(line), file_(file) {}
	auto code() const -> id_type { return code_; }
	auto line() const -> int { return line_; }
	auto file() const -> file_type { return file_; }

	string_type Win32ConsoleError::msg() const;
};

std::string ErrorDescription(unsigned long dwMessageID);

#define THROW_IF_CONSOLE_ERROR(res) { if(!res) throw Win32ConsoleError(__LINE__, __FILE__); }