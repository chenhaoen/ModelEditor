#include "IO/ReadNode.h"
#include "IO/ReadImage.h"

#include "Core/Mesh.h"
#include "Core/Node.h"
#include "Core/SkyboxNode.h"
#include "Core/Material.h"

void processMesh(std::shared_ptr<Mesh> mesh, aiMesh* aiMesh, const aiScene* scene)
{
    for (unsigned int i = 0; i < aiMesh->mNumVertices; i++)
    {
        Vertex vertex;
        vertex.pos = glm::vec3(aiMesh->mVertices[i].x, aiMesh->mVertices[i].y, aiMesh->mVertices[i].z);
        vertex.normal = glm::vec3(aiMesh->mNormals[i].x, aiMesh->mNormals[i].y, aiMesh->mNormals[i].z);

        for (uint32_t j = 0; j < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++j)
        {
            aiVector3D* textureCoord = aiMesh->mTextureCoords[j];

            if (!textureCoord)
            {
                continue;
            }

            vertex.texCoord = glm::vec3(aiMesh->mTextureCoords[j][i].x, aiMesh->mTextureCoords[j][i].y, aiMesh->mTextureCoords[j][i].z);
        }

        mesh->getVertices().push_back(vertex);
    }

    for (unsigned int i = 0; i < aiMesh->mNumFaces; i++)
    {
        const aiFace& face = aiMesh->mFaces[i];

        for (uint32_t j = 0; j < face.mNumIndices; ++j)
        {
            mesh->getIndices().emplace_back(face.mIndices[j]);
        }
    }
}

void processNode(std::shared_ptr<Mesh> mesh, aiNode* node, const aiScene* scene)
{
    // 处理节点所有的网格（如果有的话）
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* aiMesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(mesh, aiMesh, scene);
    }
    // 接下来对它的子节点重复这一过程
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(mesh, node->mChildren[i], scene);
    }
}

std::shared_ptr<Node> readNode(const std::string_view& file)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(file.data(), aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        return nullptr;
    }

    auto node = std::make_shared<Node>(scene->mRootNode->mName.C_Str());

    auto mesh = std::make_shared<Mesh>();
    processNode(mesh, scene->mRootNode, scene);
    node->setMesh(mesh);

    return node;
}

std::shared_ptr<SkyboxNode> readSkyboxNode(const std::string_view& file)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(file.data(), aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        return nullptr;
    }

    auto node = std::make_shared<SkyboxNode>();

    auto mesh = std::make_shared<Mesh>();
    processNode(mesh, scene->mRootNode, scene);
    node->setMesh(mesh);
    auto material = std::make_shared<Material>();
    node->setMaterial(material);

    return node;
}
