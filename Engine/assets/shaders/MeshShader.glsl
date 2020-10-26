#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in vec3 a_Tangent;
layout(location = 4) in vec3 a_bitangent;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec2  v_TexCoord;

void main()
{
    v_TexCoord      = a_TexCoord;
    
    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2  v_TexCoord;

uniform sampler2D u_Textures[16];

void main()
{
    vec4 texColor = texture(u_Textures[0], v_TexCoord);
    color = texColor;
}
