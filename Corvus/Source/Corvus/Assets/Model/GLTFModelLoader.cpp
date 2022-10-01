#include "CorvusPCH.h"

#include "Corvus/Assets/Model/GLTFModelLoader.h"

#include "Corvus/Assets/Model/StaticMesh.h"
#include "Corvus/Assets/Model/StaticMeshPrimitive.h"
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

        FIntVector4 ReadIntegerElement(UInt8 const *Data, SizeT NumComponents, SizeT ComponentSize)
        {
            FIntVector4 Result{};

            for (SizeT Component = 0; Component < NumComponents; ++Component)
            {
                SizeT ComponentOffset = Component * ComponentSize;

                switch (ComponentSize) // can be 1, 2 or 4 bytes
                {
                case 1:
                    Result[static_cast<FIntVector4::length_type>(Component)] =
                        *(reinterpret_cast<UInt8 const *>(Data + ComponentOffset));
                    break;
                case 2:
                    Result[static_cast<FIntVector4::length_type>(Component)] =
                        *(reinterpret_cast<UInt16 const *>(Data + ComponentOffset));
                    break;
                case 4:
                    Result[static_cast<FIntVector4::length_type>(Component)] =
                        *(reinterpret_cast<UInt32 const *>(Data + ComponentOffset));
                    break;
                }
            }

            return Result;
        }

        FVector4 ReadFloatElement(UInt8 const *Data, SizeT NumComponents, SizeT ComponentSize)
        {
            FVector4 Result{};

            for (SizeT Component = 0; Component < NumComponents; ++Component)
            {
                SizeT ComponentOffset = Component * ComponentSize;

                switch (ComponentSize) // Float components are always 4 byte float
                {
                case 4:
                    Result[static_cast<FVector4::length_type>(Component)] =
                        *(reinterpret_cast<float const *>(Data + ComponentOffset));
                    break;
                default:
                    CORVUS_ERROR("Unsupported float component type!");
                    break;
                }
            }

            return Result;
        }

        void AddIntegerElementToBuffer(
            UInt8 *BufferData, SizeT Element, UInt8 const *Data, SizeT NumComponents, SizeT ComponentSize
        )
        {
            FIntVector4 ElementData = ReadIntegerElement(Data, NumComponents, ComponentSize);
            switch (NumComponents)
            {
            case 1:
                *(reinterpret_cast<UInt32 *>(BufferData) + Element) = ElementData[0];
                break;
            case 2:
                *(reinterpret_cast<FIntVector2 *>(BufferData) + Element) = FIntVector2{ElementData};
                break;
            case 3:
                *(reinterpret_cast<FIntVector3 *>(BufferData) + Element) = FIntVector3{ElementData};
                break;
            default:
                CORVUS_ERROR("Invalid number of element components!");
                break;
            }
        }

        void AddFloatElementToBuffer(
            UInt8          *BufferData,
            SizeT           Element,
            UInt8 const    *Data,
            SizeT           NumComponents,
            SizeT           ComponentSize,
            FMatrix4 const &NodeTransformMatrix,
            CString const  &AttributeKey
        )
        {
            FVector4 ElementData = ReadFloatElement(Data, NumComponents, ComponentSize);

            if (AttributeKey == "POSITION")
            {
                ElementData[3] = 1.0f;
                ElementData    = NodeTransformMatrix * ElementData;
            }
            else if (AttributeKey == "NORMAL")
            {
                ElementData[3] = 0.0f;
                ElementData    = NodeTransformMatrix * ElementData;
            }

            switch (NumComponents)
            {
            case 1:
                *(reinterpret_cast<float *>(BufferData) + Element) = ElementData[0];
                break;
            case 2:
                *(reinterpret_cast<FVector2 *>(BufferData) + Element) = FVector2{ElementData};
                break;
            case 3:
                *(reinterpret_cast<FVector3 *>(BufferData) + Element) = FVector3{ElementData};
                break;
            default:
                CORVUS_ERROR("Invalid number of element components!");
                break;
            }
        }

        bool GetAttributeData(
            tinygltf::Model const &GLTFModel,
            FMatrix4 const        &NodeTransformMatrix,
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
            SizeT ComponentType = Accessor.componentType;
            SizeT NumComponents = 0;
            SizeT ComponentSize = 0;
            bool  bIsInteger    = false;

            switch (DataType)
            {
            case TINYGLTF_TYPE_SCALAR:
                NumComponents = 1;
                break;
            case TINYGLTF_TYPE_VEC2:
                NumComponents = 2;
                break;
            case TINYGLTF_TYPE_VEC3:
                NumComponents = 3;
                break;
            default:
                CORVUS_ERROR("Unsupported data type in gltf model!");
                return false;
            }

            switch (ComponentType)
            {
            case TINYGLTF_COMPONENT_TYPE_BYTE:
            case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
                ComponentSize = 1;
                bIsInteger    = true;
                break;
            case TINYGLTF_COMPONENT_TYPE_SHORT:
            case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
                ComponentSize = 2;
                bIsInteger    = true;
                break;
            case TINYGLTF_COMPONENT_TYPE_INT:
            case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
                ComponentSize = 4;
                bIsInteger    = true;
                break;
            case TINYGLTF_COMPONENT_TYPE_FLOAT:
                ComponentSize = 4;
                break;
            default:
                CORVUS_ERROR("Unsupported data component type in gltf model!");
                return false;
            }

            SizeT SrcElementSize = NumComponents * ComponentSize;
            SizeT DstElementSize = NumComponents * 4; // Types used in geometry buffers are int or float(4 byte)
            SizeT DataBytes      = Count * DstElementSize;

            std::vector<UInt8> AttributeData(DataBytes);

            for (SizeT Element = 0; Element < Count; ++Element)
            {
                SizeT DataStartOffset = BufferViewOffset + AccessorOffset;
                SizeT ElementOffset   = Element * (SrcElementSize + AccessorOffset);

                UInt8 const *Data = Buffer.data.data() + DataStartOffset + ElementOffset;

                if (bIsInteger)
                {
                    AddIntegerElementToBuffer(AttributeData.data(), Element, Data, NumComponents, ComponentSize);
                }
                else
                {
                    AddFloatElementToBuffer(
                        AttributeData.data(),
                        Element,
                        Data,
                        NumComponents,
                        ComponentSize,
                        NodeTransformMatrix,
                        AttributeKey
                    );
                }
            }

            OutAttributeData = std::move(AttributeData);
            OutNumElements   = static_cast<UInt32>(Count);
            OutNumComponents = static_cast<UInt32>(NumComponents);
            return true;
        }

        bool GetPrimitiveMaterial(
            tinygltf::Model const &GLTFModel, tinygltf::Primitive const &Primitive, CMaterial &OutMaterial
        )
        {
            tinygltf::Material const &MaterialData = GLTFModel.materials[Primitive.material];

            CMaterial Material;
            return true;
        }

        CStaticMeshPrimitive ProcessPrimitive(
            tinygltf::Model const &GLTFModel, tinygltf::Primitive const &Primitive, FMatrix4 const &NodeTransformMatrix
        )
        {
            TOwn<CVertexArray> VertexArray = CVertexArray::Create();

            std::array<CString, 3> const AttributeKeys = {"POSITION", "TEXCOORD_0", "NORMAL"};

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

            CMaterial Material; // Fill material data

            return CStaticMeshPrimitive{std::move(VertexArray), std::move(Material)};
        }

        CStaticMesh ProcessMesh(
            tinygltf::Model const &GLTFModel, tinygltf::Node const &Node, tinygltf::Mesh const &Mesh
        )
        {
            CStaticMesh StaticMesh;

            FMatrix4 NodeTransformMatrix = GetNodeTransformMatrix(GLTFModel, Node);
            for (tinygltf::Primitive const &Primitive : Mesh.primitives)
            {
                StaticMesh.AddPrimitive(ProcessPrimitive(GLTFModel, Primitive, NodeTransformMatrix));
            }

            return StaticMesh;
        }

        CStaticModel ProcessModel(tinygltf::Model &GLTFModel)
        {
            CStaticModel StaticModel;

            for (tinygltf::Node const &Node : GLTFModel.nodes)
            {
                tinygltf::Mesh const &Mesh = GLTFModel.meshes[Node.mesh];
                StaticModel.AddMesh(ProcessMesh(GLTFModel, Node, Mesh));
            }

            return StaticModel;
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

        return ProcessModel(GLTFModel);
    }

} // namespace Corvus
