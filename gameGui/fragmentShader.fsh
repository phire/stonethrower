#version 130

out vec4 fragColor;
in vec4 varyingColor;

void main(void)
{
  fragColor = varyingColor;
}
