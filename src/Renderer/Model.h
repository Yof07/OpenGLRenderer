#pragma once

#include "Renderer/Mesh.h"

#include <memory>
#include <string>
#include <vector>

struct aiMesh;
struct aiNode;
struct aiScene;

namespace ogl
{

// 管理从模型文件导入的一整个模型
class Model
{
public:
    /** 从模型文件路径创建Model */
    explicit Model(const std::string &path);

    /** 绘制当前Model包含的所有Mesh */
    void Draw() const;

private:
    /** 读取模型文件并开始转换Assimp数据 */
    void LoadModel(const std::string &path);

    /** 递归处理Assimp节点 */
    void ProcessNode(aiNode *node, const aiScene *scene);

    /** 将Assimp的aiMesh转换成项目自己的Mesh */
    std::unique_ptr<Mesh> ProcessMesh(aiMesh *mesh, const aiScene *scene);

private:
    // 当前Model包含的所有Mesh
    std::vector<std::unique_ptr<Mesh>> m_meshes;
};

} // namespace ogl
