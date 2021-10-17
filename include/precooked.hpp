#pragma once
/////////////////////////////////////////////
//
// License : MIT
// http://opensource.org/licenses/MIT
//

#include <string>
#include <string_view>
#include <vector>
#include <filesystem>
#include <optional>
#include <locale>
namespace peo::detail { class byte_view; }
namespace peo::type_traits {
template <typename T> constexpr auto underlying_char_f() {
	if constexpr (std::is_pointer_v<T>) { return std::remove_pointer_t<T>{}; }
	else if constexpr (std::is_array_v<T>) { return T{}[0]; }
	else { return typename T::value_type{}; }
}
template <typename T> using underlying_char_t = decltype(underlying_char_f<T>());
}



namespace peo {

// IO - Read and write files
template <typename Char = char> [[nodiscard]] auto read_file_to_string(const std::filesystem::path& filepath) -> std::basic_string<Char>;
template <typename T>           [[nodiscard]] auto read_file_to_vector(const std::filesystem::path& filepath) -> std::vector<T>;
template <typename Str>                       auto write_string_to_file(const Str& str, const std::filesystem::path& filepath) -> void;
inline                                        auto write_vector_to_file(const detail::byte_view& bytevector, const std::filesystem::path& filepath) -> void;

// Convert any type to string
template <typename T> [[nodiscard]] auto pretty_string(const T& val) -> std::string;

// String - split
template <typename Str0, typename Str1> [[nodiscard]] auto split_string(const Str0& str, const Str1& delimiters) -> std::vector<std::basic_string<type_traits::underlying_char_t<Str0>>>;
template <typename Str0, typename Str1> [[nodiscard]] auto split_string_to_views(const Str0& str, const Str1& delimiters) -> std::vector<std::basic_string_view<type_traits::underlying_char_t<Str0>>>;
template <typename Char, typename Str0> [[nodiscard]] auto split_string_to_views(std::basic_string<Char>&& str, Str0&& delimiters) -> std::vector<std::basic_string_view<Char>> = delete; // Prevent dangling std::string_view
template <typename Str>                 [[nodiscard]] auto split_string_to_lines(const Str& str) -> std::vector<std::basic_string<type_traits::underlying_char_t<Str>>>;

// String - replace
template <typename Char, typename Str0, typename Str1> [[nodiscard]] auto replace_all(std::basic_string<Char> haystack, const Str0& needle, const Str1& replacement) -> std::basic_string<Char>; // Noexcept if dst.size() <= src.size() 
template <typename Str0, typename Str1, typename Str2> [[nodiscard]] auto replace_all(const Str0& haystack, const Str1& needle, const Str2& replacement) -> std::basic_string<type_traits::underlying_char_t<Str0>>;
template <typename Char, typename Str0, typename Str1> [[nodiscard]] auto replace_all_ignore_case(std::basic_string<Char> haystack, const Str0& needle, const Str1& replacement, const std::locale& loc = std::locale{})->std::basic_string<Char>; // Noexcept if dst.size() <= src.size() 
template <typename Str0, typename Str1, typename Str2> [[nodiscard]] auto replace_all_ignore_case(const Str0& haystack, const Str1& needle, const Str2& replacement, const std::locale& loc = std::locale{}) -> std::basic_string<type_traits::underlying_char_t<Str0>>;

// String - trim
template <typename Str0>                [[nodiscard]] auto is_trimmed(const Str0& str) noexcept -> bool;
template <typename Str0, typename Str1> [[nodiscard]] auto is_trimmed(const Str0& str, const Str1& trim_chars) noexcept -> bool;
template <typename Char>                [[nodiscard]] auto trim_string(std::basic_string<Char> str) noexcept -> std::basic_string<Char>;
template <typename Char, typename Str0> [[nodiscard]] auto trim_string(std::basic_string<Char> str, const Str0& trim_chars) noexcept -> std::basic_string<Char>;
template <typename Str0>                [[nodiscard]] auto trim_string_to_view(const Str0& str) noexcept -> std::basic_string_view<type_traits::underlying_char_t<Str0>>;
template <typename Str0, typename Str1> [[nodiscard]] auto trim_string_to_view(const Str0& str, const Str1& trim_chars) noexcept -> std::basic_string_view<type_traits::underlying_char_t<Str0>>;
template <typename Char>                [[nodiscard]] auto trim_string_to_view(std::basic_string<Char>&& str) noexcept -> std::basic_string_view<Char> = delete; // Prevent dangling std::string_view
template <typename Char, typename Str0> [[nodiscard]] auto trim_string_to_view(std::basic_string<Char>&& str, const Str0& trim_chars) noexcept -> std::basic_string_view<Char> = delete; // Prevent dangling std::string_view

// String - join
template <typename Strings, typename Str>
[[nodiscard]] auto join_strings(const Strings& strings, const Str& delimiter) -> std::basic_string<type_traits::underlying_char_t<Str>>;
template <typename Strings, typename Char = typename Strings::value_type::value_type>
[[nodiscard]] auto join_strings(const Strings& strings) -> std::basic_string<Char>;

// String - case insensitive compare
template <typename Str0, typename Str1> [[nodiscard]] auto find_ignore_case(const Str0& haystack, const Str1& needle, const std::locale& loc = std::locale{}) noexcept -> size_t;
template <typename Str0, typename Str1> [[nodiscard]] auto find_ignore_case(const Str0& haystack, const Str1& needle, size_t offset, const std::locale& loc = std::locale{}) noexcept -> size_t;
template <typename Str0, typename Str1> [[nodiscard]] auto is_equal_ignore_case(const Str0& a, const Str1& b, const std::locale& loc = std::locale{}) noexcept -> bool;
template <typename Str0, typename Str1> [[nodiscard]] auto contains_substring(const Str0& haystack, const Str1& needle) noexcept -> bool;
template <typename Str0, typename Str1> [[nodiscard]] auto contains_substring_ignore_case(const Str0& haystack, const Str1& needle, const std::locale& loc = std::locale{}) noexcept -> bool;

// String - case conversion
template <typename Char>    [[nodiscard]] auto to_lower(std::basic_string<Char> str, const std::locale& loc = std::locale{}) noexcept -> std::basic_string<Char>;
template <typename Char>    [[nodiscard]] auto to_upper(std::basic_string<Char> str, const std::locale& loc = std::locale{}) noexcept -> std::basic_string<Char>;
template <typename StrView> [[nodiscard]] auto to_lower(const StrView& str, const std::locale& loc = std::locale{}) -> std::basic_string<type_traits::underlying_char_t<StrView>>;
template <typename StrView> [[nodiscard]] auto to_upper(const StrView& str, const std::locale& loc = std::locale{}) -> std::basic_string<type_traits::underlying_char_t<StrView>>;

// String to number conversion
template <typename T> [[nodiscard]] auto string_to_number(std::string_view str) noexcept -> std::optional<T>;
template <typename T> [[nodiscard]] auto number_to_string(const T& number) -> std::string; // Just here for consistency, simply uses std::to_string(...) underneath

// Scan filesystem
[[nodiscard]] inline auto list_files_in_directory(const std::filesystem::path& dir) -> std::vector<std::filesystem::path>;
[[nodiscard]] inline auto list_files_in_directory_tree(const std::filesystem::path& dir) -> std::vector<std::filesystem::path>;
[[nodiscard]] inline auto list_subdirs_in_directory(const std::filesystem::path& dir) -> std::vector<std::filesystem::path>;
[[nodiscard]] inline auto list_subdirs_in_directory_tree(const std::filesystem::path& dir) -> std::vector<std::filesystem::path>;


// Tuple iteration (convenience)
template <typename Tpl, typename Func>               auto tuple_for_each(Tpl&& tpl, Func&& func) -> void;
template <typename Tpl, typename Func> [[nodiscard]] auto tuple_any_of(const Tpl& tpl, const Func& func) -> bool;

// Safe cast
template <typename DstType, typename SrcType>
[[nodiscard]] auto safe_cast(const SrcType& src_type) -> DstType;

// Type names
template <typename T> [[nodiscard]] auto type_name() -> std::string;
template <typename T> [[nodiscard]] auto type_name(T&&) -> std::string;
template <typename T> [[nodiscard]] auto held_type_name(const T& value) -> std::string;

// Make args - convert main(argc, char* argv[]) to a vector of non-empty strings
[[nodiscard]] inline auto make_args(int argc, char* argv[]) -> std::vector<std::string>;

}



































//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
#if __cplusplus >= 201803
	#define PRECOOKED_UNLIKELY [[unlikely]]
	#define PRECOOKED_LIKELY [[likely]]
#else
	#define PRECOOKED_UNLIKELY
	#define PRECOOKED_LIKELY
#endif


#ifndef PRECOOKED_ASSERT
	#ifdef NDEBUG
		#define PRECOOKED_ASSERT(x) for(;false;) { [[maybe_unused]] auto result = (x); }
	#else
		#include <cassert>
		#define PRECOOKED_ASSERT(x) assert(x)
	#endif
#endif

#include <type_traits>


class peo::detail::byte_view {
public:
	using byte_t = char;
	// Constructor is intentionally NOT marked explicit
	template <
		typename C, 
		typename T = typename C::value_type
	>
	constexpr byte_view(const C& c) noexcept
	: ptr_{ reinterpret_cast<const byte_t*>(c.data()) }
	, num_elements_{ c.size() }
	, element_size_{ sizeof(T) } {
		using ptr_t = decltype(c.data());
		using value_t = std::decay_t<std::remove_pointer_t<ptr_t>>;
		static_assert(std::is_arithmetic_v<value_t>, "Non-arithmetic values is not allowed");
		static_assert(!std::is_same_v<std::vector<bool>, std::decay_t<C>>, "std::vector<bool> is not allowed");
	}
	[[nodiscard]] constexpr auto size() const noexcept { return num_elements_ * element_size_; } // In bytes
	[[nodiscard]] constexpr auto begin() const noexcept { return ptr_; }
	[[nodiscard]] constexpr auto end() const noexcept { return ptr_ + size(); }
	[[nodiscard]] constexpr auto data() const noexcept { return ptr_; }
	[[nodiscard]] constexpr auto empty() const noexcept { return num_elements_ == 0; }

private:
	const byte_t* ptr_{ nullptr };
	size_t num_elements_{ 0 };
	size_t element_size_{ 0 };
};



//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

namespace peo::detail {
template <typename F>
class scope_exit {
public:
	scope_exit(F&& f) : f_{ std::forward<F>(f) } {}
	scope_exit() = delete;
	scope_exit(const scope_exit&) = delete;
	scope_exit(scope_exit&&) = default;
	scope_exit& operator=(const scope_exit&) = delete;
	scope_exit& operator=(scope_exit&&) = delete;
	~scope_exit() { f_(); }
private:
	F f_{};
};
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#include <exception>
#include <filesystem>


namespace peo::exceptions {

class io_exception : public std::runtime_error {
	using inherited = std::runtime_error;
public:
	io_exception(std::string msg, std::filesystem::path path)
	: inherited{ msg + ": " + path.string() } 
	{}
};


class filesize_not_compatible_with_typesize_exception : public io_exception {
public:
	filesize_not_compatible_with_typesize_exception(
		std::filesystem::path path, 
		size_t filesize, 
		size_t element_size, 
		const std::string& type_name
	) : io_exception{ 
		"filesize " + std::to_string(filesize) + " not aligned with element size: " + std::to_string(element_size) + "[type:" + type_name + "]",
		std::move(path) 
	} 
	{}
};

class is_not_file_exception : public io_exception {
public:
	is_not_file_exception(std::filesystem::path path)
	: io_exception{ "expected regular file", std::move(path) } {}
};

class is_not_directory_exception : public io_exception {
public:
	is_not_directory_exception(std::filesystem::path path)
	: io_exception{ "expected directory", std::move(path) } {}
};

class file_not_found_exception : public io_exception {
public:
	file_not_found_exception(std::filesystem::path path)
	: io_exception{ "file not found", std::move(path) } {}
};

class dir_not_found_exception : public io_exception {
public:
	dir_not_found_exception(std::filesystem::path path)
	: io_exception{ "directory not found", std::move(path) } {}
};

class write_file_exception : public io_exception {
public:
	write_file_exception(std::filesystem::path path)
	: io_exception{ "cannot open file for write", std::move(path) } {}
};

class read_file_exception : io_exception {
public:
	read_file_exception(std::filesystem::path path)
	: io_exception{ "cannot open file file for read", std::move(path) } {}
};

class file_too_large_exception : public io_exception {
public:
	file_too_large_exception(std::filesystem::path path, uintmax_t filesize)
	: io_exception{ 
		"file too large [filesize:" + std::to_string(filesize) + "]", 
		std::move(path) } 
	{}
};

class bad_cast_exception : public std::bad_cast {
public:
	bad_cast_exception(std::string msg) noexcept : msg_{ std::move(msg) } {}
	[[nodiscard]] auto what() const noexcept -> const char* { return msg_.c_str(); }
private:
	std::string msg_{};
};


}


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////




#include <variant>
#include <memory>
#include <tuple>

namespace peo::type_traits::introspection {
	// Taken from http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4502.pdf
	template <typename...>
	using void_t = void;
	// Primary template handles all types not supporting the operation.
	template <typename, template <typename> class, typename = void_t<>>
	struct detect : std::false_type {};
	// Specialization recognizes/validates only types supporting the archetype.
	template <typename T, template <typename> class Op>
	struct detect<T, Op, void_t<Op<T>>> : std::true_type {};
}

namespace peo::type_traits {
	template <typename T>
	using is_container_t = decltype(std::declval<const T&>().begin(), std::declval<const T&>().end());
	template <typename T>
	constexpr auto is_container_v = introspection::detect<T, is_container_t>::value;

	template <typename T>
	using has_ostream_t = decltype(std::declval<std::ostream&>() << std::declval<T>());
	template <typename T>
	constexpr auto has_ostream_v = introspection::detect<T, has_ostream_t>::value;

	template <typename T>
	using is_tuple_t = decltype(std::tuple_size<T>::value);
	template <typename T>
	constexpr auto is_tuple_v = introspection::detect<T, is_tuple_t>::value;


	template <typename T> constexpr auto is_smart_ptr_f(const std::unique_ptr<T>&) { return std::true_type{}; }
	template <typename T> constexpr auto is_smart_ptr_f(const std::shared_ptr<T>&) { return std::true_type{}; }
	template <typename T> constexpr auto is_smart_ptr_f(const T&) { return std::false_type{}; }
	template <typename T>
	constexpr auto is_smart_ptr_v = decltype(is_smart_ptr_f(std::declval<T>()))::value;

	template <typename T> constexpr auto is_weak_ptr_f(const std::weak_ptr<T>&) { return std::true_type{}; }
	template <typename T> constexpr auto is_weak_ptr_f(const T&) { return std::false_type{}; }
	template <typename T>
	constexpr auto is_weak_ptr_v = decltype(is_weak_ptr_f(std::declval<T>()))::value;

	template <typename ...Ts> constexpr auto is_variant_f(const std::variant<Ts...>&) { return std::true_type{}; }
	template <typename T> constexpr auto is_variant_f(const T&) { return std::false_type{}; }
	template <typename T>
	constexpr auto is_variant_v = decltype(is_variant_f(std::declval<T>()))::value;

	template <typename T> constexpr auto is_optional_f(const std::optional<T>&) { return std::true_type{}; }
	template <typename T> constexpr auto is_optional_f(const T&) { return std::false_type{}; }
	template <typename T>
	constexpr auto is_optional_v = decltype(is_optional_f(std::declval<T>()))::value;

	template <typename T, typename Y> constexpr auto is_duration_f(const std::chrono::duration<T, Y>&) { return std::true_type{}; }
	template <typename T> constexpr auto is_duration_f(const T&) { return std::false_type{}; }
	template <typename T>
	constexpr auto is_duration_v = decltype(is_duration_f(std::declval<T>()))::value;

	template <typename Char>
	constexpr auto is_valid_char_v = std::bool_constant<
		std::is_same_v<Char, char> ||
		std::is_same_v<Char, wchar_t> ||
		std::is_same_v<Char, char16_t> ||
		std::is_same_v<Char, char32_t>
	>::value;

	template <typename Char> constexpr auto is_string_f(const std::basic_string<Char>&) { return std::true_type{}; }
	template <typename Char> constexpr auto is_string_f(const std::basic_string_view<Char>&) { return std::true_type{}; }
	template <typename T> constexpr auto is_string_f(const T&) { return std::false_type{}; }
	template <typename T>
	constexpr auto is_string_v = decltype(is_string_f(std::declval<T>()))::value;

}


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


#include <tuple>
#include <type_traits>
#include <array>

namespace peo::detail {
template<typename Tpl, typename Func, size_t Idx = 0>
auto impl_tuple_for_each(Tpl& tpl, Func&& func) -> void {
	if constexpr (Idx < std::tuple_size_v<Tpl>) {
		func(std::get<Idx>(tpl));
		impl_tuple_for_each<Tpl, Func, Idx + 1>(tpl, std::forward<Func>(func));
	}
}
template<typename Tpl, typename Func, size_t Idx = 0>
auto impl_tuple_for_each(const Tpl& tpl, Func&& func) -> void {
	if constexpr (Idx < std::tuple_size_v<Tpl>) {
		func(std::get<Idx>(tpl));
		impl_tuple_for_each<Tpl, Func, Idx + 1>(tpl, std::forward<Func>(func));
	}
}

template <typename Tpl, typename Func, size_t Idx = 0>
auto impl_tuple_any_of(const Tpl& tpl, const Func& func) -> bool {
	if constexpr (Idx < std::tuple_size_v<Tpl>) {
		return func(std::get<Idx>(tpl)) ?
			true :
			impl_tuple_any_of<Tpl, Func, Idx + 1>(tpl, func);
	}
	return false;
}

}

template<typename Tpl, typename Func>
auto peo::tuple_for_each(Tpl&& tpl, Func&& func) -> void {
	return detail::impl_tuple_for_each(
		std::forward<Tpl>(tpl), 
		std::forward<Func>(func)
	);
}

template <typename Tpl, typename Func>
auto peo::tuple_any_of(const Tpl& tpl, const Func& func) -> bool {
	return detail::impl_tuple_any_of(tpl, func);
}




//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////



template <typename DstType, typename SrcType>
auto peo::safe_cast(const SrcType& src) -> DstType {
	const auto casted = static_cast<DstType>(src);
	const auto casted_back = static_cast<SrcType>(casted);
	if (casted_back == src) PRECOOKED_LIKELY {
		return casted;
	}
	using namespace std::string_view_literals;
	const auto typename_src = peo::type_name(src);
	const auto typename_casted = peo::type_name(casted);
	auto msg = peo::join_strings(std::array{
		"bad cast from "sv,
		std::string_view{typename_src},
		" to "sv,
		std::string_view{typename_casted}
	});
	throw peo::exceptions::bad_cast_exception{ std::move(msg) };
}







//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#include <type_traits>
#include <string>

template <typename T>
auto peo::type_name() -> std::string { return typeid(T).name(); }
template <typename T>
auto peo::type_name(T&&) -> std::string { return typeid(T).name(); }



#include <any>
#include <variant>

template <typename T>
auto peo::held_type_name(const T& value) -> std::string {
	using value_t = std::decay_t<T>;
	if constexpr (type_traits::is_variant_v<T>) {
		if (value.valueless_by_exception()) {
			return "std::variant is valueless by exception";
		}
		return std::visit([](const auto& candidate) -> std::string {
			using value_t = std::decay_t<decltype(candidate)>;
			return typeid(value_t).name();
		}, value);
	}
	else if constexpr (std::is_same_v<std::any, value_t>) {
		return value.has_value() ?
			value.type().name() :
			"std::any is empty";
	}
	else {
		return typeid(value_t).name();
	}
}






//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#include <string_view>
#include <optional>
#include <charconv>

template <typename T>
auto peo::string_to_number(const std::string_view str) noexcept -> std::optional<T> {
	static_assert(std::is_arithmetic_v<T>, "T needs to be an arithmetic type");
	auto value = T{};
	const auto* ptr_end = str.data() + str.size();
	const auto result = std::from_chars(str.data(), ptr_end, value);
	const auto success = 
		result.ec == std::errc{} &&
		result.ptr == ptr_end;
	return success ? value : std::optional<T>{};
}

template <typename T> 
auto peo::number_to_string(const T& number) -> std::string {
	static_assert(std::is_arithmetic_v<T>, "T needs to be an arithmetic type");
	return std::to_string(number);
}





//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#include <string>
#include <string_view>
#include <type_traits>
#include <sstream>


namespace peo::detail {
using hexchars_t = std::array<char, 2>;
[[nodiscard]] inline auto uint8_to_hexchars(
	const uint8_t value
) noexcept -> hexchars_t {
	static_assert(std::is_same_v<std::string::value_type, char>);
	auto hex_digit_f = [](const uint8_t v) noexcept -> char {
		constexpr auto digits = std::string_view{ "0123456789abcdef" };
		PRECOOKED_ASSERT(v < digits.size());
		return digits[v];
	};
	const auto left_digit = hex_digit_f(value / 16);
	const auto right_digit = hex_digit_f(value % 16);
	return hexchars_t{ left_digit, right_digit };
};
}

template <typename T>
auto peo::pretty_string(const T& value) -> std::string {
	using value_t = std::decay_t<T>;
	using namespace std::string_literals;
	if constexpr (std::is_same_v<std::string, value_t>) {
		return value;
	}
	else if constexpr (std::is_same_v<std::string_view, value_t>) {
		return std::string{ value };
	}
	else if constexpr (type_traits::is_string_v<value_t>) {
		auto str = std::string{};
		str.resize(value.size());
		using src_char_t = type_traits::underlying_char_t<T>;
		using dst_char_t = std::string::value_type;
		const auto truncate_to_dst_char_f = [](const src_char_t& src_char) noexcept -> dst_char_t {
			const auto dst_char = static_cast<dst_char_t>(src_char);
			const auto is_truncated = static_cast<src_char_t>(dst_char) != src_char;
			return is_truncated ?
				dst_char_t{ '?' } :
				dst_char;
		};
		std::transform(value.begin(), value.end(), str.begin(), truncate_to_dst_char_f);
		return str;
	}
	else if constexpr (std::is_same_v<bool, value_t>) {
		return value ? "true"s : "false"s;
	}
	else if constexpr (std::is_arithmetic_v<value_t>) {
		return std::to_string(value);
	}
	else if constexpr (type_traits::is_smart_ptr_v<value_t>) {
		return value == nullptr ? 
			"nullptr"s : 
			pretty_string(*value);
	}
	else if constexpr (type_traits::is_weak_ptr_v<value_t>) {
		auto sptr = value.lock();
		return pretty_string(sptr);
	}
	else if constexpr (
		std::is_pointer_v<value_t> && 
		!type_traits::is_valid_char_v<std::remove_pointer_t<value_t>>
	) {
		return value == nullptr ?
			"nullptr"s :
			pretty_string(*value);
	}
	else if constexpr (type_traits::is_variant_v<value_t>) {
		return std::visit([](auto&& candidate) {
			return pretty_string(candidate);
		}, value);
	}
	else if constexpr (type_traits::is_optional_v<value_t>) {
		return value.has_value() ?
			pretty_string(*value) :
			"std::nullopt"s;
	}
	else if constexpr (std::is_same_v<std::any, value_t>) {
		return value.empty() ?
			"empty std::any"s :
			"std::any containing type "s + held_type_name(value);
	}
	else if constexpr (std::is_base_of_v<std::exception, value_t>) {
		return value.what();
	}
	else if constexpr (type_traits::has_ostream_v<value_t>) {
		auto sstr = std::ostringstream{};
		sstr << value;
		return sstr.str();
	}
	else if constexpr (type_traits::is_container_v<value_t>) {
		auto str = std::string{};
		const auto estimate_size = value.size() * 4;
		str.reserve(estimate_size);
		str += '[';
		for (auto&& element : value) {
			str += pretty_string(element);
			str += ' ';
		}
		if (str.size() == 1) {
			str.push_back(']');
		}
		else {
			str.back() = ']';
		}
		return str;
	}
	else if constexpr(type_traits::is_tuple_v<value_t>){
		auto str = std::string{};
		const auto estimate_size = std::tuple_size_v<value_t> * 4;
		str.reserve(estimate_size);
		str += '[';
		tuple_for_each(value, [&str](auto&& elem) {
			str += pretty_string(elem);
			str += ' '; 
		});
		if (str.size() == 1) {
			str.push_back(']');
		}
		else {
			str.back() = ']';
		}
		return str;
	}
	else if constexpr (std::is_enum_v<value_t>) {
		using underlying_type = std::underlying_type_t<T>;
		return pretty_string(static_cast<underlying_type>(value));
	}
	else if constexpr (type_traits::is_duration_v<value_t>) {
		return pretty_string(value.count());
	}
	else {
		const auto* first = reinterpret_cast<const uint8_t*>(&value);
		const auto* last = first + sizeof(value);
		constexpr auto prologue = std::string_view{ "unknown 0x" };
		const auto target_size =
			prologue.size() +
			(last - first) * 2;
		auto str = std::string{};
		str.reserve(target_size);
		str += prologue;
		for (auto it = first; it < last; ++it) {
			const auto hexchars = detail::uint8_to_hexchars(*it);
			str += hexchars[0];
			str += hexchars[1];
		}
		PRECOOKED_ASSERT(target_size == str.size());
		return str;
	}

}












//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#include <numeric>


namespace peo::detail {
template <typename Strings, typename Char>
[[nodiscard]] auto impl_join_strings(
	const Strings& strings, 
	const std::basic_string_view<Char>& delimiter
) -> std::basic_string<Char> {
	if (strings.empty()) {
		return {};
	}
	const auto target_size_of_strings = std::accumulate(
		strings.begin(), 
		strings.end(), 
		size_t{ 0 }, 
		[](const size_t sum, const auto& str) {
			return sum + str.size();
		}
	);
	const auto num_delimiters = strings.size() - 1;
	const auto target_size_of_delimiters = num_delimiters * delimiter.size();
	const auto target_size = target_size_of_strings + target_size_of_delimiters;
	// Merge strings
	auto joined = std::basic_string<Char>{};
	joined.reserve(target_size);
	if (delimiter.empty()) {
		for (auto&& str : strings) {
			joined.append(str);
		}
	}
	else {
		for (auto it = strings.begin(), it_end = std::prev(strings.end()); it != it_end; ++it) {
			joined.append(*it);
			joined.append(delimiter);
		}
		joined.append(strings.back());
		// No delimiter at end
	}
	PRECOOKED_ASSERT(joined.size() == target_size);
	return joined;
}
}

template <typename Strings, typename Str>
auto peo::join_strings(
	const Strings& strings, 
	const Str& delimiter
) -> std::basic_string<type_traits::underlying_char_t<Str>> {
	using Char = type_traits::underlying_char_t<Str>;
	static_assert(type_traits::is_valid_char_v<Char>);
	return detail::impl_join_strings(strings, std::basic_string_view<Char>{delimiter});
}

template <typename Strings, typename Char>
auto peo::join_strings(const Strings& strings) -> std::basic_string<Char> {
	static_assert(type_traits::is_valid_char_v<Char>);
	return detail::impl_join_strings(strings, std::basic_string_view<Char>{});
}













































































//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


namespace peo::detail {
template <typename Char>
[[nodiscard]] constexpr auto default_trim_chars() noexcept -> std::basic_string_view<Char> {
	using namespace std::string_view_literals;
	static_assert(
		std::is_same_v<Char, char> ||
		std::is_same_v<Char, wchar_t> ||
		std::is_same_v<Char, char16_t> ||
		std::is_same_v<Char, char32_t>
	);
	if constexpr (std::is_same_v<Char, char>) { return "\t\r\n "sv; }
	else if constexpr (std::is_same_v<Char, wchar_t>) { return L"\t\r\n "sv; }
	else if constexpr (std::is_same_v<Char, char16_t>) { return u"\t\r\n "sv; }
	else if constexpr (std::is_same_v<Char, char32_t>) { return U"\t\r\n "sv; }
}
template <typename Char>
[[nodiscard]] constexpr auto linebreak_chars() noexcept -> std::basic_string_view<Char> {
	using namespace std::string_view_literals;
	static_assert(
		std::is_same_v<Char, char> ||
		std::is_same_v<Char, wchar_t> ||
		std::is_same_v<Char, char16_t> ||
		std::is_same_v<Char, char32_t>
	);
	if constexpr (std::is_same_v<Char, char>) { return "\r\n"sv; }
	else if constexpr (std::is_same_v<Char, wchar_t>) { return L"\r\n"sv; }
	else if constexpr (std::is_same_v<Char, char16_t>) { return u"\r\n"sv; }
	else if constexpr (std::is_same_v<Char, char32_t>) { return U"\r\n"sv; }
}
}


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

namespace peo::detail {

template <typename Char, typename PartType>
[[nodiscard]] auto impl_split_string(
	const std::basic_string_view<Char> str,
	const std::basic_string_view<Char> delimiters
) -> std::vector<PartType> {
	constexpr auto npos = std::basic_string_view<Char>::npos;
	if (str.empty()) PRECOOKED_UNLIKELY {
		return {};
	}
	if (delimiters.empty()) PRECOOKED_UNLIKELY {
		return std::vector<PartType>{ PartType{ str } };
	}
	const auto first_not_delimiter = str.find_first_not_of(delimiters);
	if (first_not_delimiter >= str.size()) {
		return {};
	}
	// Calculate num parts in order to allocate returned vector
	auto calculate_num_parts_f = [](
		const std::basic_string_view<Char>& str, 
		const std::basic_string_view<Char>& delimiters, 
		const size_t first_not_delimiter
	) noexcept -> size_t {
		constexpr auto npos = std::basic_string_view<Char>::npos;
		const auto last_not_delimiter = str.find_last_not_of(delimiters);
		auto num_parts = size_t{ 1 };
		for (size_t i = first_not_delimiter + 1; i < last_not_delimiter; ++i) {
			PRECOOKED_ASSERT(0 < i);
			PRECOOKED_ASSERT(i < str.size());
			const auto is_new_part =
				delimiters.find(str[i - 1]) == npos &&
				delimiters.find(str[i]) != npos;
			if (is_new_part) {
				++num_parts;
			}
		}
		return num_parts;
	};
	const auto num_parts = calculate_num_parts_f(str, delimiters, first_not_delimiter);
	// Split string
	auto parts = std::vector<PartType>{};
	parts.reserve(num_parts);
	for (
		auto left = first_not_delimiter, right = size_t{ 0 };
		left != npos;
		left = str.find_first_not_of(delimiters, right)
	) {
		right = str.find_first_of(delimiters, left + 1);
		right = right == npos ? str.size() : right;
		PRECOOKED_ASSERT(right <= str.size());
		PRECOOKED_ASSERT(left <= right);
		const auto* part_offset = str.data() + left;
		const auto part_size = right - left;
		parts.emplace_back(PartType{ part_offset, part_size });
	}
	PRECOOKED_ASSERT(parts.size() == num_parts);
	return parts;
}
}


template <typename Str0, typename Str1>
auto peo::split_string(
	const Str0& str, 
	const Str1& delimiters
) -> std::vector<std::basic_string<type_traits::underlying_char_t<Str0>>> {
	using Char = type_traits::underlying_char_t<Str0>;
	return detail::impl_split_string<Char, std::basic_string<Char>>(str, delimiters);
}

template <typename Str0, typename Str1>
auto peo::split_string_to_views(
	const Str0& str, 
	const Str1& delimiters
) -> std::vector<std::basic_string_view<type_traits::underlying_char_t<Str0>>> {
	using Char = type_traits::underlying_char_t<Str0>;
	static_assert(type_traits::is_valid_char_v<Char>);
	return detail::impl_split_string<Char, std::basic_string_view<Char>>(str, delimiters);
}

template <typename Str>
auto peo::split_string_to_lines(
	const Str& str
) -> std::vector<std::basic_string<type_traits::underlying_char_t<Str>>> {
	using Char = type_traits::underlying_char_t<Str>;
	static_assert(type_traits::is_valid_char_v<Char>);
	return split_string(str, detail::linebreak_chars<Char>());
}








//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////









template <typename Char>
auto peo::trim_string(
	std::basic_string<Char> str
) noexcept -> std::basic_string<Char> {
	return trim_string(std::move(str), detail::default_trim_chars<Char>());
}

template <typename Char, typename Str0>
auto peo::trim_string(
	std::basic_string<Char> str, 
	const Str0& trim_chars
) noexcept -> std::basic_string<Char> {
	const auto trim_chars_sv = std::basic_string_view<Char>{ trim_chars };
	constexpr auto npos = std::basic_string_view<Char>::npos;
	if (str.empty() || trim_chars_sv.empty()) {
		return str;
	}
	const auto last_idx = str.find_last_not_of(trim_chars_sv);
	if (last_idx != npos) {
		str.erase(str.begin() + last_idx + 1, str.end());
	}
	const auto first_idx = str.find_first_not_of(trim_chars_sv);
	if (first_idx != npos && first_idx != 0) {
		str.erase(str.begin(), str.begin() + first_idx);
	}
	return str;
}

template <typename Str0>
auto peo::trim_string_to_view(
	const Str0& str
) noexcept -> std::basic_string_view<type_traits::underlying_char_t<Str0>> {
	using Char = type_traits::underlying_char_t<Str0>;
	static_assert(type_traits::is_valid_char_v<Char>);
	return trim_string_to_view(str, detail::default_trim_chars<Char>());
}

 template <typename Str0, typename Str1>
auto peo::trim_string_to_view(
	const Str0& str,
	const Str1& trim_chars
) noexcept -> std::basic_string_view<type_traits::underlying_char_t<Str0>> {
	using Char = type_traits::underlying_char_t<Str0>;
	static_assert(type_traits::is_valid_char_v<Char>);
	const auto trim_chars_sv = std::basic_string_view<Char>{ trim_chars };
	auto sv = std::basic_string_view<Char>{ str };
	if (sv.empty() || trim_chars_sv.empty()) {
		return sv;
	}
	const auto first_idx = sv.find_first_not_of(trim_chars_sv);
	if (first_idx != std::string_view::npos) {
		sv = sv.substr(first_idx);
	}
	const auto last_idx = sv.find_last_not_of(trim_chars_sv);
	if (last_idx != std::string_view::npos) {
		sv = sv.substr(0, last_idx + 1);
	}
	return sv;
}



template<typename Str0>
auto peo::is_trimmed(const Str0& str) noexcept -> bool {
	using Char = type_traits::underlying_char_t<Str0>;
	static_assert(type_traits::is_valid_char_v<Char>);
	const auto sv = std::basic_string_view<Char>{ str };
	return is_trimmed(str, detail::default_trim_chars<Char>());
}

template<typename Str0, typename Str1>
auto peo::is_trimmed(const Str0& str, const Str1& trim_chars) noexcept -> bool {
	using Char = type_traits::underlying_char_t<Str0>;
	static_assert(type_traits::is_valid_char_v<Char>);
	const auto sv = std::basic_string_view<Char>{ str };
	const auto is_trim_char_f = [&trim_chars](const Char& candidate) noexcept -> bool {
		constexpr auto npos = std::basic_string_view<Char>::npos;
		return trim_chars.find(candidate) != npos;
	};
	return
		sv.empty() ? true :
		is_trim_char_f(sv.front()) ? false :
		is_trim_char_f(sv.back()) ? false :
		true;
}








//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

namespace peo::detail {


constexpr auto find_case_sensitive_f = [](
	const auto& haystack, 
	const auto& needle, 
	const size_t offset
) noexcept -> size_t {
	return haystack.find(needle, offset);
};



template <typename Char>
[[nodiscard]] auto impl_is_equal_ignore_case(
	const std::basic_string_view<Char>& a,
	const std::basic_string_view<Char>& b,
	const std::locale& loc
) noexcept -> bool {
	PRECOOKED_ASSERT(a.size() == b.size());
	const auto is_chars_equal_ignore_case_f = [&loc](
		const auto& a,
		const auto& b
	) noexcept {
		return a == b || std::tolower(a, loc) == std::tolower(b, loc);
	};
	return std::equal(a.begin(), a.end(), b.begin(), is_chars_equal_ignore_case_f);
}



template <typename Char>
[[nodiscard]] auto impl_find_ignore_case(
	const std::basic_string_view<Char>& haystack,
	const std::basic_string_view<Char>& needle,
	const size_t offset,
	const std::locale& loc
) noexcept -> size_t {
	PRECOOKED_ASSERT(haystack.size() >= needle.size());
	PRECOOKED_ASSERT(!needle.empty());
	constexpr auto npos = std::basic_string_view<Char>::npos;
	const auto i_end = (1 + haystack.size()) - needle.size(); // No need to search beyond this index
	if (offset > i_end) {
		return npos;
	}
	const auto equals_needle_front_f = [
		upper = std::toupper(needle.front(), loc),
		lower = std::tolower(needle.front(), loc)
	](const Char& c) {
		return c == upper || c == lower;
	};
	const auto needle_tail = needle.substr(1);
	for (size_t i = offset; i < i_end; ++i) {
		PRECOOKED_ASSERT(i < haystack.size());
		if (!equals_needle_front_f(haystack[i])) PRECOOKED_LIKELY {
			continue;
		}
		PRECOOKED_ASSERT(i + needle.size() <= haystack.size());
		const auto candidate_tail = haystack.substr(i + 1, needle.size() - 1);
		if (detail::impl_is_equal_ignore_case(candidate_tail, needle_tail, loc)) {
			return i;
		}
	}
	return npos;
}

}



//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////



namespace peo::detail {


template <typename Char>
auto replace_string_part(
	std::basic_string<Char>& io_string, 
	const size_t offset,
	const std::basic_string_view<Char>& new_content
) noexcept -> void {
	PRECOOKED_ASSERT(offset + new_content.size() <= io_string.size());
	std::copy(new_content.begin(), new_content.end(), io_string.begin() + offset);
}


// This function replaces string of equal size
template <typename Char, typename FindFunc>
[[nodiscard]] auto impl_replace_all_equal_needle_length(
	std::basic_string<Char> haystack, 
	const std::basic_string_view<Char> needle,
	const std::basic_string_view<Char> replacement,
	const FindFunc& find_func
) noexcept -> std::string {
	PRECOOKED_ASSERT(needle.size() == replacement.size());
	constexpr auto npos = std::basic_string_view<Char>::npos;
	for (
		auto idx = find_func(haystack, needle, 0);
		idx != npos;
		idx = find_func(haystack, needle, idx + needle.size())
	) {
		replace_string_part(haystack, idx, replacement);
	}
	return haystack;
}


// This function modifies the string in-place.
// Requires replacement to be smaller than needle.
template <typename Char, typename FindFunc>
[[nodiscard]] auto impl_replace_all_shrink_string(
	std::basic_string<Char> haystack,
	const std::basic_string_view<Char> needle,
	const std::basic_string_view<Char> replacement,
	const FindFunc& find_func
) noexcept -> std::basic_string<Char> {
	PRECOOKED_ASSERT(replacement.size() < needle.size());
	constexpr auto npos = std::basic_string_view<Char>::npos;
	// Optimization to avoid copying the first part
	const auto first_needle = find_func(haystack, needle, 0);
	if (first_needle == npos) {
		return haystack;
	}
	replace_string_part(haystack, first_needle, replacement);
	auto write_pos = first_needle + replacement.size();
	for (auto read_pos = first_needle + needle.size();;) {
		PRECOOKED_ASSERT(write_pos < read_pos);
		const auto needle_pos = std::min(find_func(haystack, needle, read_pos), haystack.size());
		while (read_pos < needle_pos) {
			PRECOOKED_ASSERT(write_pos < haystack.size());
			PRECOOKED_ASSERT(read_pos < haystack.size());
			haystack[write_pos] = haystack[read_pos];
			++write_pos;
			++read_pos;
		}
		PRECOOKED_ASSERT(needle_pos <= haystack.size());
		if (needle_pos == haystack.size()) {
			break;
		}
		replace_string_part(haystack, write_pos, replacement);
		read_pos += needle.size();
		write_pos += replacement.size();
	}
	PRECOOKED_ASSERT(write_pos < haystack.size());
	haystack.resize(write_pos);
	return haystack;
}


template <typename Char, typename FindFunc>
[[nodiscard]] auto calculate_num_occurances(
	const std::basic_string_view<Char>& haystack,
	const std::basic_string_view<Char>& needle,
	const size_t start_offset,
	const FindFunc& find_func
) noexcept -> size_t {
	PRECOOKED_ASSERT(needle.size() <= haystack.size());
	auto num_occurances = size_t{ 0 };
	const auto i_start = find_func(haystack, needle, start_offset);
	const auto i_end = haystack.size();
	for (
		auto i = i_start;
		i < i_end;
		i = find_func(haystack, needle, i + needle.size())
	) {
		++num_occurances;
	}
	return num_occurances;
}

// This function builds a new string.
// Required if replacement is larger than needle, or the source is a string_view.
template <typename Char, typename FindFunc>
[[nodiscard]] auto impl_replace_all_rebuild_string(
	const std::basic_string_view<Char>& haystack,
	const std::basic_string_view<Char>& needle,
	const std::basic_string_view<Char>& replacement,
	const FindFunc& find_func
) -> std::basic_string<Char> {
	PRECOOKED_ASSERT(!needle.empty());
	PRECOOKED_ASSERT(needle.size() <= haystack.size());
	const auto first_match = std::min(find_func(haystack, needle, 0), haystack.size());
	if (first_match >= haystack.size()) {
		return std::basic_string<Char>{ haystack };
	}
	const auto lazy_num_occurances_f = [&]() noexcept {
		const auto num_occurances = calculate_num_occurances(
			haystack,
			needle,
			first_match + needle.size(),
			find_func
		);
		return num_occurances + 1; // first_match found indicates one occurances
	};
	const auto target_size =
		needle.size() < replacement.size() ? haystack.size() + (replacement.size() - needle.size()) * lazy_num_occurances_f() :
		replacement.size() < needle.size() ? haystack.size() - (needle.size() - replacement.size()) * lazy_num_occurances_f() :
		haystack.size();
		
	auto ret = std::basic_string<Char>{};
	ret.reserve(target_size);
	for (
		size_t left = 0, right = first_match;
		;
		right = std::min(find_func(haystack, needle, left), haystack.size())
	) {
		PRECOOKED_ASSERT(left <= right);
		PRECOOKED_ASSERT(right <= haystack.size());
		ret += haystack.substr(left, right - left);
		if (right == haystack.size()) {
			break;
		}
		PRECOOKED_ASSERT(right < haystack.size());
		ret += replacement;
		left = right + needle.size();
	}
	PRECOOKED_ASSERT(ret.size() == target_size);
	return ret;
}

}

template <typename Char, typename Str0, typename Str1>
auto peo::replace_all(
	std::basic_string<Char> haystack, 
	const Str0& needle, 
	const Str1& replacement
) -> std::basic_string<Char> {
	// Pick implementation based on needle/replacement size
	const auto haystack_sv = std::basic_string_view<Char>{ haystack };
	const auto needle_sv = std::basic_string_view<Char>{ needle };
	const auto replacement_sv = std::basic_string_view<Char>{ replacement };
	const auto& find_func = detail::find_case_sensitive_f;
	const auto no_replacement_possible = 
		needle_sv.empty() || 
		needle_sv.size() > haystack_sv.size();
	return
		no_replacement_possible ? haystack :
		needle_sv.length() == replacement_sv.length() ? detail::impl_replace_all_equal_needle_length(std::move(haystack), needle_sv, replacement_sv, find_func) :
		replacement_sv.length() < needle_sv.length() ? detail::impl_replace_all_shrink_string(std::move(haystack), needle_sv, replacement_sv, find_func) :
		detail::impl_replace_all_rebuild_string<Char>(haystack, needle_sv, replacement_sv, find_func);
}


template <typename Str0, typename Str1, typename Str2>
auto peo::replace_all(
	const Str0& haystack, 
	const Str1& needle, 
	const Str2& replacement
) -> std::basic_string<type_traits::underlying_char_t<Str0>> {
	using Char = type_traits::underlying_char_t<Str0>;
	static_assert(type_traits::is_valid_char_v<Char>);
	const auto haystack_sv = std::basic_string_view<Char>{ haystack };
	const auto needle_sv = std::basic_string_view<Char>{ needle };
	const auto replacement_sv = std::basic_string_view<Char>{ replacement };
	const auto& find_func = detail::find_case_sensitive_f;
	const auto no_replacement_possible = 
		needle_sv.empty() || 
		needle_sv.size() > haystack_sv.size();
	return no_replacement_possible ?
		std::basic_string<Char>{ haystack_sv } :
		detail::impl_replace_all_rebuild_string(haystack_sv, needle_sv, replacement_sv, find_func);
}


template <typename Char, typename Str0, typename Str1>
auto peo::replace_all_ignore_case(
	std::basic_string<Char> haystack, 
	const Str0& needle, 
	const Str1& replacement,
	const std::locale& loc
) -> std::basic_string<Char> {
	static_assert(type_traits::is_valid_char_v<Char>);
	const auto haystack_sv = std::basic_string_view<Char>{ haystack };
	const auto needle_sv = std::basic_string_view<Char>{ needle };
	const auto replacement_sv = std::basic_string_view<Char>{ replacement };
	const auto& find_func = [&loc](const auto& haystack, const auto& needle, size_t offset) {
		return detail::impl_find_ignore_case<Char>(haystack, needle, offset, loc);
	};
	const auto no_replacement_possible = 
		needle_sv.empty() || 
		needle_sv.size() > haystack_sv.size();
	return
		no_replacement_possible ? haystack :
		needle_sv.length() == replacement_sv.length() ? detail::impl_replace_all_equal_needle_length(std::move(haystack), needle_sv, replacement_sv, find_func) :
		replacement_sv.length() < needle_sv.length() ? detail::impl_replace_all_shrink_string(std::move(haystack), needle_sv, replacement_sv, find_func) :
		detail::impl_replace_all_rebuild_string<Char>(haystack_sv, needle_sv, replacement_sv, find_func);
}

template <typename Str0, typename Str1, typename Str2>
auto peo::replace_all_ignore_case(
	const Str0& haystack, 
	const Str1& needle, 
	const Str2& replacement, 
	const std::locale& loc
) -> std::basic_string<type_traits::underlying_char_t<Str0>> {
	using Char = type_traits::underlying_char_t<Str0>;
	static_assert(type_traits::is_valid_char_v<Char>);
	const auto haystack_sv = std::basic_string_view<Char>{ haystack };
	const auto needle_sv = std::basic_string_view<Char>{ needle };
	const auto replacement_sv = std::basic_string_view<Char>{ replacement };
	const auto& find_func = [&loc](const auto& haystack, const auto& needle, size_t offset) {
		return detail::impl_find_ignore_case<Char>(haystack, needle, offset, loc);
	};

	const auto no_replacement_possible =
		needle_sv.empty() || 
		needle_sv.size() > haystack_sv.size();
	return no_replacement_possible ?
		std::basic_string<Char>{ haystack_sv } :
		detail::impl_replace_all_rebuild_string(haystack_sv, needle_sv, replacement_sv, find_func);
}














//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


#include <algorithm>

template <typename Char>
auto peo::to_lower(
	std::basic_string<Char> str, 
	const std::locale& loc
) noexcept -> std::basic_string<Char> {
	std::transform(str.begin(), str.end(), str.begin(), [&loc](const Char& c) {
		return std::tolower(c, loc);
	});
	return str;
}

template <typename StrView> 
auto peo::to_lower(
	const StrView& strview, 
	const std::locale& loc
) -> std::basic_string<type_traits::underlying_char_t<StrView>> {
	using Char = type_traits::underlying_char_t<StrView>;
	static_assert(type_traits::is_valid_char_v<Char>);
	const auto sv = std::basic_string_view<Char>{strview};
	auto str = std::basic_string<Char>{};
	str.reserve(sv.size());
	std::transform(sv.begin(), sv.end(), std::back_inserter(str), [&loc](const Char& c) {
		return std::tolower(c, loc);
	});
	return str;
}


template <typename Char>
auto peo::to_upper(
	std::basic_string<Char> str,
	const std::locale& loc
) noexcept -> std::basic_string<Char> {
	std::transform(str.begin(), str.end(), str.begin(), [&loc](const Char& c) {
		return std::toupper(c, loc);
	});
	return str;
}

template <typename StrView>
auto peo::to_upper(
	const StrView& strview, 
	const std::locale& loc
) -> std::basic_string<type_traits::underlying_char_t<StrView>> {
	using Char = type_traits::underlying_char_t<StrView>;
	static_assert(type_traits::is_valid_char_v<Char>);
	const auto sv = std::basic_string_view<Char>{ strview };
	auto str = std::basic_string<Char>{};
	str.reserve(sv.size());
	std::transform(sv.begin(), sv.end(), std::back_inserter(str), [&loc](const Char& c) {
		return std::toupper(c, loc);
	});
	return str;
}

template <typename Str0, typename Str1>
auto peo::is_equal_ignore_case(
	const Str0& a, 
	const Str1& b, 
	const std::locale& loc
) noexcept -> bool {
	const auto a_sv = std::basic_string_view{ a };
	const auto b_sv = std::basic_string_view{ b };
	return
		a_sv.size() == b_sv.size() &&
		detail::impl_is_equal_ignore_case(a_sv, b_sv, loc);
}

template <typename Str0, typename Str1>
auto peo::contains_substring(
	const Str0& haystack, 
	const Str1& needle
) noexcept -> bool {
	using Char = type_traits::underlying_char_t<Str0>;
	static_assert(type_traits::is_valid_char_v<Char>);
	const auto haystack_sv = std::basic_string_view<Char>{ haystack };
	return haystack_sv.find(needle) != std::string::npos;
}

template <typename Str0, typename Str1>
auto peo::contains_substring_ignore_case(
	const Str0& haystack, 
	const Str1& needle, const std::locale& loc
) noexcept -> bool {
	return find_ignore_case(haystack, needle, 0, loc) != std::string::npos;
}


template <typename Str0, typename Str1>
auto peo::find_ignore_case(
	const Str0& haystack,
	const Str1& needle,
	const size_t offset,
	const std::locale& loc
) noexcept -> size_t {
	using Char = type_traits::underlying_char_t<Str0>;
	static_assert(type_traits::is_valid_char_v<Char>);
	const auto haystack_sv = std::basic_string_view<Char>{ haystack };
	const auto needle_sv = std::basic_string_view<Char>{ needle };
	const auto cannot_possibly_exist =
		haystack_sv.size() < needle_sv.size() || 
		needle_sv.empty();
	if (cannot_possibly_exist) {
		return std::string::npos;
	}
	return detail::impl_find_ignore_case(
		std::basic_string_view<Char>{haystack_sv},
		std::basic_string_view<Char>{needle_sv},
		offset,
		loc
	);
}

template <typename Str0, typename Str1>
auto peo::find_ignore_case(
	const Str0& haystack,
	const Str1& needle,
	const std::locale& loc
) noexcept -> size_t {
	return find_ignore_case(
		haystack,
		needle,
		size_t{ 0 },
		loc
	);
}


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


#include <fstream>
#include <filesystem>
#include <vector>
#include <cstdint>
#include <optional>

namespace peo::detail {


[[nodiscard]] inline auto filesize_to_size_t(
	const uintmax_t filesize_uintmax
) noexcept -> std::optional<size_t> {
	const auto file_size = static_cast<size_t>(filesize_uintmax);
	return static_cast<uintmax_t>(file_size) == filesize_uintmax ?
		file_size :
		std::optional<size_t>{};
}

template <typename Container>
[[nodiscard]] auto impl_read_file_to_container(
	const std::filesystem::path& filepath
) -> Container {
	if (!std::filesystem::exists(filepath)) {
		throw peo::exceptions::file_not_found_exception(filepath);
	}
	if (!std::filesystem::is_regular_file(filepath)) {
		throw peo::exceptions::is_not_file_exception(filepath);
	}
	using value_t = typename Container::value_type;
	using byte_t = byte_view::byte_t;
	const auto file_size_uintmax = std::filesystem::file_size(filepath);
	const auto file_size_optional = detail::filesize_to_size_t(file_size_uintmax);
	if (!file_size_optional.has_value()) {
		throw peo::exceptions::file_too_large_exception(filepath, file_size_uintmax);
	}
	const auto file_size = *file_size_optional;
	constexpr auto element_size = sizeof(value_t);
	static_assert(element_size > 0, "Element size is 0");
	const auto is_valid_size = (file_size % element_size) == 0;
	if (!is_valid_size) {
		throw peo::exceptions::filesize_not_compatible_with_typesize_exception(
			filepath, 
			file_size,
			element_size, 
			type_name<value_t>()
		);
	}
	auto data = Container{};
	const auto num_elements = file_size / element_size;
	data.resize(num_elements);
	auto file_stream = std::ifstream{ filepath, std::ios::binary };
	if (!file_stream.is_open()) {
		throw peo::exceptions::read_file_exception(filepath);
	}
	const auto scope_exit = detail::scope_exit{ [&file_stream]() {
		PRECOOKED_ASSERT(file_stream.is_open());
		file_stream.close();
		PRECOOKED_ASSERT(!file_stream.is_open());
	} };
	file_stream.seekg(0, std::ios_base::beg);
	if (file_size > 0) {
		auto* ptr_char = reinterpret_cast<byte_t*>(data.data());
		file_stream.read(ptr_char, file_size);	
	}
	return data;
}


}




// Read files
template <typename Char>
auto peo::read_file_to_string(
	const std::filesystem::path& filepath
) -> std::basic_string<Char> {
	using string_t = std::basic_string<Char>;
	return detail::impl_read_file_to_container<string_t>(filepath);
}

template <typename T>
auto peo::read_file_to_vector(
	const std::filesystem::path& filepath
) -> std::vector<T> {
	static_assert(std::is_arithmetic_v<T>, "T needs to be arithmetic");
	return detail::impl_read_file_to_container<std::vector<T>>(filepath);
}


// Write files
auto peo::write_vector_to_file(
	const detail::byte_view& byteview, 
	const std::filesystem::path& filepath
) -> void {
	if (filepath.has_parent_path()) {
		const auto dir = filepath.parent_path();
		if (!std::filesystem::exists(dir)) {
			std::filesystem::create_directories(dir);
		}
		if (!std::filesystem::exists(dir)) {
			throw peo::exceptions::dir_not_found_exception{ dir };
		}
		if (!std::filesystem::is_directory(dir)) {
			throw peo::exceptions::is_not_directory_exception{ dir };
		}
	}
	auto file_stream = std::ofstream{ filepath, std::ios::binary };
	if (!file_stream.is_open()) {
		throw peo::exceptions::write_file_exception{ filepath };
	}
	const auto scope_exit = detail::scope_exit{ [&file_stream]() {
		PRECOOKED_ASSERT(file_stream.is_open());
		file_stream.close();
		PRECOOKED_ASSERT(!file_stream.is_open());
	} };
	if (!byteview.empty()) {
		file_stream.write(byteview.data(), byteview.size());
	}
}

template <typename Str>
auto peo::write_string_to_file(
	const Str& str, 
	const std::filesystem::path& filepath
) -> void {
	static_assert(
		!std::is_same_v<Str, std::filesystem::path>, 
		"the filepath is the second argument"
	);
	using Char = type_traits::underlying_char_t<Str>;
	const auto sv = std::basic_string_view<Char>{ str };
	const auto byte_view = detail::byte_view{ sv };
	write_vector_to_file(byte_view, filepath);
}


















//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


namespace peo::detail {
template <typename Pred>
[[nodiscard]] auto impl_scan_flat(
	const std::filesystem::path& dir,
	const Pred& pred
) -> std::vector<std::filesystem::path> {
	auto paths = std::vector<std::filesystem::path>{};
	for (const auto& dir_entry : std::filesystem::directory_iterator{ dir }) {
		if (pred(dir_entry)) {
			const auto& p = dir_entry.path();
			paths.emplace_back(p);
		}
	}
	return paths;
}
}






auto peo::list_files_in_directory(
	const std::filesystem::path& dir
) -> std::vector<std::filesystem::path> {
	if (!std::filesystem::exists(dir)) {
		throw peo::exceptions::dir_not_found_exception{ dir };
	}
	if (!std::filesystem::is_directory(dir)) {
		throw peo::exceptions::is_not_directory_exception{ dir };
	}
	const auto filter_f = [](const std::filesystem::directory_entry& de) {
		return de.is_regular_file();
	};
	return detail::impl_scan_flat(dir, filter_f);
}

auto peo::list_subdirs_in_directory(
	const std::filesystem::path& dir
) -> std::vector<std::filesystem::path> {
	if (!std::filesystem::exists(dir)) {
		throw peo::exceptions::dir_not_found_exception{ dir };
	}
	if (!std::filesystem::is_directory(dir)) {
		throw peo::exceptions::is_not_directory_exception{ dir };
	}
	const auto filter_f = [](const std::filesystem::directory_entry& de) {
		return de.is_directory();
	};
	return detail::impl_scan_flat(dir, filter_f);
}



namespace peo::detail {
template <typename Pred>
[[nodiscard]] auto impl_scan_tree(
	const std::filesystem::path& rootdir,
	const Pred& pred
) -> std::vector<std::filesystem::path> {
	auto paths = std::vector<std::filesystem::path>{};
	auto entries = std::filesystem::recursive_directory_iterator{ rootdir };
	for (const auto& dir_entry : entries) {
		if (pred(dir_entry)) {
			const auto& path = dir_entry.path();
			paths.push_back(path);
		}
	}
	return paths;
}
}


auto peo::list_files_in_directory_tree(
	const std::filesystem::path& dir
) -> std::vector<std::filesystem::path> {
	if (!std::filesystem::exists(dir)) {
		throw peo::exceptions::dir_not_found_exception{ dir };
	}
	if (!std::filesystem::is_directory(dir)) {
		throw peo::exceptions::is_not_directory_exception{ dir };
	}
	const auto filter_f = [](const std::filesystem::directory_entry& de) { 
		return de.is_regular_file();
	};
	return detail::impl_scan_tree(dir, filter_f);
}

auto peo::list_subdirs_in_directory_tree(
	const std::filesystem::path& dir
) -> std::vector<std::filesystem::path> {
	if (!std::filesystem::exists(dir)) {
		throw peo::exceptions::dir_not_found_exception{ dir };
	}
	if (!std::filesystem::is_directory(dir)) {
		throw peo::exceptions::is_not_directory_exception{ dir };
	}
	const auto filter_f = [](const std::filesystem::directory_entry& de) {
		return de.is_directory();
	};
	return detail::impl_scan_tree(dir, filter_f);
}









//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// Make args
#include <vector>
#include <string>

auto peo::make_args(int argc, char* argv[]) -> std::vector<std::string> {
	if (argv == nullptr) {
		throw std::invalid_argument{ "argv cannot be nullptr" };
	}
	if (argc < 0) {
		throw std::invalid_argument{ "argc must be >= 0" };
	}
	auto ret = std::vector<std::string>{};
	const auto num_args = safe_cast<size_t>(argc);
	ret.reserve(num_args);
	for (size_t i = 0; i < num_args; ++i) {
		const auto* ptr = argv[i];
		if (ptr == nullptr) {
			continue;
		}
		auto str = std::string{ ptr };
		str = trim_string(std::move(str));
		if (str.empty()) {
			continue;
		}
		ret.emplace_back(str);
	}
	PRECOOKED_ASSERT(
		std::none_of(ret.begin(), ret.end(), [](auto&& str) { return str.empty(); })
	);
	return ret;
}