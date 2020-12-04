#version 150
//#extension GL_EXT_gpu_shader4 : require

#pragma optimize(on)

uniform sampler2D map_albedo; // 0
uniform sampler2D map_normal; // 1
//uniform sampler2D map_aux;    // 2
uniform sampler2D map_depth;  // 3
//uniform sampler2D map_shadow; // 7
uniform sampler2DShadow map_shadow; // 7

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 inverseProjectionViewMatrix;

//uniform mat4 lightModelMatrix;
uniform mat4 lightViewProjectionMatrix;

uniform float contextWidth;
uniform float contextHeight;
uniform float contextX;
uniform float contextY;

uniform vec2 cameraClip;

uniform bool has_shadow;
//uniform bool lightType;

//uniform mat4 projectionMatrix;
//in mat4 modelViewMatrix;
//in vec4 modelPosition;
//in vec4 modelViewPosition;
//varying vec4 shadowCoord;

uniform vec3 cameraPosition;
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform float lightRadius;

vec3 GetWorldPosition(vec2 texCoord, float depth) {
  vec4 projectedPos = vec4(texCoord.x * 2 - 1, texCoord.y * 2 - 1, depth * 2 - 1, 1.0f);
  vec4 worldPosition = inverseProjectionViewMatrix * projectedPos;
  worldPosition.xyz /= worldPosition.w;
  return worldPosition.xyz;
}

void main(void) {

  //gl_FragData[0] = vec4(0);
  //gl_FragData[1] = vec4(0);
  //discard;

  vec2 texCoord = gl_FragCoord.xy;
  texCoord.x -= contextX;
  texCoord.y -= contextY;
  texCoord.x /= contextWidth;
  texCoord.y /= contextHeight;
  //ivec2 texCoordi = ivec2(texCoord);

  float depth = texture2D(map_depth, texCoord).x; // non-linear (0 .. 1)

  vec3 worldPosition = GetWorldPosition(texCoord, depth);

  // convert from non-linear to linear
  float fragDepth = cameraClip.y / (depth - cameraClip.x);
  //vec3 xPosition = vec3(modelViewPosition.xy * fragDepth / modelViewPosition.z, fragDepth);

  // calculate fragments location in clipspace
  //vec2 positionClip = (positionView * gl_ProjectionMatrix).xy / (positionView * gl_ProjectionMatrix).z;

  //float depth = texelFetch(map_depth, texCoordi, 2).x;

  // convert from non-linear [0..1] to linear [near .. far]
//XX  float fragDepth = cameraClip.y / (depth - cameraClip.x);

  //XX vec3 lightPos = lightPosition;//gl_LightSource[0].position.xyz;
  //XX float radius = gl_LightSource[0].diffuse.w;

  //if (fragDepth < lightPos.z - radius) discard;
  //if (fragDepth > lightPos.z + radius) discard;

  //vec3 viewPosition = vec3(viewPosition.xy * fragDepth / viewPosition.z, fragDepth);
  //vec3 position = texture2D(map_aux, texCoord).xyz;

/*   vec4 projectedPos = vec4(texCoord.x * 2 - 1, texCoord.y * 2 - 1, depth * 2 - 1, 1.0f);
  vec4 worldPosition = inverseProjectionViewMatrix * projectedPos;
  worldPosition.xyz /= worldPosition.w; */

  // calculate fragment's location in screenspace
  //vec2 positionClip = (viewPosition * projectionMatrix).xy / (viewPosition * projectionMatrix).z;

  float dist = distance(lightPosition, worldPosition.xyz);
  // discard is slow. if (dist > radius) discard;

  vec3 lightDir = normalize(lightPosition - worldPosition.xyz);

  vec4 normalshine = texture2D(map_normal, texCoord);
  //float xyLength = length(normalspec.xy);
  vec3 normal = normalize(normalshine.xyz);

  float nDotLD = dot(lightDir, normal);// * 1.1 - 0.1;// * 0.9 + 0.1;
  //if (nDotLD < 0.0f) discard;

  // todo
  //float nDotLD;
  //if (lightType == 0)
  //  nDotLD = dot(lightDir, normal);
  //else

  // scattering emulation ;)
  float scatter_nDotLD = nDotLD * 0.5f + 0.5f;
  //scatter_nDotLD += 0.5f;
  scatter_nDotLD = pow(scatter_nDotLD, 2.0f); // more 'normalish' curve on < 90 deg, but keep some for nuance on the back of objects

  nDotLD = max(nDotLD, 0.0f);

  // combine
  float resulting_nDotLD = nDotLD * 0.3f + scatter_nDotLD * 0.7f;

  // allow for some neg ndotl to allow for smooth normals
  //if (nDotLD < -0.3f) discard;
  //nDotLD = sqrt(nDotLD);
  //if (nDotLD < 0.0f) nDotLD = 0.0f;
  //XXif (nDotLD < 0.0f) discard;

  //nDotLD = pow(nDotLD, 0.8); // 1.2

  //fragColor = clamp(fragColor, 0, 1);

  // photoshop's 'curves' in a sinewave fashion
  //nDotLD = nDotLD * 0.2 + (-cos(nDotLD * 3.14159265) * 0.5 + 0.5) * 0.8;

  float shininess = normalshine.w;

  // linear
  float falloff = max(0.0, lightRadius - dist) / lightRadius;
  // exp
  //falloff *= falloff * falloff;
  falloff = falloff * falloff;
  // (above calculation is highly incorrect but gives a nice mix between exp and linear. see
  // http://tomdalling.com/blog/modern-opengl/07-more-lighting-ambient-specular-attenuation-gamma/ for more info about the correct methods
  // (basically: "1.0 / (1.0 + x ^ 2)" )

  float brightness = 1.5f;//2.0f;
  vec3 lighted = resulting_nDotLD * falloff * lightColor * brightness;

  // should be camerapos, why isn't it? vec3(inverse(viewMatrix)[3])

  //vec3 refl = reflect(normalize(worldPosition.xyz), normal);
  vec3 eyeToFrag = worldPosition.xyz - cameraPosition;
  vec3 refl = reflect(normalize(eyeToFrag), normal);

  vec4 base = texture2D(map_albedo, texCoord);

  // blur
  //base = base + texture2D(map_albedo, texCoord + vec2(0, 1 / contextHeight));
  //base = base + texture2D(map_albedo, texCoord + vec2( 1 / contextHeight, 0));
  //base = base + texture2D(map_albedo, texCoord + vec2(0, -1 / contextHeight));
  //base = base + texture2D(map_albedo, texCoord + vec2(-1 / contextHeight, 0));
  //base = base / 5.0;


  float spec = base.w;
  //vec3 specular = vec3(0.0f);
  //spec *= 10.0;
  //if (nDotLD > 0.0f) {
    vec3 specularColor = lightColor;// * 0.2f + base.rgb * 0.8f;
    vec3 specular = pow(max(0.0f, dot(refl, lightDir)), shininess * 128.0f) * spec * falloff * specularColor;
  //}

  //lighted = clamp(lighted, 0.0, 1.0);

  float shaded = 1.0;
  if (has_shadow) {

//    vec4 coord = lightViewMatrix * modelViewMatrix * vec4(position, 1.0);
//    float A = shadow2DProj(map_shadow, coord).r;
//    float R = coord.p / coord.q;
//    float shadowColor = (R <= A + 0.0002) ? 1.0 : 0.4;

/*
    mat4 lightMVPMatrix = lightViewProjectionMatrix * modelViewMatrix;

    float shift = 0.05f;

    vec3 offset = vec3(0, 0, 0);
    vec4 coord = lightMVPMatrix * vec4(viewPosition.xyz + offset, 1.0);
    float A = shadow2DProj(map_shadow, coord).r;
    float R = coord.p / coord.q;
    float shadowColor = (R <= A + 0.0002) ? 1.0 : 0.0;
*/

  const int poisson_size = 9;
  vec2 poisson[poisson_size];
  // poisson[0] = vec2( 0.326, -0.406);
  // poisson[1] = vec2(-0.840, -0.074);
  // poisson[2] = vec2( 0.396,  0.457);

  poisson[0] = vec2( 0.500,  0.500);
  poisson[1] = vec2(-0.500,  0.500);
  poisson[2] = vec2( 0.500, -0.500);
  poisson[3] = vec2(-0.500, -0.500);

  poisson[4] = vec2( 1.000,  0.000);
  poisson[5] = vec2( 0.000,  1.000);
  poisson[6] = vec2(-1.000,  0.000);
  poisson[7] = vec2( 0.000, -1.000);

  poisson[8] = vec2( 0.000,  0.000);

/*
    float offset = 0.00f;
    vec4 projectedFrag = lightViewProjectionMatrix * vec4(worldPosition, 1.0f);
    vec2 NDC_projectedFrag = projectedFrag.xy /= projectedFrag.w;
    for (int i = 0; i < 4; i++) {
      float lightOccluderDist = texture(map_shadow, NDC_projectedFrag.xy + poissonDisk[i] / 3200.0f).z;
      if (lightOccluderDist < projectedFrag.z - offset) shaded -= 0.25;
    }
*/

    float offset = -0.0002f;
    vec4 projectedFrag = lightViewProjectionMatrix * vec4(worldPosition, 1.0f);
    for (int i = 0; i < poisson_size; i++) {
      float lightOccluderDist = textureProj(map_shadow, projectedFrag + (vec4(poisson[i], 0, 0) / 1500.0f) + vec4(0, 0, offset, 0));
      shaded -= (1.0f - lightOccluderDist) * (1.0f / poisson_size);//0.25f;
    }
    //if (projectedFrag.w > 0.0f) {
//      float lightOccluderDist = textureProj(map_shadow, projectedFrag + vec4(0, 0, offset, 0));// - vec4(0, 0, offset, 0));
//      shaded = lightOccluderDist;
    //}

    //shaded = textureProj(map_shadow, projectedFrag).r * 200.0f;
    //shaded = texture2D(map_shadow, texCoord * 1.2f + vec2(0.05, 0.05)).r;

    // nice debug
    //shaded = pow(texture2D(map_depth, texCoord * 1.1f + vec2(0.05, 0.05)).r, 3.0f) * 3.0f - 0.6f;

/*
    offset = vec3(shift, 0, 0);
    coord = lightMVPMatrix * vec4(viewPosition.xyz + offset, 1.0);
    A = shadow2DProj(map_shadow, coord).r;
    R = coord.p / coord.q;
    shadowColor += (R <= A + 0.0002) ? 1.0 : 0.0;

    offset = vec3(0, shift, 0);
    coord = lightMVPMatrix * vec4(viewPosition.xyz + offset, 1.0);
    A = shadow2DProj(map_shadow, coord).r;
    R = coord.p / coord.q;
    shadowColor += (R <= A + 0.0002) ? 1.0 : 0.0;

    offset = vec3(-shift, 0, 0);
    coord = lightMVPMatrix * vec4(viewPosition.xyz + offset, 1.0);
    A = shadow2DProj(map_shadow, coord).r;
    R = coord.p / coord.q;
    shadowColor += (R <= A + 0.0002) ? 1.0 : 0.0;

    offset = vec3(0, -shift, 0);
    coord = lightMVPMatrix * vec4(viewPosition.xyz + offset, 1.0);
    A = shadow2DProj(map_shadow, coord).r;
    R = coord.p / coord.q;
    shadowColor += (R <= A + 0.0002) ? 1.0 : 0.0;

    shaded = shadowColor / 5.0;
*/

    //shaded *= 0.25;
    //shaded += 0.75;
    //shaded *= 0.4;
    //shaded += 0.6;
    shaded *= 0.5;
    shaded += 0.5;
    //shaded *= 0.75;
    //shaded += 0.25;
  }
  //gl_FragColor = vec4(brightness);

  //base *= vec4(1.4f, 1.3f, 1.0f, 1.0f);

  vec3 fragColor = (base.rgb * lighted + specular) * shaded;
  //fragColor = vec3(1.0f, 0.6f, 0.3f);

  //float brightness = 0.9f;

  //gl_FragData[0] = vec4(worldPosition.x * 0.01f + 0.05f, worldPosition.y * 0.01f + 0.05f, worldPosition.z * 0.01f, 0.0f);
  //gl_FragData[0] = vec4(worldPosition.xyz, 0.0f);
  //gl_FragData[0] = vec4(position.x * 0.005f, position.y * 0.005f, position.z * 0.05f - 5.0f, 0.0f);
  //gl_FragData[0] = vec4(vec3(fragDepth * 0.0025f), 0); //vec4(clamp(fragColor * brightness, 0.0, 1.0), 0.0) + vec4(lightColor.r, lightColor.g, lightColor.b, 0.0) * 0.1f;
  // position debug vec4(0.5 - base.r * 0.5, 0.5 - base.g * 0.5, 0.5 - base.b * 0.5, 0.0f);
  //gl_FragData[0] = vec4(vec3(inverseProjectionViewMatrix[3]) * 0.02f + 0.2f, 0.0f);
  gl_FragData[0] = vec4(fragColor, 0.0f);
  gl_FragData[1] = vec4(0);
}
