#include "detail/scanner.hpp"
#include "klib/unit_test/unit_test.hpp"

namespace {
TEST_CASE(scanner_empty) {
	auto scanner = confy::detail::Scanner{};
	auto key_value = confy::detail::KeyValue{};
	EXPECT(!scanner.scan_line(key_value, {}));
	EXPECT(key_value.key.empty() && key_value.value.get().empty());
}

TEST_CASE(scanner_comment) {
	auto scanner = confy::detail::Scanner{};
	auto key_value = confy::detail::KeyValue{};
	EXPECT(!scanner.scan_line(key_value, "# this is a comment"));
	EXPECT(key_value.key.empty() && key_value.value.get().empty());
}

TEST_CASE(scanner_key_only) {
	auto scanner = confy::detail::Scanner{};
	auto key_value = confy::detail::KeyValue{};
	EXPECT(scanner.scan_line(key_value, "KEY="));
	EXPECT(key_value.key == "KEY");
	EXPECT(key_value.value.get().empty());

	EXPECT(scanner.scan_line(key_value, "KEY=# comment"));
	EXPECT(key_value.key == "KEY");
	EXPECT(key_value.value.get().empty());
}

TEST_CASE(scanner_key_value) {
	auto scanner = confy::detail::Scanner{};
	auto key_value = confy::detail::KeyValue{};
	EXPECT(scanner.scan_line(key_value, "KEY=value"));
	EXPECT(key_value.key == "KEY");
	EXPECT(key_value.value.get() == "value");

	EXPECT(scanner.scan_line(key_value, "KEY=value# comment"));
	EXPECT(key_value.key == "KEY");
	EXPECT(key_value.value.get() == "value");
}
} // namespace
