#ifndef CORVUS_SOURCE_CORVUS_RENDERER_CORESHADERS_RENDERSCREENQUADSHADER_H
#define CORVUS_SOURCE_CORVUS_RENDERER_CORESHADERS_RENDERSCREENQUADSHADER_H

namespace Corvus::CoreShaders
{
    // clang-format off
    static constexpr char RenderScreenQuadVertexShader[] =
    R"(
        #version 460 core

        layout (location = 0) in vec2 inPos;
        layout (location = 1) in vec2 inTex;
        
        out vec2 TextureCoord;
        
        void main()
        {
        	TextureCoord = inTex;
        	gl_Position = vec4(inPos, 0.0f, 1.0f);
        }
    )";
    // clang-format on

    // clang-format off
    static constexpr char RenderScreenQuadFragmentShader[] =
    R"(
        #version 460 core

        uniform sampler2D u_ScreenQuadTexture;

        in vec2 TextureCoord;
        out vec4 FragColor;
        
        void main()
        {
        	FragColor = texture(u_ScreenQuadTexture, TextureCoord);
        }
    )";
    // clang-format on

} // namespace Corvus::CoreShaders

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_CORESHADERS_RENDERSCREENQUADSHADER_H
