#pragma once
#include "dconf/value.hpp"

namespace dconf::detail {
struct KeyValue {
	std::string key{};
	Value value{};
};

class Scanner {
  public:
	auto scan_line(KeyValue& out, std::string_view line) -> bool;

  private:
	auto scan_key(KeyValue& out) -> bool;
	[[nodiscard]] auto scan_value() -> Value;

	std::string_view m_remain{};
};
} // namespace dconf::detail
