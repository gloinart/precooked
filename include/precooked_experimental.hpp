#include "precooked.hpp"

namespace peo {

[[nodiscard]] inline auto is_vector_equal_to_file_content(const detail::byte_view& bytevector, const std::filesystem::path& filepath) -> bool;
[[nodiscard]] inline auto is_string_equal_to_file_content(std::string_view str, const std::filesystem::path& filepath) -> bool;
template <typename Str0, typename Str1> [[nodiscard]] auto count_occurances(const Str0& haystack, const Str1& needle) noexcept -> size_t;
template <typename Str0, typename Str1> [[nodiscard]] auto count_occurances_ignore_case(const Str0& haystack, const Str1& needle, const std::locale& loc = std::locale{}) noexcept -> size_t;
}


namespace peo {
template <typename Str0, typename Str1>
[[nodiscard]] auto count_occurances(
	const Str0& haystack,
	const Str1& needle
) noexcept -> size_t {
	using Char = peo::type_traits::underlying_char_t<Str0>;
	const auto haystack_sv = std::basic_string_view<Char>{ haystack };
	const auto needle_sv = std::basic_string_view<Char>{ needle };
	if (
		haystack_sv.size() < needle_sv.size() ||
		needle_sv.empty()
	) {
		return 0;
	}
	return detail::impl_count_occurances(
		haystack_sv,
		needle_sv,
		size_t{ 0 },
		[](const auto& haystack, const auto& needle, const size_t offset) {
			return haystack.find(needle, offset);
		}
	);
}
template <typename Str0, typename Str1>
[[nodiscard]] auto count_occurances_ignore_case(
	const Str0& haystack,
	const Str1& needle,
	const std::locale& loc
) noexcept -> size_t {
	using Char = peo::type_traits::underlying_char_t<Str0>;
	const auto haystack_sv = std::basic_string_view<Char>{ haystack };
	const auto needle_sv = std::basic_string_view<Char>{ needle };
	if (
		haystack_sv.size() < needle_sv.size() ||
		needle_sv.empty()
	) {
		return 0;
	}
	return detail::impl_count_occurances(
		haystack_sv,
		needle_sv,
		size_t{ 0 },
		[&loc](const auto& haystack, const auto& needle, const size_t offset) {
			return peo::find_ignore_case(haystack, needle, offset, loc);
		}
	);
}
}























auto peo::is_vector_equal_to_file_content(
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
		throw peo::exceptions::file_too_large_exception(filepath, file_size_uintmax);
	}
	const auto file_size = *file_size_optional;
	if (file_size != byteview.size()) {
		return false;
	}
	auto buffer = std::vector<detail::byte_view::byte_t>{};
	auto file_stream = std::ifstream{ filepath, std::ios::binary };
	if (!file_stream.is_open()) {
		throw peo::exceptions::read_file_exception(filepath);
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

auto peo::is_string_equal_to_file_content(
	std::string_view str, 
	const std::filesystem::path& filepath
) -> bool {
	return is_vector_equal_to_file_content(str, filepath);
}
























