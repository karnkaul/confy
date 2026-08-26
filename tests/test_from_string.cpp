#include "dconf/from_string.hpp"
#include "klib/unit_test/unit_test.hpp"

namespace {
TEST_CASE(from_string_lowercase) {
	static constexpr std::string_view text_v{"FoObAR"};
	auto lowercase = dconf::Lowercase{};
	dconf::from_string(text_v, lowercase);
	EXPECT(lowercase.text == "foobar");
}

TEST_CASE(from_string_bool) {
	auto value = false;
	EXPECT(dconf::from_string("on", value) && value);
	value = false;
	EXPECT(dconf::from_string("TRUE", value) && value);
	value = false;
	EXPECT(dconf::from_string("Yes", value) && value);
	EXPECT(dconf::from_string("NO", value) && !value);
	value = true;
	EXPECT(dconf::from_string("false", value) && !value);
	value = true;
	EXPECT(dconf::from_string("OFF", value) && !value);
}

TEST_CASE(from_string_number) {
	auto i = 0;
	EXPECT(dconf::from_string("42", i) && i == 42);
	i = 0;
	EXPECT(dconf::from_string("-15", i) && i == -15);

	auto f = 0.0f;
	EXPECT(dconf::from_string("42", f) && f == 42.0f);
}
} // namespace
