#pragma once

#include <memory>

namespace ogl
{

class Camera;
class Material;
class Model;
class Transform;

// 表示一个对象身上的渲染组件
class RenderComponent
{
public:
    /** 创建渲染组件 */
    RenderComponent(std::shared_ptr<Model> model, std::shared_ptr<Material> material);

    /** 使用当前组件的Model和Material绘制物体 */
    void Draw(const Transform &transform, const Camera &camera, float aspectRatio) const;

private:
    // 当前组件要绘制的模型资源
    std::shared_ptr<Model> m_model;

    // 当前组件要使用的材质
    std::shared_ptr<Material> m_material;
};

} // namespace ogl
