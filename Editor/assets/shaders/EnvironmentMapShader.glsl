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

void main()
{
    v_Position  = a_Position;//vec3(u_Transform * vec4(a_Position, 1.0));
    v_Normal    = mat3(transpose(inverse(u_Transform))) * a_Normal;
    v_TexCoord  = a_TexCoord;
    
    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_Position;
in vec3 v_Normal;
in vec2 v_TexCoord;

uniform int       u_NumTextureSlots;
uniform sampler2D u_Texture;
uniform vec3      u_Color;

uniform vec3        u_CameraPos;
uniform samplerCube u_Skybox;

uniform int u_Refract; // Note: if this is false then bydefault it will use reflection
uniform int u_MaterialIndex;

void main()
{
    float materialType[5] = float[5]( 1.00f, 1.33f, 1.309, 1.52, 2.42 );
    /* -----------------------------------------
      Index  |    Material   |  Refractive index
     -------------------------------------------
        0    |     Air       |      1.00
        1    |     Water     |      1.33
        2    |     Ice       |      1.309
        3    |     Glass     |      1.52
        4    |     Diamond   |      2.42
     ------------------------------------------ */

    float ratio = 1.00 / materialType[u_MaterialIndex];
    
    vec3 I = normalize(v_Position - u_CameraPos);
    vec3 R;
    
    if (bool(u_Refract))
        R = refract(I, normalize(v_Normal), ratio);
    else
        R = reflect(I, normalize(v_Normal));
    
    color = vec4(texture(u_Skybox, R).rgb, 1.0);
}
