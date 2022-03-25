#include "CorvusPCH.h"
#include "Corvus/Renderer/VertexArrayBase.h"

#include "Corvus/Renderer/IndexBufferBase.h"
#include "Corvus/Renderer/VertexBufferBase.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Corvus
{

    Own<VertexArrayBase> VertexArrayBase::Create()
    {
        return MakeOwned<OpenGLVertexArray>();
    }

    VertexArrayBase::~VertexArrayBase()
    {
    }

}
