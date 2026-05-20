#include "Scene/GameObject.h"

#include "Scene/Camera.h"
#include "Scene/RenderComponent.h"

namespace ogl
{

GameObject::GameObject(std::unique_ptr<RenderComponent> renderComponent) : m_renderComponent(std::move(renderComponent))
{
}

GameObject::~GameObject() = default;

Transform &GameObject::GetTransform() { return m_transform; }

const Transform &GameObject::GetTransform() const { return m_transform; }

void GameObject::SetRenderComponent(std::unique_ptr<RenderComponent> renderComponent)
{
    m_renderComponent = std::move(renderComponent);
}

void GameObject::Draw(const Camera &camera, float aspectRatio) const
{
    // 没有渲染组件的对象不会被绘制
    if (!m_renderComponent)
    {
        return;
    }

    // 把对象自己的Transform交给渲染组件，完成本对象绘制
    m_renderComponent->Draw(m_transform, camera, aspectRatio);
}

} // namespace ogl
