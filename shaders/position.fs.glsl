#version 330 core
in  vec3 PositionColor;
out vec4 FragColor;

void main()
{
  FragColor = vec4(PositionColor.xyz + vec3(0.5f, 0.5f, 0.5f), 1.0);
  // FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}