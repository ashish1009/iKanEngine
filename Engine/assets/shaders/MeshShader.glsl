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
    v_Position = a_Position;
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
    vec3 Position;
    
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
};

in vec3 v_Position;
in vec3 v_Normal;
in vec2 v_TexCoord;

uniform vec3     u_ViewPos;
uniform Material u_Material;
uniform Light    u_Light;

uniform sampler2D u_Textures[16];
uniform int u_NumTextureSlots;

vec4 GetLightEffect(vec3 lightDir)
{
    int slotBinded = 0;
    
    vec3 result = vec3(0.0f, 0.0f, 0.0f);
    
    vec3 norm = normalize(v_Normal);

    if (slotBinded < u_NumTextureSlots)
    {
        // ambient
        vec3 ambient = u_Light.Ambient * texture(u_Textures[0], v_TexCoord).rgb; // Diffuse Texture
        
        result += ambient;
        
        // diffuse
        float diff    = max(dot(norm, lightDir), 0.0);
        vec3  diffuse = u_Light.Diffuse * diff * texture(u_Textures[0], v_TexCoord).rgb; // Diffuse Texture
        
        result += diffuse;

        slotBinded++;
    }
    
    if (slotBinded < u_NumTextureSlots)
    {
        // specular
        vec3  viewDir    = normalize(u_ViewPos - v_Position);
        vec3  reflectDir = reflect(-lightDir, norm);
        float spec       = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.Shininess);
        vec3  specular   = u_Light.Specular * spec * texture(u_Textures[1], v_TexCoord).rgb; // Specular Texture
        
        result += specular;

        slotBinded++;
    }
    return vec4(result, 1.0f);
}

void main()
{
    vec3 lightDir = normalize(u_Light.Position - v_Position);

    vec4 diffuseColor = texture(u_Textures[0], v_TexCoord);
    vec4 lightEffect  = GetLightEffect(lightDir);
    color = lightEffect;
}
