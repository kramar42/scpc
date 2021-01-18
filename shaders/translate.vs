#version 330 core
layout (location = 0) in vec3 position;
uniform vec2 translate;
out vec3 PositionColor;
void main()
{
  gl_Position = vec4(position.xy + translate.xy,
                     position.z, 1.0);
  PositionColor = gl_Position.xyz + vec3(0.5f, 0.5f, 0.5f);
}