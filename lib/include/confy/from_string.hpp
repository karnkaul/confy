#pragma once
#include <charconv>
#include <concepts>
#include <string>

namespace confy {
namespace detail {
template <typename Type>
[[nodiscard]] auto to_number(std::string_view const str, Type& out, [[maybe_unused]] int const base) -> bool {
	auto const [ptr, ec] = [&] {
		auto const* end = str.data() + str.size();
		if constexpr (std::integral<Type>) {
			return std::from_chars(str.data(), end, out, base);
		} else {
			return std::from_chars(str.data(), end, out);
		}
	}();
	return ec == std::errc{};
}
} // namespace detail

struct Lowercase {
	std::string text{};
};

[[nodiscard]] auto from_string(std::string_view str, bool& out) -> bool;

auto from_string(std::string_view str, Lowercase& out) -> bool;

template <std::integral Type>
[[nodiscard]] auto from_string(std::string_view const str, Type& out, int const base = 10) -> bool {
	return detail::to_number(str, out, base);
}

template <std::floating_point Type>
[[nodiscard]] auto from_string(std::string_view const str, Type& out) -> bool {
	return detail::to_number(str, out, 0);
}

template <typename Type>
	requires(std::assignable_from<Type&, std::string_view>)
auto from_string(std::string_view const str, Type& out) -> bool {
	out = str;
	return true;
}
} // namespace confy
