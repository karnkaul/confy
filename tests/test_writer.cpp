#include "confy/reader.hpp"
#include "confy/writer.hpp"
#include "klib/unit_test/unit_test.hpp"
#include <sstream>

namespace {
TEST_CASE(writer_empty) {
	auto writer = confy::Writer{};
	writer.write_uncommented({}, "");
	EXPECT(writer.text.empty());
}

TEST_CASE(writer_without_comments) {
	auto writer = confy::Writer{};
	writer.write_uncommented("FOO", "bar");
	writer.write_commented("KEY", 42);
	EXPECT(writer.text == R"(FOO=bar

# KEY=42
)");
}

TEST_CASE(writer_with_comments) {
	auto writer = confy::Writer{};
	writer.write_uncommented("FOO", "bar", "Variable FOO (STRING)");
	writer.write_commented("KEY", 42, "Variable KEY (INT)");
	EXPECT(writer.text == R"(## Variable FOO (STRING)
FOO=bar

## Variable KEY (INT)
# KEY=42
)");
}

TEST_CASE(writer_then_reader) {
	auto writer = confy::Writer{};
	writer.write_uncommented("FOO", "bar");
	writer.write_uncommented("KEY", 42);

	auto str = std::istringstream{std::move(writer.text)};
	auto reader = confy::Reader{};
	EXPECT(reader.read_stream(str));
	EXPECT(reader.value_count() == 2);

	auto foo = std::string{};
	auto key = int{};
	EXPECT(reader.assign_if(foo, "FOO"));
	EXPECT(reader.assign_if(key, "KEY"));

	EXPECT(foo == "bar");
	EXPECT(key == 42);
}
} // namespace
