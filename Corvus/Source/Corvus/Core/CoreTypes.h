#ifndef CORVUS_SOURCE_CORVUS_CORE_CORETYPES_H
#define CORVUS_SOURCE_CORVUS_CORE_CORETYPES_H

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <type_traits>

using Int8  = std::int8_t;
using Int16 = std::int16_t;
using Int32 = std::int32_t;
using Int64 = std::int64_t;

using UInt8  = std::uint8_t;
using UInt16 = std::uint16_t;
using UInt32 = std::uint32_t;
using UInt64 = std::uint64_t;

using SizeT    = std::size_t;
using PtrDiffT = std::ptrdiff_t;

using Char    = char;
using CString = std::basic_string<Char>;
using CName   = std::basic_string_view<Char>;

#endif //! CORVUS_SOURCE_CORVUS_CORE_CORETYPES_H