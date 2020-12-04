#version 150
//#extension GL_EXT_gpu_shader4 : require

#pragma optimize(on)

uniform sampler2D map_albedo;
uniform sampler2D map_normal;
uniform sampler2D map_specular;
uniform sampler2D map_illumination;

uniform vec3 materialparams;
uniform vec3 materialbools;

// all these are in eye-space
in vec4 frag_position;
in vec3 frag_normal;
in vec3 frag_texcoord;
in vec3 frag_tangent;
in vec3 frag_bitangent;

void main(void) {

  vec4 base = texture2D(map_albedo, frag_texcoord.st);
  //if (base.rgb == vec3(0, 1, 0)) discard;
  //if (base.g > 0.8 && base.r < 0.2 && base.b < 0.2) discard;
  if (base.a < 0.12) discard;
  vec3 bump;
  if (materialbools.x == 1.0f) {
    bump = normalize(texture2D(map_normal, frag_texcoord.st).xyz * 2.0 - 1.0);
  } else {
    bump = vec3(0, 0, 1);
  }
  float spec;
  if (materialbools.y == 1.0f) {
    spec = texture2D(map_specular, frag_texcoord.st).x * materialparams.y;
  } else {
    spec = materialparams.y;
  }
  float illumination;
  if (materialbools.z == 1.0f) {
    illumination = texture2D(map_illumination, frag_texcoord.st).x;
  } else {
    illumination = materialparams.z;
  }

  // more bump
  //bump.z *= 0.1;
  //bump = normalize(bump);

  vec3 n = normalize(frag_normal);

  vec3 bumpNormal = bump.x * frag_tangent + bump.y * frag_bitangent + bump.z * n;
  //bumpNormal = faceforward(bumpNormal, normalize(position.xyz), bumpNormal);
  bumpNormal = normalize(bumpNormal);

  //vec2 depth;
  //depth.xy = frag_position.zw;

  //gl_FragData[0] = vec4(normalize(frag_normal), spec);
  gl_FragData[0] = vec4(base.rgb, spec);
  gl_FragData[1] = vec4(bumpNormal.xyz, materialparams.x);
  //gl_FragData[2] = vec4(frag_position.xyz, illumination); // *fixed* position data is needed for the ssao ambient shader. maybe a bit inefficient? in the other shaders, position is calculated on the fly
  //gl_FragData[2] = vec4(depth.x, depth.y, 0, illumination);
  gl_FragData[2] = vec4(0, 0, 0, illumination);
}
