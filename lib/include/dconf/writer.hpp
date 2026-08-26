#pragma once
#include <cstdint>
#include <format>
#include <iosfwd>
#include <string>

namespace dconf {
enum class WriteAs : std::int8_t { Commented, Uncommented };

class Writer {
  public:
	void write(std::string_view key, std::string_view value, std::string_view comment, WriteAs as);

	template <typename Type>
	void write_uncommented(std::string_view key, Type const& value, std::string_view comment = {}) {
		write_uncommented(key, std::format("{}", value), comment);
	}

	template <typename Type>
	void write_commented(std::string_view key, Type const& value, std::string_view comment = {}) {
		write_commented(key, std::format("{}", value), comment);
	}

	template <std::convertible_to<std::string_view> Type>
	void write_uncommented(std::string_view key, Type const& value, std::string_view comment = {}) {
		write(key, value, comment, WriteAs::Uncommented);
	}

	template <std::convertible_to<std::string_view> Type>
	void write_commented(std::string_view key, Type const& value, std::string_view comment = {}) {
		write(key, value, comment, WriteAs::Commented);
	}

	void print_to(std::ostream& out) const;
	auto save_as(char const* path) const -> bool;

	std::string text{};
};
} // namespace dconf
