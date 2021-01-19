#version 330 core
layout (location = 0) in vec2 position;
out vec3 PositionColor;
void main()
{
  gl_Position = vec4(position.xy, 0.0, 1.0);
  PositionColor = gl_Position.xyz + vec3(0.5f, 0.5f, 0.5f);
}