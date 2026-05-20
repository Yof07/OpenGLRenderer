#pragma once

#include "Renderer/Shader.h"

#include <glm/glm.hpp>

#include <memory>
#include <string>

namespace ogl
{

// 管理Shader和后续材质参数的封装类
class Material
{
public:
    /** 使用已有Shader创建材质 */
    explicit Material(std::shared_ptr<Shader> shader);

    /** 绑定材质使用的Shader */
    void Bind() const;

    /** 给材质使用的Shader传入mat4 uniform变量 */
    void SetMat4(const std::string &name, const glm::mat4 &value) const;

    /** 给材质使用的Shader传入vec3 uniform变量 */
    void SetVec3(const std::string &name, const glm::vec3 &value) const;

    /** 给材质使用的Shader传入float uniform变量 */
    void SetFloat(const std::string &name, float value) const;

private:
    // 当前材质使用的Shader
    std::shared_ptr<Shader> m_shader;
};

} // namespace ogl
