#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

uniform mat4 model;
uniform mat4 u_ViewProjection;

out vec2 v_TexCoord;
out vec3 fragmentNormal;
out vec3 fragmetPosition;

void main()
{
    v_TexCoord = a_TexCoord;
    fragmetPosition = a_Position;
    fragmentNormal = a_Normal;
    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;

void main()
{
    //    color = vec4(v_TexCoord.x, v_TexCoord.y, 0.0f, 1.0f);
    //    vec3 outAmbient     =   vec3(texture(texture_diffuse1 , v_TexCoord));
    //    vec3 outDiffuse     =   vec3(texture(texture_specular1 , v_TexCoord));
    //    vec3 outSpecular    =   vec3(texture(texture_normal1 , v_TexCoord));
    ////
    //    vec3 result = (outAmbient + outDiffuse + outSpecular);
    //    color = vec4(result, 1.0);
    //
    color = texture(texture_diffuse1, v_TexCoord);// + (texture(texture_specular1, v_TexCoord)) + (texture(texture_normal1, v_TexCoord));
}
