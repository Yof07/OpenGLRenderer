#pragma once

#include "Scene/Camera.h"
#include "Scene/GameObject.h"

#include <memory>
#include <vector>

namespace ogl
{

// 管理场景相机和场景对象
class Scene
{
public:
    /** 获取场景相机 */
    Camera &GetCamera();

    /** 获取场景相机 */
    const Camera &GetCamera() const;

    /** 向场景中添加对象 */
    GameObject &AddGameObject(std::unique_ptr<GameObject> gameObject);

    /** 绘制场景中的所有对象 */
    void Draw(float aspectRatio) const;

private:
    // 当前场景使用的相机
    Camera m_camera;

    // 当前场景中的所有对象
    std::vector<std::unique_ptr<GameObject>> m_gameObjects;
};

} // namespace ogl
