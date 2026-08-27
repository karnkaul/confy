#include "confy/reader.hpp"
#include "klib/unit_test/unit_test.hpp"
#include <sstream>

namespace {
TEST_CASE(reader_read_line) {
	static constexpr auto line_v = std::string_view{"FOO=bar#comment"};
	auto reader = confy::Reader{};
	EXPECT(reader.read_line(line_v));

	auto const* value = reader.find_value("FOO");
	ASSERT(value);
	EXPECT(value->get() == "bar");

	auto foo = std::string{};
	EXPECT(reader.assign_if(foo, "FOO"));
	EXPECT(foo == "bar");
}

TEST_CASE(reader_read_stream) {
	auto str = std::istringstream{R"(
# comment
WORD=word
INTEGER=42
BOOLEAN=true
PHRASE=more than one word
	)"};

	auto reader = confy::Reader{};
	EXPECT(reader.read_stream(str));

	EXPECT(reader.value_count() == 4);

	auto word = std::string{};
	auto integer = int{};
	auto boolean = bool{};
	auto phrase = std::string{};
	EXPECT(reader.assign_if(word, "WORD"));
	EXPECT(reader.assign_if(integer, "INTEGER"));
	EXPECT(reader.assign_if(boolean, "BOOLEAN"));
	EXPECT(reader.assign_if(phrase, "PHRASE"));

	EXPECT(word == "word");
	EXPECT(integer == 42);
	EXPECT(boolean == true);
	EXPECT(phrase == "more than one word");
}
} // namespace
