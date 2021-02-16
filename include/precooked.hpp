#pragma once
/////////////////////////////////////////////
//
// License : MIT
// http://opensource.org/licenses/MIT
//


// Configuration
#define PRECOOKED_NO_LIB
#ifdef PRECOOKED_NO_LIB
	#define PRECOOKED_INLINE inline
#else
	#define PRECOOKED_INLINE
#endif

#ifndef PRECOOKED_ASSERT
	#ifdef NDEBUG
		#define PRECOOKED_ASSERT(x) for(;false;) { [[maybe_unused]] auto result = (x); }
	#else
		#include <cassert>
		#define PRECOOKED_ASSERT(x) assert(x)
	#endif
#endif

// Includes
#include <string>
#include <string_view>
#include <vector>
namespace std { template <typename T> class optional; }
namespace std::filesystem { class path; }
namespace prc::detail { class byte_view; }


namespace prc {

// String case insesitive
[[nodiscard]] PRECOOKED_INLINE auto to_lower(std::string str) noexcept -> std::string;
[[nodiscard]] PRECOOKED_INLINE auto to_upper(std::string str) noexcept -> std::string;
[[nodiscard]] PRECOOKED_INLINE auto to_lower(std::string_view str) -> std::string;
[[nodiscard]] PRECOOKED_INLINE auto to_upper(std::string_view str) -> std::string;
[[nodiscard]] PRECOOKED_INLINE auto is_equal_ignore_case(std::string_view a, std::string_view b) noexcept -> bool;
[[nodiscard]] PRECOOKED_INLINE auto contains_substring(std::string_view haystack, std::string_view needle) noexcept -> bool;
[[nodiscard]] PRECOOKED_INLINE auto contains_substring_ignore_case(std::string_view haystack, std::string_view needle) noexcept -> bool;
[[nodiscard]] PRECOOKED_INLINE auto find_ignore_case(std::string_view haystack, std::string_view needle, size_t offset = 0) noexcept -> size_t;

// String - trim
constexpr auto default_trimchars = std::string_view{"\t\r\t\n "};
[[nodiscard]] PRECOOKED_INLINE auto trim_string(std::string str, std::string_view trim_chars = default_trimchars) noexcept -> std::string;
[[nodiscard]] PRECOOKED_INLINE auto trim_string_to_sv(std::string_view str, std::string_view trim_chars = default_trimchars) noexcept -> std::string_view;
[[nodiscard]] PRECOOKED_INLINE auto trim_string_to_sv(std::string&& str, std::string_view trim_chars = default_trimchars) noexcept -> std::string_view = delete; // Prevent dangling std::string_views
[[nodiscard]] PRECOOKED_INLINE auto is_trimmed(std::string_view str, std::string_view trim_chars = default_trimchars) noexcept -> bool;

// String - replace
[[nodiscard]] PRECOOKED_INLINE auto replace_all(std::string str, std::string_view src, std::string_view dst) -> std::string; // Does not throw if dst.size() <= src.size() 
[[nodiscard]] PRECOOKED_INLINE auto replace_all(std::string_view str, std::string_view src, std::string_view dst) -> std::string;
[[nodiscard]] PRECOOKED_INLINE auto replace_all(const char* str, std::string_view src, std::string_view dst) -> std::string; // Avoid ambiguity between std::string and std::string_view conversion
[[nodiscard]] PRECOOKED_INLINE auto replace_all_ignore_case(std::string str, std::string_view src, std::string_view dst)->std::string; // Does not throw if dst.size() <= src.size() 
[[nodiscard]] PRECOOKED_INLINE auto replace_all_ignore_case(std::string_view str, std::string_view src, std::string_view dst)->std::string;
[[nodiscard]] PRECOOKED_INLINE auto replace_all_ignore_case(const char* str, std::string_view src, std::string_view dst)->std::string; // Avoid ambiguity between std::string and std::string_view conversion

// String - remove
[[nodiscard]] PRECOOKED_INLINE auto remove_all(std::string str, std::string_view src) noexcept ->std::string;
[[nodiscard]] PRECOOKED_INLINE auto remove_all(std::string_view str, std::string_view src)->std::string;
[[nodiscard]] PRECOOKED_INLINE auto remove_all(const char* str, std::string_view src)->std::string; // Avoid ambiguity between std::string and std::string_view conversion
[[nodiscard]] PRECOOKED_INLINE auto remove_all_ignore_case(std::string str, std::string_view src) noexcept->std::string;
[[nodiscard]] PRECOOKED_INLINE auto remove_all_ignore_case(std::string_view str, std::string_view src)->std::string;
[[nodiscard]] PRECOOKED_INLINE auto remove_all_ignore_case(const char* str, std::string_view src)->std::string; // Avoid ambiguity between std::string and std::string_view conversion

// String - split
[[nodiscard]] PRECOOKED_INLINE auto split_string(std::string_view str, const std::string_view delimiters) -> std::vector<std::string>;
[[nodiscard]] PRECOOKED_INLINE auto split_string_to_sv(std::string_view str, const std::string_view delimiters) -> std::vector<std::string_view>;
[[nodiscard]] PRECOOKED_INLINE auto split_string_to_sv(std::string&& str, const std::string_view delimiters) -> std::vector<std::string_view> = delete; // Prevent dangling std::string_views
[[nodiscard]] PRECOOKED_INLINE auto split_string_to_lines(std::string_view str)->std::vector<std::string>;

// String - join
template <typename Range>
[[nodiscard]] auto join_strings(const Range& strings, const std::string_view delimiter)-> std::string;
template <typename Range>
[[nodiscard]] auto join_strings(const Range& strings)->std::string;

// String conversion
template <typename T> [[nodiscard]] auto string_to_number(std::string_view str) noexcept -> std::optional<T>;
template <typename T> [[nodiscard]] auto number_to_string(const T& number) -> std::string; // Just here for consistency, uses std::to_string
template <typename T> [[nodiscard]] auto to_string(const T& val)->std::string;

	
// IO - Read files
[[nodiscard]] PRECOOKED_INLINE auto read_file_to_string(const std::filesystem::path& filepath)->std::string;
template <typename T>
[[nodiscard]] PRECOOKED_INLINE auto read_file_to_vector(const std::filesystem::path& filepath)->std::vector<T>;
[[nodiscard]] PRECOOKED_INLINE auto is_vector_equal_to_file_content(const detail::byte_view& bytevector, const std::filesystem::path& filepath) -> bool;
[[nodiscard]] PRECOOKED_INLINE auto is_string_equal_to_file_content(std::string_view str, const std::filesystem::path& filepath) -> bool;

// IO - Write files
PRECOOKED_INLINE auto write_string_to_file(const std::string_view& str, const std::filesystem::path& filepath) -> void;
PRECOOKED_INLINE auto write_bytevector_to_file(const detail::byte_view& bytevector, const std::filesystem::path& filepath) -> void;

// Scan filesystem
[[nodiscard]] PRECOOKED_INLINE auto files_in_directory(const std::filesystem::path& dir)->std::vector<std::filesystem::path>;
[[nodiscard]] PRECOOKED_INLINE auto subdirs_in_directory(const std::filesystem::path& dir)->std::vector<std::filesystem::path>;
[[nodiscard]] PRECOOKED_INLINE auto files_in_directory_tree(const std::filesystem::path& dir)->std::vector<std::filesystem::path>;
[[nodiscard]] PRECOOKED_INLINE auto subdirs_in_directory_tree(const std::filesystem::path& dir)->std::vector<std::filesystem::path>;


// Convenience
template <typename Tpl, typename Func, size_t Idx = 0>
auto tuple_for_each(Tpl& tpl, Func&& func) -> void;
template <typename Tpl, typename Func, size_t Idx = 0>
[[nodiscard]] auto tuple_any_of(const Tpl& tpl, const Func& func) -> bool;
template <typename DstType, typename SrcType>
[[nodiscard]] auto cast(const SrcType& src_type)->DstType;

// Type names
template <typename T> [[nodiscard]] auto type_name()->std::string;
template <typename T> [[nodiscard]] auto type_name(T&&)->std::string;
template <typename T> [[nodiscard]] auto held_type_name(const T& value)->std::string;

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
		static_assert(std::is_arithmetic_v<value_t>);
		static_assert(!std::is_same_v<std::vector<bool>, std::decay_t<C>>);
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
	~scope_exit() { std::invoke(f_); }
private:
	F f_{};
};
}


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#include <exception>
#include <filesystem>


namespace prc::detail::exceptions {

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

namespace prc::detail::introspection {
	// See http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4502.pdf.
	template <typename...>
	using void_t = void;
	// Primary template handles all types not supporting the operation.
	template <typename, template <typename> class, typename = void_t<>>
	struct detect : std::false_type {};
	// Specialization recognizes/validates only types supporting the archetype.
	template <typename T, template <typename> class Op>
	struct detect<T, Op, void_t<Op<T>>> : std::true_type {};
}

namespace prc::detail {
	template <typename T>
	using is_container_t = decltype(std::declval<const T&>().begin(), std::declval<const T&>().end());

	template <typename T>
	using is_container = introspection::detect<T, is_container_t>;

	template <typename T>
	using has_ostream_t = decltype(std::declval<std::ostream&>() << std::declval<T>());

	template <typename T>
	using has_ostream = introspection::detect<T, has_ostream_t>;

	template <typename T>
	using is_tuple_t = decltype(std::tuple_size<T>::value);

	template <typename T>
	using is_tuple = introspection::detect<T, is_tuple_t>;


	template <typename T> constexpr auto is_smart_ptr(const std::unique_ptr<T>&) { return std::true_type{}; }
	template <typename T> constexpr auto is_smart_ptr(const std::shared_ptr<T>&) { return std::true_type{}; }
	template <typename T> constexpr auto is_smart_ptr(const T&) { return std::false_type{}; }

	template <typename ...Ts> constexpr auto is_variant(const std::variant<Ts...>&) { return std::true_type{}; }
	template <typename T> constexpr auto is_variant(const T&) { return std::false_type{}; }

	template <typename T> constexpr auto is_optional(const std::optional<T>&) { return std::true_type{}; }
	template <typename T> constexpr auto is_optional(const T&) { return std::false_type{}; }
}


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


#include <tuple>
#include <type_traits>

template<typename Tpl, typename Func, size_t Idx>
auto prc::tuple_for_each(Tpl& tpl, Func&& func) -> void {
	if constexpr (Idx < std::tuple_size_v<Tpl>) {
		std::invoke(func, std::get<Idx>(tpl));
		tuple_for_each<Tpl, Func, Idx + 1>(tpl, std::forward<Func>(func));
	}
}

template <typename Tpl, typename Func, size_t Idx>
auto prc::tuple_any_of(const Tpl& tpl, const Func& func) -> bool {
	if constexpr (Idx < std::tuple_size_v<Tpl>) {
		return std::invoke(func, std::get<Idx>(tpl)) ?
			true :
			tuple_any_of<Tpl, Func, Idx + 1>(tpl, func);
	}
	return false;
}

template <typename DstType, typename SrcType>
auto prc::cast(const SrcType& src)->DstType {
	const auto casted = static_cast<DstType>(src);
	const auto casted_back = static_cast<SrcType>(casted);
	if (casted_back != src) {
		throw detail::exceptions::bad_cast_exception{
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
	if constexpr (std::is_same_v<decltype(detail::is_variant(value)), std::true_type>) {
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
	static_assert(std::is_arithmetic_v<T>);
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

template <typename T>
auto prc::to_string(const T& value) -> std::string {
	using value_t = std::decay_t<T>;
	if constexpr (std::is_same_v<std::string, value_t>) {
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
	else if constexpr (std::is_same_v<decltype(detail::is_smart_ptr(value)), std::true_type>) {
		return value == nullptr ? 
			"nullptr" : 
			std::to_string(*value);
	}
	else if constexpr (std::is_same_v<decltype(detail::is_variant(value)), std::true_type>) {
		return std::visit([](auto&& candidate) {
			return to_string(candidate);
		}, value);
	}
	else if constexpr (std::is_same_v<decltype(detail::is_optional(value)), std::true_type>) {
		return value.has_value() ?
			to_string(*value) :
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
	else if constexpr (detail::has_ostream<value_t>::value) {
		auto sstr = std::ostringstream{};
		sstr << value;
		return sstr.str();
	}
	else if constexpr (detail::is_container<value_t>::value) {
		auto str = std::string{};
		str.reserve(value.size() * 4); // Guess
		str += '[';
		for (auto&& element : value) {
			str += to_string(element);
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
	else if constexpr(detail::is_tuple<value_t>::value){
		auto str = std::string{};
		str.reserve(std::tuple_size_v<value_t> * 4); // Guess
		str += '[';
		tuple_for_each(value, [&str](auto&& elem) {
			str += to_string(elem); 
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
	else {
		return "unknown";
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
) noexcept -> std::optional<size_t>;


template <typename Container>
[[nodiscard]] auto impl_read_file_to_container(const std::filesystem::path& filepath) -> Container {
	if (!std::filesystem::exists(filepath)) {
		throw detail::exceptions::file_not_found_exception(filepath);
	}
	if (!std::filesystem::is_regular_file(filepath)) {
		throw detail::exceptions::is_not_file_exception(filepath);
	}
	using value_t = typename Container::value_type;
	using byte_t = byte_view::byte_t;
	const auto file_size_uintmax = std::filesystem::file_size(filepath);
	const auto file_size_optional = detail::filesize_to_size_t(file_size_uintmax);
	if (!file_size_optional.has_value()) {
		throw detail::exceptions::file_too_large_exception(filepath, file_size_uintmax);
	}
	const auto file_size = *file_size_optional;
	const auto element_size = sizeof(value_t);
	if (!(file_size % element_size) == 0) {
		throw detail::exceptions::filesize_not_compatible_with_typesize_exception(filepath, file_size, element_size, type_name<value_t>());
	}
	auto file_stream = std::ifstream{ filepath, std::ios::binary };
	if (!file_stream.is_open()) {
		throw detail::exceptions::read_file_exception(filepath);
	}
	const auto scope_exit = detail::scope_exit{ [&file_stream]() {
		file_stream.close();
	} };
	auto data = Container{};
	const auto num_elements = file_size / element_size;
	data.resize(num_elements);
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
	static_assert(std::is_arithmetic_v<T>);
	return detail::impl_read_file_to_container<std::vector<T>>(filepath);
}







//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#include <numeric>

template <typename Range>
auto prc::join_strings(const Range& strings, const std::string_view delimiter)->std::string {
	if (strings.empty()) {
		return {};
	}
	const auto target_size = 
		std::accumulate(strings.begin(), strings.end(), size_t{ 0 }, [](size_t sum, auto&& str) {
			return sum + str.size();
		})
		+ (strings.size() - 1) * delimiter.size();
	// Merge strings
	auto joined = std::string{};
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

template <typename Range>
auto prc::join_strings(const Range& strings)->std::string {
	return join_strings(strings, std::string_view{});
}













































































//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


#include <algorithm>


auto prc::detail::filesize_to_size_t(
	uintmax_t filesize_uintmax
) noexcept ->std::optional<size_t>{
	const auto file_size = static_cast<size_t>(filesize_uintmax);
	return static_cast<uintmax_t>(file_size) == filesize_uintmax ?
		file_size :
		std::optional<size_t>{};
}



//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


namespace prc::detail {
template <typename PartType>
[[nodiscard]] auto impl_split_string(
	const std::string_view str,
	const std::string_view delimiters
) -> std::vector<PartType> {
	auto parts = std::vector<PartType>{};
	for (
		auto left = str.find_first_not_of(delimiters), right = size_t{ 0 };
		left != std::string_view::npos;
		left = str.find_first_not_of(delimiters, right)
	) {
		right = str.find_first_of(delimiters, left + 1);
		right = right == std::string::npos ? str.size() : right;
		PRECOOKED_ASSERT(right <= str.size());
		PRECOOKED_ASSERT(left <= right);
		parts.emplace_back(PartType{ str.data() + left, right - left });
	}
	return parts;
}
}



auto prc::split_string(
	const std::string_view str, 
	const std::string_view delimiters
) -> std::vector<std::string> {
	return detail::impl_split_string<std::string>(str, delimiters);
}

auto prc::split_string_to_sv(
	const std::string_view str, 
	const std::string_view delimiters
) -> std::vector<std::string_view> {
	return detail::impl_split_string<std::string_view>(str, delimiters);
}

auto prc::split_string_to_lines(
	const std::string_view str
) -> std::vector<std::string> {
	using namespace std::string_view_literals;
	return split_string(str, "\n\r"sv);
}

auto prc::trim_string(
	std::string str, 
	const std::string_view trim_chars
) noexcept -> std::string {
	if (str.empty() || trim_chars.empty()) {
		return str;
	}
	const auto last_idx = str.find_last_not_of(trim_chars);
	if (last_idx != std::string::npos) {
		str.erase(str.begin() + last_idx + 1, str.end());
	}
	const auto first_idx = str.find_first_not_of(trim_chars);
	if (first_idx != std::string::npos && first_idx != 0) {
		str.erase(str.begin(), str.begin() + first_idx);
	}
	return str;
}

auto prc::trim_string_to_sv(
	std::string_view str,
	const std::string_view trim_chars
) noexcept -> std::string_view {
	if (str.empty() || trim_chars.empty()) {
		return str;
	}
	const auto first_idx = str.find_first_not_of(trim_chars);
	if (first_idx != std::string_view::npos) {
		str = str.substr(first_idx);
	}
	const auto last_idx = str.find_last_not_of(trim_chars);
	if (last_idx != std::string_view::npos) {
		str = str.substr(0, last_idx + 1);
	}
	return str;
}


auto prc::is_trimmed(const std::string_view str, const std::string_view trim_chars) noexcept -> bool {
	return
		str.empty() ? true :
		trim_chars.find(str.front()) != std::string_view::npos ? false :
		trim_chars.find(str.back()) != std::string_view::npos ? false :
		true;
}











namespace prc::detail {


constexpr auto find_case_sensitive = [](const std::string_view& str, const std::string_view& key, const size_t offset) noexcept -> size_t {
	return str.find(key, offset);
};
constexpr auto find_ignore_case = [](const std::string_view& str, const std::string_view& key, const size_t offset) noexcept -> size_t {
	return prc::find_ignore_case(str, key, offset);
};

PRECOOKED_INLINE auto replace_string_part(
	std::string& io_string, 
	const size_t offset,
	const std::string_view& new_content
) noexcept -> void {
	PRECOOKED_ASSERT(offset + new_content.size() <= io_string.size());
	std::copy(new_content.begin(), new_content.end(), io_string.begin() + offset);
}


// This function replaces string of equal size
template <typename FindFunc>
[[nodiscard]] PRECOOKED_INLINE auto impl_replace_all_equal_key_length(
	std::string str, 
	const std::string_view key, 
	const std::string_view dst,
	const FindFunc& find_func
) noexcept -> std::string {
	PRECOOKED_ASSERT(key.size() == dst.size());
	for (
		auto idx = find_func(str, key, 0);
		idx != std::string::npos;
		idx = find_func(str, key, idx + key.size())
	) {
		replace_string_part(str, idx, dst);
	}
	return str;
}


// This function modifies the string in-place.
// Requires dst to be smaller than key.
template <typename FindFunc>
[[nodiscard]] PRECOOKED_INLINE auto impl_replace_all_shrink_string(
	std::string str, 
	const std::string_view key,
	const std::string_view dst,
	const FindFunc& find_func
) noexcept -> std::string {
	PRECOOKED_ASSERT(dst.size() < key.size());
	// Optimization to avoid copying the first part
	const auto first_key = find_func(str, key, 0);
	if (first_key == std::string::npos) {
		return str;
	}
	replace_string_part(str, first_key, dst);
	auto write_pos = first_key + dst.size();
	for (auto read_pos = first_key + key.size();;) {
		PRECOOKED_ASSERT(write_pos < read_pos);
		const auto key_pos = std::min(find_func(str, key, read_pos), str.size());
		while (read_pos < key_pos) {
			PRECOOKED_ASSERT(write_pos < str.size());
			PRECOOKED_ASSERT(read_pos < str.size());
			str[write_pos] = str[read_pos];
			++write_pos;
			++read_pos;
		}
		if (key_pos == str.size()) {
			break;
		}
		replace_string_part(str, write_pos, dst);
		read_pos += key.size();
		write_pos += dst.size();
	}
	PRECOOKED_ASSERT(write_pos < str.size());
	str.resize(write_pos);
	return str;
}

// This function builds a new string.
// Required if dst is larger then key or the source is a string_view.
template <typename FindFunc>
[[nodiscard]] PRECOOKED_INLINE auto impl_replace_all_rebuild_string(
	const std::string_view str, 
	const std::string_view& key, 
	const std::string_view& dst,
	const FindFunc& find_func
) -> std::string {
	auto ret = std::string{};
	ret.reserve(str.size());
	for (
		size_t left = 0, right = std::min(find_func(str, key, 0), str.size());
		;
		right = std::min(find_func(str, key, left), str.size())
	) {
		PRECOOKED_ASSERT(left <= right);
		ret += str.substr(left, right - left);
		if (right == str.size()) {
			return ret;
		}
		ret += dst;
		left = right + key.size();
	}
	return ret;
}

}


auto prc::replace_all(std::string str, const std::string_view key, const std::string_view dst) -> std::string {
	// Pick implementation based on key/dst size
	return
		(str.empty() || key.empty()) ? str :
		key.length() == dst.length() ? detail::impl_replace_all_equal_key_length(std::move(str), key, dst, detail::find_case_sensitive) :
		dst.length() < key.length() ? detail::impl_replace_all_shrink_string(std::move(str), key, dst, detail::find_case_sensitive) :
		detail::impl_replace_all_rebuild_string(str, key, dst, detail::find_case_sensitive);
}

auto prc::replace_all(const std::string_view str, const std::string_view key, const std::string_view dst) -> std::string {
	return (str.empty() || key.empty()) ?
		std::string{ str } :
		detail::impl_replace_all_rebuild_string(str, key, dst, detail::find_case_sensitive);
}

auto prc::replace_all(const char* str, const std::string_view key, const std::string_view dst) -> std::string {
	return replace_all(std::string_view{ str }, key, dst);
}

auto prc::replace_all_ignore_case(std::string str, const std::string_view key, const std::string_view dst) -> std::string {
	// Pick implementation based on key/dst size
	return
		(str.empty() || key.empty()) ? str :
		key.length() == dst.length() ? detail::impl_replace_all_equal_key_length(std::move(str), key, dst, detail::find_ignore_case) :
		dst.length() < key.length() ? detail::impl_replace_all_shrink_string(std::move(str), key, dst, detail::find_ignore_case) :
		detail::impl_replace_all_rebuild_string(str, key, dst, detail::find_ignore_case);
}

auto prc::replace_all_ignore_case(const std::string_view str, const std::string_view key, const std::string_view dst) -> std::string {
	return (str.empty() || key.empty()) ?
		std::string{ str } :
		detail::impl_replace_all_rebuild_string(str, key, dst, detail::find_ignore_case);
}

auto prc::replace_all_ignore_case(const char* str, const std::string_view key, const std::string_view dst) -> std::string {
	return replace_all_ignore_case(std::string_view{ str }, key, dst);
}










//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////







// String - remove
auto prc::remove_all(std::string str, std::string_view src) noexcept ->std::string {
	return str.empty() || src.empty() ?
		std::string{}:
		detail::impl_replace_all_shrink_string(std::move(str), src, std::string_view{}, detail::find_case_sensitive);
}
auto prc::remove_all(std::string_view str, std::string_view src)->std::string {
	return str.empty() || src.empty() ?
		std::string{} : 
		detail::impl_replace_all_rebuild_string(std::move(str), src, std::string_view{}, detail::find_case_sensitive);
}
auto prc::remove_all(const char* str, std::string_view src)->std::string {
	return remove_all(std::string_view(str), src);

}
auto prc::remove_all_ignore_case(std::string str, std::string_view src) noexcept->std::string {
	return str.empty() || src.empty() ?
		std::string{} : 
		detail::impl_replace_all_shrink_string(std::move(str), src, std::string_view{}, detail::find_ignore_case);
}
auto prc::remove_all_ignore_case(std::string_view str, std::string_view src)->std::string {
	return str.empty() || src.empty() ?
		std::string{} : 
		detail::impl_replace_all_rebuild_string(std::move(str), src, std::string_view{}, detail::find_ignore_case);
}
auto prc::remove_all_ignore_case(const char* str, std::string_view src)->std::string {
	return remove_all_ignore_case(std::string_view(str), src);
}











//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


#include <algorithm>
#include <cctype>

namespace prc::detail {
const auto is_chars_equal_ignore_case = [](unsigned char a, unsigned char b) noexcept {
	return a == b || std::tolower(a) == std::tolower(b);
};
[[nodiscard]] PRECOOKED_INLINE auto impl_is_equal_ignore_case(const std::string_view& a, const std::string_view& b) noexcept -> bool {
	PRECOOKED_ASSERT(a.size() == b.size());
	return std::equal(a.begin(), a.end(), b.begin(), detail::is_chars_equal_ignore_case);
}
}

auto prc::to_lower(std::string str) noexcept -> std::string {
	std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {
		return std::tolower(c);
	});
	return str;
}

auto prc::to_upper(std::string str) noexcept -> std::string {
	std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {
		return std::toupper(c);
	});
	return str;
}

auto prc::to_lower(std::string_view sv) -> std::string {
	auto str = std::string{};
	str.reserve(sv.size());
	std::transform(sv.begin(), sv.end(), std::back_inserter(str), [](unsigned char c) {
		return std::tolower(c);
	});
	return str;
}

auto prc::to_upper(std::string_view sv) -> std::string {
	auto str = std::string{};
	str.reserve(sv.size());
	std::transform(sv.begin(), sv.end(), std::back_inserter(str), [](unsigned char c) {
		return std::toupper(c);
	});
	return str;
}



auto prc::is_equal_ignore_case(const std::string_view a, const std::string_view b) noexcept -> bool {
	return
		a.size() == b.size() &&
		detail::impl_is_equal_ignore_case(a, b);
}

auto prc::contains_substring(const std::string_view haystack, const std::string_view needle) noexcept -> bool {
	return haystack.find(needle) != std::string::npos;
}

auto prc::contains_substring_ignore_case(const std::string_view haystack, const std::string_view needle) noexcept -> bool {
	return find_ignore_case(haystack, needle, 0) != std::string::npos;
}


[[nodiscard]] PRECOOKED_INLINE auto prc::find_ignore_case(
	const std::string_view haystack, 
	const std::string_view needle, 
	const size_t offset
) noexcept -> size_t {
	static_assert(std::string::npos == std::string_view::npos);
	if (haystack.size() < needle.size() || needle.empty()) {
		return std::string::npos;
	}
	const auto i_end = (1 + haystack.size()) - needle.size();
	if (offset > i_end) {
		return std::string::npos;
	}
	const auto equals_needle_front = [
		upper = std::toupper(needle.front()),
		lower = std::tolower(needle.front())
	](const unsigned char c) {
		return c == upper || c == lower;
	};

	const auto needle_tail = needle.substr(1);
	for (size_t i = offset; i < i_end; ++i) {
		PRECOOKED_ASSERT(i < haystack.size());
		if (!equals_needle_front(haystack[i])) {
			continue;
		}
		PRECOOKED_ASSERT(i + needle.size() <= haystack.size());
		const auto candidate_tail = haystack.substr(i + 1, needle.size() - 1);
		if (detail::impl_is_equal_ignore_case(candidate_tail, needle_tail)) {
			return i;
		}
	}
	return std::string::npos;
}



//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


#include <fstream>
#include <filesystem>




// Read files
auto prc::read_file_to_string(const std::filesystem::path& filepath) -> std::string {
	return detail::impl_read_file_to_container<std::string>(filepath);
}


// Write files
auto prc::write_bytevector_to_file(const detail::byte_view& byteview, const std::filesystem::path& filepath) -> void {
	if (filepath.has_parent_path()) {
		const auto dir = filepath.parent_path();
		if (!std::filesystem::exists(dir)) {
			std::filesystem::create_directories(dir);
		}
		if (!std::filesystem::exists(dir)) {
			throw detail::exceptions::dir_not_found_exception(dir);
		}
		if (!std::filesystem::is_directory(dir)) {
			throw detail::exceptions::is_not_directory_exception(dir);
		}
	}
	auto file_stream = std::ofstream{ filepath, std::ios::binary };
	if (!file_stream.is_open()) {
		throw detail::exceptions::write_file_exception(filepath);
	}
	const auto scope_exit = detail::scope_exit{ [&file_stream]() {
		file_stream.close();
	} };
	if (!byteview.empty()) {
		file_stream.write(byteview.data(), byteview.size());
	}
}

auto prc::write_string_to_file(const std::string_view& str, const std::filesystem::path& filepath) -> void {
	write_bytevector_to_file(str, filepath);
}


auto prc::is_vector_equal_to_file_content(
	const detail::byte_view& byteview, 
	const std::filesystem::path& filepath
) -> bool {
	if (!std::filesystem::exists(filepath)) {
		return false;
	}
	using byte_t = detail::byte_view::byte_t;
	const auto file_size_uintmax = std::filesystem::file_size(filepath);
	const auto file_size_optional = detail::filesize_to_size_t(file_size_uintmax);
	if (!file_size_optional.has_value()) {
		throw detail::exceptions::file_too_large_exception(filepath, file_size_uintmax);
	}
	const auto file_size = *file_size_optional;
	if (file_size != byteview.size()) {
		return false;
	}
	auto buffer = std::vector<detail::byte_view::byte_t>{};
	auto file_stream = std::ifstream{ filepath, std::ios::binary };
	if (!file_stream.is_open()) {
		throw detail::exceptions::read_file_exception(filepath);
	}
	const auto scope_exit = detail::scope_exit{ [&file_stream]() {
		file_stream.close();
	} };
	constexpr auto buffer_size = size_t{ 1024 };
	for (auto i = size_t{ 0 }; i < file_size; i += buffer_size) {
		PRECOOKED_ASSERT(i <= file_size);
		const auto bytes_left = file_size - i;
		const auto chunk_size = std::min(bytes_left, buffer_size);
		buffer.resize(std::max(chunk_size, buffer.size())); // Never shrink buffer
		file_stream.read(buffer.data(), chunk_size);
		PRECOOKED_ASSERT(i + chunk_size <= byteview.size());
		const auto is_equal = std::equal(
			buffer.begin(), 
			buffer.begin() + chunk_size,
			byteview.begin() + i,
			byteview.begin() + i + chunk_size
		);
		if (!is_equal) {
			return false;
		}
	}
	return true;
}

auto prc::is_string_equal_to_file_content(std::string_view str, const std::filesystem::path& filepath) -> bool {
	return is_vector_equal_to_file_content(str, filepath);
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
		throw detail::exceptions::dir_not_found_exception(dir);
	}
	if (!std::filesystem::is_directory(dir)) {
		throw detail::exceptions::is_not_directory_exception(dir);
	}
	return detail::impl_scan_flat(dir, [](auto&& p) { return std::filesystem::is_regular_file(p); });
}

auto prc::subdirs_in_directory(const std::filesystem::path& dir) -> std::vector<std::filesystem::path> {
	if (!std::filesystem::exists(dir)) {
		throw detail::exceptions::dir_not_found_exception(dir);
	}
	if (!std::filesystem::is_directory(dir)) {
		throw detail::exceptions::is_not_directory_exception(dir);
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
		throw detail::exceptions::dir_not_found_exception(dir);
	}
	if (!std::filesystem::is_directory(dir)) {
		throw detail::exceptions::is_not_directory_exception(dir);
	}
	return detail::impl_scan_tree(dir, [](auto&& p) { return std::filesystem::is_regular_file(p); });
}

auto prc::subdirs_in_directory_tree(const std::filesystem::path& dir) -> std::vector<std::filesystem::path> {
	if (!std::filesystem::exists(dir)) {
		throw detail::exceptions::dir_not_found_exception(dir);
	}
	if (!std::filesystem::is_directory(dir)) {
		throw detail::exceptions::is_not_directory_exception(dir);
	}
	return detail::impl_scan_tree(dir, [](auto&& p) { return std::filesystem::is_directory(p); });
}
