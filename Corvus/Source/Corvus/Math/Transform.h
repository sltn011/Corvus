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
        Transform(glm::vec3 const &Position);
        Transform(glm::vec3 const &Position, glm::vec3 const &Scale);
        Transform(glm::vec3 const &Position, Rotation const &Rotation);
        Transform(glm::vec3 const &Position, glm::vec3 const &Scale, Rotation const &Rotation);

        glm::mat4 GetTransformMatrix();
        glm::mat4 GetTranslationMatrix() const;
        glm::mat4 GetScaleMatrix() const;
        glm::mat4 GetRotationMatrix();

        glm::vec3 GetPosition() const;
        void SetPosition(glm::vec3 const &Position);

        glm::vec3 GetScale() const;
        void SetScale(glm::vec3 const &Scale);

        Rotation GetRotation() const;
        void SetRotation(Rotation const &Rotation);

    private:

        void RecalculateMatrix();

        glm::mat4 m_TransformMatrix = glm::mat4(1.0f);
        bool      m_bIsDirty = true;

        glm::vec3 m_Position;
        Rotation  m_Rotation;
        glm::vec3 m_Scale;

    };
}

#endif //!CORVUS_SOURCE_CORVUS_MATH_TRANSFORM_H