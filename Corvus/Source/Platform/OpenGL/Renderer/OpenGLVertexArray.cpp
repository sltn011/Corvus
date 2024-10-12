#include "CorvusPCH.h"

#include "Platform/OpenGL/Renderer/OpenGLVertexArray.h"

#include "Platform/OpenGL/Renderer/OpenGLIndexBuffer.h"
#include "Platform/OpenGL/Renderer/OpenGLVertexBuffer.h"

namespace Corvus
{
    POpenGLVertexArray::POpenGLVertexArray()
    {
        glCreateVertexArrays(1, &m_VAO);
    }

    POpenGLVertexArray::~POpenGLVertexArray()
    {
        glDeleteVertexArrays(1, &m_VAO);
    }

    POpenGLVertexArray::POpenGLVertexArray(POpenGLVertexArray &&Rhs) noexcept
        : Super{std::move(Rhs)}, m_VAO{std::exchange(Rhs.m_VAO, 0)}
    {
        m_IndexBuffer   = std::move(Rhs.m_IndexBuffer);
        m_VertexBuffers = std::move(Rhs.m_VertexBuffers);
    }

    POpenGLVertexArray &POpenGLVertexArray::operator=(POpenGLVertexArray &&Rhs) noexcept
    {
        if (this != &Rhs)
        {
            Super::operator=(std::move(Rhs));
            std::swap(m_VAO, Rhs.m_VAO);
            m_IndexBuffer   = std::move(Rhs.m_IndexBuffer);
            m_VertexBuffers = std::move(Rhs.m_VertexBuffers);
        }
        return *this;
    }

    void POpenGLVertexArray::Bind()
    {
        glBindVertexArray(m_VAO);
    }

    void POpenGLVertexArray::Unbind()
    {
        glBindVertexArray(0);
    }

    void POpenGLVertexArray::AddIndexBuffer(TOwn<CIndexBuffer> &&IndexBuffer)
    {
        CORVUS_CORE_ASSERT(IndexBuffer);

        m_IndexBuffer = std::move(IndexBuffer);

        GLuint const EBO = ((POpenGLIndexBuffer *)m_IndexBuffer.get())->GetID();
        glVertexArrayElementBuffer(m_VAO, EBO);
    }

    SizeT POpenGLVertexArray::AddVertexBuffer(TOwn<CVertexBuffer> &&VertexBuffer)
    {
        SizeT BufferIndex = m_VertexBuffers.size();
        AddVertexBuffer(std::move(VertexBuffer), BufferIndex);
        return BufferIndex;
    }

    void POpenGLVertexArray::AddVertexBuffer(TOwn<CVertexBuffer> &&VertexBuffer, SizeT BufferIndex)
    {
        CORVUS_CORE_ASSERT(VertexBuffer != nullptr);
        CORVUS_CORE_ASSERT(BufferIndex <= m_VertexBuffers.size());

        if (BufferIndex == m_VertexBuffers.size())
        {
            m_VertexBuffers.emplace_back(std::move(VertexBuffer));
        }
        else
        {
            m_VertexBuffers[BufferIndex] = std::move(VertexBuffer);
        }
        EnableVertexAttributes(BufferIndex);
    }

    void POpenGLVertexArray::EnableVertexAttributes(SizeT const BufferIndex)
    {
        CORVUS_CORE_ASSERT(m_VertexBuffers.size() > 0);
        CORVUS_CORE_ASSERT(BufferIndex < m_VertexBuffers.size());

        GLuint BindingPoint    = 0;
        GLuint VertexAttribute = 0;
        for (SizeT i = 0; i < m_VertexBuffers.size(); ++i)
        {
            TOwn<CVertexBuffer> &VertexBuffer = m_VertexBuffers[i];
            CVertexBufferLayout &Layout       = VertexBuffer->GetLayout();
            GLsizei const        Stride       = static_cast<GLsizei>(Layout.Stride());
            GLuint const         VBO          = ((POpenGLVertexBuffer *)VertexBuffer.get())->GetID();

            BindingPoint = static_cast<GLuint>(i);
            glVertexArrayVertexBuffer(m_VAO, BindingPoint, VBO, 0, Stride);

            GLuint Offset = 0;
            for (UInt32 j = 0; j < Layout.Size(); ++j)
            {
                CBufferLayoutElement &Element = Layout[j];

                GLint const     NumComponents = static_cast<GLint>(Element.GetNumComponents());
                GLenum const    Type = POpenGLVertexBuffer::BufferLayoutTypeToGLType(Element.GetType());
                GLboolean const bShouldNormalize = Element.ShouldNormalize() ? GL_TRUE : GL_FALSE;

                glEnableVertexArrayAttrib(m_VAO, VertexAttribute);
                glVertexArrayAttribBinding(m_VAO, VertexAttribute, BindingPoint);
                glVertexArrayAttribFormat(
                    m_VAO, VertexAttribute, NumComponents, Type, bShouldNormalize, Offset
                );

                VertexAttribute++;
                Offset += static_cast<GLuint>(Element.GetSize());
            }
        }
    }

} // namespace Corvus