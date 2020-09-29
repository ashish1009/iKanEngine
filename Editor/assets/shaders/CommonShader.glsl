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

in vec3 v_Position;
in vec3 v_Normal;
in vec2 v_TexCoord;
in vec3 v_Tangent;
in vec3 v_Bitangent;

uniform float u_NumTexture;

uniform sampler2D u_Textures[16];

void main()
{
    vec4 texColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    for (int i = 0; i < int(u_NumTexture); i++)
    {
        texColor *= texture(u_Textures[i], v_TexCoord);
    }
    color = texColor;
}
