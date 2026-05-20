#version 430 core

// 阶段6的光照片元着色器
// 根据世界空间位置、法线、相机位置、方向光、点光源和聚光灯计算Blinn-Phong光照

// 当前片元在世界空间中的位置
in vec3 vWorldPosition;

// 当前片元在世界空间中的法线方向
in vec3 vNormal;

// 当前片元插值后的顶点颜色
in vec3 vColor;

struct PointLight
{
    // 点光源在世界空间中的位置
    vec3 position;

    // 点光源的颜色
    vec3 color;

    // 点光源衰减公式中的常数项
    float constant;

    // 点光源衰减公式中的一次项
    float linear;

    // 点光源衰减公式中的二次项
    float quadratic;
};

struct SpotLight
{
    // 聚光灯在世界空间中的位置
    vec3 position;

    // 聚光灯的照射方向
    vec3 direction;

    // 聚光灯的颜色
    vec3 color;

    // 聚光灯内圈边界的余弦值
    float cutOff;

    // 聚光灯外圈边界的余弦值
    float outerCutOff;

    // 聚光灯衰减公式中的常数项
    float constant;

    // 聚光灯衰减公式中的一次项
    float linear;

    // 聚光灯衰减公式中的二次项
    float quadratic;
};

// 相机在世界空间中的位置
uniform vec3 u_ViewPosition;

// 物体基础颜色
uniform vec3 u_ObjectColor;

// 方向光的照射方向
uniform vec3 u_LightDirection;

// 方向光的颜色
uniform vec3 u_LightColor;

// 当前使用的点光源参数
uniform PointLight u_PointLight;

// 当前使用的聚光灯参数
uniform SpotLight u_SpotLight;

// 环境光强度
uniform float u_AmbientStrength;

// 高光强度
uniform float u_SpecularStrength;

// 高光指数，数值越大高光越集中
uniform float u_Shininess;

// 当前片元最终输出到framebuffer的颜色
out vec4 FragColor;

vec3 CalculateDirectionalLight(
    vec3 normal,        // 当前片元的世界空间法线方向
    vec3 viewDirection) // 当前片元指向相机的方向
{
    // 当前片元指向方向光光源的方向
    vec3 lightDirection = normalize(-u_LightDirection);

    // 漫反射亮度系数
    float diffuseFactor = max(dot(normal, lightDirection), 0.0);

    // 方向光漫反射颜色
    vec3 diffuse = diffuseFactor * u_LightColor;

    // 法线方向和视线方向之间的半程向量
    vec3 halfwayDirection = normalize(lightDirection + viewDirection);

    // 高光亮度系数
    float specularFactor = 0.0;

    if (diffuseFactor > 0.0)
    {
        specularFactor = pow(max(dot(normal, halfwayDirection), 0.0), u_Shininess);
    }

    // 方向光高光颜色
    vec3 specular = u_SpecularStrength * specularFactor * u_LightColor;

    return diffuse + specular;
}

vec3 CalculatePointLight(
    vec3 normal,        // 当前片元的世界空间法线方向
    vec3 viewDirection) // 当前片元指向相机的方向
{
    // 当前片元指向点光源的向量
    vec3 toLight = u_PointLight.position - vWorldPosition;

    // 当前片元到点光源的距离
    float distanceToLight = length(toLight);

    // 当前片元指向点光源的方向
    vec3 lightDirection = normalize(toLight);

    // 点光源漫反射亮度系数
    float diffuseFactor = max(dot(normal, lightDirection), 0.0);

    // 点光源漫反射颜色
    vec3 diffuse = diffuseFactor * u_PointLight.color;

    // 点光源方向和视线方向之间的半程向量
    vec3 halfwayDirection = normalize(lightDirection + viewDirection);

    // 点光源高光亮度系数
    float specularFactor = 0.0;

    if (diffuseFactor > 0.0)
    {
        specularFactor = pow(max(dot(normal, halfwayDirection), 0.0), u_Shininess);
    }

    // 点光源高光颜色
    vec3 specular = u_SpecularStrength * specularFactor * u_PointLight.color;

    // 点光源衰减公式的分母
    float denominator = u_PointLight.constant +
                        u_PointLight.linear * distanceToLight +
                        u_PointLight.quadratic * distanceToLight * distanceToLight;

    // 点光源衰减系数
    float attenuation = 1.0 / max(denominator, 0.0001);

    return (diffuse + specular) * attenuation;
}

vec3 CalculateSpotLight(
    vec3 normal,        // 当前片元的世界空间法线方向
    vec3 viewDirection) // 当前片元指向相机的方向
{
    // 当前片元指向聚光灯的向量
    vec3 toLight = u_SpotLight.position - vWorldPosition;

    // 当前片元到聚光灯的距离
    float distanceToLight = length(toLight);

    // 当前片元指向聚光灯的方向
    vec3 lightDirection = normalize(toLight);

    // 聚光灯中心轴的反方向，用于和片元指向光源方向比较
    vec3 spotDirection = normalize(-u_SpotLight.direction);

    // 当前片元相对聚光灯中心方向的夹角余弦值
    float theta = dot(lightDirection, spotDirection);

    // 内圈和外圈之间的过渡范围
    float epsilon = u_SpotLight.cutOff - u_SpotLight.outerCutOff;

    // 聚光灯强度，内圈为1，外圈外为0，中间平滑过渡
    float intensity = clamp((theta - u_SpotLight.outerCutOff) / max(epsilon, 0.0001), 0.0, 1.0);

    // 聚光灯漫反射亮度系数
    float diffuseFactor = max(dot(normal, lightDirection), 0.0);

    // 聚光灯漫反射颜色
    vec3 diffuse = diffuseFactor * u_SpotLight.color;

    // 聚光灯方向和视线方向之间的半程向量
    vec3 halfwayDirection = normalize(lightDirection + viewDirection);

    // 聚光灯高光亮度系数
    float specularFactor = 0.0;

    if (diffuseFactor > 0.0)
    {
        specularFactor = pow(max(dot(normal, halfwayDirection), 0.0), u_Shininess);
    }

    // 聚光灯高光颜色
    vec3 specular = u_SpecularStrength * specularFactor * u_SpotLight.color;

    // 聚光灯衰减公式的分母
    float denominator = u_SpotLight.constant +
                        u_SpotLight.linear * distanceToLight +
                        u_SpotLight.quadratic * distanceToLight * distanceToLight;

    // 聚光灯衰减系数
    float attenuation = 1.0 / max(denominator, 0.0001);

    return (diffuse + specular) * attenuation * intensity;
}

void main()
{
    // 当前片元归一化后的世界空间法线方向
    vec3 normal = normalize(vNormal);

    // 当前片元指向相机的方向
    vec3 viewDirection = normalize(u_ViewPosition - vWorldPosition);

    // 环境光颜色
    vec3 ambient = u_AmbientStrength * u_LightColor;

    // 方向光计算结果
    vec3 directionalLight = CalculateDirectionalLight(normal, viewDirection);

    // 点光源计算结果
    vec3 pointLight = CalculatePointLight(normal, viewDirection);

    // 聚光灯计算结果
    vec3 spotLight = CalculateSpotLight(normal, viewDirection);

    // 当前片元的基础颜色
    vec3 baseColor = vColor * u_ObjectColor;

    // 当前片元经过光照计算后的最终RGB颜色
    vec3 result = baseColor * (ambient + directionalLight + pointLight + spotLight);

    FragColor = vec4(result, 1.0);
}