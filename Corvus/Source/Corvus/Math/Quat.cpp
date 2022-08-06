#include "CorvusPCH.h"
#include "Corvus/Math/Quat.h"

namespace Corvus::Quaternion
{

    Quat FromEuler(Vec3 const &Degrees, RotationOrder Order)
    {
        switch (Order)
        {
        case Corvus::RotationOrder::XYZ:
            return FromEulerXYZ(Degrees);

        case Corvus::RotationOrder::XZY:
            return FromEulerXZY(Degrees);

        case Corvus::RotationOrder::YXZ:
            return FromEulerYXZ(Degrees);

        case Corvus::RotationOrder::YZX:
            return FromEulerYZX(Degrees);

        case Corvus::RotationOrder::ZXY:
            return FromEulerZXY(Degrees);

        case Corvus::RotationOrder::ZYX:
            return FromEulerZYX(Degrees);

        default:
            return Quat{1.0f, 0.0f, 0.0f, 0.0f};

        }
    }

    Quat FromEulerXYZ(Vec3 const &Degrees)
    {
        Vec3 C = Vector::Cos(Degrees / 2.0f);
        Vec3 S = Vector::Sin(Degrees / 2.0f);
        Quat R{};
        R.w = C.x * C.y * C.z - S.x * S.y * S.z;
        R.x = C.x * S.y * S.z + S.x * C.y * C.z;
        R.y = C.x * S.y * C.z - S.x * C.y * S.z;
        R.z = C.x * C.y * S.z + S.x * S.y * C.z;
        return R;
    }

    Quat FromEulerXZY(Vec3 const &Degrees)
    {
        Vec3 C = Vector::Cos(Degrees / 2.0f);
        Vec3 S = Vector::Sin(Degrees / 2.0f);
        Quat R{};
        R.w = C.x * C.z * C.y + S.x * S.z * S.y;
        R.x = S.x * C.z * C.y - C.x * S.z * S.y;
        R.y = C.x * C.z * S.y - S.x * S.z * C.y;
        R.z = C.x * S.z * C.y + S.x * C.z * S.y;
        return R;
    }

    Quat FromEulerYXZ(Vec3 const &Degrees)
    {
        Vec3 C = Vector::Cos(Degrees / 2.0f);
        Vec3 S = Vector::Sin(Degrees / 2.0f);
        Quat R{};
        R.w = C.y * C.x * C.z + S.y * S.x * S.z;
        R.x = C.y * S.x * C.z + S.y * C.x * S.z;
        R.y = S.y * C.x * C.z - C.y * S.x * S.z;
        R.z = C.y * C.x * S.z - S.y * S.x * C.z;
        return R;
    }

    Quat FromEulerYZX(Vec3 const &Degrees)
    {
        Vec3 C = Vector::Cos(Degrees / 2.0f);
        Vec3 S = Vector::Sin(Degrees / 2.0f);
        Quat R{};
        R.w = C.y * C.z * C.x - S.y * S.z * S.x;
        R.x = C.y * C.z * S.x + S.y * S.z * C.x;
        R.y = S.y * C.z * C.x + C.y * S.z * S.x;
        R.z = C.y * S.z * C.x - S.y * C.z * S.x;
        return R;
    }

    Quat FromEulerZXY(Vec3 const &Degrees)
    {
        Vec3 C = Vector::Cos(Degrees / 2.0f);
        Vec3 S = Vector::Sin(Degrees / 2.0f);
        Quat R{};
        R.w = C.z * C.x * C.y - S.z * S.x * S.y;
        R.x = C.z * S.x * C.y - S.z * C.x * S.y;
        R.y = S.z * S.x * C.y + C.z * C.x * S.y;
        R.z = S.z * C.x * C.y + C.z * S.x * S.y;
        return R;
    }

    Quat FromEulerZYX(Vec3 const &Degrees)
    {
        Vec3 C = Vector::Cos(Degrees / 2.0f);
        Vec3 S = Vector::Sin(Degrees / 2.0f);
        Quat R{};
        R.w = C.z * C.y * C.x + S.z * S.y * S.x;
        R.x = C.z * C.y * S.x - S.z * S.y * C.x;
        R.y = C.z * S.y * C.x + S.z * C.y * S.x;
        R.z = S.z * C.y * C.x - C.z * S.y * S.x;
        return R;
    }

}