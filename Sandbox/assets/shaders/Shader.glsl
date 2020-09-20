#type vertex
#version 330 core

layout (location = 0) in vec3  a_Position;
layout (location = 1) in vec4  a_Color;
layout (location = 2) in vec2  a_TexCoord;
layout (location = 3) in float a_TextureIndex;
layout (location = 4) in float a_TilingFactor;
layout (location = 5) in vec3  a_Normal;

out vec3  v_Position;
out vec4  v_Color;
out vec2  v_TexCoord;
out float v_TextureIndex;
out float v_TilingFactor;
out vec3  v_Normal;

uniform mat4 u_Model;
uniform mat4 u_ProjectionView;

void main()
{
    v_Position     = vec3(u_Model * vec4(a_Position, 1.0));
    v_Color        = a_Color;
    v_TexCoord     = a_TexCoord;
    v_TextureIndex = a_TextureIndex;
    v_TilingFactor = a_TilingFactor;
    v_Normal       = a_Normal;
    
    gl_Position = u_ProjectionView * u_Model * vec4(a_Position.x, a_Position.y, a_Position.z, 1.0);
}

#type fragment
#version 330 core

out vec4 FragColor;

struct Material
{
    vec3  Diffuse;
    vec3  Specular;
    float Shininess;
};

struct Light
{
    vec3 Position;
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
    
    float Constant;
    float Linear;
    float Quadratic;
};

in vec3  v_Position;
in vec4  v_Color;
in vec2  v_TexCoord;
in float v_TextureIndex;
in float v_TilingFactor;
in vec3  v_Normal;

uniform sampler2D u_Texture[16];
uniform Material  u_Material;
uniform Light     u_Light;
uniform vec3      u_ViewPos;

uniform int u_IsAmbient;
uniform int u_IsDiffuse;
uniform int u_IsSpecular;
uniform int u_IsAttenuation;

void LightProp(vec3 lightDir)
{
    vec4 result = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    
    vec3  norm          = normalize(v_Normal);
    float attenuation   = 0.0f;
    
    // Attenuation
    if (1 == int(u_IsAttenuation))
    {
        float distance = length(lightDir);
        attenuation    = 1.0 / (u_Light.Constant + u_Light.Linear * distance + u_Light.Quadratic * (distance * distance));
    }
    
    // ambient
    if (1 == int(u_IsAmbient))
    {
        vec3 ambient = u_Light.Ambient * u_Material.Diffuse;
        
        if (1 == int(u_IsAttenuation))
            ambient *= attenuation;
        
        result += vec4(ambient, 1.0f);
    }
    
    // diffuse
    if (1 == int(u_IsDiffuse))
    {
        float diff    = max(dot(norm, lightDir), 0.0);
        vec3  diffuse = u_Light.Diffuse * (diff * u_Material.Diffuse);
        
        if (1 == int(u_IsAttenuation))
            diffuse  *= attenuation;
        
        result += vec4(diffuse, 1.0f);
    }
    
    // specular
    if (1 == int(u_IsSpecular))
    {
        vec3 viewDir    = normalize(u_ViewPos - v_Position);
        vec3 reflectDir = reflect(-lightDir, norm);
        
        float spec      = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.Shininess);
        vec3  specular  = u_Light.Specular * (spec * u_Material.Specular);
        
        if (1 == int(u_IsAttenuation))
            specular *= attenuation;
        
        result += vec4(specular, 1.0f);
    }
    
    if (0 == int(u_IsAmbient) && 0 == int(u_IsDiffuse) && 0 == int(u_IsSpecular))
        result = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    
    vec4 Color = texture(u_Texture[int(v_TextureIndex)], v_TexCoord * v_TilingFactor) * v_Color * result;
    if (Color.a < 0.1)
        discard;
    FragColor = Color;
}

void main()
{
    vec3  lightDir = normalize(u_Light.Position - v_Position);
    
    LightProp(lightDir);
}
