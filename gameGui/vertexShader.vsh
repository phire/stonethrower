#version 120

uniform mat4 mvpMatrix;
attribute vec4 vertex;
attribute vec4 color;
varying vec4 varyingColor;

void main(void)
{
  varyingColor = color;
  gl_Position = mvpMatrix * vertex;
}
