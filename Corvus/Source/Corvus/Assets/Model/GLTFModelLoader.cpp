#include "CorvusPCH.h"

#include "Corvus/Assets/Model/GLTFModelLoader.h"

#include "Corvus/Assets/Model/StaticMesh.h"
#include "Corvus/Assets/Model/StaticMeshPrimitive.h"
#include "Corvus/Assets/Model/StaticModel.h"
#include "Corvus/Assets/Texture/ImageData.h"
#include "Corvus/Assets/Texture/ImageDataLoader.h"
#include "Corvus/Assets/Texture/Texture2D.h"
#include "Corvus/Math/Matrix.h"
#include "Corvus/Math/Quaternion.h"
#include "Corvus/Renderer/IndexBuffer.h"
#include "Corvus/Renderer/Shader.h"
#include "Corvus/Renderer/Texture2DBuffer.h"
#include "Corvus/Renderer/VertexArray.h"
#include "Corvus/Renderer/VertexBuffer.h"

#define TINYGLTF_IMPLEMENTATION
#define TINYGLTF_NO_STB_IMAGE_WRITE
#include <tiny_gltf.h>

namespace Corvus
{
    namespace
    {
        struct SElementFormat
        {
            SizeT NumComponents = 0;
            SizeT ComponentSize = 0;
            bool  bIsInteger    = false;
        };

        struct SBufferReadParams
        {
            UInt8 const *SrcData   = nullptr;
            SizeT        SrcStride = 0;

            UInt8 *DstData   = nullptr;
            SizeT  DstStride = 0;

            SElementFormat ElementFormat;
            SizeT          NumElements = 0;
        };

        std::vector<SizeT> GetRootNodesIndexes(tinygltf::Model const &GLTFModel)
        {
            tinygltf::Scene const &Scene = GLTFModel.scenes[0];

            std::vector<SizeT> RootNodesIndexes(Scene.nodes.size());
            SizeT              Cnt = 0;
            for (Int32 const Index : Scene.nodes)
            {
                RootNodesIndexes[Cnt++] = static_cast<SizeT>(Index);
            }
            return RootNodesIndexes;
        }

        std::vector<SizeT> GetChildNodesIndexes(tinygltf::Node const &Node)
        {
            std::vector<SizeT> RootNodesIndexes(Node.children.size());
            SizeT              Cnt = 0;
            for (Int32 const Index : Node.children)
            {
                RootNodesIndexes[Cnt++] = static_cast<SizeT>(Index);
            }
            return RootNodesIndexes;
        }

        SElementFormat GetElementFormatFromAccessor(tinygltf::Accessor const &Accessor)
        {
            SizeT          DataType      = Accessor.type;
            SizeT          ComponentType = Accessor.componentType;
            SElementFormat ElementFormat;

            switch (DataType)
            {
            case TINYGLTF_TYPE_SCALAR:
                ElementFormat.NumComponents = 1;
                break;
            case TINYGLTF_TYPE_VEC2:
                ElementFormat.NumComponents = 2;
                break;
            case TINYGLTF_TYPE_VEC3:
                ElementFormat.NumComponents = 3;
                break;
            default:
                CORVUS_ERROR("Unsupported data type in gltf model!");
            }

            switch (ComponentType)
            {
            case TINYGLTF_COMPONENT_TYPE_BYTE:
            case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
                ElementFormat.ComponentSize = 1;
                ElementFormat.bIsInteger    = true;
                break;
            case TINYGLTF_COMPONENT_TYPE_SHORT:
            case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
                ElementFormat.ComponentSize = 2;
                ElementFormat.bIsInteger    = true;
                break;
            case TINYGLTF_COMPONENT_TYPE_INT:
            case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
                ElementFormat.ComponentSize = 4;
                ElementFormat.bIsInteger    = true;
                break;
            case TINYGLTF_COMPONENT_TYPE_FLOAT:
                ElementFormat.ComponentSize = 4;
                ElementFormat.bIsInteger    = false;
                break;
            default:
                CORVUS_ERROR("Unsupported data component type in gltf model!");
            }

            return ElementFormat;
        }

        STextureParameters ProcessNoTextureSampler()
        {
            STextureParameters TextureParameters;
            TextureParameters.MinFiltering = ETextureFiltering::LinearMipMap_Linear;
            TextureParameters.MagFiltering = ETextureFiltering::Linear;
            TextureParameters.WrappingS    = ETextureWrapping::Repeat;
            TextureParameters.WrappingT    = ETextureWrapping::Repeat;
            TextureParameters.WrappingR    = ETextureWrapping::Repeat;

            TextureParameters.bHasMipmaps              = true;
            TextureParameters.bHasAnisotropicFiltering = true;
            return TextureParameters;
        }

        STextureParameters ProcessTextureSampler(tinygltf::Sampler const &Sampler)
        {
            static const std::unordered_map<int, ETextureFiltering> TextureFiltering{
                // clang-format off
                {-1,                                             ETextureFiltering::Linear},
                {TINYGLTF_TEXTURE_FILTER_NEAREST,                ETextureFiltering::Nearest},
                {TINYGLTF_TEXTURE_FILTER_LINEAR,                 ETextureFiltering::Linear},
                {TINYGLTF_TEXTURE_FILTER_NEAREST_MIPMAP_NEAREST, ETextureFiltering::NearestMipMap_Nearest},
                {TINYGLTF_TEXTURE_FILTER_NEAREST_MIPMAP_LINEAR,  ETextureFiltering::LinearMipMap_Nearest},
                {TINYGLTF_TEXTURE_FILTER_LINEAR_MIPMAP_NEAREST,  ETextureFiltering::NearestMipMap_Linear},
                {TINYGLTF_TEXTURE_FILTER_LINEAR_MIPMAP_LINEAR,   ETextureFiltering::LinearMipMap_Linear}
                // clang-format on
            };

            static const std::unordered_map<int, ETextureWrapping> TextureWrapping{
                // clang-format off
                {TINYGLTF_TEXTURE_WRAP_CLAMP_TO_EDGE,   ETextureWrapping::ClampToEdge},
                {TINYGLTF_TEXTURE_WRAP_MIRRORED_REPEAT, ETextureWrapping::MirrorRepeat},
                {TINYGLTF_TEXTURE_WRAP_REPEAT,          ETextureWrapping::Repeat}
                // No value for ETextureWrapping::Border
                // clang-format on
            };

            STextureParameters TextureParameters;
            TextureParameters.MinFiltering = TextureFiltering.at(Sampler.minFilter);
            TextureParameters.MagFiltering = TextureFiltering.at(Sampler.magFilter);
            TextureParameters.WrappingS    = TextureWrapping.at(Sampler.wrapS);
            TextureParameters.WrappingT    = TextureWrapping.at(Sampler.wrapT);
            TextureParameters.WrappingR    = ETextureWrapping::Repeat; // no value in gltf

            TextureParameters.bHasMipmaps              = true;
            TextureParameters.bHasAnisotropicFiltering = true;
            return TextureParameters;
        }

        CImageData ProcessImage(tinygltf::Image const &Image)
        {
            CORVUS_CORE_ASSERT_FMT(
                Image.pixel_type == TINYGLTF_COMPONENT_TYPE_BYTE ||
                    Image.pixel_type == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE,
                "Only 8 bit images are allowed in GLTF models!"
            );

            EPixelFormat PixelFormat = EPixelFormat::RGBA8;
            switch (Image.component)
            {
            case 1:
                PixelFormat = EPixelFormat::R8;
                break;
            case 2:
                PixelFormat = EPixelFormat::RG8;
                break;
            case 3:
                PixelFormat = EPixelFormat::RGB8;
                break;
            case 4:
                PixelFormat = EPixelFormat::RGBA8;
                break;
            default:
                CORVUS_ERROR("Invalid number of components in GLTF Image");
                break;
            }

            // TODO: sRGB check?
            return CImageDataLoader::LoadFromMemory(
                Image.image.data(), Image.width, Image.height, PixelFormat, true
            );
        }

        std::vector<CTexture2D> LoadTextures(tinygltf::Model const &GLTFModel)
        {
            std::vector<CTexture2D> Textures(GLTFModel.textures.size());

            for (SizeT i = 0; i < Textures.size(); ++i)
            {
                tinygltf::Texture const &Texture = GLTFModel.textures[i];

                STextureParameters TextureParameters;
                Int32              SamplerIndex = Texture.sampler;
                if (SamplerIndex != -1)
                {
                    TextureParameters = ProcessTextureSampler(GLTFModel.samplers[SamplerIndex]);
                }
                else
                {
                    TextureParameters = ProcessNoTextureSampler();
                }

                Int32 ImageIndex = Texture.source;
                CORVUS_CORE_ASSERT(ImageIndex != -1);
                CImageData Image = ProcessImage(GLTFModel.images[ImageIndex]);

                Textures[i] = CTexture2D{CTexture2DBuffer::Create(Image, TextureParameters)};
            }

            return Textures;
        }

        std::vector<CMaterial> LoadMaterials(
            tinygltf::Model const &GLTFModel, std::vector<CTexture2D> const &Textures
        )
        {
            std::vector<CMaterial> Materials(GLTFModel.materials.size());

            for (SizeT i = 0; i < Materials.size(); ++i)
            {
                tinygltf::Material const &MaterialInfo = GLTFModel.materials[i];
                CMaterial                &Material     = Materials[i];

                // Albedo
                if (MaterialInfo.pbrMetallicRoughness.baseColorTexture.index == -1) // Vertex color
                {
                    std::vector<double> const &VertexColorData =
                        MaterialInfo.pbrMetallicRoughness.baseColorFactor;

                    FVector4 VertexColor{};
                    VertexColor.r = static_cast<float>(VertexColorData[0]);
                    VertexColor.g = static_cast<float>(VertexColorData[1]);
                    VertexColor.b = static_cast<float>(VertexColorData[2]);
                    VertexColor.a = static_cast<float>(VertexColorData[3]);

                    Material.AlbedoMap.Other = VertexColor;
                    Material.AlbedoMap.UseOther();
                }
                else // Albedo map
                {
                    Int32 AlbedoIndex = MaterialInfo.pbrMetallicRoughness.baseColorTexture.index;
                    Material.AlbedoMap.TextureRef.SetUUID(Textures[AlbedoIndex].UUID);
                    Material.AlbedoMap.UseTexture();
                }

                // TODO: Normals
                // TODO: MetallicRoughness
            }

            return Materials;
        }

        void FillBufferReadParams(
            tinygltf::Accessor const   *Accessor, // Is null when working with image data
            tinygltf::BufferView const &BufferView,
            tinygltf::Buffer const     &Buffer,
            SElementFormat const       &ElementFormat,
            std::vector<UInt8>         &OutDstBuffer,
            SBufferReadParams          &OutBufferReadParams
        )
        {
            // clang-format off
            ///////////////////////////////////////////////////////////////////////
            // [--Buffer Data-----------------------------------------------------]
            // ---BV Offset-[--BufferView------------------------------------]----
            // --------------------AccessorOffset-[--------------------------]----
            // -----------------------------------[---Stride---]------------------
            // -----------------------------------[-Data1-]-----[-Data2-]---------
            // -----------------------------------[x--y--z]-----[x--y--z]---------
            ///////////////////////////////////////////////////////////////////////
            // clang-format on

            SizeT ElementSize = ElementFormat.NumComponents * ElementFormat.ComponentSize;

            // BufferView
            SizeT BufferViewOffset = BufferView.byteOffset;
            SizeT BufferViewLength = BufferView.byteLength;
            SizeT BufferViewStride = BufferView.byteStride; // 0 for tightly packed

            // Accessor
            SizeT AccessorOffset = 0;
            SizeT NumElements    = 0;
            if (Accessor)
            {
                AccessorOffset = Accessor->byteOffset;
                NumElements    = Accessor->count;
            }
            else
            {
                NumElements = BufferViewLength / ElementSize;
            }

            SizeT SrcElementSize     = ElementSize;
            SizeT SrcStride          = BufferViewStride == 0 ? SrcElementSize : BufferViewStride;
            SizeT SrcDataStartOffset = BufferViewOffset + AccessorOffset;

            SizeT DstElementSize =
                ElementFormat.NumComponents * 4; // Types used in buffers are int or float(4 byte)
            SizeT DstStride    = DstElementSize;
            SizeT DstDataBytes = NumElements * DstElementSize;

            OutDstBuffer.resize(DstDataBytes);

            OutBufferReadParams.SrcData       = Buffer.data.data() + SrcDataStartOffset;
            OutBufferReadParams.SrcStride     = SrcStride;
            OutBufferReadParams.DstData       = OutDstBuffer.data();
            OutBufferReadParams.DstStride     = DstStride;
            OutBufferReadParams.ElementFormat = ElementFormat;
            OutBufferReadParams.NumElements   = NumElements;
        }

        FMatrix4 GetNodeLocalTransformMatrix(tinygltf::Node const &Node)
        {
            FVector3 Translation{};
            if (!Node.translation.empty())
            {
                Translation.x = static_cast<float>(Node.translation[0]);
                Translation.y = static_cast<float>(Node.translation[1]);
                Translation.z = static_cast<float>(Node.translation[2]);
            }

            FVector3 Scale{1.0f};
            if (!Node.scale.empty())
            {
                Scale.x = static_cast<float>(Node.scale[0]);
                Scale.y = static_cast<float>(Node.scale[1]);
                Scale.z = static_cast<float>(Node.scale[2]);
            }

            FQuaternion Rotation = FQuaternion::Unit;
            if (!Node.rotation.empty())
            {
                Rotation.x = static_cast<float>(Node.rotation[0]);
                Rotation.y = static_cast<float>(Node.rotation[1]);
                Rotation.z = static_cast<float>(Node.rotation[2]);
                Rotation.w = static_cast<float>(Node.rotation[3]);
            }

            FMatrix4 EyeMatrix       = FMatrix4{1.0f};
            FMatrix4 TranslateMatrix = FMatrix::Translate(EyeMatrix, Translation);
            FMatrix4 RotateMatrix    = FQuaternion::ToMat4(Rotation);
            FMatrix4 ScaleMatrix     = FMatrix::Scale(EyeMatrix, Scale);
            return TranslateMatrix * RotateMatrix * ScaleMatrix;
        }

        FIntVector4 ReadIntegerElement(UInt8 const *SrcData, SElementFormat const ElementFormat)
        {
            FIntVector4 Result{};

            for (SizeT Component = 0; Component < ElementFormat.NumComponents; ++Component)
            {
                SizeT ComponentOffset = Component * ElementFormat.ComponentSize;

                switch (ElementFormat.ComponentSize) // can be 1, 2 or 4 bytes
                {
                case 1:
                    Result[static_cast<FIntVector4::length_type>(Component)] =
                        *(reinterpret_cast<UInt8 const *>(SrcData + ComponentOffset));
                    break;
                case 2:
                    Result[static_cast<FIntVector4::length_type>(Component)] =
                        *(reinterpret_cast<UInt16 const *>(SrcData + ComponentOffset));
                    break;
                case 4:
                    Result[static_cast<FIntVector4::length_type>(Component)] =
                        *(reinterpret_cast<UInt32 const *>(SrcData + ComponentOffset));
                    break;
                }
            }

            return Result;
        }

        void WriteIntegerElement(FIntVector4 const &SrcData, SizeT const NumComponents, UInt8 *DstData)
        {
            switch (NumComponents)
            {
            case 1:
                *(reinterpret_cast<UInt32 *>(DstData)) = SrcData[0];
                break;
            case 2:
                *(reinterpret_cast<FIntVector2 *>(DstData)) = FIntVector2{SrcData};
                break;
            case 3:
                *(reinterpret_cast<FIntVector3 *>(DstData)) = FIntVector3{SrcData};
                break;
            default:
                CORVUS_ERROR("Invalid number of element components!");
                break;
            }
        }

        FVector4 ReadFloatElement(UInt8 const *SrcData, SElementFormat const ElementFormat)
        {
            FVector4 Result{};

            for (SizeT Component = 0; Component < ElementFormat.NumComponents; ++Component)
            {
                SizeT ComponentOffset = Component * ElementFormat.ComponentSize;

                switch (ElementFormat.ComponentSize) // Float components are always 4 byte float
                {
                case 4:
                    Result[static_cast<FVector4::length_type>(Component)] =
                        *(reinterpret_cast<float const *>(SrcData + ComponentOffset));
                    break;
                default:
                    CORVUS_ERROR("Unsupported float component type!");
                    break;
                }
            }

            return Result;
        }

        void WriteFloatElement(FVector4 const &SrcData, SizeT const NumComponents, UInt8 *DstData)
        {
            switch (NumComponents)
            {
            case 1:
                *(reinterpret_cast<float *>(DstData)) = SrcData[0];
                break;
            case 2:
                *(reinterpret_cast<FVector2 *>(DstData)) = FVector2{SrcData};
                break;
            case 3:
                *(reinterpret_cast<FVector3 *>(DstData)) = FVector3{SrcData};
                break;
            default:
                CORVUS_ERROR("Invalid number of element components!");
                break;
            }
        }

        void TransformAttributeData(
            FVector3       *SrcData,
            SizeT const     NumElements,
            CString const  &AttributeKey,
            FMatrix4 const &TransformMatrix
        )
        {
            float WValue = 0.0f;
            if (AttributeKey == "POSITION")
            {
                WValue = 1.0f;
            }
            else if (AttributeKey == "NORMAL")
            {
                WValue = 0.0f;
            }
            else
            {
                return;
            }

            for (SizeT Element = 0; Element < NumElements; ++Element)
            {
                FVector3 OldValue         = SrcData[Element];
                FVector4 TransformedValue = FVector4{OldValue, WValue};
                TransformedValue          = TransformMatrix * TransformedValue;
                SrcData[Element]          = FVector3{TransformedValue};
            }
        }

        template<typename TReadElementReturnType>
        void ReadBufferData(
            SBufferReadParams BufferReadParams,
            TReadElementReturnType (*ReadElementFunction)(UInt8 const *SrcData, SElementFormat ElementFormat),
            void (*WriteElementFunction
            )(TReadElementReturnType const &SrcData, SizeT NumComponents, UInt8 *DstData)
        )
        {
            SizeT SrcOffset = 0;
            SizeT DstOffset = 0;
            for (SizeT Element = 0; Element < BufferReadParams.NumElements; ++Element)
            {
                TReadElementReturnType Value =
                    ReadElementFunction(BufferReadParams.SrcData + SrcOffset, BufferReadParams.ElementFormat);

                WriteElementFunction(
                    Value, BufferReadParams.ElementFormat.NumComponents, BufferReadParams.DstData + DstOffset
                );

                SrcOffset += BufferReadParams.SrcStride;
                DstOffset += BufferReadParams.DstStride;
            }
        }

        bool GetAttributeData(
            tinygltf::Model const &GLTFModel,
            FMatrix4 const        &TransformMatrix,
            CString const         &AttributeKey,
            UInt32 const           AttributeValue,
            std::vector<UInt8>    &OutAttributeData,
            UInt32                &OutNumElements,
            UInt32                &OutNumComponents
        )
        {
            tinygltf::Accessor const   &Accessor   = GLTFModel.accessors[AttributeValue];
            tinygltf::BufferView const &BufferView = GLTFModel.bufferViews[Accessor.bufferView];
            tinygltf::Buffer const     &Buffer     = GLTFModel.buffers[BufferView.buffer];

            SElementFormat ElementFormat = GetElementFormatFromAccessor(Accessor);

            std::vector<UInt8> AttributeData;
            SBufferReadParams  BufferReadParams;
            FillBufferReadParams(
                &Accessor, BufferView, Buffer, ElementFormat, AttributeData, BufferReadParams
            );

            if (ElementFormat.bIsInteger)
            {
                ReadBufferData(BufferReadParams, ReadIntegerElement, WriteIntegerElement);
            }
            else
            {
                ReadBufferData(BufferReadParams, ReadFloatElement, WriteFloatElement);

                TransformAttributeData(
                    reinterpret_cast<FVector3 *>(BufferReadParams.DstData),
                    BufferReadParams.NumElements,
                    AttributeKey,
                    TransformMatrix
                );
            }

            OutAttributeData = std::move(AttributeData);
            OutNumElements   = static_cast<UInt32>(BufferReadParams.NumElements);
            OutNumComponents = static_cast<UInt32>(BufferReadParams.ElementFormat.NumComponents);
            return true;
        }

        CMaterial const &GetPrimitiveMaterial(
            tinygltf::Primitive const &Primitive, std::vector<CMaterial> const &Materials
        )
        {
            return Materials[Primitive.material];
        }

        CStaticMeshPrimitive ProcessPrimitive(
            tinygltf::Model const        &GLTFModel,
            std::vector<CMaterial> const &Materials,
            tinygltf::Primitive const    &Primitive,
            FMatrix4 const               &TransformMatrix
        )
        {
            TOwn<CVertexArray> VertexArray = CVertexArray::Create();

            // clang-format off
            std::array<CString, 3> const AttributeKeys = {
                "POSITION",
                "TEXCOORD_0", // For now engine only works with one texcoord array per primitive
                "NORMAL"
            };
            // clang-format on

            // Vertex data
            for (CString const &AttributeKey : AttributeKeys)
            {
                auto AttributeIt = Primitive.attributes.find(AttributeKey);
                if (AttributeIt == Primitive.attributes.end())
                {
                    continue;
                }

                UInt32             AttributeValue = AttributeIt->second;
                std::vector<UInt8> AttributeData;
                UInt32             AttributeNumElements;
                UInt32             AttributeNumComponents;
                if (!GetAttributeData(
                        GLTFModel,
                        TransformMatrix,
                        AttributeKey,
                        AttributeValue,
                        AttributeData,
                        AttributeNumElements,
                        AttributeNumComponents
                    ))
                {
                    CORVUS_ERROR("Failed to get data for attribute {}:{}!", AttributeKey, AttributeValue);
                    continue;
                }

                CVertexBufferLayout VertexBufferLayout;
                switch (AttributeNumComponents)
                {
                case 1:
                    VertexBufferLayout.PushBack(CBufferLayoutElement{EBufferDataType::Float});
                    break;
                case 2:
                    VertexBufferLayout.PushBack(CBufferLayoutElement{EBufferDataType::Vec2});
                    break;
                case 3:
                    VertexBufferLayout.PushBack(CBufferLayoutElement{EBufferDataType::Vec3});
                    break;
                default:
                    CORVUS_ERROR("Unsupported attribute data type!");
                    continue;
                }

                TOwn<CVertexBuffer> VertexBuffer =
                    CVertexBuffer::Create(AttributeData.data(), AttributeNumElements, VertexBufferLayout);
                VertexArray->AddVertexBuffer(std::move(VertexBuffer));
            }

            // Index data
            {
                std::vector<UInt8> IndexesData;
                UInt32             NumIndexes;
                UInt32             IndexesNumComponents;
                if (!GetAttributeData(
                        GLTFModel,
                        TransformMatrix, // Not used here
                        "",              // Not used here
                        Primitive.indices,
                        IndexesData,
                        NumIndexes,
                        IndexesNumComponents // Not really needed
                    ))
                {
                    CORVUS_ERROR("Failed to get indices data!");
                }

                TOwn<CIndexBuffer> IndexBuffer =
                    CIndexBuffer::Create(reinterpret_cast<UInt32 *>(IndexesData.data()), NumIndexes);
                VertexArray->AddIndexBuffer(std::move(IndexBuffer));
            }

            CStaticMeshPrimitive MeshPrimitive{std::move(VertexArray)};
            CMaterial const     &Material = GetPrimitiveMaterial(Primitive, Materials);
            MeshPrimitive.MaterialRef.SetUUID(Material.UUID);

            return MeshPrimitive;
        }

        CStaticMesh ProcessMesh(
            tinygltf::Model const        &GLTFModel,
            std::vector<CMaterial> const &Materials,
            tinygltf::Node const         &Node,
            tinygltf::Mesh const         &Mesh,
            FMatrix4 const               &TransformMatrix
        )
        {
            CStaticMesh StaticMesh;

            for (tinygltf::Primitive const &Primitive : Mesh.primitives)
            {
                StaticMesh.AddPrimitive(ProcessPrimitive(GLTFModel, Materials, Primitive, TransformMatrix));
            }

            return StaticMesh;
        }

        void ProcessMeshNodesTree(
            tinygltf::Model const        &GLTFModel,
            std::vector<CMaterial> const &Materials,
            CStaticModel                 &StaticModel,
            tinygltf::Node const         &RootNode,
            FMatrix4 const               &NodeTransformMatrix
        )
        {
            Int32 MeshIndex = RootNode.mesh;
            if (MeshIndex == -1)
            {
                CORVUS_CORE_WARN(
                    "Not-a-model node {} found in gltf model file and was skipped!", RootNode.name
                );
                return;
            }

            tinygltf::Mesh const &Mesh = GLTFModel.meshes[MeshIndex];
            StaticModel.AddMesh(ProcessMesh(GLTFModel, Materials, RootNode, Mesh, NodeTransformMatrix));

            std::vector<SizeT> ChildNodesIndexes = GetChildNodesIndexes(RootNode);
            for (SizeT const ChildNodeIndex : ChildNodesIndexes)
            {
                tinygltf::Node const &ChildNode = GLTFModel.nodes[ChildNodeIndex];
                FMatrix4 const       &ChildNodeTransformMatrix =
                    NodeTransformMatrix * GetNodeLocalTransformMatrix(ChildNode);
                ProcessMeshNodesTree(GLTFModel, Materials, StaticModel, ChildNode, ChildNodeTransformMatrix);
            }
        }

        SStaticModelLoadedData ProcessModel(tinygltf::Model const &GLTFModel)
        {
            std::vector<CTexture2D> Textures  = LoadTextures(GLTFModel);
            std::vector<CMaterial>  Materials = LoadMaterials(GLTFModel, Textures);

            CStaticModel StaticModel;

            std::vector<SizeT> const RootNodesIndexes = GetRootNodesIndexes(GLTFModel);
            for (SizeT const RootNodeIndex : RootNodesIndexes)
            {
                tinygltf::Node const RootNode = GLTFModel.nodes[RootNodeIndex];
                ProcessMeshNodesTree(
                    GLTFModel, Materials, StaticModel, RootNode, GetNodeLocalTransformMatrix(RootNode)
                );
            }

            SStaticModelLoadedData LoadedData;
            LoadedData.StaticModel = std::move(StaticModel);
            LoadedData.Textures    = std::move(Textures);
            LoadedData.Materials   = std::move(Materials);
            return LoadedData;
        }

    } // namespace

    SStaticModelLoadedData CGLTFModelLoader::LoadStaticModelFromFile(CString const &FilePath, bool bIsBinary)
    {
        tinygltf::Model    GLTFModel;
        tinygltf::TinyGLTF Parser;
        CString            Errors;
        CString            Warnings;
        bool               bResult = false;

        Parser.SetPreserveImageChannels(true);
        Parser.SetStoreOriginalJSONForExtrasAndExtensions(true);

        if (bIsBinary)
        {
            bResult = Parser.LoadBinaryFromFile(&GLTFModel, &Errors, &Warnings, FilePath);
        }
        else
        {
            bResult = Parser.LoadASCIIFromFile(&GLTFModel, &Errors, &Warnings, FilePath);
        }

        if (!Warnings.empty())
        {
            CORVUS_CORE_WARN("While loading StaticModel from {}: {}", FilePath, Warnings);
        }

        if (!Errors.empty())
        {
            CORVUS_CORE_ERROR("While loading StaticModel from {}: {}", FilePath, Errors);
        }

        if (!bResult)
        {
            CORVUS_CORE_ERROR("Failed to load StaticModel from {}", FilePath);
        }

        if (GLTFModel.scenes.size() != 1)
        {
            CORVUS_ERROR("GLTF model file must contain only one scene!");
            return {};
        }

        return ProcessModel(GLTFModel);
    }

} // namespace Corvus