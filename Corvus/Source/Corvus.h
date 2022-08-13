#ifndef CORVUS_SOURCE_CORVUS_H
#define CORVUS_SOURCE_CORVUS_H

// For use by Corvus Engine applications

// Core
#include "Corvus/Core/Base.h"
#include "Corvus/Core/Application.h"
#include "Corvus/Core/CoreTypes.h"
#include "Corvus/Core/KeyCodes.h"
#include "Corvus/Core/Input.h"
#include "Corvus/Core/Log.h"
#include "Corvus/Core/Assert.h"
#include "Corvus/Core/Delegate.h"
#include "Corvus/Core/Layer.h"

// Events
#include "Corvus/Events/ApplicationEvent.h"
#include "Corvus/Events/KeyboardEvent.h"
#include "Corvus/Events/MouseEvent.h"

// Camera
#include "Corvus/Camera/OrthographicCamera.h"
#include "Corvus/Camera/PerspectiveCamera.h"

// Scene
#include "Corvus/Scene/Entity.h"
#include "Corvus/Scene/Scene.h"

// Renderer
#include "Corvus/Renderer/IndexBuffer.h"
#include "Corvus/Renderer/Renderer.h"
#include "Corvus/Renderer/Shader.h"
#include "Corvus/Renderer/VertexArray.h"
#include "Corvus/Renderer/VertexBuffer.h"

// Math
#include "Corvus/Math/Math.h"
#include "Corvus/Math/Matrix.h"
#include "Corvus/Math/Quat.h"
#include "Corvus/Math/Vector.h"

// Time
#include "Corvus/Time/TimeDelta.h"
#include "Corvus/Time/TimePoint.h"

// Memory
#include "Corvus/Memory/AppPools.h"
#include "Corvus/Memory/Poolable.h"

// Entry Point
#include "Corvus/Core/EntryPoint.h"

#endif // !CORVUS_SOURCE_CORVUS_H

