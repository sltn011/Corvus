#ifndef CORVUS_SOURCE_CORVUS_CORE_CORETYPES_H
#define CORVUS_SOURCE_CORVUS_CORE_CORETYPES_H

#include <cstdint>
#include <string>
#include <string_view>

using Int8  = std::int8_t;
using Int16 = std::int16_t;
using Int32 = std::int32_t;
using Int64 = std::int64_t;

using UInt8  = std::uint8_t;
using UInt16 = std::uint16_t;
using UInt32 = std::uint32_t;
using UInt64 = std::uint64_t;

using Char   = wchar_t;
using String = std::basic_string<Char>;
using Name   = std::basic_string_view<Char>;
using Label  = std::basic_string<char>; // Default std::string

#endif //!CORVUS_SOURCE_CORVUS_CORE_CORETYPES_H