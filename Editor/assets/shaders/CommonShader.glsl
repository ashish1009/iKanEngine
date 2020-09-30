#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in vec3 a_Tangent;
layout(location = 4) in vec3 a_Bitangent;

uniform mat4 u_ViewProjection;

out vec3 v_Position;
out vec3 v_Normal;
out vec2 v_TexCoord;
out vec3 v_Tangent;
out vec3 v_Bitangent;

void main()
{
    v_Position  = a_Position;
    v_Normal    = a_Normal;
    v_TexCoord  = a_TexCoord;
    v_Tangent   = a_Tangent;
    v_Bitangent = a_Bitangent;
    
    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

struct Material
{
//    sampler2D Diffuse;
//    sampler2D Specular;
    
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
in vec3 v_Tangent;
in vec3 v_Bitangent;

uniform float       u_NumTexture;
uniform sampler2D   u_Textures[16];

uniform vec3     u_ViewPos;
uniform Material u_Material;
uniform Light    u_Light;

void main()
{
    // ambient
    vec3 ambient = u_Light.Ambient * texture(u_Textures[0], v_TexCoord).rgb; // Diffuse
    
    // diffuse
    vec3  norm      = normalize(v_Normal);
    vec3  lightDir  = normalize(u_Light.Position - v_Position);
    float diff      = max(dot(norm, lightDir), 0.0);
    vec3  diffuse   = u_Light.Diffuse * diff * texture(u_Textures[0], v_TexCoord).rgb; // Diffuse
    
    // specular
    vec3  viewDir    = normalize(u_ViewPos - v_Position);
    vec3  reflectDir = reflect(-lightDir, norm);
    float spec       = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.Shininess);
    vec3  specular   = u_Light.Specular * spec * texture(u_Textures[1], v_TexCoord).rgb;
    
    vec3 result = ambient + diffuse + specular;
    color = vec4(result, 1.0);
}
