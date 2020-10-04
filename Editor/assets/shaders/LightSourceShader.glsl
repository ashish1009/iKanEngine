#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in vec3 a_Tangent;
layout(location = 4) in vec3 a_Bitangent;

uniform mat4 u_Transform;
uniform mat4 u_ViewProjection;

out vec2 v_TexCoord;

void main()
{
    v_TexCoord  = a_TexCoord;
    
    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform int       u_NumTextureSlots;
uniform sampler2D u_Texture;
uniform vec3      u_Color;

void main()
{
    color = vec4(u_Color, 1.0f);
    if (u_NumTextureSlots > 0)
        color *= vec4(texture(u_Texture, v_TexCoord).rgb, 1.0f);
}
