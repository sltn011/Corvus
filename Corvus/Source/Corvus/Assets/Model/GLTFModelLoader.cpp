#include "CorvusPCH.h"

#include "Corvus/Assets/Model/GLTFModelLoader.h"

#include "Corvus/Assets/Model/StaticMesh.h"
#include "Corvus/Assets/Model/StaticModel.h"
#include "Corvus/Math/Matrix.h"
#include "Corvus/Math/Quaternion.h"
#include "Corvus/Renderer/IndexBuffer.h"
#include "Corvus/Renderer/VertexArray.h"
#include "Corvus/Renderer/VertexBuffer.h"

#define TINYGLTF_IMPLEMENTATION
#define TINYGLTF_NO_STB_IMAGE_WRITE
#include <tiny_gltf.h>

namespace Corvus
{
    namespace
    {
        FMatrix4 GetNodeTransformMatrix(tinygltf::Model const &GLTFModel, tinygltf::Node const &Node)
        {
            FVector3 Translation{};
            if (!Node.translation.empty())
            {
                Translation.x = static_cast<float>(Node.translation[0]);
                Translation.y = static_cast<float>(Node.translation[1]);
                Translation.z = static_cast<float>(Node.translation[2]);
            }
            CORVUS_TRACE("{} Translation: {} {} {}", Node.name, Translation.x, Translation.y, Translation.z);

            FVector3 Scale{1.0f};
            if (!Node.scale.empty())
            {
                Scale.x = static_cast<float>(Node.scale[0]);
                Scale.y = static_cast<float>(Node.scale[1]);
                Scale.z = static_cast<float>(Node.scale[2]);
            }
            CORVUS_TRACE("{} Scale: {} {} {}", Node.name, Scale.x, Scale.y, Scale.z);

            FQuaternion Rotation = FQuaternion::Unit;
            if (!Node.rotation.empty())
            {
                Rotation.x = static_cast<float>(Node.rotation[0]);
                Rotation.y = static_cast<float>(Node.rotation[1]);
                Rotation.z = static_cast<float>(Node.rotation[2]);
                Rotation.w = static_cast<float>(Node.rotation[3]);
            }
            CORVUS_TRACE("{} Rotation: {} {} {} {}", Node.name, Rotation.x, Rotation.y, Rotation.z, Rotation.w);

            FMatrix4 EyeMatrix       = FMatrix4{1.0f};
            FMatrix4 TranslateMatrix = FMatrix::Translate(EyeMatrix, Translation);
            FMatrix4 RotateMatrix    = FQuaternion::ToMat4(Rotation);
            FMatrix4 ScaleMatrix     = FMatrix::Scale(EyeMatrix, Scale);
            return TranslateMatrix * RotateMatrix * ScaleMatrix;
        }

        bool GetAttributeData(
            tinygltf::Model const &GLTFModel,
            FMatrix4 const        &NodeTransformMatrix,
            CString const         &AttributeKey,
            int const              AttributeValue,
            std::vector<UInt8>    &OutAttributeData,
            UInt32                &OutNumElements,
            UInt32                &OutNumComponents
        )
        {
            CORVUS_INFO("{} {}", AttributeKey, AttributeValue);

            tinygltf::Accessor const   &Accessor   = GLTFModel.accessors[AttributeValue];
            tinygltf::BufferView const &BufferView = GLTFModel.bufferViews[Accessor.bufferView];
            tinygltf::Buffer const     &Buffer     = GLTFModel.buffers[BufferView.buffer];

            SizeT BufferViewOffset = BufferView.byteOffset;
            SizeT BufferViewLength = BufferView.byteLength;
            SizeT AccessorOffset   = Accessor.byteOffset;
            SizeT BufferViewStride = BufferView.byteStride; // 0 for tightly packed

            ///////////////////////////////////////////////////////////////////////
            // [--Buffer Data-----------------------------------------------------]
            // ---BV Offset-[--BufferView------------------------------------]----
            // --------------------AccessorOffset-[--------------------------]----
            // -----------------------------------[---Stride---]------------------
            // -----------------------------------[-Data1-]-----[-Data2-]---------
            // -----------------------------------[x--y--z]-----[x--y--z]---------
            ///////////////////////////////////////////////////////////////////////

            SizeT Count         = Accessor.count;
            SizeT DataType      = Accessor.type;
            SizeT NumComponents = 0;
            SizeT ComponentSize = 4; // All are either ints or floats (4 bytes per component)
            if (DataType == TINYGLTF_TYPE_SCALAR)
            {
                NumComponents = 1;
            }
            else if (DataType == TINYGLTF_TYPE_VEC2)
            {
                NumComponents = 2;
            }
            else if (DataType == TINYGLTF_TYPE_VEC3)
            {
                NumComponents = 3;
            }
            else
            {
                CORVUS_ERROR("Unsupported data type in gltf model!");
                return false;
            }

            SizeT ElementSize = NumComponents * ComponentSize;
            SizeT DataBytes   = Count * ElementSize;

            std::vector<UInt8> AttributeData(DataBytes);

            FVector4 Data = {};
            for (SizeT Element = 0; Element < Count; ++Element)
            {
                for (SizeT Component = 0; Component < NumComponents; ++Component)
                {
                    SizeT DataStartOffset = BufferViewOffset + AccessorOffset;
                    SizeT ElementOffset   = Element * (ElementSize + AccessorOffset);
                    SizeT ComponentOffset = Component * ComponentSize;

                    Data[static_cast<FVector4::length_type>(Component)] = *(reinterpret_cast<float const *>(
                        Buffer.data.data() + DataStartOffset + ElementOffset + ComponentOffset
                    ));
                }

                if (AttributeKey == "POSITION")
                {
                    Data[3] = 1.0f;
                    Data    = NodeTransformMatrix * Data;
                }
                else if (AttributeKey == "NORMAL")
                {
                    Data[3] = 0.0f;
                    Data    = NodeTransformMatrix * Data;
                }

                // Copy 4byte components into data, don't care about type here
                if (NumComponents == 1)
                {
                    *(reinterpret_cast<FVector4::value_type *>(AttributeData.data()) + Element) = Data[0];
                }
                else if (NumComponents == 2)
                {
                    *(reinterpret_cast<FVector2 *>(AttributeData.data()) + Element) = FVector2{Data};
                }
                else if (NumComponents == 3)
                {
                    *(reinterpret_cast<FVector3 *>(AttributeData.data()) + Element) = FVector3{Data};
                }
            }

            OutAttributeData = std::move(AttributeData);
            OutNumElements   = static_cast<UInt32>(Count);
            OutNumComponents = static_cast<UInt32>(NumComponents);
            return true;
        }

        TOwn<CVertexArray> ProcessPrimitive(
            tinygltf::Model const &GLTFModel, tinygltf::Primitive const &Primitive, FMatrix4 const &NodeTransformMatrix
        )
        {
            TOwn<CVertexArray> VertexArray = CVertexArray::Create();

            // Vertex data
            for (auto const &[AttributeKey, AttributeValue] : Primitive.attributes)
            {
                std::vector<UInt8> AttributeData;
                UInt32             AttributeNumElements;
                UInt32             AttributeNumComponents;
                if (!GetAttributeData(
                        GLTFModel,
                        NodeTransformMatrix,
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
                        NodeTransformMatrix, // Not used here
                        "",                  // Not used here
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

            return VertexArray;
        }

        CStaticMesh ProcessMesh(
            tinygltf::Model const &GLTFModel, tinygltf::Node const &Node, tinygltf::Mesh const &Mesh
        )
        {
            FMatrix4 NodeTransformMatrix = GetNodeTransformMatrix(GLTFModel, Node);

            for (tinygltf::Primitive const &Primitive : Mesh.primitives)
            {
                ProcessPrimitive(GLTFModel, Primitive, NodeTransformMatrix);
            }
            return {};
        }

        void ParseGLTFModel(tinygltf::Model &GLTFModel)
        {
            for (tinygltf::Node const &Node : GLTFModel.nodes)
            {
                auto const &MeshIndex = Node.mesh;
                auto const &Mesh      = GLTFModel.meshes[MeshIndex];
                ProcessMesh(GLTFModel, Node, Mesh);
            }
        }
    } // namespace

    CStaticModel CGLTFModelLoader::LoadStaticModelFromFile(CString const &FilePath, bool bIsBinary)
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

        ParseGLTFModel(GLTFModel);

        return CStaticModel();
    }

} // namespace Corvus
