#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in vec3 a_Tangent;
layout(location = 4) in vec3 a_Bitangent;

uniform mat4 u_Transform;
uniform mat4 u_ViewProjection;

out vec3 v_Position;
out vec3 v_Normal;
out vec2 v_TexCoord;
out vec3 v_Tangent;
out vec3 v_Bitangent;

void main()
{
    v_Position  = vec3(u_Transform * vec4(a_Position, 1.0));
    v_Normal    = mat3(transpose(inverse(u_Transform))) * a_Normal;
    v_TexCoord  = a_TexCoord;
    v_Tangent   = a_Tangent;
    v_Bitangent = a_Bitangent;
    
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
    vec3 Direction;
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
    
    float Constant;
    float Linear;
    float Quadratic;
    float CutOff;
};

struct LightFlag
{
    bool IsAmbient;
    bool IsDiffuse;
    bool IsSpecular;
    bool IsAttenuation;
    bool IsSpotLight;
};

in vec3 v_Position;
in vec3 v_Normal;
in vec2 v_TexCoord;
in vec3 v_Tangent;
in vec3 v_Bitangent;

uniform int       u_NumTextureSlots;
uniform sampler2D u_Textures[16];

uniform vec3      u_ViewPos;
uniform Material  u_Material;
uniform Light     u_Light;
uniform LightFlag u_LightFlag;

vec4 LightContent(vec3 lightDir)
{
    int slotBinded = 0;

    vec4 result = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    vec3 norm   = normalize(v_Normal);

    float distance, attenuation;
    vec3 ambient, diffuse, specular;
    if (u_LightFlag.IsAttenuation)
    {
        // attenuation
        distance    = length(u_Light.Position - v_Position);
        attenuation = 1.0 / (u_Light.Constant + u_Light.Linear * distance + u_Light.Quadratic * (distance * distance));
    }
    
    if (slotBinded < u_NumTextureSlots)
    {
        // ambient
        if (u_LightFlag.IsAmbient)
        {
            ambient = u_Light.Ambient * texture(u_Textures[0], v_TexCoord).rgb; // Diffuse
            
            if ((u_LightFlag.IsAttenuation) && (!u_LightFlag.IsSpotLight))
                ambient  *= attenuation;
            
            result += vec4(ambient, 1.0f);
        }
        
        // diffuse
        if (u_LightFlag.IsDiffuse)
        {
            float diff   = max(dot(norm, lightDir), 0.0);
            
            diffuse = u_Light.Diffuse * diff * texture(u_Textures[0], v_TexCoord).rgb; // Diffuse
            
            if (u_LightFlag.IsAttenuation)
                diffuse  *= attenuation;
            
            result += vec4(diffuse, 1.0f);
        }
        slotBinded++;
    }
    
    if (slotBinded < u_NumTextureSlots)
    {
        // specular
        if (u_LightFlag.IsSpecular)
        {
            vec3  viewDir    = normalize(u_ViewPos - v_Position);
            vec3  reflectDir = reflect(-lightDir, norm);
            float spec       = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.Shininess);
            
            specular = u_Light.Specular * spec * texture(u_Textures[1], v_TexCoord).rgb;
            
            if (u_LightFlag.IsAttenuation)
                specular *= attenuation;
            
            result  += vec4(specular, 1.0f);
        }
        slotBinded++;
    }
    return result;
}

void main()
{
    vec3 lightDir = normalize(u_Light.Position - v_Position);
    
    // check if lighting is inside the spotlight cone
    float theta = dot(lightDir, normalize(-u_Light.Direction));

    if ((u_LightFlag.IsSpotLight) && (theta < u_Light.CutOff))
        color = vec4(u_Light.Ambient * texture(u_Textures[0], v_TexCoord).rgb, 1.0);
    else
        color = LightContent(lightDir);
}
