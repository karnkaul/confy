#pragma once
#include "confy/from_string.hpp"

namespace confy {
class Value {
  public:
	explicit(false) Value(std::string value = {}) : m_value(std::move(value)) {}

	[[nodiscard]] auto get() const -> std::string_view { return m_value; }

	template <typename Type>
	[[nodiscard]] auto as(Type const& fallback = {}) const -> Type {
		auto ret = Type{};
		if (!assign_if(ret)) { return fallback; }
		return ret;
	}

	template <typename Type>
	auto assign_if(Type& out) const -> bool {
		return from_string(m_value, out);
	}

  private:
	std::string m_value{};
};
} // namespace confy
