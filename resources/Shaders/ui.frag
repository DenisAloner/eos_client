#version 330 core

uniform sampler2DArray atlas;

in vec3 fragmentTexture;
in vec4 fragmentColor;

out vec4 fragColor;

float median(float r, float g, float b) {
  return max(min(r, g), min(max(r, g), b));
}

float linearStep(float lo, float hi, float x) {
  return (clamp(x, lo, hi) - lo) / (hi - lo);
}

    const vec3 bottomColor = vec3(1.0, 1.0, 1.0);
    const vec3 borderColor = vec3(0.0, 0.0, 0.0);
    float pxRange = 2.5f; // 3.0 is better for fullscreen; //: logrange(0.25, 256.0);
    float thickness = 0.15f; //: range(-1.0, +1.0);
    float border = 0.25f;  //: range(0.0, 0.25);

void main(void) {
  if (fragmentTexture.z < 1.5f) {
//     vec3 sample = texture(atlas, fragmentTexture).rgb;
//     ivec2 sz = textureSize(atlas, 0).xy;
//     float dx = dFdx(fragmentTexture.x) * sz.x;
//     float dy = dFdy(fragmentTexture.y) * sz.y;
//     float toPixels = 8.0 * inversesqrt(dx * dx + dy * dy);
//     float sigDist = median(sample.r, sample.g, sample.b) - 0.5;
//     float opacity = clamp(sigDist * toPixels + 0.5, 0.0, 1.0);
// fragColor = vec4(1.0f, 1.0f, 1.0f, opacity);


    vec2 msdfSize = vec2(textureSize(atlas, 0));
    float pxSize = min(0.5 / pxRange *
                           (fwidth(fragmentTexture.x) * msdfSize.x +
                            fwidth(fragmentTexture.y) * msdfSize.y),
                       0.25);

    vec3 msd = texture(atlas, fragmentTexture).rgb;
    float sd = 2.0 * median(msd.r, msd.g, msd.b) - 1.0 + thickness;
    float inside = linearStep(-border - pxSize, -border + pxSize, sd);
    float outsideBorder =
        border > 0.0 ? linearStep(+border - pxSize, +border + pxSize, sd) : 1.0;

    fragColor =
        vec4(mix(borderColor, bottomColor,
                 outsideBorder),
             inside);

    
  } else
    fragColor = texture(atlas, fragmentTexture) * fragmentColor;
}