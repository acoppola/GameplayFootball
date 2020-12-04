#version 150

#pragma optimize(on)

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

//uniform mat4 lightViewMatrix;

// eye-space
in vec4 position;

//out vec4 modelPosition;
out mat4 modelViewMatrix;

out vec4 modelViewPosition;

void main(void) {

  modelViewMatrix = viewMatrix * modelMatrix;
  //viewPosition = modelViewMatrix * position;
  //viewPosition = viewMatrix * modelMatrix * position;
  modelViewPosition = modelViewMatrix * position;
  gl_Position = projectionMatrix * modelViewPosition;

  //gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;

/*
  gl_Position = ftransform();
  //gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
  //gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
 */
  //positionView = gl_ModelViewMatrix * gl_Vertex;

  //shadowCoord = gl_TextureMatrix[7] * gl_Vertex;
  //lightViewMatrix = gl_TextureMatrix[7];
}

/* old version
#version 150
//#extension GL_EXT_gpu_shader4 : require

#pragma optimize(on)

uniform float contextWidth;
uniform float contextHeight;

uniform vec2 cameraClip;

//uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;

varying vec4 positionView;
//varying vec4 shadowCoord;
varying mat4 lightViewMatrix;

void main(void) {

  gl_Position = ftransform();
  //gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
  //gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
  positionView = gl_ModelViewMatrix * gl_Vertex;

  //shadowCoord = gl_TextureMatrix[7] * gl_Vertex;
  lightViewMatrix = gl_TextureMatrix[7];
}
*/
