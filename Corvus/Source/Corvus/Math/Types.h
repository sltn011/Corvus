#ifndef CORVUS_SOURCE_CORVUS_MATH_TYPES_H
#define CORVUS_SOURCE_CORVUS_MATH_TYPES_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using FUByteVector2 = glm::u8vec2;
using FUByteVector3 = glm::u8vec3;
using FUByteVector4 = glm::u8vec4;

using FSByteVector2 = glm::i8vec2;
using FSByteVector3 = glm::i8vec3;
using FSByteVector4 = glm::i8vec4;

using FUWordVector2 = glm::u8vec2;
using FUWordVector3 = glm::u8vec3;
using FUWordVector4 = glm::u8vec4;

using FSWordVector2 = glm::i16vec2;
using FSWordVector3 = glm::i16vec3;
using FSWordVector4 = glm::i16vec4;

using FUIntVector2 = glm::uvec2;
using FUIntVector3 = glm::uvec3;
using FUIntVector4 = glm::uvec4;

using FSIntVector2 = glm::ivec2;
using FSIntVector3 = glm::ivec3;
using FSIntVector4 = glm::ivec4;

using FVector2 = glm::vec2;
using FVector3 = glm::vec3;
using FVector4 = glm::vec4;

using FMatrix2   = glm::mat2;
using FMatrix2_2 = FMatrix2;
using FMatrix2_3 = glm::mat2x3;
using FMatrix2_4 = glm::mat2x4;

using FMatrix3   = glm::mat3;
using FMatrix3_2 = glm::mat3x2;
using FMatrix3_3 = FMatrix3;
using FMatrix3_4 = glm::mat3x4;

using FMatrix4   = glm::mat4;
using FMatrix4_2 = glm::mat4x2;
using FMatrix4_3 = glm::mat4x3;
using FMatrix4_4 = FMatrix4;

#endif //! CORVUS_SOURCE_CORVUS_MATH_TYPES_H