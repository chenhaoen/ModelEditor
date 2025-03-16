#include <iostream>

#include "IO/ReadNode.h"
#include "IO/ReadImage.h"

#include "Core/Mesh.h"
#include "Core/Node.h"
#include "Core/Material.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

void processMesh(std::shared_ptr<Mesh> mesh, aiMesh* aiMesh, const aiScene* scene)
{
    for (unsigned int i = 0; i < aiMesh->mNumVertices; i++)
    {
        Vertex vertex;
        vertex.pos = glm::vec3(aiMesh->mVertices[i].x, aiMesh->mVertices[i].y, aiMesh->mVertices[i].z);

        for (uint32_t j = 0; j < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++j)
        {
            aiVector3D* textureCoord = aiMesh->mTextureCoords[j];

            if (!textureCoord)
            {
                continue;
            }

            vertex.texCoord = glm::vec2(aiMesh->mTextureCoords[j][i].x, aiMesh->mTextureCoords[j][i].y);
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
    // ����ڵ����е���������еĻ���
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* aiMesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(mesh, aiMesh, scene);
    }
    // �������������ӽڵ��ظ���һ����
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
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    processNode(mesh, scene->mRootNode, scene);
    node->setMesh(mesh);

    auto material = std::make_shared<Material>();
    for (unsigned int i = 0; i < scene->mNumMaterials; ++i) {
        const aiMaterial* material = scene->mMaterials[i];

        aiString name;
        if (material->Get(AI_MATKEY_NAME, name) == AI_SUCCESS) {
            std::cout << "  Name: " << name.C_Str() << std::endl;
        }

        // ��ȡ��������ɫ
        aiColor3D diffuseColor;
        if (material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor) == AI_SUCCESS) {
            std::cout << "  Diffuse Color: (" << diffuseColor.r << ", " << diffuseColor.g << ", " << diffuseColor.b << ")" << std::endl;
        }

        // ��ȡ���淴����ɫ
        aiColor3D specularColor;
        if (material->Get(AI_MATKEY_COLOR_SPECULAR, specularColor) == AI_SUCCESS) {
            std::cout << "  Specular Color: (" << specularColor.r << ", " << specularColor.g << ", " << specularColor.b << ")" << std::endl;
        }

        // ��ȡ�����
        float shininess;
        if (material->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS) {
            std::cout << "  Shininess: " << shininess << std::endl;
        }

        // ��ȡ����������·��
        aiString texturePath;
        if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == AI_SUCCESS) {
            std::cout << "  Diffuse Texture: " << texturePath.C_Str() << std::endl;
        }

        // ��ȡ��������·��
        if (material->GetTexture(aiTextureType_NORMALS, 0, &texturePath) == AI_SUCCESS) {
            std::cout << "  Normal Texture: " << texturePath.C_Str() << std::endl;
        }

        // ��ȡ���������羵�淴�䡢������ȣ�
        if (material->GetTexture(aiTextureType_SPECULAR, 0, &texturePath) == AI_SUCCESS) {
            std::cout << "  Specular Texture: " << texturePath.C_Str() << std::endl;
        }
    }

    auto image = readImage("E:/code/ModelEditer/build/bin/Debug/models/viking_room.png");
    material->setImage(image);
    node->setMaterial(material);

    return node;
}