#version 330 core
in vec4 vertex_color;
uniform vec4 ufm_color;
out vec4 fc;
void main()
{
  fc = ufm_color;
}
