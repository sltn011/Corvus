#ifndef CORVUS_SOURCE_CORVUS_MATH_TRANSFORM_H
#define CORVUS_SOURCE_CORVUS_MATH_TRANSFORM_H

#include "Corvus/Math/Matrix.h"
#include "Corvus/Math/Rotation.h"
#include "Corvus/Math/Vector.h"

namespace Corvus
{
    class Transform
    {
    public:

        Transform();
        Transform(glm::vec3 const &WorldPosition);
        Transform(glm::vec3 const &WorldPosition, glm::vec3 const &Scale);
        Transform(glm::vec3 const &WorldPosition, Rotation const &Rotation);
        Transform(glm::vec3 const &WorldPosition, glm::vec3 const &Scale, Rotation const &Rotation);

        glm::mat4 GetTransformMatrix();
        glm::mat4 GetTranslationMatrix() const;
        glm::mat4 GetScaleMatrix() const;
        glm::mat4 GetRotationMatrix();

        glm::vec3 GetWorldPosition() const;
        void SetWorldPosition(glm::vec3 const &WorldPosition);

        glm::vec3 GetScale() const;
        void SetScale(glm::vec3 const &Scale);

        Rotation GetRotation() const;
        void SetRotation(Rotation const &Rotation);

    private:

        glm::vec3 m_WorldPosition;
        glm::vec3 m_Scale;
        Rotation  m_Rotation;

    };
}

#endif //!CORVUS_SOURCE_CORVUS_MATH_TRANSFORM_H