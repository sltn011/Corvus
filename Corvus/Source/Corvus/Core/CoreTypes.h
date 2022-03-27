#ifndef CORVUS_SOURCE_CORVUS_CORE_CORETYPES_H
#define CORVUS_SOURCE_CORVUS_CORE_CORETYPES_H

#include <cstdint>
#include <string>
#include <string_view>

#include <glm/glm.hpp>

using Int8  = std::int8_t;
using Int16 = std::int16_t;
using Int32 = std::int32_t;
using Int64 = std::int64_t;

using UInt8  = std::uint8_t;
using UInt16 = std::uint16_t;
using UInt32 = std::uint32_t;
using UInt64 = std::uint64_t;

using Char   = char;
using String = std::basic_string<Char>;
using Name   = std::basic_string_view<Char>;

namespace Vector
{
    static constexpr glm::vec3 WorldForward = glm::vec3{ 1.0f, 0.0f, 0.0f };
    static constexpr glm::vec3 WorldUp      = glm::vec3{ 0.0f, 1.0f, 0.0f };
    static constexpr glm::vec3 WorldRight   = glm::vec3{ 0.0f, 0.0f, 1.0f };

    static constexpr glm::vec3 ZeroVec = glm::vec3{ 0.0f, 0.0f, 0.0f };
}

namespace Color
{
    static constexpr glm::vec3 White     = glm::vec3{ 1.0f, 1.0f, 1.0f };
    static constexpr glm::vec3 Gray      = glm::vec3{ 0.5f, 0.5f, 0.5f };
    static constexpr glm::vec3 Black     = glm::vec3{ 0.0f, 0.0f, 0.0f };

    static constexpr glm::vec3 Red       = glm::vec3{ 255, 0, 0 } / 255.0f;
    static constexpr glm::vec3 Green     = glm::vec3{ 0, 255, 0 } / 255.0f;
    static constexpr glm::vec3 Blue      = glm::vec3{ 0, 0, 255 } / 255.0f;

    static constexpr glm::vec3 Yellow    = glm::vec3{ 255, 255, 0 }   / 255.0f;
    static constexpr glm::vec3 Cyan      = glm::vec3{ 0, 255, 255 }   / 255.0f;
    static constexpr glm::vec3 Magenta   = glm::vec3{ 255, 0, 255 }   / 255.0f;
    static constexpr glm::vec3 Orange    = glm::vec3{ 243, 156, 18 }  / 255.0f;
    static constexpr glm::vec3 Purple    = glm::vec3{ 169, 7, 228 }   / 255.0f;
    static constexpr glm::vec3 Turquoise = glm::vec3{ 26, 188, 156 }  / 255.0f;
    static constexpr glm::vec3 Silver    = glm::vec3{ 189, 195, 199 } / 255.0f;
    static constexpr glm::vec3 Emerald   = glm::vec3{ 46, 204, 113 }  / 255.0f;
}

#endif //!CORVUS_SOURCE_CORVUS_CORE_CORETYPES_H