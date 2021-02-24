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


	template <typename C>
	auto get_chars() {
		using namespace std::string_view_literals;
		if constexpr (std::is_same_v<C, char>) { return "abcdef"sv; }
		else if constexpr (std::is_same_v<C, wchar_t>) { return L"abcdef"sv; }
		else if constexpr (std::is_same_v<C, char16_t>) { return u"abcdef"sv; }
		else if constexpr (std::is_same_v<C, char32_t>) { return U"abcdef"sv; }
	}


	template <typename C>
	constexpr auto chars = std::basic_string_view<C>{ "abcdef" };




	template <typename T>
	constexpr auto to_char_func() {
		if constexpr (std::is_pointer_v<T>) { return std::remove_pointer_t<T>{}; }
		else if constexpr (std::is_array_v<T>) { return T{}[0]; }
		else if constexpr (std::is_compound_v<T>) { return T{}[0]; }
		else { return typename T::value_type{}; }
	}

	template <typename T>
	struct CharClass {
		using char_t = decltype(to_char_func<T>());
	};

	template <typename T>
	using to_char_t = typename CharClass<T>::char_t;



	template <typename Str>
	auto foo(Str&& str, std::basic_string_view<to_char_t<Str>> delim = chars < to_char_t<Str>>) {

	}

	auto test() {

		{
			foo(std::string{});
			//foo(std::wstring{});

			auto x = get_chars<wchar_t>();

			auto str = std::string{ "abc" };
			auto wstr = std::wstring{str.begin(), str.end()};

		}

		{
			auto a = "abc";
			auto b = std::string{ "abc" };
			auto c = std::string_view{ "abc" };
			using x = to_char_t<decltype(a)>;
			static_assert(std::is_same_v<to_char_t<decltype(a)>, to_char_t<decltype(b)>>);
			static_assert(std::is_same_v<to_char_t<decltype(b)>, to_char_t<decltype(c)>>);
			static_assert(std::is_same_v<to_char_t<decltype(a)>, to_char_t<decltype(c)>>);
		}

		using namespace prc::detail;


		prc::find_ignore_case("abc", "hej", 0);
		//auto x = std::basic_string<char>{};
		//f(x);
	}

}


TEST_CASE("to_lower") {
	using namespace std::string_view_literals;
	REQUIRE( prc::to_lower("ABC"sv) == "abc"sv );
	REQUIRE( prc::to_lower("ABC") == "abc"sv );
	//REQUIRE(prc::to_lower("ABC") == "abc");
}

TEST_CASE("to_upper()") {
	using namespace std::string_literals;
	REQUIRE(prc::to_upper(std::string_view{ "abc" }) == "ABC");
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






// String splitting
TEST_CASE("split_string"){

	REQUIRE(
		prc::split_string("", "").empty()
	);
	REQUIRE(
		prc::split_string("xxxyxyxy", "xy").empty()
	);

	const auto strs = std::vector<std::string>{
		"a b c",
		"   a b c",
		"  a  b c",
		"  a  b    c ",
		"a b c ",
		"a b c  ",
	};
	for (const auto& str : strs) {
		const auto splitted_sv = prc::split_string_to_views(str, " ");
		REQUIRE(splitted_sv.size() == 3);
		REQUIRE(splitted_sv[0] == "a");
		REQUIRE(splitted_sv[1] == "b");
		REQUIRE(splitted_sv[2] == "c");
		REQUIRE(
			prc::split_string(str, " ") ==
			(std::vector<std::string>{"a", "b", "c"})
		);
	}
	for (const auto& str : strs) {
		const auto splitted_sv = prc::split_string(str, " ");
		REQUIRE(splitted_sv.size() == 3);
		REQUIRE(splitted_sv[0] == "a");
		REQUIRE(splitted_sv[1] == "b");
		REQUIRE(splitted_sv[2] == "c");
		REQUIRE(
			prc::split_string(str, " ") ==
			(std::vector<std::string>{"a", "b", "c"})
		);
	}

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
		prc::to_string(std::variant<int, float, std::string>{std::string{ "hej" }}) ==
		prc::to_string(std::string{ "hej" })
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










