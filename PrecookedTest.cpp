// Basic.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "pch.h"
#include "precooked.hpp"
#include <iostream>
#include <map>
#include <type_traits>
#include <mutex>
#include <array>
#include <chrono>

#define LOG(x) std::cout << x << std::endl; 

#define ASSERT_EQ(a, b) \
	if (!((a) == (b))) { \
		std::cout << "Error:" << std::endl; \
		std::cout << "- a:" << #a << std::endl; \
		std::cout << "- a:" << pre::to_string(a) << std::endl; \
		std::cout << "- b:" << #b << std::endl; \
		std::cout << "- b:" << pre::to_string(b) << std::endl; \
	} 



struct dummy_t {
};

struct derived_t : dummy_t {

};

int main()
{
	


	{
		namespace fs = std::filesystem;
		auto tmpdir = fs::temp_directory_path();
		auto tmpfile = tmpdir / "test.txt";
		auto content = std::string{ "abcdefg" };
		pre::write_string_to_file(content, tmpfile);
		ASSERT_EQ(true, fs::exists(tmpfile));
		auto back = pre::read_file_to_string(tmpfile);
		ASSERT_EQ(back, content);

	}
	{

		namespace fs = std::filesystem;
		auto tmpdir = fs::temp_directory_path();
		auto tmpfile = tmpdir / "test.txt";
		auto data = pre::read_file_to_vector<char>(tmpfile);
		pre::write_bytevector_to_file(data, tmpdir / "test.bin");
		ASSERT_EQ(true, pre::is_vector_equal_to_file_content(data, tmpfile));
	}


	
	

	// Tuple
	{
		ASSERT_EQ(
			true,
			pre::tuple_any_of(std::make_tuple(1, 5, 15), [](auto&& v) {
				return v == 5;
				})
		);
		ASSERT_EQ(
			false,
			pre::tuple_any_of(std::make_tuple(1, 5, 15), [](auto&& v) {
				return v == 6;
				})
		);

		auto src = std::make_tuple(int(0), 0.0f, 0.0);
		pre::tuple_for_each(src, [](auto&& v) { v += 1; });
		ASSERT_EQ(
			src,
			std::make_tuple(int(1), 1.0f, 1.0)
		);

	}

	// String splitting
	{
		const auto strs = std::vector<std::string>{
			"a b c",
			"   a b c",
			"  a  b c",
			"  a  b    c ",
			"a b c ",
			"a b c  ",
		};
		for (const auto& str : strs) {
			const auto splitted_sv = pre::split_string_to_sv(str, " ");
			ASSERT_EQ(splitted_sv.size(), 3);
			ASSERT_EQ(splitted_sv[0], "a");
			ASSERT_EQ(splitted_sv[1], "b");
			ASSERT_EQ(splitted_sv[2], "c");
			ASSERT_EQ(
				pre::split_string(str, " "), 
				(std::vector<std::string>{"a", "b", "c"})
			);
		}
		for (const auto& str : strs) {
			const auto splitted_sv = pre::split_string(str, " ");
			ASSERT_EQ(splitted_sv.size(), 3);
			ASSERT_EQ(splitted_sv[0], "a");
			ASSERT_EQ(splitted_sv[1], "b");
			ASSERT_EQ(splitted_sv[2], "c");
			ASSERT_EQ(
				pre::split_string(str, " "),
				(std::vector<std::string>{"a", "b", "c"})
			);
		}

		{
			const auto str = std::string{ "abc" };
			ASSERT_EQ(
				pre::split_string(str, ""),
				std::vector<std::string>{"abc"}
			);
			ASSERT_EQ(
				pre::split_string_to_sv(str, ""),
				std::vector<std::string_view>{"abc"}
			);
		}

	}

	// String trim
	{
		const auto strs = std::vector<std::string>{
			"a b c",
			"   a b c",
			"  a b c",
			"  a b c ",
			"a b c ",
			"a b c  ",
		};
		for (const auto& str : strs) {
			const auto trimmed = pre::trim_string_to_sv(str);
			ASSERT_EQ(trimmed, std::string_view{ "a b c" });
			ASSERT_EQ(pre::trim_string(str), std::string_view{ "a b c" });
			ASSERT_EQ(
				pre::is_trimmed(str),
				str == pre::trim_string(str)
			)

		}
		for (const auto& str : strs) {
			const auto trimmed = pre::trim_string(str);
			ASSERT_EQ(trimmed, std::string_view{ "a b c" });
			ASSERT_EQ(pre::trim_string(str), std::string_view{ "a b c" });
			ASSERT_EQ(
				pre::is_trimmed(str),
				str == pre::trim_string(str)
			)
		}
		{
			const auto str = std::string{ "abc" };
			ASSERT_EQ(
				pre::trim_string(str, ""),
				"abc"
			);
			ASSERT_EQ(
				pre::trim_string_to_sv(str, ""),
				"abc"
			);
		}
	}

	// String to number
	{
		ASSERT_EQ(pre::string_to_number<int>("1234"), 1234);
		ASSERT_EQ(pre::string_to_number<double>("1"), 1);
		ASSERT_EQ(pre::string_to_number<double>("1.5"), 1.5);
		ASSERT_EQ(pre::string_to_number<double>("abc"), std::nullopt);


		ASSERT_EQ(pre::string_to_number<int>("a 1234"), std::nullopt);
		ASSERT_EQ(pre::string_to_number<int>("1234 abc"), std::nullopt);
		ASSERT_EQ(pre::string_to_number<int>("1234 "), std::nullopt);
		ASSERT_EQ(pre::string_to_number<double>("1  a"), std::nullopt);
		ASSERT_EQ(pre::string_to_number<double>("a1.5"), std::nullopt);
		ASSERT_EQ(pre::string_to_number<double>("abc"), std::nullopt);
	}

	{
		LOG(pre::to_string(std::chrono::steady_clock::now()));
		LOG(pre::to_string(std::filesystem::current_path()));
		LOG(pre::to_string(std::mutex{}));
	}


	{
		using namespace std::string_view_literals;
		const auto str = "aa01234abc"sv;
		for (auto i = 0; i < 100; ++i) {
			ASSERT_EQ(
				pre::find_ignore_case(str, "aBc", i),
				str.find("abc", i)
			);
		}
		for (auto i = 0; i < 100; ++i) {
			ASSERT_EQ(
				pre::find_ignore_case(str, "aA", i),
				str.find("aa", i)
			);
		}
		for (auto i = 0; i < 100; ++i) {
			ASSERT_EQ(
				pre::find_ignore_case(str, "3", i),
				str.find("3", i)
			);
		}

	}


	{
		using namespace std::string_literals;
		ASSERT_EQ(pre::to_upper(std::string_view{ "abc" }), "ABC");


	}

	// To string
	{
		ASSERT_EQ(pre::to_string("abc"), "abc");
		ASSERT_EQ(pre::to_string(5), "5");
		ASSERT_EQ(pre::to_string(false), "false");
	}

	// String replace
	{
		using namespace std::string_literals;
		ASSERT_EQ(
			pre::replace_all("abcabcb"s, "b", "dd"),
			"addcaddcdd"
		);

		ASSERT_EQ(
			pre::replace_all("abcabcb"s, "b", ""),
			"acac"
		);

		ASSERT_EQ(
			pre::replace_all("aa bbb aa"s, "aa", "xxxx"),
			"xxxx bbb xxxx"
		);

		ASSERT_EQ(
			pre::replace_all("aa bbb aa"s, "aa", "x"),
			"x bbb x"
		);

		ASSERT_EQ(
			pre::replace_all("aa bbb aa"s, "bbb", ""),
			"aa  aa"
		);

		ASSERT_EQ(
			pre::replace_all(" aa bbb aa "s, "aa", "xxxx"),
			" xxxx bbb xxxx "
		);

		ASSERT_EQ(
			pre::replace_all(" aa bbb aa "s, "aa", "bb"),
			" bb bbb bb "
		);

	}


	{
		ASSERT_EQ(
			pre::contains_substring_ignore_case("ABCCBA", "cba"),
			true
		);
		ASSERT_EQ(
			pre::contains_substring_ignore_case("ABCCBA", "abc"),
			true
		);
		ASSERT_EQ(
			pre::contains_substring_ignore_case("ABCCBA", "cc"),
			true
		);
		ASSERT_EQ(
			pre::contains_substring_ignore_case("ABCCBA", "b"),
			true
		);
		ASSERT_EQ(
			pre::contains_substring_ignore_case("ABCCBA", ""),
			false
		);
		ASSERT_EQ(
			pre::contains_substring_ignore_case("", ""),
			false
		);
		ASSERT_EQ(
			pre::contains_substring_ignore_case("", "xyz"),
			false
		);
		ASSERT_EQ(
			pre::is_equal_ignore_case("aaa", "AAA"),
			true
		);
		ASSERT_EQ(
			pre::is_equal_ignore_case("aAa", "AaA"),
			true
		);
	}


	{
		using namespace std::string_view_literals;
		ASSERT_EQ(
			pre::join_strings(std::array{ "a"sv, "b"sv, "c"sv }, "-"),
			"a-b-c"
		);
		ASSERT_EQ(
			pre::join_strings(std::array{ "a"sv, "b"sv, "c"sv }, ""),
			"abc"
		);
	}

	// To string containers and tuples
	{
		using map_t = std::map<int, std::string>;
		const auto map = map_t{
			{21, "two-one"},
			{33, "three-three"},
		};
		ASSERT_EQ(
			pre::to_string(map),
			"[[21 two-one] [33 three-three]]"
		);
		ASSERT_EQ(
			pre::to_string(map_t{}),
			"[]"
		);
		ASSERT_EQ(
			pre::to_string(std::vector<int>{}),
			"[]"
		);
		ASSERT_EQ(
			pre::to_string(std::make_tuple(31, 41, 51)),
			"[31 41 51]"
		);
		ASSERT_EQ(
			pre::to_string(std::array{ 31, 41, 51 }),
			"[31 41 51]"
		);
		ASSERT_EQ(
			pre::to_string(std::vector<int>{ 31, 41, 51 }),
			"[31 41 51]"
		);
		ASSERT_EQ(
			pre::to_string(std::make_pair(6, 7)),
			"[6 7]"
		);
		ASSERT_EQ(
			pre::to_string(std::variant<int, float, std::string>{std::string{ "hej" }}),
			pre::to_string(std::string{ "hej" })
		);
	}




	// Type info
	{
		ASSERT_EQ(
			pre::held_type_name(std::variant<int, float, std::string>{std::string{ "hej" }}),
			pre::held_type_name(std::string{ "hej" })
		);

		ASSERT_EQ(
			pre::held_type_name(std::variant<int, float, std::string>{int{}}),
			pre::held_type_name(int{})
		);
	}




	if(false){
		auto dir = std::filesystem::path{ "c:\\Users\\vikse\\Dropbox" };
		ASSERT_EQ(true, std::filesystem::is_directory(dir));
		auto files = std::vector<std::filesystem::path>{};
		try {
			files = pre::files_in_directory_tree(dir);
		}
		catch (std::exception e) {
			std::cout << e.what();
		}
		size_t bytes = 0;
		for (auto&& file : files) {
			try {
				auto bv = pre::read_file_to_vector<char>(file);
				std::cout << file << " " << bv.size() << std::endl;
				bytes += bv.size();
			}
			catch (...) {
				std::cout << "failure" << std::endl;
			}
		}
		std::cout << bytes << std::endl;
	}




}
