#ifndef __MESH_H__
#define __MESH_H__

#include "Common.h"
#include "Buffer.h"
#include "VertexLayout.h"

struct Vertex 
{
    glm::vec3 Pos;
    glm::vec3 Normal;
    glm::vec2 TexCoord;
};

CLASS_PTR(Mesh);
class Mesh 
{
public:
  static std::unique_ptr<Mesh> CreateOrNull(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, uint32_t primitiveType);
  static std::unique_ptr<Mesh> MakeBoxOrNull();

  const VertexLayout* GetVertexLayout() const { return mVertexLayout.get(); }
  Buffer* GetVertexBuffer() const { return mVertexBuffer.get(); }
  Buffer* GetIndexBuffer() const { return mIndexBuffer.get(); }

  void Draw() const;

private:
  Mesh() = default;
  void Init(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, uint32_t primitiveType);

  uint32_t mPrimitiveType { GL_TRIANGLES };
  std::unique_ptr<VertexLayout> mVertexLayout;
  std::unique_ptr<Buffer> mVertexBuffer;
  std::unique_ptr<Buffer> mIndexBuffer;
};

#endif // __MESH_H__