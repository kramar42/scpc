#version 330 core
out vec4 FragColor;
in  vec3 PositionColor;
void main()
{
  FragColor = vec4(PositionColor, 1.0f);
}