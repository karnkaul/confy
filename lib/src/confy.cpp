#include "confy/from_string.hpp"
#include "confy/reader.hpp"
#include "confy/writer.hpp"
#include "detail/scanner.hpp"
#include <algorithm>
#include <array>
#include <format>
#include <fstream>
#include <ostream>
#include <print>

auto confy::from_string(std::string_view const str, bool& out) -> bool {
	struct Match {
		std::string_view phrase{};
		bool value{};
	};
	static constexpr auto matches_v = std::array{
		Match{.phrase = "true", .value = true},	  Match{.phrase = "on", .value = true},	  Match{.phrase = "yes", .value = true},
		Match{.phrase = "false", .value = false}, Match{.phrase = "off", .value = false}, Match{.phrase = "no", .value = false},
	};

	auto lowercase = Lowercase{};
	from_string(str, lowercase);
	// NOLINTNEXTLINE(readability-qualified-auto)
	auto const it = std::ranges::find_if(matches_v, [&lowercase](Match const& m) { return m.phrase == lowercase.text; });
	if (it != matches_v.end()) {
		out = it->value;
		return true;
	}

	auto number = 0;
	if (from_string(str, number)) {
		out = number != 0;
		return true;
	}

	return false;
}

auto confy::from_string(std::string_view str, Lowercase& out) -> bool {
	out.text.reserve(out.text.size() + str.size());
	for (char const c : str) { out.text.push_back(char(std::tolower(static_cast<unsigned char>(c)))); }
	return true;
}

namespace confy::detail {
namespace {
[[nodiscard]] constexpr auto is_space(char const c) { return c == ' ' || c == '\t'; }

[[nodiscard]] constexpr auto trim_leading_spaces(std::string_view in) {
	while (!in.empty() && is_space(in.front())) { in.remove_prefix(1); }
	return in;
}
} // namespace

auto Scanner::scan_line(KeyValue& out, std::string_view const line) -> bool {
	m_remain = trim_leading_spaces(line);
	if (m_remain.empty()) { return {}; }

	if (!scan_key(out)) { return false; }
	out.value = scan_value();
	return true;
}

auto Scanner::scan_key(KeyValue& out) -> bool {
	auto key = std::string{};
	while (!m_remain.empty()) {
		char const c = m_remain.front();
		m_remain.remove_prefix(1);

		if (is_space(c)) { return false; }

		switch (c) {
		case '#': m_remain = {}; return !out.key.empty();
		case '=': out.key = std::move(key); return !out.key.empty();
		default: key.push_back(c); break;
		}
	}
	return false;
}

auto Scanner::scan_value() -> Value {
	auto ret = std::string{};
	while (!m_remain.empty()) {
		char const c = m_remain.front();
		m_remain.remove_prefix(1);

		switch (c) {
		case '#': m_remain = {}; break;
		default: ret.push_back(c); break;
		}
	}
	return ret;
}
} // namespace confy::detail

namespace confy {
auto Reader::read_line(std::string_view const line) -> bool {
	auto key_value = detail::KeyValue{};
	if (!detail::Scanner{}.scan_line(key_value, line)) { return false; }
	if (key_value.key.empty()) { return false; }

	m_table.insert_or_assign(std::move(key_value.key), std::move(key_value.value));
	return true;
}

auto Reader::read_stream(std::istream& out) -> bool {
	auto ret = false;
	for (auto line = std::string{}; std::getline(out, line);) { ret |= read_line(line); }
	return ret;
}

auto Reader::read_file(char const* path) -> bool {
	auto file = std::ifstream{path};
	return read_stream(file);
}

auto Reader::find_value(std::string_view key) const -> Value const* {
	auto const it = m_table.find(key);
	if (it == m_table.end()) { return nullptr; }
	return &it->second;
}
} // namespace confy

namespace confy {
void Writer::write(std::string_view key, std::string_view value, std::string_view comment, WriteAs as) {
	if (key.empty()) { return; }

	if (!text.empty()) { text.push_back('\n'); }
	if (!comment.empty()) { std::format_to(std::back_inserter(text), "## {}\n", comment); }
	if (as == WriteAs::Commented) { text.append("# "); }
	std::format_to(std::back_inserter(text), "{}={}\n", key, value);
}

void Writer::print_to(std::ostream& out) const {
	if (text.empty()) { return; }
	std::print(out, "{}", text);
}

auto Writer::save_as(char const* path) const -> bool {
	if (!path) { return false; }
	auto file = std::ofstream{path};
	if (!file) { return false; }
	print_to(file);
	return true;
}
} // namespace confy
