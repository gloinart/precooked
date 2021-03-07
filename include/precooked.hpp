#pragma once
/////////////////////////////////////////////
//
// License : MIT
// http://opensource.org/licenses/MIT
//

#define PRECOOKED_INLINE inline
#ifndef PRECOOKED_ASSERT
	#ifdef NDEBUG
		#define PRECOOKED_ASSERT(x) for(;false;) { [[maybe_unused]] auto result = (x); }
	#else
		#include <cassert>
		#define PRECOOKED_ASSERT(x) assert(x);
	#endif
#endif
#include <string>
#include <string_view>
#include <vector>
#include <filesystem>
#include <optional>
namespace prc::detail {
	class byte_view;
}
namespace prc::type_traits {
template <typename T> constexpr auto underlying_char_f() {
	if constexpr (std::is_pointer_v<T>) { return std::remove_pointer_t<T>{}; }
	else if constexpr (std::is_array_v<T>) { return T{}[0]; }
	else { return typename T::value_type{}; }
}
template <typename T> using underlying_char_t = decltype(underlying_char_f<T>());
}



namespace prc {

// IO - Read and write files
template <typename Char = char> [[nodiscard]] auto read_file_to_string(const std::filesystem::path& filepath)->std::basic_string<Char>;
template <typename T> [[nodiscard]] auto read_file_to_vector(const std::filesystem::path& filepath)->std::vector<T>;
template <typename Str0> auto write_string_to_file(const Str0& str, const std::filesystem::path& filepath) -> void;
PRECOOKED_INLINE auto write_vector_to_file(const detail::byte_view& bytevector, const std::filesystem::path& filepath) -> void;

// Convert anything to string
template <typename T> [[nodiscard]] auto as_string(const T& val)->std::string;

// String - split
template <typename Str0, typename Str1> [[nodiscard]] auto split_string(const Str0& str, const Str1& delimiters)->std::vector<std::basic_string<type_traits::underlying_char_t<Str0>>>;
template <typename Str0, typename Str1> [[nodiscard]] auto split_string_to_views(const Str0& str, const Str1& delimiters)->std::vector<std::basic_string_view<type_traits::underlying_char_t<Str0>>>;
template <typename Char, typename Str0> [[nodiscard]] auto split_string_to_views(std::basic_string<Char>&& str, Str0&& delimiters)->std::vector<std::basic_string_view<Char>> = delete; // Prevent dangling std::string_views
template <typename Str> [[nodiscard]] auto split_string_to_lines(const Str& str)->std::vector<std::basic_string<type_traits::underlying_char_t<Str>>>;

// String - replace
template <typename Char, typename Str0, typename Str1> [[nodiscard]] auto replace_all(std::basic_string<Char> haystack, const Str0& needle, const Str1& replacement)->std::basic_string<Char>; // Noexcept if dst.size() <= src.size() 
template <typename Str0, typename Str1, typename Str2> [[nodiscard]] auto replace_all(const Str0& haystack, const Str1& needle, const Str2& replacement)->std::basic_string<type_traits::underlying_char_t<Str0>>;
template <typename Char, typename Str0, typename Str1> [[nodiscard]] auto replace_all_ignore_case(std::basic_string<Char> haystack, const Str0& needle, const Str1& replacement)->std::basic_string<Char>; // Noexcept if dst.size() <= src.size() 
template <typename Str0, typename Str1, typename Str2> [[nodiscard]] auto replace_all_ignore_case(const Str0& haystack, const Str1& needle, const Str2& replacement) -> std::basic_string<type_traits::underlying_char_t<Str0>>;

// String - trim
template <typename Str0> [[nodiscard]] auto is_trimmed(const Str0& str) noexcept -> bool;
template <typename Char> [[nodiscard]] auto trim_string(std::basic_string<Char> str) noexcept -> std::basic_string<Char>;
template <typename Str0> [[nodiscard]] auto trim_string_to_view(const Str0& str) noexcept -> std::basic_string_view<type_traits::underlying_char_t<Str0>>;
template <typename Char> [[nodiscard]] auto trim_string_to_view(std::basic_string<Char>&& str) noexcept -> std::basic_string_view<Char> = delete; // Prevent dangling std::string_views
template <typename Str0, typename Str1> [[nodiscard]] auto is_trimmed(const Str0& str, const Str1& trim_chars) noexcept -> bool;
template <typename Char, typename Str0> [[nodiscard]] auto trim_string(std::basic_string<Char> str, const Str0& trim_chars) noexcept -> std::basic_string<Char>;
template <typename Str0, typename Str1> [[nodiscard]] auto trim_string_to_view(const Str0& str, const Str1& trim_chars) noexcept -> std::basic_string_view<type_traits::underlying_char_t<Str0>>;
template <typename Char, typename Str0> [[nodiscard]] auto trim_string_to_view(std::basic_string<Char>&& str, const Str0& trim_chars) noexcept -> std::basic_string_view<Char> = delete; // Prevent dangling std::string_views

// String - join
template <typename Strings, typename Str>
[[nodiscard]] auto join_strings(const Strings& strings, const Str& delimiter) -> std::basic_string<type_traits::underlying_char_t<Str>>;
template <typename Strings, typename Char = typename Strings::value_type::value_type>
[[nodiscard]] auto join_strings(const Strings& strings) -> std::basic_string<Char>;

// String case insensitive compare
template <typename Str0, typename Str1> [[nodiscard]] auto find_ignore_case(const Str0& haystack, const Str1& needle, size_t offset = 0) noexcept -> size_t;
template <typename Str0, typename Str1> [[nodiscard]] auto is_equal_ignore_case(const Str0& a, const Str1& b) noexcept -> bool;
template <typename Str0, typename Str1> [[nodiscard]] auto contains_substring(const Str0& haystack, const Str1& needle) noexcept -> bool;
template <typename Str0, typename Str1> [[nodiscard]] auto contains_substring_ignore_case(const Str0& haystack, const Str1& needle) noexcept -> bool;

template <typename Char> [[nodiscard]] auto to_lower(std::basic_string<Char> str) noexcept -> std::basic_string<Char>;
template <typename Char> [[nodiscard]] auto to_upper(std::basic_string<Char> str) noexcept -> std::basic_string<Char>;
template <typename StrView> [[nodiscard]] auto to_lower(const StrView& str)->std::basic_string<type_traits::underlying_char_t<StrView>>;
template <typename StrView> [[nodiscard]] auto to_upper(const StrView& str)->std::basic_string<type_traits::underlying_char_t<StrView>>;

// String to number conversion
template <typename T> [[nodiscard]] auto string_to_number(std::string_view str) noexcept -> std::optional<T>;
template <typename T> [[nodiscard]] auto number_to_string(const T& number) -> std::string; // Just here for consistency, simply uses std::to_string(...) underneath

// Scan filesystem
[[nodiscard]] PRECOOKED_INLINE auto files_in_directory(const std::filesystem::path& dir) -> std::vector<std::filesystem::path>;
[[nodiscard]] PRECOOKED_INLINE auto subdirs_in_directory(const std::filesystem::path& dir) -> std::vector<std::filesystem::path>;
[[nodiscard]] PRECOOKED_INLINE auto files_in_directory_tree(const std::filesystem::path& dir) -> std::vector<std::filesystem::path>;
[[nodiscard]] PRECOOKED_INLINE auto subdirs_in_directory_tree(const std::filesystem::path& dir) -> std::vector<std::filesystem::path>;


// Convenience
template <typename Tpl, typename Func>
auto tuple_for_each(Tpl&& tpl, Func&& func) -> void;
template <typename Tpl, typename Func>
[[nodiscard]] auto tuple_any_of(const Tpl& tpl, const Func& func) -> bool;
template <typename DstType, typename SrcType>
[[nodiscard]] auto cast(const SrcType& src_type)->DstType;

// Type names
template <typename T> [[nodiscard]] auto type_name() -> std::string;
template <typename T> [[nodiscard]] auto type_name(T&&) -> std::string;
template <typename T> [[nodiscard]] auto held_type_name(const T& value) -> std::string;

}



































//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////



#include <type_traits>


class prc::detail::byte_view {
public:
	using byte_t = char;
	template <
		typename C, 
		typename T = typename C::value_type
	>
	constexpr byte_view(const C& c) noexcept
	: ptr_{ reinterpret_cast<const byte_t*>(c.data()) }
	, num_elements_{ c.size() }
	, element_size_{sizeof(T)} {
		using ptr_t = decltype(c.data());
		using value_t = std::decay_t<std::remove_pointer_t<ptr_t>>;
		static_assert(std::is_arithmetic_v<value_t>, "Non-arithmetic values is not allowed");
		static_assert(!std::is_same_v<std::vector<bool>, std::decay_t<C>>, "vector<bool> is not allowed");
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

namespace prc::detail {
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


namespace prc::exceptions {

class io_exception : public std::runtime_error {
	using inherited = std::runtime_error;
public:
	io_exception(std::string msg, std::filesystem::path path)
	: inherited{ msg + ": " + path.string() } {}
};


class filesize_not_compatible_with_typesize_exception : public io_exception {
public:
	filesize_not_compatible_with_typesize_exception(std::filesystem::path path, size_t filesize, size_t element_size, const std::string& type_name)
	: io_exception{ 
		"filesize " + std::to_string(filesize) + " not aligned with element size: " + std::to_string(element_size) + "[type:" + type_name + "]",
		std::move(path) } 
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
	bad_cast_exception(std::string msg) : msg_{ std::move(msg) } {}
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

namespace prc::type_traits::introspection {
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

namespace prc::type_traits {
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

namespace prc::detail {
template<typename Tpl, typename Func, size_t Idx = 0>
auto impl_tuple_for_each(Tpl& tpl, Func&& func) -> void {
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
auto prc::tuple_for_each(Tpl&& tpl, Func&& func) -> void {
	return detail::impl_tuple_for_each(std::forward<Tpl>(tpl), std::forward<Func>(func));
}

template <typename Tpl, typename Func>
auto prc::tuple_any_of(const Tpl& tpl, const Func& func) -> bool {
	return detail::impl_tuple_any_of(tpl, func);
}

template <typename DstType, typename SrcType>
auto prc::cast(const SrcType& src)->DstType {
	const auto casted = static_cast<DstType>(src);
	const auto casted_back = static_cast<SrcType>(casted);
	if (casted_back != src) {
		throw prc::exceptions::bad_cast_exception{
			std::string{ "bad cast from " } + type_name(src) + " to " + type_name(casted)
		};
	}
	return casted;
}







//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#include <type_traits>
#include <string>

template <typename T>
auto prc::type_name() -> std::string { return typeid(T).name(); }
template <typename T>
auto prc::type_name(T&&) -> std::string { return typeid(T).name(); }



#include <any>
#include <variant>

template <typename T>
auto prc::held_type_name(const T& value) -> std::string {
	using value_t = std::decay_t<T>;
	if constexpr (type_traits::is_variant_v<T>) {
		if (value.valueless_by_exception()) {
			return "variant valueless by exception";
		}
		return std::visit([](const auto& candidate) -> std::string {
			using value_t = std::decay_t<decltype(candidate)>;
			return typeid(value_t).name();
		}, value);
	}
	else if constexpr (std::is_same_v<std::any, value_t>) {
		return value.has_value() ?
			value.type().name() :
			"empty any";
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
auto prc::string_to_number(const std::string_view str) noexcept -> std::optional<T> {
	static_assert(std::is_arithmetic_v<T>, "T needs to be an arithmetic type");
	auto value = T{};
	const auto* ptr_end = str.data() + str.size();
	const auto result = std::from_chars(str.data(), ptr_end, value);
	return 
		result.ec != std::errc{} ? std::optional<T>{} :
		result.ptr != ptr_end ? std::optional<T>{} :
		value;
}

template <typename T> 
auto prc::number_to_string(const T& number) -> std::string {
	return std::to_string(number);
}



#include <string>
#include <string_view>
#include <type_traits>
#include <sstream>


namespace prc::detail {
[[nodiscard]] PRECOOKED_INLINE auto uint8_to_hexstring(const uint8_t value) noexcept -> std::string {
	auto hex_digit_f = [](const uint8_t v) noexcept -> char {
		constexpr auto digits = std::string_view{ "0123456789abcdef" };
		PRECOOKED_ASSERT(v < digits.size());
		return digits[v];
	};
	auto str = std::string{};
	str.resize(2);
	str[0] = hex_digit_f(value / 16);
	str[1] = hex_digit_f(value % 16);
	return str;
};
}

template <typename T>
auto prc::as_string(const T& value) -> std::string {
	using value_t = std::decay_t<T>;
	if constexpr (type_traits::is_string_v<value_t>) {
		auto str = std::string{};
		str.resize(value.size());
		using src_char_t = type_traits::underlying_char_t<T>;
		using dst_char_t = std::string::value_type;
		auto truncate_to_char_t = [](const src_char_t& c) {
			return c > std::numeric_limits<dst_char_t>::max() ?
				'?' :
				static_cast<dst_char_t>(c);
		};
		std::transform(value.begin(), value.end(), str.begin(), truncate_to_char_t);
		return str;
	}
	else if constexpr (std::is_same_v<std::string, value_t>) {
		return value;
	}
	else if constexpr (std::is_same_v<std::string_view, value_t>) {
		return std::string{ value };
	}
	else if constexpr (std::is_same_v<bool, value_t>) {
		return value ? "true" : "false";
	}
	else if constexpr (std::is_arithmetic_v<value_t>) {
		return std::to_string(value);
	}
	else if constexpr (type_traits::is_smart_ptr_v<value_t>) {
		return value == nullptr ? 
			"nullptr" : 
			as_string(*value);
	}
	else if constexpr (type_traits::is_variant_v<value_t>) {
		return std::visit([](auto&& candidate) {
			return as_string(candidate);
		}, value);
	}
	else if constexpr (type_traits::is_optional_v<value_t>) {
		return value.has_value() ?
			as_string(*value) :
			"std::nullopt";
	}
	else if constexpr (std::is_same_v<std::any, value_t>) {
		return value.empty() ?
			"empty any" :
			"any containing type " + held_type_name(value);
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
		str.reserve(value.size() * 4); // Guess
		str += '[';
		for (auto&& element : value) {
			str += as_string(element);
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
		str.reserve(std::tuple_size_v<value_t> * 4); // Guess
		str += '[';
		tuple_for_each(value, [&str](auto&& elem) {
			str += as_string(elem);
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
		using underlying_type = typename std::underlying_type<T>::type;
		return as_string(static_cast<underlying_type>(value));
	}
	else if constexpr (type_traits::is_duration_v<value_t>) {
		return as_string(value.count());
	}
	else {
		const auto* first = reinterpret_cast<const uint8_t*>(&value);
		const auto* last = first + sizeof(value);
		auto str = std::string{ "unknown 0x" };
		for (auto it = first; it < last; ++it) {
			str += detail::uint8_to_hexstring(*it);
		}
		return str;
	}

}






//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


#include <fstream>
#include <vector>
#include <cstdint>
#include <optional>

namespace prc::detail {


[[nodiscard]] PRECOOKED_INLINE auto filesize_to_size_t(
	uintmax_t filesize_uintmax
) noexcept ->std::optional<size_t> {
	const auto file_size = static_cast<size_t>(filesize_uintmax);
	return static_cast<uintmax_t>(file_size) == filesize_uintmax ?
		file_size :
		std::optional<size_t>{};
}

template <typename Container>
[[nodiscard]] auto impl_read_file_to_container(const std::filesystem::path& filepath) -> Container {
	if (!std::filesystem::exists(filepath)) {
		throw prc::exceptions::file_not_found_exception(filepath);
	}
	if (!std::filesystem::is_regular_file(filepath)) {
		throw prc::exceptions::is_not_file_exception(filepath);
	}
	using value_t = typename Container::value_type;
	using byte_t = byte_view::byte_t;
	const auto file_size_uintmax = std::filesystem::file_size(filepath);
	const auto file_size_optional = detail::filesize_to_size_t(file_size_uintmax);
	if (!file_size_optional.has_value()) {
		throw prc::exceptions::file_too_large_exception(filepath, file_size_uintmax);
	}
	const auto file_size = *file_size_optional;
	constexpr auto element_size = sizeof(value_t);
	static_assert(element_size > 0);
	const auto is_valid_size = (file_size % element_size) == 0;
	if (!is_valid_size) {
		throw prc::exceptions::filesize_not_compatible_with_typesize_exception(
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
		throw prc::exceptions::read_file_exception(filepath);
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






template <typename T>
auto prc::read_file_to_vector(const std::filesystem::path& filepath) -> std::vector<T> {
	static_assert(std::is_arithmetic_v<T>, "T needs to be arithmetic");
	return detail::impl_read_file_to_container<std::vector<T>>(filepath);
}

template <typename Str0>
auto prc::write_string_to_file(const Str0& str, const std::filesystem::path& filepath) -> void {
	using Char = type_traits::underlying_char_t<Str0>;
	const auto sv = std::basic_string_view<Char>{ str };
	const auto byte_view = detail::byte_view{ sv };
	write_vector_to_file(byte_view, filepath);
}




//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#include <numeric>


namespace prc::detail {
template <typename Strings, typename Char>
[[nodiscard]] auto impl_join_strings(const Strings& strings, const std::basic_string_view<Char>& delimiter) -> std::basic_string<Char> {
	if (strings.empty()) {
		return {};
	}
	const auto target_size_strings =
		std::accumulate(strings.begin(), strings.end(), size_t{ 0 }, [](size_t sum, const auto& str) {
			return sum + str.size();
		});
	const auto target_size_delimiters = (strings.size() - 1) * delimiter.size();
	const auto target_size = target_size_strings + target_size_delimiters;
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
	}
	PRECOOKED_ASSERT(joined.size() == target_size);
	return joined;
}
}

template <typename Strings, typename Str>
auto prc::join_strings(const Strings& strings, const Str& delimiter) -> std::basic_string<type_traits::underlying_char_t<Str>> {
	using Char = type_traits::underlying_char_t<Str>;
	static_assert(type_traits::is_valid_char_v<Char>);
	return detail::impl_join_strings(strings, std::basic_string_view<Char>{delimiter});
}

template <typename Strings, typename Char>
auto prc::join_strings(const Strings& strings) -> std::basic_string<Char> {
	static_assert(type_traits::is_valid_char_v<Char>);
	return detail::impl_join_strings(strings, std::basic_string_view<Char>{});
}













































































//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


#include <algorithm>





//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////



namespace prc::detail {
template <typename Char>
[[nodiscard]] constexpr auto default_trim_chars() noexcept -> std::basic_string_view<Char> {
	using namespace std::string_view_literals;
	if constexpr (std::is_same_v<Char, char>) { return "\t\r\t\n "sv; }
	else if constexpr (std::is_same_v<Char, wchar_t>) { return L"\t\r\t\n "sv; }
	else if constexpr (std::is_same_v<Char, char16_t>) { return u"\t\r\t\n "sv; }
	else if constexpr (std::is_same_v<Char, char32_t>) { return U"\t\r\t\n "sv; }
}
template <typename Char>
[[nodiscard]] constexpr auto linebreaks() noexcept -> std::basic_string_view<Char> {
	using namespace std::string_view_literals;
	if constexpr (std::is_same_v<Char, char>) { return "\r\n"sv; }
	else if constexpr (std::is_same_v<Char, wchar_t>) { return L"\r\n"sv; }
	else if constexpr (std::is_same_v<Char, char16_t>) { return u"\r\n"sv; }
	else if constexpr (std::is_same_v<Char, char32_t>) { return U"\r\n"sv; }
}
}


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

namespace prc::detail {

template <typename Char, typename PartType>
[[nodiscard]] auto impl_split_string(
	const std::basic_string_view<Char> str,
	const std::basic_string_view<Char> delimiters
) -> std::vector<PartType> {
	constexpr auto npos = std::basic_string_view<Char>::npos;
	if (str.empty()) {
		return {};
	}
	if (delimiters.empty()) {
		return std::vector<PartType>{ PartType{ str } };
	}
	const auto first_not_delimiter = str.find_first_not_of(delimiters);
	if (first_not_delimiter >= str.size()) {
		return {};
	}
	// Calculate num parts in order to allocated vector
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
		parts.emplace_back(PartType{ str.data() + left, right - left });
	}
	PRECOOKED_ASSERT(parts.size() == num_parts);
	return parts;
}
}


template <typename Str0, typename Str1>
auto prc::split_string(
	const Str0& str, 
	const Str1& delimiters
) -> std::vector<std::basic_string<type_traits::underlying_char_t<Str0>>> {
	using Char = type_traits::underlying_char_t<Str0>;
	return detail::impl_split_string<Char, std::basic_string<Char>>(str, delimiters);
}

template <typename Str0, typename Str1>
auto prc::split_string_to_views(
	const Str0& str, 
	const Str1& delimiters
) -> std::vector<std::basic_string_view<type_traits::underlying_char_t<Str0>>> {
	using Char = type_traits::underlying_char_t<Str0>;
	static_assert(type_traits::is_valid_char_v<Char>);
	return detail::impl_split_string<Char, std::basic_string_view<Char>>(str, delimiters);
}

template <typename Str>
auto prc::split_string_to_lines(
	const Str& str
) -> std::vector<std::basic_string<type_traits::underlying_char_t<Str>>> {
	using Char = type_traits::underlying_char_t<Str>;
	static_assert(type_traits::is_valid_char_v<Char>);
	return split_string(str, detail::linebreaks<Char>());
}








//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////









template <typename Char>
auto prc::trim_string(
	std::basic_string<Char> str
) noexcept -> std::basic_string<Char> {
	return trim_string(std::move(str), detail::default_trim_chars<Char>());
}

template <typename Char, typename Str0>
auto prc::trim_string(
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
auto prc::trim_string_to_view(
	const Str0& str
) noexcept -> std::basic_string_view<type_traits::underlying_char_t<Str0>> {
	using Char = type_traits::underlying_char_t<Str0>;
	static_assert(type_traits::is_valid_char_v<Char>);
	return trim_string_to_view(str, detail::default_trim_chars<Char> ());
}

 template <typename Str0, typename Str1>
auto prc::trim_string_to_view(
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
auto prc::is_trimmed(const Str0& str) noexcept -> bool {
	using Char = type_traits::underlying_char_t<Str0>;
	static_assert(type_traits::is_valid_char_v<Char>);
	const auto sv = std::basic_string_view<Char>{ str };
	return is_trimmed(str, detail::default_trim_chars<Char>());
}

template<typename Str0, typename Str1>
auto prc::is_trimmed(const Str0& str, const Str1& trim_chars) noexcept -> bool {
	using Char = type_traits::underlying_char_t<Str0>;
	static_assert(type_traits::is_valid_char_v<Char>);
	const auto sv = std::basic_string_view<Char>{ str };
	constexpr auto npos = std::basic_string_view<Char>::npos;
	return
		sv.empty() ? true :
		trim_chars.find(sv.front()) != npos ? false :
		trim_chars.find(sv.back()) != npos ? false :
		true;
}








//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

namespace prc::detail {


constexpr auto find_case_sensitive_func = [](const auto& str, const auto& key, const size_t offset) noexcept -> size_t {
	return str.find(key, offset);
};

const auto is_chars_equal_ignore_case = [](auto&& a, auto&& b) noexcept {
	return a == b || std::tolower(a) == std::tolower(b);
};
template <typename Char>
[[nodiscard]] auto impl_is_equal_ignore_case(
	const std::basic_string_view<Char>& a,
	const std::basic_string_view<Char>& b
) noexcept -> bool {
	PRECOOKED_ASSERT(a.size() == b.size());
	return std::equal(a.begin(), a.end(), b.begin(), detail::is_chars_equal_ignore_case);
}



template <typename Char>
[[nodiscard]] auto impl_find_ignore_case(
	const std::basic_string_view<Char>& haystack,
	const std::basic_string_view<Char>& needle,
	const size_t offset
) noexcept -> size_t {
	PRECOOKED_ASSERT(haystack.size() >= needle.size());
	PRECOOKED_ASSERT(!needle.empty());
	constexpr auto npos = std::basic_string_view<Char>::npos;
	const auto i_end = (1 + haystack.size()) - needle.size();
	if (offset > i_end) {
		return npos;
	}
	const auto equals_needle_front_f = [
		upper = std::toupper(needle.front()),
		lower = std::tolower(needle.front())
	](const Char& c) {
		return c == upper || c == lower;
	};
	const auto needle_tail = needle.substr(1);
	for (size_t i = offset; i < i_end; ++i) {
		PRECOOKED_ASSERT(i < haystack.size());
		if (!equals_needle_front_f(haystack[i])) {
			continue;
		}
		PRECOOKED_ASSERT(i + needle.size() <= haystack.size());
		const auto candidate_tail = haystack.substr(i + 1, needle.size() - 1);
		if (detail::impl_is_equal_ignore_case(candidate_tail, needle_tail)) {
			return i;
		}
	}
	return npos;
}

}



//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////



namespace prc::detail {


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
[[nodiscard]] auto impl_replace_all_equal_key_length(
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
	const auto first_key = find_func(haystack, needle, 0);
	if (first_key == npos) {
		return haystack;
	}
	replace_string_part(haystack, first_key, replacement);
	auto write_pos = first_key + replacement.size();
	for (auto read_pos = first_key + needle.size();;) {
		PRECOOKED_ASSERT(write_pos < read_pos);
		const auto key_pos = std::min(find_func(haystack, needle, read_pos), haystack.size());
		while (read_pos < key_pos) {
			PRECOOKED_ASSERT(write_pos < haystack.size());
			PRECOOKED_ASSERT(read_pos < haystack.size());
			haystack[write_pos] = haystack[read_pos];
			++write_pos;
			++read_pos;
		}
		if (key_pos == haystack.size()) {
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
// Required if replacement is larger then needle or the source is a string_view.
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
	const auto calculate_num_occurances_f = [&]() noexcept {
		return calculate_num_occurances(
			haystack, 
			needle, 
			first_match + needle.size(), 
			find_func
		) + 1; // first_match indicates one occurances
	};
	const auto target_size =
		needle.size() < replacement.size() ? haystack.size() + (replacement.size() - needle.size()) * calculate_num_occurances_f() :
		replacement.size() < needle.size() ? haystack.size() - (needle.size() - replacement.size()) * calculate_num_occurances_f() :
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
auto prc::replace_all(std::basic_string<Char> haystack, const Str0& needle, const Str1& replacement) -> std::basic_string<Char> {
	// Pick implementation based on key/dst size
	const auto haystack_sv = std::basic_string_view<Char>{ haystack };
	const auto needle_sv = std::basic_string_view<Char>{ needle };
	const auto replacement_sv = std::basic_string_view<Char>{ replacement };
	const auto& find_func = detail::find_case_sensitive_func;
	const auto no_replacement_possible = needle_sv.empty() || needle_sv.size() > haystack_sv.size();
	return
		no_replacement_possible ? haystack :
		needle_sv.length() == replacement_sv.length() ? detail::impl_replace_all_equal_key_length(std::move(haystack), needle_sv, replacement_sv, find_func) :
		replacement_sv.length() < needle_sv.length() ? detail::impl_replace_all_shrink_string(std::move(haystack), needle_sv, replacement_sv, find_func) :
		detail::impl_replace_all_rebuild_string<Char>(haystack, needle_sv, replacement_sv, find_func);
}


template <typename Str0, typename Str1, typename Str2>
auto prc::replace_all(const Str0& haystack, const Str1& needle, const Str2& replacement) -> std::basic_string<type_traits::underlying_char_t<Str0>> {
	using Char = type_traits::underlying_char_t<Str0>;
	static_assert(type_traits::is_valid_char_v<Char>);
	const auto haystack_sv = std::basic_string_view<Char>{ haystack };
	const auto needle_sv = std::basic_string_view<Char>{ needle };
	const auto replacement_sv = std::basic_string_view<Char>{ replacement };
	const auto& find_func = detail::find_case_sensitive_func;
	const auto no_replacement_possible = needle_sv.empty() || needle_sv.size() > haystack_sv.size();
	return no_replacement_possible ?
		std::basic_string<Char>{ haystack_sv } :
		detail::impl_replace_all_rebuild_string(haystack_sv, needle_sv, replacement_sv, find_func);
}


template <typename Char, typename Str0, typename Str1>
auto prc::replace_all_ignore_case(std::basic_string<Char> haystack, const Str0& needle, const Str1& replacement) -> std::basic_string<Char> {
	static_assert(type_traits::is_valid_char_v<Char>);
	const auto haystack_sv = std::basic_string_view<Char>{ haystack };
	const auto needle_sv = std::basic_string_view<Char>{ needle };
	const auto replacement_sv = std::basic_string_view<Char>{ replacement };
	const auto& find_func = detail::impl_find_ignore_case<Char>;
	const auto no_replacement_possible = needle_sv.empty() || needle_sv.size() > haystack_sv.size();
	return
		no_replacement_possible ? haystack :
		needle_sv.length() == replacement_sv.length() ? detail::impl_replace_all_equal_key_length(std::move(haystack), needle_sv, replacement_sv, find_func) :
		replacement_sv.length() < needle_sv.length() ? detail::impl_replace_all_shrink_string(std::move(haystack), needle_sv, replacement_sv, find_func) :
		detail::impl_replace_all_rebuild_string<Char>(haystack_sv, needle_sv, replacement_sv, find_func);
}

template <typename Str0, typename Str1, typename Str2>
auto prc::replace_all_ignore_case(const Str0& haystack, const Str1& needle, const Str2& replacement) -> std::basic_string<type_traits::underlying_char_t<Str0>> {
	using Char = type_traits::underlying_char_t<Str0>;
	static_assert(type_traits::is_valid_char_v<Char>);
	const auto haystack_sv = std::basic_string_view<Char>{ haystack };
	const auto needle_sv = std::basic_string_view<Char>{ needle };
	const auto replacement_sv = std::basic_string_view<Char>{ replacement };
	const auto& find_func = detail::impl_find_ignore_case<Char>;
	const auto no_replacement_possible = needle_sv.empty() || needle_sv.size() > haystack_sv.size();
	return no_replacement_possible ?
		std::basic_string<Char>{ haystack_sv } :
		detail::impl_replace_all_rebuild_string(haystack_sv, needle_sv, replacement_sv, find_func);
}














//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


#include <algorithm>
#include <cctype>

template <typename Char>
auto prc::to_lower(std::basic_string<Char> str) noexcept -> std::basic_string<Char> {
	std::transform(str.begin(), str.end(), str.begin(), [](const Char& c) {
		return std::tolower(c);
	});
	return str;
}

template <typename StrView> 
auto prc::to_lower(const StrView& strview) -> std::basic_string<type_traits::underlying_char_t<StrView>> {
	using Char = type_traits::underlying_char_t<StrView>;
	static_assert(type_traits::is_valid_char_v<Char>);
	const auto sv = std::basic_string_view<Char>{strview};
	auto str = std::basic_string<Char>{};
	str.reserve(sv.size());
	std::transform(sv.begin(), sv.end(), std::back_inserter(str), [](const Char& c) {
		return std::tolower(c);
	});
	return str;
}


template <typename Char>
auto prc::to_upper(std::basic_string<Char> str) noexcept -> std::basic_string<Char> {
	std::transform(str.begin(), str.end(), str.begin(), [](const Char& c) {
		return std::toupper(c);
	});
	return str;
}

template <typename StrView>
auto prc::to_upper(const StrView& strview) -> std::basic_string<type_traits::underlying_char_t<StrView>> {
	using Char = type_traits::underlying_char_t<StrView>;
	static_assert(type_traits::is_valid_char_v<Char>);
	const auto sv = std::basic_string_view<Char>{ strview };
	auto str = std::basic_string<Char>{};
	str.reserve(sv.size());
	std::transform(sv.begin(), sv.end(), std::back_inserter(str), [](const Char& c) {
		return std::toupper(c);
	});
	return str;
}

template <typename Str0, typename Str1>
auto prc::is_equal_ignore_case(const Str0& a, const Str1& b) noexcept -> bool {
	const auto a_sv = std::basic_string_view{ a };
	const auto b_sv = std::basic_string_view{ b };
	return
		a_sv.size() == b_sv.size() &&
		detail::impl_is_equal_ignore_case(a_sv, b_sv);
}

template <typename Str0, typename Str1>
auto prc::contains_substring(const Str0& haystack, const Str1& needle) noexcept -> bool {
	using Char = type_traits::underlying_char_t<Str0>;
	static_assert(type_traits::is_valid_char_v<Char>);
	const auto haystack_sv = std::basic_string_view<Char>{ haystack };
	return haystack_sv.find(needle) != std::string::npos;
}

template <typename Str0, typename Str1>
auto prc::contains_substring_ignore_case(const Str0& haystack, const Str1& needle) noexcept -> bool {
	return find_ignore_case(haystack, needle, 0) != std::string::npos;
}


template <typename Str0, typename Str1>
auto prc::find_ignore_case(
	const Str0& haystack,
	const Str1& needle,
	const size_t offset
) noexcept -> size_t {
	using Char = type_traits::underlying_char_t<Str0>;
	static_assert(type_traits::is_valid_char_v<Char>);
	const auto haystack_sv = std::basic_string_view<Char>{ haystack };
	const auto needle_sv = std::basic_string_view<Char>{ needle };
	if (haystack_sv.size() < needle_sv.size() || needle_sv.empty()) {
		return std::string::npos;
	}
	return detail::impl_find_ignore_case(
		std::basic_string_view<Char>{haystack_sv},
		std::basic_string_view<Char>{needle_sv},
		offset
	);
}



//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


#include <fstream>
#include <filesystem>




// Read files
template <typename Char>
auto prc::read_file_to_string(const std::filesystem::path& filepath) -> std::basic_string<Char> {
	using string_t = std::basic_string<Char>;
	return detail::impl_read_file_to_container<string_t>(filepath);
}


// Write files
auto prc::write_vector_to_file(const detail::byte_view& byteview, const std::filesystem::path& filepath) -> void {
	if (filepath.has_parent_path()) {
		const auto dir = filepath.parent_path();
		if (!std::filesystem::exists(dir)) {
			std::filesystem::create_directories(dir);
		}
		if (!std::filesystem::exists(dir)) {
			throw prc::exceptions::dir_not_found_exception(dir);
		}
		if (!std::filesystem::is_directory(dir)) {
			throw prc::exceptions::is_not_directory_exception(dir);
		}
	}
	auto file_stream = std::ofstream{ filepath, std::ios::binary };
	if (!file_stream.is_open()) {
		throw prc::exceptions::write_file_exception(filepath);
	}
	const auto scope_exit = detail::scope_exit{ [&file_stream]() {
		file_stream.close();
	} };
	if (!byteview.empty()) {
		file_stream.write(byteview.data(), byteview.size());
	}
}





//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


namespace prc::detail {
template <typename Pred>
[[nodiscard]] auto impl_scan_flat(
	const std::filesystem::path& dir,
	const Pred& pred
) -> std::vector<std::filesystem::path> {
	auto paths = std::vector<std::filesystem::path>{};
	for (const auto& dir_entry : std::filesystem::directory_iterator{ dir }) {
		const auto& p = dir_entry.path();
		if (pred(p)) {
			paths.emplace_back(p);
		}
	}
	return paths;
}
}






auto prc::files_in_directory(const std::filesystem::path& dir) -> std::vector<std::filesystem::path> {
	if (!std::filesystem::exists(dir)) {
		throw prc::exceptions::dir_not_found_exception(dir);
	}
	if (!std::filesystem::is_directory(dir)) {
		throw prc::exceptions::is_not_directory_exception(dir);
	}
	return detail::impl_scan_flat(dir, [](auto&& p) { return std::filesystem::is_regular_file(p); });
}

auto prc::subdirs_in_directory(const std::filesystem::path& dir) -> std::vector<std::filesystem::path> {
	if (!std::filesystem::exists(dir)) {
		throw prc::exceptions::dir_not_found_exception(dir);
	}
	if (!std::filesystem::is_directory(dir)) {
		throw prc::exceptions::is_not_directory_exception(dir);
	}
	return detail::impl_scan_flat(dir, [](auto&& p) { return std::filesystem::is_directory(p); });
}



namespace prc::detail {
template <typename Pred>
[[nodiscard]] auto impl_scan_tree(
	const std::filesystem::path& rootdir,
	const Pred& pred
) -> std::vector<std::filesystem::path> {
	auto paths = std::vector<std::filesystem::path>{};
	auto entries = std::filesystem::recursive_directory_iterator{ rootdir };
	for (const auto& dir_entry : entries) {
		const auto& path = dir_entry.path();
		if (pred(path)) {
			paths.push_back(path);
		}
	}
	return paths;
}
}


auto prc::files_in_directory_tree(const std::filesystem::path& dir) -> std::vector<std::filesystem::path> {
	if (!std::filesystem::exists(dir)) {
		throw prc::exceptions::dir_not_found_exception(dir);
	}
	if (!std::filesystem::is_directory(dir)) {
		throw prc::exceptions::is_not_directory_exception(dir);
	}
	return detail::impl_scan_tree(dir, [](auto&& p) { return std::filesystem::is_regular_file(p); });
}

auto prc::subdirs_in_directory_tree(const std::filesystem::path& dir) -> std::vector<std::filesystem::path> {
	if (!std::filesystem::exists(dir)) {
		throw prc::exceptions::dir_not_found_exception(dir);
	}
	if (!std::filesystem::is_directory(dir)) {
		throw prc::exceptions::is_not_directory_exception(dir);
	}
	return detail::impl_scan_tree(dir, [](auto&& p) { return std::filesystem::is_directory(p); });
}



static_assert(std::string_view::npos == std::string::npos);