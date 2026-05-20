#include "Scene/RenderComponent.h"

#include "Renderer/Material.h"
#include "Renderer/Model.h"
#include "Scene/Camera.h"
#include "Scene/Transform.h"

#include <stdexcept>
#include <utility>

namespace ogl
{

RenderComponent::RenderComponent(std::shared_ptr<Model> model, std::shared_ptr<Material> material)
    : m_model(std::move(model)), m_material(std::move(material))
{
    if (!m_model)
    {
        throw std::runtime_error("RenderComponent model is null");
    }

    if (!m_material)
    {
        throw std::runtime_error("RenderComponent material is null");
    }
}

void RenderComponent::Draw(const Transform &transform, const Camera &camera, float aspectRatio) const
{
    // 指定让当前材质里的ShaderProgram来进行绘制
    m_material->Bind();

    // 将本次绘制需要的矩阵参数交给Material
    m_material->SetMat4("u_Model", transform.GetModelMatrix());
    m_material->SetMat4("u_View", camera.GetViewMatrix());
    m_material->SetMat4("u_Projection", camera.GetProjectionMatrix(aspectRatio));

    // 使用Mesh提交Draw Call，用当前OpenGL已经绑定好的shader program来绘制
    m_model->Draw();
}

} // namespace ogl
