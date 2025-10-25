#include "Model.h"
#include "Texture.h"

using namespace std;

unique_ptr<Model> Model::LoadOrNull(const string& filename)
{
    auto model = unique_ptr<Model>(new Model());
    if (!model->TryLoadByAssimp(filename))
    {
        return nullptr;
    }
    return std::move(model);
}

bool Model::TryLoadByAssimp(const string& filename)
{
    Assimp::Importer importer;
    const aiScene* pScene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!pScene || pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !pScene->mRootNode) 
    {
        SPDLOG_ERROR("failed to load model: {}", filename);
        return false;
    }

    auto dirname = filename.substr(0, filename.find_last_of("/"));
    auto LoadTexture = [&](aiMaterial* material, aiTextureType type) -> std::unique_ptr<Texture>
    {
        if (material->GetTextureCount(type) <= 0)
        {
            return nullptr;
        }
        aiString filepath;
        material->GetTexture(aiTextureType_DIFFUSE, 0, &filepath);
        unique_ptr<Image> image = Image::LoadOrNull(fmt::format("{}/{}", dirname, filepath.C_Str()));
        if (image == nullptr)
        {
            return nullptr;
        }
        return Texture::CreateFromImg(image.get());
    };

    for (uint32_t i = 0; i < pScene->mNumMaterials; i++)
    {
        aiMaterial* pAiMaterial = pScene->mMaterials[i];
        unique_ptr<Material> glMaterial = Material::Create();
        glMaterial->Diffuse = LoadTexture(pAiMaterial, aiTextureType_DIFFUSE);
        glMaterial->Specular = LoadTexture(pAiMaterial, aiTextureType_SPECULAR);
        mMaterials.push_back(std::move(glMaterial));
    }
    ProcessNodeRecursive(pScene->mRootNode, pScene);
    return true;
}

void Model::ProcessNodeRecursive(aiNode* pNode, const aiScene* pScene)
{
    for (uint32_t i = 0; i < pNode->mNumMeshes; i++)
    {
        auto meshIndex = pNode->mMeshes[i];
        auto mesh = pScene->mMeshes[meshIndex];
        ProcessMesh(mesh, pScene);
    }

    for (uint32_t i = 0; i < pNode->mNumChildren; i++)
    {
        ProcessNodeRecursive(pNode->mChildren[i], pScene);
    }
}

void Model::ProcessMesh(aiMesh* pMesh, const aiScene* pScene)
{
    SPDLOG_INFO("process mesh: {}, #vert: {}, #face: {}",
        pMesh->mName.C_Str(), pMesh->mNumVertices, pMesh->mNumFaces);

    std::vector<Vertex> vertices;
    vertices.resize(pMesh->mNumVertices);
    for (uint32_t i = 0; i < pMesh->mNumVertices; i++)
    {
        auto& v = vertices[i];
        v.Pos = glm::vec3(pMesh->mVertices[i].x, pMesh->mVertices[i].y, pMesh->mVertices[i].z);
        v.Normal = glm::vec3(pMesh->mNormals[i].x, pMesh->mNormals[i].y, pMesh->mNormals[i].z);
        v.TexCoord = glm::vec2(pMesh->mTextureCoords[0][i].x, pMesh->mTextureCoords[0][i].y);
    }

    std::vector<uint32_t> indices;
    indices.resize(pMesh->mNumFaces * 3);
    for (uint32_t i = 0; i < pMesh->mNumFaces; i++)
    {
        indices[3*i  ] = pMesh->mFaces[i].mIndices[0];
        indices[3*i+1] = pMesh->mFaces[i].mIndices[1];
        indices[3*i+2] = pMesh->mFaces[i].mIndices[2];
    }

    unique_ptr<Mesh> glMesh = Mesh::CreateOrNull(vertices, indices, GL_TRIANGLES);
    if (pMesh->mMaterialIndex >= 0)
    {
        glMesh->SetMaterial(std::move(mMaterials[pMesh->mMaterialIndex]));
    }
    mMeshs.push_back(std::move(glMesh));
}

void Model::Draw(const Program* pProgram) const
{
    for (auto& mesh: mMeshs)
    {
        mesh->Draw(pProgram);
    }
}