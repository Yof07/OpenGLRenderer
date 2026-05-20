#pragma once

#include "Scene/Transform.h"

#include <memory>

namespace ogl
{

class Camera;
class RenderComponent;

// 表示场景中的一个对象
class GameObject
{
public:
    /** 创建一个空的场景对象 */
    GameObject() = default;

    /** 使用渲染组件创建场景对象 */
    explicit GameObject(std::unique_ptr<RenderComponent> renderComponent);

    /** 释放场景对象 */
    ~GameObject();

    /** 获取对象的Transform */
    Transform &GetTransform();

    /** 获取对象的Transform */
    const Transform &GetTransform() const;

    /** 设置对象的渲染组件 */
    void SetRenderComponent(std::unique_ptr<RenderComponent> renderComponent);

    /** 绘制当前对象 */
    void Draw(const Camera &camera, float aspectRatio) const;

private:
    // 当前对象的位置、旋转和缩放
    Transform m_transform;

    // 当前对象的渲染组件，没有渲染组件时对象不会被绘制
    std::unique_ptr<RenderComponent> m_renderComponent;
};

} // namespace ogl
