#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 coord;

out vec3 our_color;
out vec2 tex_coord;

uniform mat4 trans_mat4;

void main()
{
  gl_Position = trans_mat4 * vec4(pos, 1.0f);
  our_color = color;
  tex_coord = coord;
}
