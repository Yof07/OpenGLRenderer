#include "Renderer/Material.h"

#include <stdexcept>

namespace ogl
{

Material::Material(std::shared_ptr<Shader> shader) : m_shader(std::move(shader))
{
    if (!m_shader)
    {
        throw std::runtime_error("Material requires a valid shader");
    }
}

void Material::Bind() const { m_shader->Bind(); }

void Material::SetMat4(const std::string &name, const glm::mat4 &value) const
{
    // 将mat4 uniform参数转交给材质内部使用的Shader
    m_shader->SetMat4(name, value);
}

void Material::SetVec3(const std::string &name, const glm::vec3 &value) const
{
    // 将vec3 uniform参数转交给材质内部使用的Shader
    m_shader->SetVec3(name, value);
}

void Material::SetFloat(const std::string &name, float value) const
{
    // 将float uniform参数转交给材质内部使用的Shader
    m_shader->SetFloat(name, value);
}

} // namespace ogl
