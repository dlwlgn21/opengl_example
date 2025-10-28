#ifndef __MESH_H__
#define __MESH_H__

#include "Common.h"
#include "Buffer.h"
#include "VertexLayout.h"
#include "Texture.h"
#include "Program.h"

struct Vertex 
{
    glm::vec3 Pos;
    glm::vec3 Normal;
    glm::vec2 TexCoord;
};

CLASS_PTR(Material);
class Material
{
public:
    static std::unique_ptr<Material> Create()
    {
        return std::unique_ptr<Material>(new Material());
    }
    std::unique_ptr<Texture> Diffuse;
    std::unique_ptr<Texture> Specular;
    float Shininess { 32.0f };

    void SetToProgram(const Program* pProgram) const;
private:
    Material() = default;
};

CLASS_PTR(Mesh);
class Mesh 
{
public:
    static std::unique_ptr<Mesh> CreateOrNull(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, uint32_t primitiveType);
    static std::unique_ptr<Mesh> MakeBoxOrNull();
    static std::unique_ptr<Mesh> MakePlaneOrNull();

    const VertexLayout* GetVertexLayout() const { return mVertexLayout.get(); }
    Buffer* GetVertexBuffer() const { return mVertexBuffer.get(); }
    Buffer* GetIndexBuffer() const { return mIndexBuffer.get(); }
    void SetMaterial(Material* pMaterial) { mpMaterial = pMaterial; }
    Material* GetMaterial() const { return mpMaterial; }
    void Draw(const Program* pProgram) const;

private:
    Mesh() = default;
    void Init(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, uint32_t primitiveType);

    uint32_t mPrimitiveType { GL_TRIANGLES };
    std::unique_ptr<VertexLayout> mVertexLayout;
    std::unique_ptr<Buffer> mVertexBuffer;
    std::unique_ptr<Buffer> mIndexBuffer;
    Material* mpMaterial { nullptr }; 
};

#endif // __MESH_H__