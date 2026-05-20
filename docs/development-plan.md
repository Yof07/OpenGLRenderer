# OpenGLRenderer 开发计划

## 参考项目描述

参考项目名称：

```text
《基于OpenGL的渲染器》
```

参考项目时间：

```text
2026.01-至今
```

参考项目能力画像：

- 渲染核心架构搭建
  - 基于GLAD/GLFW搭建OpenGL渲染底层框架
  - 设计“场景-游戏对象-渲染组件”三层架构
  - 封装Shader、Mesh、Material、Texture、Buffer、VertexArray等核心渲染类
  - 实现渲染资源的模块化管理

- 光影系统开发
  - 实现Shadow Mapping阴影系统
  - 封装ShadowMap类管理深度纹理和FBO
  - 支持PCF软阴影
  - 实现方向光、点光源、聚光灯三种光源类型
  - 支持多光源的实时渲染与衰减计算

- 后处理系统开发
  - 设计通用后处理管理器
  - 支持可扩展的链式后处理流程
  - 通过Ping-Pong FBO技术解决多步后处理的帧缓冲读写冲突
  - 保证渲染结果正确性
  - 内置Bloom等典型后处理效果

- 相机系统实现
  - 封装鼠标、键盘输入回调
  - 实现视角旋转、位移、裁剪面、灵敏度等参数动态调节
  - 保障3D场景交互的流畅性

## 计划原则

本项目以参考项目《基于OpenGL的渲染器》为主干能力边界：

- 参考项目已有的核心能力，本项目在能力层面尽量覆盖
- 覆盖能力不等于照搬实现，代码结构、命名、封装边界和实现顺序都按本项目的学习路线重新设计
- 改进点是在同类能力基础上增强，而不是用改进点替代原功能
- 第一版避免过度封装，优先保证每条数据流能跑通、能解释、能用RenderDoc验证

第一版主干能力：

```text
组件式场景架构
-> OBJ模型导入和Texture材质落地
-> Blinn-Phong多光源
-> 交互式CameraController
-> Shadow Mapping + PCF
-> HDR SceneFBO
-> 轻量Pass化Bloom后处理
-> Tone Mapping / Gamma Correction
-> ImGui调参
-> RenderDoc复盘
```

本项目相对参考项目的主要改进：

- 后处理从写死Bloom流程整理为轻量Pass管线
- Bloom采用HDR SceneFBO，补全Tone Mapping和Gamma Correction
- 使用ImGui调节光照、相机、阴影和后处理参数
- 使用RenderDoc检查ShadowMap、SceneFBO、BrightTexture、Ping-Pong Blur和最终Composite结果
- 文档按阶段记录数据如何从CPU侧变成屏幕画面

## 与参考项目的关系

本项目不是复刻参考项目源码，而是在参考项目能力画像上做自己的实现。

能力对齐：

- 都有GLAD/GLFW底层框架
- 都有场景、游戏对象、渲染组件的组织方式
- 都有Shader、Mesh、Material、Texture、Buffer、VertexArray等核心渲染资源
- 都有Shadow Mapping、ShadowMap、PCF、多光源
- 都有后处理流程、Ping-Pong FBO和Bloom
- 都有鼠标键盘控制的交互相机

实现差异：

- 本项目按照学习顺序逐步搭建，不一次性堆完整框架
- 本项目优先记录每阶段数据流、API、问题和面试问法
- 本项目的后处理会在参考项目Bloom能力基础上，明确拆成HDR SceneFBO、BloomPass、PingPongBuffer、Tone Mapping和Gamma Correction
- 本项目先做够用的轻量PostProcessPipeline，不做RenderGraph、RenderTargetPool和Pass自动依赖分析
- 本项目要求阶段完成后编译运行，并使用RenderDoc做最终复盘

## 阶段 0：工程初始化

目标：项目能编译，后续可以接入 GLFW / GLAD。

验收：

- CMake 配置成功
- 程序能运行

## 阶段 1：第一个三角形

目标：跑通最小 OpenGL 渲染链路。

数据流：

```text
CPU 顶点数组
-> VBO
-> VAO
-> Vertex Shader
-> Rasterization
-> Fragment Shader
-> 默认帧缓冲
-> 屏幕
```

## 阶段 2：核心渲染资源封装

目标：将 OpenGL 资源生命周期和绑定逻辑收敛到类中。

阶段2先完成直接支撑几何绘制的基础类：

- Shader
- Buffer
- VertexArray
- Mesh
- Material

随后在对应功能阶段补齐专用资源类：

- Texture在阶段5贴图材质中补齐
- Framebuffer在阶段7 HDR渲染中补齐

## 阶段 3：ImGui 调试面板

目标：运行时调节光照、阴影和后处理参数。

后续会用于调节：

- Camera移动速度、鼠标灵敏度、FOV、near/far裁剪面
- 方向光、点光源、聚光灯参数
- Shadow bias、PCF开关或kernel大小
- Bloom threshold、intensity、blur iterations
- Exposure和Gamma

## 阶段 4：相机交互与场景架构

目标：实现组件式场景组织，并补齐参考项目中的交互式相机能力。

当前阶段拆分：

- 阶段4基础版：Camera、Transform、GameObject、RenderComponent、Scene，已完成
- 阶段4补齐：CameraController交互相机，已完成
- 阶段4补充：鼠标滚轮调节Camera FOV，已完成

结构：

```text
Scene
-> GameObject
-> Transform
-> RenderComponent
-> Mesh + Material
```

相机系统：

- `Camera` 保存观察矩阵和投影矩阵需要的参数
- `CameraController` 处理键盘和鼠标输入
- WASD控制相机移动
- 鼠标控制视角旋转
- 鼠标滚轮调节FOV
- 支持调节FOV、near/far裁剪面
- 支持调节移动速度和鼠标灵敏度
- ImGui显示和调节相机参数

验收：

```text
可以用键盘和鼠标从不同角度观察3D模型，滚轮可以调节FOV，并能在ImGui中调节相机参数
```

## 阶段 5：OBJ 模型导入

目标：使用 Assimp 导入 OBJ 模型，并生成 Mesh / Material。

当前阶段拆分：

- 阶段5基础版：导入OBJ几何数据并渲染测试模型，已完成
- 阶段5补齐：Texture封装和Diffuse / Specular贴图材质，阶段4补齐后完成

第一步先导入几何数据：

- position
- normal
- texCoord
- index
- 一个Model包含多个Mesh

参考项目明确包含Texture核心类；本项目后续用Diffuse / Specular贴图作为Texture能力的具体落地：

- `Texture` 封装
- diffuse texture
- specular texture
- `Material` 绑定Shader、Texture和Uniform参数

验收：

```text
能导入OBJ模型，并支持基础Diffuse / Specular贴图材质
```

## 阶段 6：Blinn-Phong 多光源

目标：实现方向光、点光源、聚光灯和贴图光照。

## 阶段 7：Framebuffer 与 HDR

目标：将场景先渲染到 HDR SceneFBO，再输出到屏幕。

核心任务：

- 封装 `Framebuffer`
- 创建HDR颜色附件，格式使用 `GL_RGBA16F`
- 创建深度模板附件
- 创建屏幕四边形
- 将SceneFBO颜色纹理绘制回默认Framebuffer

理解重点：

- 默认Framebuffer和自定义Framebuffer的区别
- FBO本身是容器，颜色纹理才保存画面
- HDR颜色纹理为什么可以保存超过 `1.0` 的亮度
- 后处理为什么必须先把场景画成一张纹理

## 阶段 8：Shadow Mapping + PCF

目标：实现深度贴图阴影和 PCF 软阴影。

核心任务：

- 封装 `ShadowMap`
- 创建只保存深度的FBO
- 从光源视角渲染Depth Pass
- 主渲染Pass采样ShadowMap
- 实现bias避免阴影痤疮
- 实现PCF软阴影
- ImGui调节bias和PCF参数

## 阶段 9：Bloom 后处理

目标：在参考项目Bloom + Ping-Pong FBO的基础上，实现轻量Pass化HDR Bloom后处理流程。

核心类：

- `PostProcessPass`
  - 后处理Pass的基础接口
  - 每个Pass负责一次屏幕空间处理

- `PostProcessPipeline`
  - 管理后处理Pass执行顺序
  - 第一版只需要串联Bloom相关Pass，不做RenderGraph

- `BloomPass`
  - 管理亮部提取、Ping-Pong Blur和Bloom合成
  - 作为第一版实际接入的后处理Pass

- `PingPongBuffer`
  - 封装两个FBO和两张颜色纹理
  - 用于多次模糊时交替读写，避免同一纹理同时读写

不做的过度封装：

- RenderGraph
- RenderTargetPool
- Pass自动依赖分析
- 后处理插件注册系统
- Shader反射系统

流程：

```text
HDR SceneFBO
-> Bright Extract
-> Ping-Pong Gaussian Blur
-> Bloom Composite
-> Tone Mapping
-> Gamma Correction
-> Screen
```

ImGui参数：

- Bloom开关
- threshold
- intensity
- blur iterations
- exposure
- gamma

RenderDoc验证：

- SceneFBO颜色附件
- BrightTexture亮部提取结果
- PingPongBuffer每轮模糊结果
- Bloom Composite合成结果
- Tone Mapping / Gamma Correction后的最终输出

相对参考项目的改进：

```text
参考项目：Bloom + Ping-Pong FBO
本项目：HDR SceneFBO + 轻量PostProcessPipeline + BloomPass + PingPongBuffer + Tone Mapping + Gamma Correction + ImGui调参 + RenderDoc验证
```

## 阶段 10：整理与复盘

目标：完成 RenderDoc 截帧分析、README、面试问答和截图演示。
