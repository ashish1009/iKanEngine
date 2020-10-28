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

struct DirLight
{
    bool IsAmbient;
    bool IsDiffuse;
    bool IsSpecular;
    
    vec3 Position;
    
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
};

in vec3 v_Position;
in vec3 v_Normal;
in vec2 v_TexCoord;

uniform bool     u_IsSceneLight;
uniform vec3     u_ViewPos;
uniform Material u_Material;
uniform DirLight u_DirLight;

uniform sampler2D u_Textures[16];
uniform int u_NumTextureSlots;

vec4 CalcDirLight(vec3 norm, vec3 viewDir)
{
    vec3 result   = vec3(0.0f, 0.0f, 0.0f);
    vec3 lightDir = normalize(u_DirLight.Position - v_Position);

    int slotBinded = 0;
    if (slotBinded < u_NumTextureSlots)
    {
        if (u_DirLight.IsAmbient)
        {
            // ambient
            vec3 ambient = u_DirLight.Ambient * texture(u_Textures[0], v_TexCoord).rgb; // Diffuse Texture
            result += ambient;
        }
        
        if (u_DirLight.IsDiffuse)
        {
            // diffuse
            float diff    = max(dot(norm, lightDir), 0.0);
            vec3  diffuse = u_DirLight.Diffuse * diff * texture(u_Textures[0], v_TexCoord).rgb; // Diffuse Texture
            result += diffuse;
        }

        if (u_DirLight.IsDiffuse || u_DirLight.IsAmbient)
            slotBinded++;
    }
    
    if (slotBinded < u_NumTextureSlots)
    {
        if (u_DirLight.IsSpecular)
        {
            // specular
            vec3  reflectDir = reflect(-lightDir, norm);
            float spec       = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.Shininess);
            vec3  specular   = u_DirLight.Specular * spec * texture(u_Textures[1], v_TexCoord).rgb; // Specular Texture
            result += specular;
            slotBinded++;
        }
    }
    return vec4(result, 1.0f);
}

void main()
{
    // properties
    vec3 norm    = normalize(v_Normal);
    vec3 viewDir = normalize(u_ViewPos - v_Position);

    if (u_IsSceneLight)
        color = CalcDirLight(norm, viewDir);
    else
        color = texture(u_Textures[0], v_TexCoord);
}
