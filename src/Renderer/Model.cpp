#include "Renderer/Model.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <stdexcept>

namespace ogl
{

Model::Model(const std::string &path) { LoadModel(path); }

void Model::Draw() const
{
    // 一个Model可能包含多个Mesh，所以绘制时要逐个提交
    for (const auto &mesh : m_meshes)
    {
        mesh->Draw();
    }
}

void Model::LoadModel(const std::string &path)
{
    Assimp::Importer importer;

    // 读取模型文件，并让Assimp把面片转成三角形、翻转UV、补充缺失法线
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

    // 检查模型是否读取成功
    if (scene == nullptr || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || scene->mRootNode == nullptr)
    {
        throw std::runtime_error("Failed to load model: " + std::string(importer.GetErrorString()));
    }

    // 从根节点开始递归处理整个模型层级
    ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode *node, const aiScene *scene)
{
    // 处理当前节点引用的所有Mesh，存储到m_meshes中
    for (unsigned int i = 0; i < node->mNumMeshes; ++i)
    {
        const unsigned int meshIndex = node->mMeshes[i];
        aiMesh *mesh = scene->mMeshes[meshIndex];

        m_meshes.push_back(ProcessMesh(mesh, scene));
    }

    // 递归处理当前节点的所有子节点
    for (unsigned int i = 0; i < node->mNumChildren; ++i)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}

std::unique_ptr<Mesh> Model::ProcessMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    // 预先分配空间，减少vector扩容次数
    vertices.reserve(mesh->mNumVertices);

    // 将Assimp顶点数据转换成项目自己的Vertex数据
    for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
    {
        Vertex vertex;

        // 读取顶点位置
        vertex.position = {
            mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z,
        };

        // 当前阶段先给模型一个默认颜色，后续会由材质和贴图决定颜色
        vertex.color = {0.8f, 0.8f, 0.8f};

        // 读取顶点法线，如果模型没有法线则使用默认方向
        if (mesh->HasNormals())
        {
            vertex.normal = {
                mesh->mNormals[i].x,
                mesh->mNormals[i].y,
                mesh->mNormals[i].z,
            };
        }

        // 读取第0套纹理坐标，如果模型没有纹理坐标则使用默认值
        if (mesh->HasTextureCoords(0))
        {
            vertex.texCoord = {
                mesh->mTextureCoords[0][i].x,
                mesh->mTextureCoords[0][i].y,
            };
        }

        vertices.push_back(vertex);
    }

    // 将Assimp的面索引转换成项目自己的索引数组
    for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
    {
        const aiFace &face = mesh->mFaces[i];

        for (unsigned int j = 0; j < face.mNumIndices; ++j)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    return std::make_unique<Mesh>(vertices, indices);
}

} // namespace ogl
