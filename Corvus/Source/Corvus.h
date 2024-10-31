#ifndef CORVUS_SOURCE_CORVUS_H
#define CORVUS_SOURCE_CORVUS_H

// For use by Corvus Engine applications

// Core
#include "Corvus/Core/Application.h"
#include "Corvus/Core/Assert.h"
#include "Corvus/Core/Base.h"
#include "Corvus/Core/CoreTypes.h"
#include "Corvus/Core/Delegate.h"
#include "Corvus/Core/Layer.h"
#include "Corvus/Core/Log.h"

// Input
#include "Corvus/Input/Input.h"
#include "Corvus/Input/KeyCodes.h"

// Events
#include "Corvus/Event/ApplicationEvent.h"
#include "Corvus/Event/KeyboardEvent.h"
#include "Corvus/Event/MouseEvent.h"

// Camera
#include "Corvus/Camera/OrthographicCamera.h"
#include "Corvus/Camera/PerspectiveCamera.h"

// Scene
#include "Corvus/Scene/Entity.h"
#include "Corvus/Scene/Scene.h"

// Renderer
#include "Corvus/Renderer/Renderer.h"

// Math
#include "Corvus/Math/Math.h"
#include "Corvus/Math/Matrix.h"
#include "Corvus/Math/Quaternion.h"
#include "Corvus/Math/Vector.h"

// Time
#include "Corvus/Time/TimeDelta.h"
#include "Corvus/Time/TimePoint.h"

// Memory
#include "Corvus/Memory/ApplicationPools.h"
#include "Corvus/Memory/Pool.h"
#include "Corvus/Memory/Poolable.h"

// Entry Point
#include "Corvus/Core/EntryPoint.h"

#endif // !CORVUS_SOURCE_CORVUS_H
