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


		static_assert(prc::detail::is_container_v<std::vector<int>&&>);
		static_assert(prc::detail::is_container_v<std::array<int, 3>>);
		static_assert(prc::detail::is_container_v<std::string>);
		static_assert(prc::detail::is_optional_v<std::optional<int>>);

	}



}


TEST_CASE("to_lower") {
	using namespace std::string_view_literals;
	using namespace std::string_literals;
	REQUIRE( prc::to_lower("ABC"sv) == "abc"sv );
	REQUIRE( prc::to_lower("ABC") == "abc"sv );

	REQUIRE(prc::to_lower(L"ABC") == L"abc"sv);
	REQUIRE(prc::to_lower(u"ABC") == u"abc"sv);
	REQUIRE(prc::to_lower(U"ABC") == U"abc"sv);
	//REQUIRE(prc::to_lower("ABC") == "abc");


	REQUIRE(prc::to_lower(L"ABC"s) == L"abc"sv);
	REQUIRE(prc::to_lower(u"ABC"s) == u"abc"sv);
	REQUIRE(prc::to_lower(U"ABC"s) == U"abc"sv);
}

TEST_CASE("to_upper()") {
	using namespace std::string_view_literals;
	using namespace std::string_literals;
	REQUIRE(prc::to_upper("abc"sv) == "ABC");
	REQUIRE(prc::to_upper("abc") == "ABC");
	REQUIRE(prc::to_upper("abc"s) == "ABC");
}




TEST_CASE("file_to_string"){
	namespace fs = std::filesystem;
	auto tmpdir = fs::temp_directory_path();
	auto tmpfile = tmpdir / "test.txt";
	auto content = std::string{ "abcdefg" };
	prc::write_string_to_file(content, tmpfile);
	REQUIRE(fs::exists(tmpfile));
	auto back = prc::read_file_to_string(tmpfile);
	REQUIRE(back == content);


	{
		prc::write_string_to_file("abc", tmpfile);
		prc::write_string_to_file(u"abc", tmpfile);
		prc::write_string_to_file(U"abc", tmpfile);

	}
}




// Tuple
TEST_CASE("tuple"){
	REQUIRE(
		prc::tuple_any_of(std::make_tuple(1, 5, 15),[](auto&& v) {
			return v == 5;
		})
	);
	REQUIRE_FALSE(
		prc::tuple_any_of(std::make_tuple(1, 5, 15), [](auto&& v) {
			return v == 6;
		})
	);

	auto src = std::make_tuple(int(0), 0.0f, 0.0);
	prc::tuple_for_each(src, [](auto&& v) { v += 1; });
	REQUIRE(
		src ==
		std::make_tuple(int(1), 1.0f, 1.0)
	);

}


template <typename Char>
auto convert_string(std::string str) {
	auto ret = std::basic_string<Char>{ str.begin(), str.end() };
	return ret;
}



// String splitting
TEST_CASE("split_string"){

	REQUIRE(
		prc::split_string("", "").empty()
	);
	REQUIRE(
		prc::split_string("xxxyxyxy", "xy").empty()
	);

	auto test_for_string_type = [](auto&& string_type_identifier) {
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
			const auto splitted_sv = prc::split_string_to_views(str, space);
			REQUIRE(splitted_sv.size() == 3);
			REQUIRE(splitted_sv[0] == a);
			REQUIRE(splitted_sv[1] == b);
			REQUIRE(splitted_sv[2] == c);
			REQUIRE(
				prc::split_string(str, space) ==
				(std::vector<string_t>{a, b, c})
			);
		}
		for (const auto& str : strs) {
			const auto splitted_sv = prc::split_string(str, space);
			REQUIRE(splitted_sv.size() == 3);
			REQUIRE(splitted_sv[0] == a);
			REQUIRE(splitted_sv[1] == b);
			REQUIRE(splitted_sv[2] == c);
			REQUIRE(
				prc::split_string(str, space) ==
				(std::vector<string_t>{a, b, c})
			);
		}
	};

	test_for_string_type(std::basic_string<char>{});
	test_for_string_type(std::basic_string<wchar_t>{});
	test_for_string_type(std::basic_string<char16_t>{});
	test_for_string_type(std::basic_string<char32_t>{});


	{
		const auto str = std::string{ "abc" };
		REQUIRE(
			prc::split_string(str, "") ==
			std::vector<std::string>{"abc"}
		);
		REQUIRE(
			prc::split_string_to_views(str, "") ==
			std::vector<std::string_view>{"abc"}
		);
	}

	{
		const auto str = std::string{ "abc" };
		REQUIRE(
			prc::split_string(str, "a") ==
			std::vector<std::string>{"bc"}
		);
		REQUIRE(
			prc::split_string_to_views(str, "c") ==
			std::vector<std::string_view>{"ab"}
		);
	}

}



TEST_CASE("find_ignore_case"){
	using namespace std::string_view_literals;
	const auto str = "aa01234abc"sv;
	for (auto i = 0; i < 100; ++i) {
		REQUIRE(
			prc::find_ignore_case(str, "aBc", i) ==
			str.find("abc", i)
		);
	}
	for (auto i = 0; i < 100; ++i) {
		REQUIRE(
			prc::find_ignore_case(str, "aA", i) ==
			str.find("aa", i)
		);
	}
	for (auto i = 0; i < 100; ++i) {
		REQUIRE(
			prc::find_ignore_case(str, "3", i) ==
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
		const auto trimmed = prc::trim_string_to_view(str);
		REQUIRE(trimmed == std::string_view{ "a b c" });
		REQUIRE(prc::trim_string(str) == std::string_view{ "a b c" });
		REQUIRE(
			prc::is_trimmed(str) ==
			(str == prc::trim_string(str))
		);

	}
	for (const auto& str : strs) {
		const auto trimmed = prc::trim_string(str);
		REQUIRE(trimmed == std::string_view{ "a b c" });
		REQUIRE(prc::trim_string(str) == std::string_view{ "a b c" });
		REQUIRE(
			prc::is_trimmed(str) ==
			(str == prc::trim_string(str))
		);
	}
	{
		using namespace std::string_view_literals;
		const auto str = std::string{ "abc" };
		REQUIRE(
			prc::trim_string(str, "") ==
			"abc"
		);
		REQUIRE(
			prc::trim_string_to_view(str, "") ==
			"abc"
		);
	}
}



// String to number
TEST_CASE("string_to_number"){
	REQUIRE(prc::string_to_number<int>("1234") == 1234);
	REQUIRE(prc::string_to_number<double>("1") == 1);
	REQUIRE(prc::string_to_number<double>("1.5") == 1.5);
	REQUIRE(prc::string_to_number<double>("abc") == std::nullopt);


	REQUIRE(prc::string_to_number<int>("a 1234") == std::nullopt);
	REQUIRE(prc::string_to_number<int>("1234 abc") == std::nullopt);
	REQUIRE(prc::string_to_number<int>("1234 ") == std::nullopt);
	REQUIRE(prc::string_to_number<double>("1  a") == std::nullopt);
	REQUIRE(prc::string_to_number<double>("a1.5") == std::nullopt);
	REQUIRE(prc::string_to_number<double>("abc") == std::nullopt);
}

TEST_CASE("replace_all_ignore_case") {
	using namespace std::string_literals;
	using namespace std::string_view_literals;
	REQUIRE(
		prc::replace_all_ignore_case("ABC", "b", "") ==
		"AC"
	);

	REQUIRE(
		prc::replace_all_ignore_case("ABC"s, "b"sv, ""s) ==
		"AC"s
	);

	REQUIRE(
		prc::replace_all_ignore_case("ABC"sv, "b"sv, ""sv) ==
		"AC"sv
	);

};



// String replace
TEST_CASE("replace_all (std::string)"){
	using namespace std::string_literals;
	REQUIRE(
		prc::replace_all("abcabcb"s, "b", "dd") ==
		"addcaddcdd"
	);

	REQUIRE(
		prc::replace_all("aaaa", "aaaa", "dd") ==
		"dd"
	);


	REQUIRE(
		prc::replace_all("abcabcb"s, "b", "") ==
		"acac"
	);

	REQUIRE(
		prc::replace_all("aa bbb aa"s, "aa", "xxxx") ==
		"xxxx bbb xxxx"
	);

	REQUIRE(
		prc::replace_all("aa bbb aa"s, "aa", "x") ==
		"x bbb x"
	);

	REQUIRE(
		prc::replace_all("aa bbb aa"s, "bbb", "") ==
		"aa  aa"
	);

	REQUIRE(
		prc::replace_all(" aa bbb aa "s, "aa", "xxxx") ==
		" xxxx bbb xxxx "
	);

	REQUIRE(
		prc::replace_all(" aa bbb aa "s, "aa", "bb") ==
		" bb bbb bb "
	);
}

TEST_CASE("replace_all (const char*)") {
	REQUIRE(
		prc::replace_all("abcabcb", "b", "dd") ==
		"addcaddcdd"
	);

	REQUIRE(
		prc::replace_all("abcabcb", "b", "") ==
		"acac"
	);

	REQUIRE(
		prc::replace_all("aa bbb aa", "aa", "xxxx") ==
		"xxxx bbb xxxx"
	);

	REQUIRE(
		prc::replace_all("aa bbb aa", "aa", "x") ==
		"x bbb x"
	);

	REQUIRE(
		prc::replace_all("aa bbb aa", "bbb", "") ==
		"aa  aa"
	);

	REQUIRE(
		prc::replace_all(" aa bbb aa ", "aa", "xxxx") ==
		" xxxx bbb xxxx "
	);
	REQUIRE(
		prc::replace_all(" aa bbb aa ", "aa", "bb") ==
		" bb bbb bb "
	);
}





TEST_CASE("contains_substring_ignore_case"){
	REQUIRE(
		prc::contains_substring_ignore_case("ABCCBA", "cba") ==
		true
	);
	REQUIRE(
		prc::contains_substring_ignore_case("ABCCBA", "abc") ==
		true
	);
	REQUIRE(
		prc::contains_substring_ignore_case("ABCCBA", "cc") ==
		true
	);
	REQUIRE(
		prc::contains_substring_ignore_case("ABCCBA", "b") ==
		true
	);
	REQUIRE(
		prc::contains_substring_ignore_case("ABCCBA", "") ==
		false
	);
	REQUIRE(
		prc::contains_substring_ignore_case("", "") ==
		false
	);
	REQUIRE(
		prc::contains_substring_ignore_case("", "xyz") ==
		false
	);
	REQUIRE(
		prc::is_equal_ignore_case("aaa", "AAA") ==
		true
	);
	REQUIRE(
		prc::is_equal_ignore_case("aAa", "AaA") ==
		true
	);
};



TEST_CASE("join_string"){
	using namespace std::string_view_literals;
	REQUIRE(
		prc::join_strings(std::array{ "a"sv, "b"sv, "c"sv }, "-") ==
		"a-b-c"
	);
	REQUIRE(
		prc::join_strings(std::array{ "a"sv, "b"sv, "c"sv }, "") ==
		"abc"
	);

	REQUIRE(
		prc::join_strings(std::array{ "aa"sv, "bb"sv, "cc"sv }) ==
		"aabbcc"
	);
	REQUIRE(
		prc::join_strings(std::array{ "a"sv, "b"sv, "c"sv }) ==
		"abc"
	);





	REQUIRE(
		prc::join_strings(std::array{ u"a"sv, u"b"sv, u"c"sv }) ==
		u"abc"
	);

};









// To string containers and tuples
TEST_CASE("to_string"){

	REQUIRE(prc::to_string("abc") == "abc");
	REQUIRE(prc::to_string(5) == "5");
	REQUIRE(prc::to_string(false) == "false");


	using map_t = std::map<int, std::string>;
	const auto map = map_t{
		{21, "two-one"},
		{33, "three-three"},
	};
	REQUIRE(
		prc::to_string(map) ==
		"[[21 two-one] [33 three-three]]"
	);
	REQUIRE(
		prc::to_string(map_t{}) ==
		"[]"
	);
	REQUIRE(
		prc::to_string(std::vector<int>{}) ==
		"[]"
	);
	REQUIRE(
		prc::to_string(std::make_tuple(31, 41, 51)) ==
		"[31 41 51]"
	);
	REQUIRE(
		prc::to_string(std::array{ 31, 41, 51 }) ==
		"[31 41 51]"
	);
	REQUIRE(
		prc::to_string(std::vector<int>{ 31, 41, 51 }) ==
		"[31 41 51]"
	);
	REQUIRE(
		prc::to_string(std::make_pair(6, 7)) ==
		"[6 7]"
	);
	REQUIRE(
		prc::to_string(std::optional<int>{}) ==
		"std::nullopt"
	);
	REQUIRE(
		prc::to_string(std::variant<int, float, std::string>{std::string{ "hej" }}) ==
		prc::to_string(std::string{ "hej" })
	);

	REQUIRE(
		prc::to_string(std::make_shared<std::string>("hej")) ==
		"hej"
	);

	REQUIRE(
		prc::to_string(std::shared_ptr<std::string>{}) ==
		"nullptr"
	);

	REQUIRE(
		prc::to_string(std::unique_ptr<std::string>{}) ==
		"nullptr"
	);

	enum class Enum { A = 0, B = 7};
	REQUIRE(
		prc::to_string(Enum::A) ==
		"0"
	);
	REQUIRE(
		prc::to_string(Enum::B) ==
		"7"
	);

	;
	REQUIRE(
		prc::to_string(std::chrono::seconds{ 1 }) ==
		"1"
	);
};







TEST_CASE("held_type_name"){
	REQUIRE(
		prc::held_type_name(std::variant<int, float, std::string>{std::string{ "hej" }}) ==
		prc::held_type_name(std::string{ "hej" })
	);

	REQUIRE(
		prc::held_type_name(std::variant<int, float, std::string>{int{}}) ==
		prc::held_type_name(int{})
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
	auto data = prc::read_file_to_vector<char>(tmpfile_path);
	prc::write_vector_to_file(data, tmpfile_path);
	REQUIRE(prc::is_vector_equal_to_file_content(data, tmpfile_path));
}










