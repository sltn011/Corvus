#include "CorvusPCH.h"

#include "Corvus/Assets/Default/DefaultTextures.h"

#include "Corvus/Assets/Texture/ImageData.h"
#include "Corvus/Assets/Texture/ImageDataLoader.h"
#include "Corvus/Core/Application.h"
#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{
    void CDefaultTextures::Create()
    {
        // Default Albedo
        {
            FUByteVector4 P = {255, 20, 147, 255};
            FUByteVector4 B = {10, 10, 10, 255};

            SizeT Dimension = 16;

            std::vector<FUByteVector4> TextureData(Dimension * Dimension);
            for (SizeT i = 0; i < Dimension; ++i)
            {
                for (SizeT j = 0; j < Dimension; ++j)
                {
                    if (i % 2 == 0)
                    {
                        TextureData[i * Dimension + j] = j % 2 == 0 ? P : B;
                    }
                    else
                    {
                        TextureData[i * Dimension + j] = j % 2 == 0 ? B : P;
                    }
                }
            }

            CImageData DefaultAlbedoData = CImageDataLoader::LoadFromMemory(
                TextureData.data(), Dimension, Dimension, VK_FORMAT_R8G8B8A8_UNORM, true
            );

            Albedo = Renderer().CreateTexture2D(DefaultAlbedoData, 1, Renderer().Samplers.PixelatedSampler);

            CApplication::GetInstance().AssetDrawer.Textures2D[Albedo.UUID] = Albedo;
        }

        // Default Normal
        {
            FUByteVector4 N = {0, 0, 255, 255};

            // clang-format off
            std::vector<FUByteVector4> TextureData = {
                N, N, N, N,
                N, N, N, N,
                N, N, N, N,
                N, N, N, N,
            };
            // clang-format on

            CImageData DefaultNormalData =
                CImageDataLoader::LoadFromMemory(TextureData.data(), 4, 4, VK_FORMAT_R8G8B8A8_UNORM, true);

            Normal = Renderer().CreateTexture2D(DefaultNormalData, 1, Renderer().Samplers.FilteredSampler);

            CApplication::GetInstance().AssetDrawer.Textures2D[Normal.UUID] = Normal;
        }
    }

} // namespace Corvus
