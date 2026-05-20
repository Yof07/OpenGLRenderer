# OpenGLRenderer - Codex 协作规则

## 项目定位

- 项目名：`OpenGLRenderer`
- 类型：基于 C++ / OpenGL 的小型实时渲染器
- 当前目标：循序渐进完成渲染器，并在每个阶段理解数据如何从CPU侧变成屏幕画面
- 技术栈：CMake、GLFW、GLAD、OpenGL、GLSL，后续加入 ImGui、Assimp、stb_image

## 对话与开发方式

- 用户希望自己手敲核心代码
- Codex 不要一次性输出大量文件
- 每次只给出一个 `.h`、`.cpp` 或 `.glsl` 文件的内容
- 每次给出代码文件时，要对该代码文件进行说明
- 如果是旧代码文件做出改动，要指明具体的改动内容，如增删改了哪些变量或函数等
- 用户再次提问“下一个”或类似表达时，才给下一个文件
- 每个阶段开始前，先说明：
  - 这一阶段要做什么
  - 为什么现在做它
  - 大概会创建哪些文件
  - 代码设计思路是什么
  - 这一阶段要理解什么
- 讲解要遵循自然理解顺序，优先解释数据流和职责边界
- 不要过度封装，先实现当前阶段真正需要的内容

## 哪些内容由 Codex 直接改

- 文档由 Codex 维护
- `CMakeLists.txt` 由 Codex 维护
- `.vscode`、`.clang-format`、`compile_flags.txt` 等项目配置可以由 Codex 维护
- 注释类改动可以由 Codex 直接写
- 核心业务代码默认给用户手敲，除非用户明确要求 Codex 直接修改

## 文档维护规则

- 每完成一个阶段，Codex 负责更新：
  - `TODO.md`
  - `docs/learning-log.md`
  - `docs/pipeline.md`
- 如果新增 OpenGL / GLFW / GLAD / GLSL API，先更新：
  - `docs/api-notes.md`
- 在给出下一个代码文件之前，必须先把本文件新增或复用的重要API写入 `docs/api-notes.md`
- 每个阶段完成后，Codex负责帮用户编译并运行程序进行验证
- 文档应强调：
  - 当前实现了什么
  - 数据如何流动
  - 理解了什么
  - 遇到的问题
  - 面试可能怎么问
- 用户之前要求文档不同步到GitHub，当前 `.gitignore` 已忽略 `docs/`、`README.md`、`TODO.md`

## 注释风格

- 注释使用中文
- 注释措辞要直白，少用含混术语
- 注释说明“这段代码具体做什么”
- 英文单词和中文之间不需要额外空格，例如写成 `创建OpenGL Buffer对象`
- 函数声明前使用单行文档注释，格式是 `/** 说明内容 */`
- 不要写成多行星号格式，例如不要写 `/**` 后面每行 `*` 的形式

```cpp
/** 绑定当前Shader Program */
void Bind() const;
```

- 函数声明注释要说明这个函数整体负责什么
- 函数定义内部按功能块写普通行注释，说明这一块代码具体做什么
- 同一个小功能块里的连续代码不要用空行拆碎
- 只有切换到不同功能块时才用空行分隔，并在该功能块前写注释
- 类说明使用普通行注释
- 类成员变量分组注释使用 `/* 分组说明 */`
- 类成员变量必须在声明前使用 `//` 注释，说明它保存什么或控制什么

```cpp
// 管理OpenGL Buffer对象的封装类
class Buffer
{
private:
    // OpenGL Buffer对象ID
    unsigned int m_id = 0;
};
```

- GLSL文件开头必须写概括注释，说明这个shader文件负责什么
- 代码注释不要写句号

## 格式化规则

- 使用项目根目录 `.clang-format`
- 当前期望风格：
  - 行宽 `ColumnLimit: 120`
  - 大括号单独占一行
  - 缩进4空格
  - `public:` / `private:` 贴近类体左侧

## 当前学习路线

- 阶段0：GLFW窗口、OpenGL Context、GLAD、Render Loop
- 阶段1：三角形、Shader、VAO、VBO、Draw Call
- 阶段2：封装 `Shader`、`Buffer`、`VertexArray`、`Mesh`、`Material`
- 阶段3：接入 ImGui 调试面板
- 阶段4：Camera、CameraController、Transform、GameObject、RenderComponent、Scene
- 阶段5：Assimp 导入 OBJ 模型、Texture 贴图材质
- 阶段6：Blinn-Phong 多光源
- 阶段7：SceneFBO 和 HDR
- 阶段8：Shadow Mapping + PCF
- 阶段9：Bloom + Ping-Pong FBO + Tone Mapping + Gamma Correction
- 阶段10：RenderDoc 截帧分析、README、面试问答整理
