#include "CorvusPCH.h"

#include "Corvus/Math/Quaternion.h"

#include <glm/gtx/euler_angles.hpp>

namespace Corvus
{

    FQuaternion FQuaternion::FromEuler(FVector3 const &Degrees, ERotationOrder const Order)
    {
        switch (Order)
        {
        case Corvus::ERotationOrder::XYZ:
            return FromEulerXYZ(Degrees);

        case Corvus::ERotationOrder::XZY:
            return FromEulerXZY(Degrees);

        case Corvus::ERotationOrder::YXZ:
            return FromEulerYXZ(Degrees);

        case Corvus::ERotationOrder::YZX:
            return FromEulerYZX(Degrees);

        case Corvus::ERotationOrder::ZXY:
            return FromEulerZXY(Degrees);

        case Corvus::ERotationOrder::ZYX:
            return FromEulerZYX(Degrees);

        default:
            return FQuaternion{1.0f, 0.0f, 0.0f, 0.0f};
        }
    }

    FQuaternion FQuaternion::FromEulerXYZ(FVector3 const &Degrees)
    {
        FVector3    C = FVector::Cos(Degrees / 2.0f);
        FVector3    S = FVector::Sin(Degrees / 2.0f);
        FQuaternion R{};
        R.w = C.x * C.y * C.z - S.x * S.y * S.z;
        R.x = C.x * S.y * S.z + S.x * C.y * C.z;
        R.y = C.x * S.y * C.z - S.x * C.y * S.z;
        R.z = C.x * C.y * S.z + S.x * S.y * C.z;
        return R;
    }

    FQuaternion FQuaternion::FromEulerXZY(FVector3 const &Degrees)
    {
        FVector3    C = FVector::Cos(Degrees / 2.0f);
        FVector3    S = FVector::Sin(Degrees / 2.0f);
        FQuaternion R{};
        R.w = C.x * C.z * C.y + S.x * S.z * S.y;
        R.x = S.x * C.z * C.y - C.x * S.z * S.y;
        R.y = C.x * C.z * S.y - S.x * S.z * C.y;
        R.z = C.x * S.z * C.y + S.x * C.z * S.y;
        return R;
    }

    FQuaternion FQuaternion::FromEulerYXZ(FVector3 const &Degrees)
    {
        FVector3    C = FVector::Cos(Degrees / 2.0f);
        FVector3    S = FVector::Sin(Degrees / 2.0f);
        FQuaternion R{};
        R.w = C.y * C.x * C.z + S.y * S.x * S.z;
        R.x = C.y * S.x * C.z + S.y * C.x * S.z;
        R.y = S.y * C.x * C.z - C.y * S.x * S.z;
        R.z = C.y * C.x * S.z - S.y * S.x * C.z;
        return R;
    }

    FQuaternion FQuaternion::FromEulerYZX(FVector3 const &Degrees)
    {
        FVector3    C = FVector::Cos(Degrees / 2.0f);
        FVector3    S = FVector::Sin(Degrees / 2.0f);
        FQuaternion R{};
        R.w = C.y * C.z * C.x - S.y * S.z * S.x;
        R.x = C.y * C.z * S.x + S.y * S.z * C.x;
        R.y = S.y * C.z * C.x + C.y * S.z * S.x;
        R.z = C.y * S.z * C.x - S.y * C.z * S.x;
        return R;
    }

    FQuaternion FQuaternion::FromEulerZXY(FVector3 const &Degrees)
    {
        FVector3    C = FVector::Cos(Degrees / 2.0f);
        FVector3    S = FVector::Sin(Degrees / 2.0f);
        FQuaternion R{};
        R.w = C.z * C.x * C.y - S.z * S.x * S.y;
        R.x = C.z * S.x * C.y - S.z * C.x * S.y;
        R.y = S.z * S.x * C.y + C.z * C.x * S.y;
        R.z = S.z * C.x * C.y + C.z * S.x * S.y;
        return R;
    }

    FQuaternion FQuaternion::FromEulerZYX(FVector3 const &Degrees)
    {
        FVector3    C = FVector::Cos(Degrees / 2.0f);
        FVector3    S = FVector::Sin(Degrees / 2.0f);
        FQuaternion R{};
        R.w = C.z * C.y * C.x + S.z * S.y * S.x;
        R.x = C.z * C.y * S.x - S.z * S.y * C.x;
        R.y = C.z * S.y * C.x + S.z * C.y * S.x;
        R.z = S.z * C.y * C.x - C.z * S.y * S.x;
        return R;
    }

    FVector3 FQuaternion::ToEuler(FQuaternion const &Quaternion, ERotationOrder Order)
    {
        switch (Order)
        {
        case Corvus::ERotationOrder::XYZ:
            return ToEulerXYZ(Quaternion);

        case Corvus::ERotationOrder::XZY:
            return ToEulerXZY(Quaternion);

        case Corvus::ERotationOrder::YXZ:
            return ToEulerYXZ(Quaternion);

        case Corvus::ERotationOrder::YZX:
            return ToEulerYZX(Quaternion);

        case Corvus::ERotationOrder::ZXY:
            return ToEulerZXY(Quaternion);

        case Corvus::ERotationOrder::ZYX:
            return ToEulerZYX(Quaternion);

        default:
            return FVector3{};
        }
    }

    FVector3 FQuaternion::ToEulerXYZ(FQuaternion const &Quaternion)
    {
        FVector3 V{};
        FMatrix4 Mat4 = FQuaternion::ToMat4(Quaternion);
        glm::extractEulerAngleXYZ(Mat4, V.x, V.y, V.z);
        V = FVector::Degrees(V);
        return V;
    }

    FVector3 FQuaternion::ToEulerXZY(FQuaternion const &Quaternion)
    {
        FVector3 V{};
        FMatrix4 Mat4 = FQuaternion::ToMat4(Quaternion);
        glm::extractEulerAngleXZY(Mat4, V.x, V.z, V.y);
        V = FVector::Degrees(V);
        return V;
    }

    FVector3 FQuaternion::ToEulerYXZ(FQuaternion const &Quaternion)
    {
        FVector3 V{};
        FMatrix4 Mat4 = FQuaternion::ToMat4(Quaternion);
        glm::extractEulerAngleYXZ(Mat4, V.y, V.x, V.z);
        V = FVector::Degrees(V);
        return V;
    }

    FVector3 FQuaternion::ToEulerYZX(FQuaternion const &Quaternion)
    {
        FVector3 V{};
        FMatrix4 Mat4 = FQuaternion::ToMat4(Quaternion);
        glm::extractEulerAngleYZX(Mat4, V.y, V.z, V.x);
        V = FVector::Degrees(V);
        return V;
    }

    FVector3 FQuaternion::ToEulerZXY(FQuaternion const &Quaternion)
    {
        FVector3 V{};
        FMatrix4 Mat4 = FQuaternion::ToMat4(Quaternion);
        glm::extractEulerAngleZXY(Mat4, V.z, V.x, V.y);
        V = FVector::Degrees(V);
        return V;
    }

    FVector3 FQuaternion::ToEulerZYX(FQuaternion const &Quaternion)
    {
        FVector3 V{};
        FMatrix4 Mat4 = FQuaternion::ToMat4(Quaternion);
        glm::extractEulerAngleZYX(Mat4, V.z, V.y, V.x);
        V = FVector::Degrees(V);
        return V;
    }

} // namespace Corvus