#version 150
//#extension GL_EXT_gpu_shader4 : require

#pragma optimize(on)

//uniform mat4 modelMatrix;
uniform mat4 orthoViewMatrix;
uniform mat4 orthoProjectionMatrix;

// eye-space
in vec4 position;

void main(void) {
  gl_Position = orthoProjectionMatrix * orthoViewMatrix * position;
}

/* old version
#version 150
//#extension GL_EXT_gpu_shader4 : require

#pragma optimize(on)

void main(void) {
  gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;//ftransform();
}
*/
