#pragma once
#include "confy/string_table.hpp"
#include "confy/value.hpp"
#include <iosfwd>

namespace confy {
class Reader {
  public:
	/// \returns true if a valid key was extracted.
	auto read_line(std::string_view line) -> bool;

	/// \returns true if a valid key was extracted.
	auto read_stream(std::istream& out) -> bool;

	/// \returns true if a valid key was extracted.
	auto read_file(char const* path) -> bool;

	/// \returns Pointer to Value if mapped to key, else nullptr.
	[[nodiscard]] auto find_value(std::string_view key) const -> Value const*;

	/// \returns true if Value mapped to key exists and was assigned to out.
	template <typename Type>
	auto assign_if(Type& out, std::string_view const key) const -> bool {
		auto const* entry = find_value(key);
		return entry && entry->assign_if(out);
	}

	[[nodiscard]] auto value_count() const -> std::size_t { return m_table.size(); }
	void clear_values() { m_table.clear(); }

  private:
	StringTable<Value> m_table{};
};
} // namespace confy
