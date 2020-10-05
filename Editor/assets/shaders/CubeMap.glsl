#type vertex
#version 330 core
layout (location = 0) in vec3 a_Position;

uniform mat4 u_ProjectionView;
//uniform mat4 u_Transform;

out vec3 v_TexCoord;

void main()
{
    v_TexCoord = a_Position;
//    gl_Position = u_ProjectionView * u_Transform * vec4(a_Position, 1.0);
    gl_Position = u_ProjectionView * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

out vec4 FragColor;

in vec3 v_TexCoord;

uniform samplerCube u_Skybox;

void main()
{
    FragColor = texture(u_Skybox, v_TexCoord);
}
