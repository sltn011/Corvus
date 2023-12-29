#include "CorvusPCH.h"

#include "Corvus/Assets/Model/GLTFModelLoader.h"

#include "Corvus/Assets/Material/Material.h"
#include "Corvus/Assets/Model/StaticMesh.h"
#include "Corvus/Assets/Model/StaticMeshPrimitive.h"
#include "Corvus/Assets/Model/StaticModel.h"
#include "Corvus/Assets/Texture/ImageData.h"
#include "Corvus/Assets/Texture/ImageDataLoader.h"
#include "Corvus/Assets/Texture/Texture2D.h"
#include "Corvus/Core/Application.h"
#include "Corvus/Math/Matrix.h"
#include "Corvus/Math/Quaternion.h"
#include "Corvus/Renderer/Data/Vertex.h"
#include "Corvus/Renderer/Renderer.h"

#define TINYGLTF_IMPLEMENTATION
#define TINYGLTF_NO_STB_IMAGE_WRITE
#include <tiny_gltf.h>

//
#include <stb_image.h>

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
            case TINYGLTF_TYPE_VEC4:
                ElementFormat.NumComponents = 4;
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

        CImageData ProcessImage(tinygltf::Image const &Image)
        {
            CORVUS_CORE_ASSERT_FMT(
                Image.pixel_type == TINYGLTF_COMPONENT_TYPE_BYTE ||
                    Image.pixel_type == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE,
                "Only 8 bit images are allowed in GLTF models!"
            );

            VkFormat PixelFormat;
            switch (Image.component)
            {
            case 1:
                PixelFormat = VK_FORMAT_R8_SRGB;
                break;
            case 2:
                PixelFormat = VK_FORMAT_R8G8_SRGB;
                break;
            case 3:
                PixelFormat = VK_FORMAT_R8G8B8_SRGB;
                break;
            case 4:
                PixelFormat = VK_FORMAT_R8G8B8A8_SRGB;
                break;
            default:
                CORVUS_CORE_ERROR("Invalid number of components in GLTF Image");
                break;
            }

            // TODO: sRGB check?
            return CImageDataLoader::LoadFromMemory(Image.image.data(), Image.width, Image.height, PixelFormat, true);
        }

        std::vector<CTexture2D> LoadTextures(tinygltf::Model const &GLTFModel)
        {
            std::vector<CTexture2D> Textures(GLTFModel.textures.size());

            for (SizeT i = 0; i < Textures.size(); ++i)
            {
                tinygltf::Texture const &Texture = GLTFModel.textures[i];

                // STextureParameters TextureParameters;
                // Int32              SamplerIndex = Texture.sampler;
                // if (SamplerIndex != -1)
                //{
                //     TextureParameters = ProcessTextureSampler(GLTFModel.samplers[SamplerIndex]);
                // }
                // else
                //{
                //     TextureParameters = ProcessNoTextureSampler();
                // }

                Int32 ImageIndex = Texture.source;
                CORVUS_CORE_ASSERT(ImageIndex != -1);
                CImageData Image = ProcessImage(GLTFModel.images[ImageIndex]);

                Textures[i] =
                    Renderer().CreateTexture2D(Image, Image.GetMaxMipLevel(), Renderer().Samplers.FilteredSampler);
            }

            return Textures;
        }

        std::vector<CMaterial> LoadMaterials(tinygltf::Model const &GLTFModel, std::vector<CTexture2D> const &Textures)
        {
            std::vector<CMaterial> Materials(GLTFModel.materials.size());

            for (SizeT i = 0; i < Materials.size(); ++i)
            {
                tinygltf::Material const &MaterialInfo = GLTFModel.materials[i];
                CMaterial                &Material     = Materials[i];

                // Albedo
                if (MaterialInfo.pbrMetallicRoughness.baseColorTexture.index == -1)
                {
                    Material.Albedo.UUID = CApplication::GetInstance().AssetDrawer.DefaultTextures.Albedo.UUID;
                }
                else
                {
                    Int32 AlbedoIndex    = MaterialInfo.pbrMetallicRoughness.baseColorTexture.index;
                    Material.Albedo.UUID = Textures[AlbedoIndex].UUID;
                }

                // Normal
                if (MaterialInfo.normalTexture.index == -1)
                {
                    Material.Normal.UUID = CApplication::GetInstance().AssetDrawer.DefaultTextures.Normal.UUID;
                }
                else
                {
                    Int32 NormalIndex    = MaterialInfo.normalTexture.index;
                    Material.Normal.UUID = Textures[NormalIndex].UUID;
                }

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

            SizeT DstElementSize = ElementFormat.NumComponents * 4; // Types used in buffers are int or float(4 byte)
            SizeT DstStride      = DstElementSize;
            SizeT DstDataBytes   = NumElements * DstElementSize;

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

        FSIntVector4 ReadIntegerElement(UInt8 const *SrcData, SElementFormat const ElementFormat)
        {
            FSIntVector4 Result{};

            for (SizeT Component = 0; Component < ElementFormat.NumComponents; ++Component)
            {
                SizeT ComponentOffset = Component * ElementFormat.ComponentSize;

                switch (ElementFormat.ComponentSize) // can be 1, 2 or 4 bytes
                {
                case 1:
                    Result[static_cast<FSIntVector4::length_type>(Component)] =
                        *(reinterpret_cast<UInt8 const *>(SrcData + ComponentOffset));
                    break;
                case 2:
                    Result[static_cast<FSIntVector4::length_type>(Component)] =
                        *(reinterpret_cast<UInt16 const *>(SrcData + ComponentOffset));
                    break;
                case 4:
                    Result[static_cast<FSIntVector4::length_type>(Component)] =
                        *(reinterpret_cast<UInt32 const *>(SrcData + ComponentOffset));
                    break;
                }
            }

            return Result;
        }

        void WriteIntegerElement(FSIntVector4 const &SrcData, SizeT const NumComponents, UInt8 *DstData)
        {
            switch (NumComponents)
            {
            case 1:
                *(reinterpret_cast<UInt32 *>(DstData)) = SrcData[0];
                break;
            case 2:
                *(reinterpret_cast<FSIntVector2 *>(DstData)) = FSIntVector2{SrcData};
                break;
            case 3:
                *(reinterpret_cast<FSIntVector3 *>(DstData)) = FSIntVector3{SrcData};
                break;
            case 4:
                *(reinterpret_cast<FSIntVector4 *>(DstData)) = FSIntVector4{SrcData};
                break;
            default:
                CORVUS_CORE_ERROR("Invalid number of element components!");
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
            case 4:
                *(reinterpret_cast<FVector4 *>(DstData)) = FVector4{SrcData};
                break;
            default:
                CORVUS_CORE_ERROR("Invalid number of element components!");
                break;
            }
        }

        FVector4 CalculateTangentVec(FVector3 const Position[3], FVector2 const UV[3])
        {
            FVector3 const E[2]  = {Position[0] - Position[1], Position[2] - Position[1]};
            float const    dU[2] = {UV[0].x - UV[1].x, UV[2].x - UV[1].x};
            float const    dV[2] = {UV[0].y - UV[1].y, UV[2].y - UV[1].y};
            float const    Coeff = 1.f / (dU[0] * dV[1] - dU[1] * dV[0]);

            FMatrix2 const   Lhs = {dV[1], -dU[1], -dV[0], dU[0]};
            FMatrix3_2 const Rhs = {E[0].x, E[1].x, E[0].y, E[1].y, E[0].z, E[1].z};

            FMatrix3_2 const TB = Coeff * Lhs * Rhs;
            FVector3 const   T  = {TB[0].x, TB[1].x, TB[2].x};

            return FVector4{FVector::Normalize(T), 1.f};
        }

        void TransformAttributeData(
            FVector3 *SrcData, SizeT const NumElements, CString const &AttributeKey, FMatrix4 const &TransformMatrix
        )
        {
            float WValue = 0.0f;
            if (AttributeKey == "POSITION")
            {
                WValue = 1.0f;
            }
            else if (AttributeKey == "NORMAL" || AttributeKey == "TANGENT")
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
            void (*WriteElementFunction)(TReadElementReturnType const &SrcData, SizeT NumComponents, UInt8 *DstData)
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
            FillBufferReadParams(&Accessor, BufferView, Buffer, ElementFormat, AttributeData, BufferReadParams);

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

        template<typename T>
        std::vector<T> GetAttributeDataBuffer(
            tinygltf::Model const     &GLTFModel,
            CString const             &AttributeKey,
            tinygltf::Primitive const &Primitive,
            FMatrix4 const            &TransformMatrix
        )
        {
            std::vector<T> DataBuffer;

            auto AttributeIt = Primitive.attributes.find(AttributeKey);
            if (AttributeIt == Primitive.attributes.end())
            {
                return DataBuffer;
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
                CORVUS_CORE_ERROR("Failed to get data for attribute {}:{}!", AttributeKey, AttributeValue);
            }

            SizeT NumVertexPositions = AttributeData.size() / sizeof(DataBuffer[0]);
            DataBuffer.resize(NumVertexPositions);
            std::memcpy(DataBuffer.data(), AttributeData.data(), AttributeData.size());

            return DataBuffer;
        }

        std::vector<UInt16> GetIndicesDataBuffer(tinygltf::Model const &GLTFModel, tinygltf::Primitive const &Primitive)
        {
            std::vector<UInt16> Indices;

            std::vector<UInt8> IndicesDataBuffer;
            UInt32             NumIndices;
            UInt32             IndicesNumComponents;
            if (!GetAttributeData(
                    GLTFModel,
                    FMatrix::Identity<FMatrix4>(), // Not used here
                    "",                            // Not used here
                    Primitive.indices,
                    IndicesDataBuffer,
                    NumIndices,
                    IndicesNumComponents // Not really needed, always 1
                ))
            {
                CORVUS_CORE_ERROR("Failed to get indices data!");
                Indices = {0, 1, 2};
                return Indices;
            }

            Indices.resize(NumIndices);
            for (SizeT i = 0; i < NumIndices; ++i)
            {
                Indices[i] = static_cast<UInt16>(reinterpret_cast<UInt32 *>(IndicesDataBuffer.data())[i]);
            }

            return Indices;
        }

        CStaticMeshPrimitive ProcessPrimitive(
            tinygltf::Model const        &GLTFModel,
            std::vector<CMaterial> const &Materials,
            tinygltf::Primitive const    &Primitive,
            FMatrix4 const               &TransformMatrix
        )
        {
            std::vector<FVector3> PositionVec =
                GetAttributeDataBuffer<FVector3>(GLTFModel, "POSITION", Primitive, TransformMatrix);
            std::vector<FVector2> TexCoord0Vec =
                GetAttributeDataBuffer<FVector2>(GLTFModel, "TEXCOORD_0", Primitive, TransformMatrix);
            std::vector<FVector3> NormalVec =
                GetAttributeDataBuffer<FVector3>(GLTFModel, "NORMAL", Primitive, TransformMatrix);
            std::vector<FVector4> TangentVec; // Calculate tangent vectors manually

            SizeT NumElements = PositionVec.size();
            NumElements       = FMath::Max(NumElements, TexCoord0Vec.size());
            NumElements       = FMath::Max(NumElements, NormalVec.size());
            NumElements       = FMath::Max(NumElements, TangentVec.size());

            PositionVec.resize(NumElements);
            TexCoord0Vec.resize(NumElements);
            NormalVec.resize(NumElements);
            TangentVec.resize(NumElements);

            // Index data
            std::vector<UInt16> IndicesData = GetIndicesDataBuffer(GLTFModel, Primitive);

            CORVUS_CORE_ASSERT(IndicesData.size() % 3 == 0);

            // Combine Vertex Data
            std::vector<CVertex> VertexData(NumElements);
            for (SizeT i = 0; i < NumElements; ++i)
            {
                VertexData[i].Position = PositionVec[i];
                VertexData[i].UVCoord  = TexCoord0Vec[i];
                VertexData[i].Normal   = NormalVec[i];
            }

            // Calculate Tangent vectors for each polygon
            for (SizeT i = 0; i < IndicesData.size(); i += 3)
            {
                UInt16 const  Index[3]  = {IndicesData[i], IndicesData[i + 1], IndicesData[i + 2]};
                CVertex const Vertex[3] = {VertexData[Index[0]], VertexData[Index[1]], VertexData[Index[2]]};

                FVector3 const Position[3] = {Vertex[0].Position, Vertex[1].Position, Vertex[2].Position};
                FVector2 const UV[3]       = {Vertex[0].UVCoord, Vertex[1].UVCoord, Vertex[2].UVCoord};

                FVector4 const Tangent       = CalculateTangentVec(Position, UV);
                VertexData[Index[0]].Tangent = Tangent;
                VertexData[Index[1]].Tangent = Tangent;
                VertexData[Index[2]].Tangent = Tangent;
            }

            CVulkanBuffer VertexBuffer = Renderer().CreateVertexBuffer(VertexData);
            CVulkanBuffer IndexBuffer  = Renderer().CreateIndexBuffer(IndicesData);

            CMaterial const &Material = GetPrimitiveMaterial(Primitive, Materials);

            CStaticMeshPrimitive MeshPrimitive;
            MeshPrimitive.VertexBuffer  = VertexBuffer;
            MeshPrimitive.VertexData    = std::move(VertexData);
            MeshPrimitive.IndexBuffer   = IndexBuffer;
            MeshPrimitive.IndexData     = std::move(IndicesData);
            MeshPrimitive.Material.UUID = Material.UUID;

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
                if (Primitive.attributes.find("POSITION") != Primitive.attributes.end())
                {
                    StaticMesh.AddPrimitive(ProcessPrimitive(GLTFModel, Materials, Primitive, TransformMatrix));
                }
                else
                {
                    CORVUS_CORE_WARN("Mesh Primitive without Vertex Position found and skipped");
                }
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
                CORVUS_CORE_WARN("Not-a-model node {} found in gltf model file and was skipped!", RootNode.name);
                return;
            }

            tinygltf::Mesh const &Mesh = GLTFModel.meshes[MeshIndex];
            StaticModel.AddMesh(ProcessMesh(GLTFModel, Materials, RootNode, Mesh, NodeTransformMatrix));

            std::vector<SizeT> ChildNodesIndexes = GetChildNodesIndexes(RootNode);
            for (SizeT const ChildNodeIndex : ChildNodesIndexes)
            {
                tinygltf::Node const &ChildNode          = GLTFModel.nodes[ChildNodeIndex];
                FMatrix4 const &ChildNodeTransformMatrix = NodeTransformMatrix * GetNodeLocalTransformMatrix(ChildNode);
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