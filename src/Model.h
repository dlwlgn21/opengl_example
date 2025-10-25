#ifndef __MODEL_H__
#define __MODEL_H__

#include "Common.h"
#include "Mesh.h"
#include "Program.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

CLASS_PTR(Model);
class Model 
{
public:
    static std::unique_ptr<Model> LoadOrNull(const std::string& filename);

    int GetMeshCount() const { return (int)mMeshs.size(); }
    Mesh* GetMeshAt(int idx) const { return mMeshs[idx].get(); }
    void Draw(const Program* pProgram) const;

private:
    Model() {}
    bool TryLoadByAssimp(const std::string& filename);
    void ProcessMesh(aiMesh* pMesh, const aiScene* pScene);
    void ProcessNodeRecursive(aiNode* pNode, const aiScene* pScene);
        
    std::vector<std::unique_ptr<Mesh>> mMeshs;
    std::vector<std::unique_ptr<Material>> mMaterials;
};

#endif // __MODEL_H__