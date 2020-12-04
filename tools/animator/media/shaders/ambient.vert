#version 150
//#extension GL_EXT_gpu_shader4 : require

#pragma optimize(on)

// for the fullscreen quad
uniform mat4 orthoViewMatrix;
uniform mat4 orthoProjectionMatrix;

//uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

// eye-space
in vec4 position;

void main(void) {
  //viewPosition = viewMatrix * modelMatrix * position;
  //gl_Position = projectionMatrix * viewPosition;
  gl_Position = orthoProjectionMatrix * orthoViewMatrix * position;

  //gl_TexCoord[0] = gl_MultiTexCoord0;
}

/*
// deprecated

uniform float contextWidth;
uniform float contextHeight;
uniform vec2 cameraClip;

varying vec4 positionView;

void main(void) {
  gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;//ftransform();
  gl_TexCoord[0] = gl_MultiTexCoord0;
  positionView = gl_ModelViewMatrix * gl_Vertex;
}
*/
