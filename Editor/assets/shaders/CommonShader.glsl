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

void main()
{
    FragColor = vec4(0.4, 0.5, 0.6, 1.0);
}
