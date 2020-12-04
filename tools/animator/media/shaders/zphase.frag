#version 150
//#extension GL_EXT_gpu_shader4 : require

#pragma optimize(on)

void main(void) {
  gl_FragData[0] = vec4(0, 0, 0, 0);
  //gl_FragData[1] = vec4(0, 0, 0, 0);
  //gl_FragDepth = 0.7f;//gl_FragCoord.z;
}
