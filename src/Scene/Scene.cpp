#include "Scene/Scene.h"

#include <stdexcept>

namespace ogl
{

Camera &Scene::GetCamera() { return m_camera; }

const Camera &Scene::GetCamera() const { return m_camera; }

GameObject &Scene::AddGameObject(std::unique_ptr<GameObject> gameObject)
{
    if (!gameObject)
    {
        throw std::runtime_error("Scene gameObject is null");
    }

    // 保存对象所有权，并返回对象引用方便外部继续设置它
    m_gameObjects.push_back(std::move(gameObject));
    return *m_gameObjects.back();
}

void Scene::Draw(float aspectRatio) const
{
    // 遍历场景中的所有对象，并使用场景相机绘制它们
    for (const auto &gameObject : m_gameObjects)
    {
        gameObject->Draw(m_camera, aspectRatio);
    }
}

} // namespace ogl
