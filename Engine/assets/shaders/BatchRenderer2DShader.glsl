// ******************************************************************************
//   File    : BatchRenderer.glsl
//   Project : i-Kan
//
//   Created by Ashish
// ******************************************************************************

#type vertex
#version 330 core

layout(location = 0) in vec3  a_Position;
layout(location = 1) in vec4  a_Color;
layout(location = 2) in vec2  a_TexCoord;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TilingFactor;
layout(location = 5) in int   a_ObjectID;

uniform mat4 u_ViewProjection;

out VS_OUT
{
    vec4  Color;
    vec2  TexCoord;
    float TexIndex;
    float TilingFactor;
    float ObjectID;
} vs_out;

void main()
{
    vs_out.Color         = a_Color;
    vs_out.TexCoord      = a_TexCoord;
    vs_out.TexIndex      = a_TexIndex;
    vs_out.TilingFactor  = a_TilingFactor;
    vs_out.ObjectID      = float(a_ObjectID);

    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core
layout(location = 0) out vec4 o_Color;
layout(location = 1) out int  o_IDBuffer;

in VS_OUT
{
    vec4  Color;
    vec2  TexCoord;
    float TexIndex;
    float TilingFactor;
    float ObjectID;
} fs_in;

uniform sampler2D u_Textures[16];

void main()
{
    vec4 texColor = fs_in.Color;
    switch(int(fs_in.TexIndex))
    {
        case 0: texColor *= texture(u_Textures[0], fs_in.TexCoord * fs_in.TilingFactor); break;
        case 1: texColor *= texture(u_Textures[1], fs_in.TexCoord * fs_in.TilingFactor); break;
        case 2: texColor *= texture(u_Textures[2], fs_in.TexCoord * fs_in.TilingFactor); break;
        case 3: texColor *= texture(u_Textures[3], fs_in.TexCoord * fs_in.TilingFactor); break;
        case 4: texColor *= texture(u_Textures[4], fs_in.TexCoord * fs_in.TilingFactor); break;
        case 5: texColor *= texture(u_Textures[5], fs_in.TexCoord * fs_in.TilingFactor); break;
        case 6: texColor *= texture(u_Textures[6], fs_in.TexCoord * fs_in.TilingFactor); break;
        case 7: texColor *= texture(u_Textures[7], fs_in.TexCoord * fs_in.TilingFactor); break;
        case 8: texColor *= texture(u_Textures[8], fs_in.TexCoord * fs_in.TilingFactor); break;
        case 9: texColor *= texture(u_Textures[9], fs_in.TexCoord * fs_in.TilingFactor); break;
        case 10: texColor *= texture(u_Textures[10], fs_in.TexCoord * fs_in.TilingFactor); break;
        case 11: texColor *= texture(u_Textures[11], fs_in.TexCoord * fs_in.TilingFactor); break;
        case 12: texColor *= texture(u_Textures[12], fs_in.TexCoord * fs_in.TilingFactor); break;
        case 13: texColor *= texture(u_Textures[13], fs_in.TexCoord * fs_in.TilingFactor); break;
        case 14: texColor *= texture(u_Textures[14], fs_in.TexCoord * fs_in.TilingFactor); break;
        case 15: texColor *= texture(u_Textures[15], fs_in.TexCoord * fs_in.TilingFactor); break;
    }
    o_Color = texColor;
    o_IDBuffer = int(fs_in.ObjectID);
}

#type geometry
#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT
{
    vec4  Color;
    vec2  TexCoord;
    float TexIndex;
    float TilingFactor;
    float ObjectID;
} gs_in[];

out VS_OUT
{
    vec4  Color;
    vec2  TexCoord;
    float TexIndex;
    float TilingFactor;
    float ObjectID;
} gs_out;

void main()
{
    gs_out.Color         = gs_in[0].Color;
    gs_out.TexCoord      = gs_in[0].TexCoord;
    gs_out.TexIndex      = gs_in[0].TexIndex;
    gs_out.TilingFactor  = gs_in[0].TilingFactor;
    gs_out.ObjectID      = float(gs_in[0].ObjectID);

    gl_Position = gl_in[0].gl_Position;
    EmitVertex();
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();

    EndPrimitive();
}
