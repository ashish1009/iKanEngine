#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in vec3 a_Tangent;
layout(location = 4) in vec3 a_bitangent;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec3 v_Position;
out vec3 v_Normal;
out vec2 v_TexCoord;

void main()
{
    v_Position = vec3(u_Transform * vec4(a_Position, 1.0));
    v_Normal   = mat3(transpose(inverse(u_Transform))) * a_Normal;
    v_TexCoord = a_TexCoord;

    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

struct Material
{
    float Shininess;
};

struct Light
{
    bool IsAmbient;
    bool IsDiffuse;
    bool IsSpecular;
    
    vec3 Position;
    
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
};

struct PointLight
{
    bool Present;
    
    float Constant;
    float Linear;
    float Quadratic;
};

struct SpotLight
{
    vec3 Direction;

    bool Present;
    
    float CutOff;
    float OuterCutOff;
};

in vec3 v_Position;
in vec3 v_Normal;
in vec2 v_TexCoord;

uniform bool u_IsSceneLight;
uniform bool u_Blinn;

uniform vec3 u_ViewPos;

uniform Material   u_Material;
uniform Light      u_Light;
uniform PointLight u_PointLight;
uniform SpotLight  u_SpotLight;

uniform sampler2D u_Textures[16];
uniform int u_NumTextureSlots;

// calculates the color when using a directional light. with point light flag.
vec4 CalcDirLight(vec3 norm, vec3 viewDir, bool pointLight, bool spotLight)
{
    vec3 result   = vec3(0.0f, 0.0f, 0.0f);
    vec3 lightDir = normalize(u_Light.Position - v_Position);
    
    float attenuation;
    float intensity;
    if (pointLight || spotLight)
    {
        float distance = length(u_Light.Position - v_Position);
        attenuation = 1.0 / (u_PointLight.Constant + u_PointLight.Linear * distance + u_PointLight.Quadratic * (distance * distance));
        
        if (spotLight)
        {
            // spotlight intensity
            float theta   = dot(lightDir, normalize(-u_SpotLight.Direction));
            float epsilon = u_SpotLight.CutOff - u_SpotLight.OuterCutOff;
            intensity     = clamp((theta - u_SpotLight.OuterCutOff) / epsilon, 0.0, 1.0);
        }
    }
    
    int slotBinded = 0;
    if (slotBinded < u_NumTextureSlots)
    {
        if (u_Light.IsAmbient)
        {
            // ambient
            vec3 ambient = u_Light.Ambient * texture(u_Textures[0], v_TexCoord).rgb; // Diffuse Texture
            if (pointLight || spotLight)
            {
                ambient *= attenuation;
                if (spotLight)
                    ambient *= intensity;
            }
            result += ambient;
        }
        
        if (u_Light.IsDiffuse)
        {
            // diffuse
            float diff    = max(dot(norm, lightDir), 0.0);
            vec3  diffuse = u_Light.Diffuse * diff * texture(u_Textures[0], v_TexCoord).rgb; // Diffuse Texture
            if (pointLight || spotLight)
            {
                diffuse *= attenuation;
                if (spotLight)
                    diffuse *= intensity;
            }
            result += diffuse;
        }
        
        if (u_Light.IsDiffuse || u_Light.IsAmbient)
            slotBinded++;
    }
    
    if (u_Light.IsSpecular)
    {
        // specular
        vec3  reflectDir = reflect(-lightDir, norm);
        float spec;
        vec3  specular;
        if (slotBinded < u_NumTextureSlots)
        {
            spec     = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.Shininess);
            specular = u_Light.Specular * spec * texture(u_Textures[1], v_TexCoord).rgb; // Specular Texture
            slotBinded++;
        }
        else
        {
            if (u_Blinn)
            {
                vec3 halfwayDir = normalize(lightDir + viewDir);
                spec = pow(max(dot(norm, halfwayDir), 0.0), 32.0);
            }
            else
            {
                spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
            }
            specular = vec3(0.3) * spec;
        }
        
        if (pointLight || spotLight)
        {
            specular *= attenuation;
            if (spotLight)
                specular *= intensity;
        }
        result += specular;
    }
    return vec4(result, 1.0f);
}


void main()
{
    // properties
    vec3 norm    = normalize(v_Normal);
    vec3 viewDir = normalize(u_ViewPos - v_Position);
    
    if (u_IsSceneLight)
        color = CalcDirLight(norm, viewDir, u_PointLight.Present, u_SpotLight.Present);
    else
        color = texture(u_Textures[0], v_TexCoord);
}
