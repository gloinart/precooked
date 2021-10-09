#include "../include/precooked.hpp"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"


#include <iostream>
#include <map>
#include <type_traits>
#include <mutex>
#include <array>
#include <chrono>
#include <string>
#include <cstddef>








namespace {

auto verify_type_traits() -> void {
	// is_container_v
	static_assert(peo::type_traits::is_container_v<std::vector<int>&&>);
	static_assert(peo::type_traits::is_container_v<std::array<int, 3>>);
	static_assert(peo::type_traits::is_container_v<std::string>);
	static_assert(peo::type_traits::is_container_v<std::map<int, int>>);
	static_assert(!peo::type_traits::is_container_v<int>);

	// is_optional_v
	static_assert(peo::type_traits::is_optional_v<std::optional<int>>);
	static_assert(!peo::type_traits::is_optional_v<std::vector<int>>);

	// is_string_v
	static_assert(peo::type_traits::is_string_v<std::wstring>);
	static_assert(peo::type_traits::is_string_v<std::basic_string_view<char>>);
	static_assert(peo::type_traits::is_string_v<std::basic_string_view<wchar_t>>);
	static_assert(peo::type_traits::is_string_v<std::basic_string_view<char32_t>>);
	static_assert(peo::type_traits::is_string_v<std::basic_string_view<char16_t>>);
	static_assert(peo::type_traits::is_string_v<std::basic_string<char>>);
	static_assert(peo::type_traits::is_string_v<std::basic_string<wchar_t>>);
	static_assert(peo::type_traits::is_string_v<std::basic_string<char32_t>>);
	static_assert(peo::type_traits::is_string_v<std::basic_string<char16_t>>);
	static_assert(!peo::type_traits::is_string_v<int>);

	// is_duration_v
	static_assert(!peo::type_traits::is_duration_v<int>);

	// underlying_char_f
	static_assert(std::is_same_v<peo::type_traits::underlying_char_t<std::string>, char>);
	static_assert(std::is_same_v<peo::type_traits::underlying_char_t<std::string_view>, char>);
	static_assert(std::is_same_v<peo::type_traits::underlying_char_t<char*>, char>);
	static_assert(std::is_same_v<peo::type_traits::underlying_char_t<char[4]>, char>);
	{
		const auto str = "abc";
		using Str = decltype(str);
		static_assert(std::is_same_v<peo::type_traits::underlying_char_t<Str>, char>);
	}

	int x = 0, y = 0;
	peo::tuple_for_each(std::tie(x, y), [](auto&&) {});
}

}



TEST_CASE("to_lower") {
	using namespace std::string_view_literals;
	using namespace std::string_literals;
	REQUIRE(peo::to_lower("ABC"sv) == "abc"sv);
	REQUIRE(peo::to_lower("ABC") == "abc"sv);

	REQUIRE(peo::to_lower(L"ABC") == L"abc"sv);
	REQUIRE(peo::to_lower(u"ABC") == u"abc"sv);
	REQUIRE(peo::to_lower(U"ABC") == U"abc"sv);
	REQUIRE(peo::to_lower("ABC") == "abc");


	REQUIRE(peo::to_lower(L"ABC"s) == L"abc"sv);
	REQUIRE(peo::to_lower(u"ABC"s) == u"abc"sv);
	REQUIRE(peo::to_lower(U"ABC"s) == U"abc"sv);
}

TEST_CASE("to_upper()") {
	using namespace std::string_view_literals;
	using namespace std::string_literals;
	REQUIRE(peo::to_upper("abc"sv) == "ABC");
	REQUIRE(peo::to_upper("abc") == "ABC");
	REQUIRE(peo::to_upper("abc"s) == "ABC");
}




TEST_CASE("file_to_string"){
	namespace fs = std::filesystem;
	auto tmpdir = fs::temp_directory_path();
	auto tmpfile = tmpdir / "test.txt";
	{
		auto content = std::string{ "abcdefgh" };
		peo::write_string_to_file(content, tmpfile);
		REQUIRE(fs::exists(tmpfile));
		auto back = peo::read_file_to_string(tmpfile);
		REQUIRE(back == content);


		{
			peo::write_string_to_file("abc", tmpfile);
			peo::write_string_to_file(u"abc", tmpfile);
			peo::write_string_to_file(U"abc", tmpfile);
		}
	}


	{
		auto content = std::wstring{ L"abcdefgh" };
		peo::write_string_to_file(content, tmpfile);
		REQUIRE(fs::exists(tmpfile));
		auto back = peo::read_file_to_string<wchar_t>(tmpfile);
		REQUIRE(back == content);
	}

	{
		auto content = std::string{ "abcdefg" };
		peo::write_string_to_file(content, tmpfile);
		REQUIRE(fs::exists(tmpfile));
		REQUIRE_THROWS(
			peo::read_file_to_string<wchar_t>(tmpfile)
		);

	}
}




// Tuple
TEST_CASE("tuple"){
	REQUIRE(
		peo::tuple_any_of(std::make_tuple(1, 5, 15),[](auto&& v) {
			return v == 5;
		})
	);
	REQUIRE_FALSE(
		peo::tuple_any_of(std::make_tuple(1, 5, 15), [](auto&& v) {
			return v == 6;
		})
	);

	auto src = std::make_tuple(int(0), 0.0f, 0.0);
	peo::tuple_for_each(src, [](auto&& v) { v += 1; });
	REQUIRE(
		src ==
		std::make_tuple(int(1), 1.0f, 1.0)
	);

}

namespace {
	template <typename Char>
	[[nodiscard]] auto convert_string(std::string str) {
		auto ret = std::basic_string<Char>{ str.begin(), str.end() };
		return ret;
	}
}


// String splitting
TEST_CASE("split_string"){

	REQUIRE(
		peo::split_string("", "").empty()
	);
	REQUIRE(
		peo::split_string("xxxyxyxy", "xy").empty()
	);

	auto test_for_string_type_f = [](auto&& string_type_identifier) {
		using string_t = std::decay_t<decltype(string_type_identifier)>;
		using char_t = typename string_t::value_type;
		const auto char_strs = std::vector<std::string>{
			"a b c",
			"   a b c",
			"  a  b c",
			"  a  b    c ",
			"a b c ",
			"a b c  ",
		};

		auto strs = std::vector<string_t>{};
		for (auto&& char_str : char_strs) {
			strs.emplace_back(convert_string<char_t>(char_str));
		}

		const auto space = convert_string<char_t>(" ");
		const auto a = convert_string<char_t>("a");
		const auto b = convert_string<char_t>("b");
		const auto c = convert_string<char_t>("c");

		for (const auto& str : strs) {
			const auto splitted_sv = peo::split_string_to_views(str, space);
			REQUIRE(splitted_sv.size() == 3);
			REQUIRE(splitted_sv[0] == a);
			REQUIRE(splitted_sv[1] == b);
			REQUIRE(splitted_sv[2] == c);
			REQUIRE(
				peo::split_string(str, space) ==
				(std::vector<string_t>{a, b, c})
			);
		}
		for (const auto& str : strs) {
			const auto splitted_sv = peo::split_string(str, space);
			REQUIRE(splitted_sv.size() == 3);
			REQUIRE(splitted_sv[0] == a);
			REQUIRE(splitted_sv[1] == b);
			REQUIRE(splitted_sv[2] == c);
			REQUIRE(
				peo::split_string(str, space) ==
				(std::vector<string_t>{a, b, c})
			);
		}
	};

	test_for_string_type_f(std::basic_string<char>{});
	test_for_string_type_f(std::basic_string<wchar_t>{});
	test_for_string_type_f(std::basic_string<char16_t>{});
	test_for_string_type_f(std::basic_string<char32_t>{});


	{
		const auto str = std::string{ "abc" };
		REQUIRE(
			peo::split_string(str, "") ==
			std::vector<std::string>{"abc"}
		);
		REQUIRE(
			peo::split_string_to_views(str, "") ==
			std::vector<std::string_view>{"abc"}
		);
	}

	{
		const auto str = std::string{ "abc" };
		REQUIRE(
			peo::split_string(str, "a") ==
			std::vector<std::string>{"bc"}
		);
		REQUIRE(
			peo::split_string_to_views(str, "c") ==
			std::vector<std::string_view>{"ab"}
		);
	}

}



TEST_CASE("find_ignore_case"){
	using namespace std::string_view_literals;
	const auto str = "aa01234abc"sv;
	for (auto i = 0; i < 100; ++i) {
		REQUIRE(
			peo::find_ignore_case(str, "aBc", i) ==
			str.find("abc", i)
		);
	}
	for (auto i = 0; i < 100; ++i) {
		REQUIRE(
			peo::find_ignore_case(str, "aA", i) ==
			str.find("aa", i)
		);
	}
	for (auto i = 0; i < 100; ++i) {
		REQUIRE(
			peo::find_ignore_case(str, "3", i) ==
			str.find("3", i)
		);
	}

}




// String trim
TEST_CASE("trim_string"){

	const auto strs = std::vector<std::string>{
		"a b c",
		"   a b c",
		"  a b c",
		"  a b c ",
		"a b c ",
		"a b c  ",
	};
	for (const auto& str : strs) {
		const auto trimmed = peo::trim_string_to_view(str);
		REQUIRE(trimmed == std::string_view{ "a b c" });
		REQUIRE(peo::trim_string(str) == std::string_view{ "a b c" });
		REQUIRE(
			peo::is_trimmed(str) ==
			(str == peo::trim_string(str))
		);

	}
	for (const auto& str : strs) {
		const auto trimmed = peo::trim_string(str);
		REQUIRE(trimmed == std::string_view{ "a b c" });
		REQUIRE(peo::trim_string(str) == std::string_view{ "a b c" });
		REQUIRE(
			peo::is_trimmed(str) ==
			(str == peo::trim_string(str))
		);
	}
	{
		using namespace std::string_view_literals;
		const auto str = std::string{ "abc" };
		REQUIRE(
			peo::trim_string(str, "") ==
			"abc"
		);
		REQUIRE(
			peo::trim_string_to_view(str, "") ==
			"abc"
		);
	}
}



// String to number
TEST_CASE("string_to_number"){
	REQUIRE(peo::string_to_number<int>("1234") == 1234);
	REQUIRE(peo::string_to_number<double>("1") == 1);
	REQUIRE(peo::string_to_number<double>("1.5") == 1.5);
	REQUIRE(peo::string_to_number<float>("1.5") == 1.5);
	REQUIRE(peo::string_to_number<double>("abc") == std::nullopt);


	REQUIRE(peo::string_to_number<int>("a 1234") == std::nullopt);
	REQUIRE(peo::string_to_number<int>("1234 abc") == std::nullopt);
	REQUIRE(peo::string_to_number<int>("1234 ") == std::nullopt);
	REQUIRE(peo::string_to_number<double>("1  a") == std::nullopt);
	REQUIRE(peo::string_to_number<double>("a1.5") == std::nullopt);
	REQUIRE(peo::string_to_number<double>("abc") == std::nullopt);
}

TEST_CASE("replace_all_ignore_case") {
	using namespace std::string_literals;
	using namespace std::string_view_literals;
	REQUIRE(
		peo::replace_all_ignore_case("ABC", "b", "") ==
		"AC"
	);

	REQUIRE(
		peo::replace_all_ignore_case("ABC"s, "b"sv, ""s) ==
		"AC"s
	);

	REQUIRE(
		peo::replace_all_ignore_case("ABC"sv, "b"sv, ""sv) ==
		"AC"sv
	);

};



// String replace
TEST_CASE("replace_all (std::string)"){
	using namespace std::string_literals;
	REQUIRE(
		peo::replace_all("abcabcb"s, "b", "dd") ==
		"addcaddcdd"
	);

	REQUIRE(
		peo::replace_all("aaaa", "aaaa", "dd") ==
		"dd"
	);


	REQUIRE(
		peo::replace_all("abcabcb"s, "b", "") ==
		"acac"
	);

	REQUIRE(
		peo::replace_all("aa bbb aa"s, "aa", "xxxx") ==
		"xxxx bbb xxxx"
	);

	REQUIRE(
		peo::replace_all("aa bbb aa"s, "aa", "x") ==
		"x bbb x"
	);

	REQUIRE(
		peo::replace_all("aa bbb aa"s, "bbb", "") ==
		"aa  aa"
	);

	REQUIRE(
		peo::replace_all(" aa bbb aa "s, "aa", "xxxx") ==
		" xxxx bbb xxxx "
	);

	REQUIRE(
		peo::replace_all(" aa bbb aa "s, "aa", "bb") ==
		" bb bbb bb "
	);
}

TEST_CASE("replace_all (const char*)") {
	REQUIRE(
		peo::replace_all("abcabcb", "b", "dd") ==
		"addcaddcdd"
	);

	REQUIRE(
		peo::replace_all("abcabcb", "b", "") ==
		"acac"
	);

	REQUIRE(
		peo::replace_all("aa bbb aa", "aa", "xxxx") ==
		"xxxx bbb xxxx"
	);

	REQUIRE(
		peo::replace_all("aa bbb aa", "aa", "x") ==
		"x bbb x"
	);

	REQUIRE(
		peo::replace_all("aa bbb aa", "bbb", "") ==
		"aa  aa"
	);

	REQUIRE(
		peo::replace_all(" aa bbb aa ", "aa", "xxxx") ==
		" xxxx bbb xxxx "
	);
	REQUIRE(
		peo::replace_all(" aa bbb aa ", "aa", "bb") ==
		" bb bbb bb "
	);
}





TEST_CASE("contains_substring_ignore_case"){
	REQUIRE(
		peo::contains_substring_ignore_case("ABCCBA", "cba") ==
		true
	);
	REQUIRE(
		peo::contains_substring_ignore_case("ABCCBA", "abc") ==
		true
	);
	REQUIRE(
		peo::contains_substring_ignore_case("ABCCBA", "cc") ==
		true
	);
	REQUIRE(
		peo::contains_substring_ignore_case("ABCCBA", "b") ==
		true
	);
	REQUIRE(
		peo::contains_substring_ignore_case("ABCCBA", "") ==
		false
	);
	REQUIRE(
		peo::contains_substring_ignore_case("", "") ==
		false
	);
	REQUIRE(
		peo::contains_substring_ignore_case("", "xyz") ==
		false
	);
	REQUIRE(
		peo::is_equal_ignore_case("aaa", "AAA") ==
		true
	);
	REQUIRE(
		peo::is_equal_ignore_case("aAa", "AaA") ==
		true
	);
};



TEST_CASE("join_string"){
	using namespace std::string_view_literals;
	REQUIRE(
		peo::join_strings(std::array{ "a"sv, "b"sv, "c"sv }, "-") ==
		"a-b-c"
	);
	REQUIRE(
		peo::join_strings(std::array{ "a"sv, "b"sv, "c"sv }, "") ==
		"abc"
	);

	REQUIRE(
		peo::join_strings(std::array{ "aa"sv, "bb"sv, "cc"sv }) ==
		"aabbcc"
	);
	REQUIRE(
		peo::join_strings(std::array{ "a"sv, "b"sv, "c"sv }) ==
		"abc"
	);





	REQUIRE(
		peo::join_strings(std::array{ u"a"sv, u"b"sv, u"c"sv }) ==
		u"abc"
	);

};









// To string containers and tuples
TEST_CASE("pretty_string"){

	REQUIRE(peo::pretty_string("abc") == "abc");
	REQUIRE(peo::pretty_string(5) == "5");
	REQUIRE(peo::pretty_string(false) == "false");


	using map_t = std::map<int, std::string>;
	const auto map = map_t{
		{21, "two-one"},
		{33, "three-three"},
	};
	REQUIRE(
		peo::pretty_string(map) ==
		"[[21 two-one] [33 three-three]]"
	);
	REQUIRE(
		peo::pretty_string(map_t{}) ==
		"[]"
	);
	REQUIRE(
		peo::pretty_string(std::vector<int>{}) ==
		"[]"
	);
	REQUIRE(
		peo::pretty_string(std::make_tuple(31, 41, 51)) ==
		"[31 41 51]"
	);
	REQUIRE(
		peo::pretty_string(std::array{ 31, 41, 51 }) ==
		"[31 41 51]"
	);
	REQUIRE(
		peo::pretty_string(std::vector<int>{ 31, 41, 51 }) ==
		"[31 41 51]"
	);
	REQUIRE(
		peo::pretty_string(std::make_pair(6, 7)) ==
		"[6 7]"
	);
	REQUIRE(
		peo::pretty_string(std::optional<int>{}) ==
		"std::nullopt"
	);
	REQUIRE(
		peo::pretty_string(std::variant<int, float, std::string>{std::string{ "hej" }}) ==
		peo::pretty_string(std::string{ "hej" })
	);

	REQUIRE(
		peo::pretty_string(std::make_shared<std::string>("hej")) ==
		"hej"
	);

	REQUIRE(
		peo::pretty_string(std::shared_ptr<std::string>{}) ==
		"nullptr"
	);

	REQUIRE(
		peo::pretty_string(std::unique_ptr<std::string>{}) ==
		"nullptr"
	);

	enum class Enum { A = 0, B = 7};
	REQUIRE(
		peo::pretty_string(Enum::A) ==
		"0"
	);
	REQUIRE(
		peo::pretty_string(Enum::B) ==
		"7"
	);

	;
	REQUIRE(
		peo::pretty_string(std::chrono::seconds{ 1 }) ==
		"1"
	);


	REQUIRE(
		peo::pretty_string(std::chrono::milliseconds{ 1 }) ==
		"1"
	);


	REQUIRE(
		peo::pretty_string(std::wstring{ L"abc" }) ==
		"abc"
	);


	{
		auto sptr = std::make_shared<std::string>( "abc" );

		REQUIRE(
			peo::pretty_string(sptr.get()) ==
			"abc"
		);

	}


	{
		auto sptr = std::make_shared<int>(5);
		auto wptr = std::weak_ptr<int>(sptr);

		REQUIRE(
			peo::pretty_string(wptr) ==
			"5"
		);
		sptr.reset();
		REQUIRE(
			peo::pretty_string(wptr) ==
			"expired"
		);

	}

	struct dummy_t {
		uint8_t a = 10;
		uint8_t b = 11;
		uint8_t c = 12;
		uint8_t d = 13;
	};
	REQUIRE(
		peo::pretty_string(dummy_t{}) ==
		"unknown 0x0a0b0c0d"
	);
};


TEST_CASE("detail::uint8_to_hexstring") {
	auto to_hex_via_sstr_f = [](const int value) {
		auto sstr = std::stringstream{};
		sstr << std::hex << value;
		const auto str = sstr.str();
		PRECOOKED_ASSERT(str.size() == 1 || str.size() == 2);
		return str.size() == 1 ?
			"0" + str :
			str;
	};

	for (int v = 0; v < 256; ++v) {
		REQUIRE(
			to_hex_via_sstr_f(v) ==
			peo::detail::uint8_to_hexstring(static_cast<uint8_t>(v))
		);
	}
	REQUIRE(
		peo::detail::uint8_to_hexstring(0) == "00"
	);
	REQUIRE(
		peo::detail::uint8_to_hexstring(1) == "01"
	);
	REQUIRE(
		peo::detail::uint8_to_hexstring(255) == "ff"
	);
	REQUIRE(
		peo::detail::uint8_to_hexstring(254) == "fe"
	);

}





TEST_CASE("held_type_name"){
	REQUIRE(
		peo::held_type_name(std::variant<int, float, std::string>{std::string{ "hej" }}) ==
		peo::held_type_name(std::string{ "hej" })
	);

	REQUIRE(
		peo::held_type_name(std::variant<int, float, std::string>{int{}}) ==
		peo::held_type_name(int{})
	);
}







TEST_CASE("safe_cast") {
	REQUIRE_THROWS(
		peo::safe_cast<uint8_t>(-1)
	);
	REQUIRE(
		peo::safe_cast<uint8_t>(1) == 1
	);
}





TEST_CASE("detail::scope_exit") {
	int a = 0;
	{
		REQUIRE(a == 0);
		auto scope_exit = peo::detail::scope_exit{ [&a]() {
			a = 1;
		} };
		REQUIRE(a == 0);
	}
	REQUIRE(a == 1);

}










TEST_CASE("make_args") {

	{
		using namespace std::string_literals;
		auto strings = std::array{
			"a\0"s,
			"b\0"s,
			"c\0"s
		};
		auto ptrs = std::array{
			strings[0].data(),
			strings[1].data(),
			strings[2].data()
		};
		{
			const auto facit = std::vector<std::string>{ "a", "b", "c" };
			const auto argc = 3;
			auto argv = ptrs.data();
			auto args = peo::make_args(3, argv);
			REQUIRE(args == facit);
		}
		{
			const auto argc = 0;
			const auto facit = std::vector<std::string>{};
			auto argv = ptrs.data();
			auto args = peo::make_args(0, argv);
			REQUIRE(args == facit);
		}
	}
	REQUIRE_THROWS(
		peo::make_args(-1, nullptr)
	);
	REQUIRE_THROWS(
		peo::make_args(1, nullptr)
	);
}



























////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

#include "../include/precooked_excluded.hpp"

TEST_CASE("vector_to_string") {
	namespace fs = std::filesystem;
	auto tmpdir = fs::temp_directory_path();
	auto tmpfile_path = tmpdir / "test.txt";
	auto data = peo::read_file_to_vector<char>(tmpfile_path);
	peo::write_vector_to_file(data, tmpfile_path);
	REQUIRE(peo::is_vector_equal_to_file_content(data, tmpfile_path));
}










