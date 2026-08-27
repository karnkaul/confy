#include "confy/from_string.hpp"
#include "klib/unit_test/unit_test.hpp"

namespace {
TEST_CASE(from_string_lowercase) {
	static constexpr std::string_view text_v{"FoObAR"};
	auto lowercase = confy::Lowercase{};
	confy::from_string(text_v, lowercase);
	EXPECT(lowercase.text == "foobar");
}

TEST_CASE(from_string_bool) {
	auto value = false;
	EXPECT(confy::from_string("on", value) && value);
	value = false;
	EXPECT(confy::from_string("TRUE", value) && value);
	value = false;
	EXPECT(confy::from_string("Yes", value) && value);
	EXPECT(confy::from_string("NO", value) && !value);
	value = true;
	EXPECT(confy::from_string("false", value) && !value);
	value = true;
	EXPECT(confy::from_string("OFF", value) && !value);
}

TEST_CASE(from_string_number) {
	auto i = 0;
	EXPECT(confy::from_string("42", i) && i == 42);
	i = 0;
	EXPECT(confy::from_string("-15", i) && i == -15);

	auto f = 0.0f;
	EXPECT(confy::from_string("42", f) && f == 42.0f);
}
} // namespace
