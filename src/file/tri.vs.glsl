#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
out vec3 array_color;
uniform float offset_x;
void main()
{
   gl_Position = vec4(offset_x + aPos.x, -aPos.y, aPos.z, 1.0);
   array_color = aPos;
}
