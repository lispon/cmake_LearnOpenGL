#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 coord;

out vec3 our_color;
out vec2 tex_coord;

void main()
{
  gl_Position = vec4(pos, 1.0);
  our_color = color;
  tex_coord = coord;
}
