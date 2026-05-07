#version 450

in vec2 texCoord;

uniform sampler2D txtr;
uniform vec4 vertexColor;

// TODO: add uniform for colormap mode (int) and blend factor (float)

uniform int colormapMode;     
uniform float blendFactor;  
out vec4 fragColor;

// TODO: implement colormap functions
// Hint: define at least two colormaps (e.g. rainbow and cool-warm)
//   - Rainbow: interpolate between blue -> cyan -> green -> yellow -> red
//     using 4 segments at thresholds 0.25, 0.50, 0.75 with mix()
//   - Cool-warm: interpolate blue -> white -> red
//     using 2 segments at threshold 0.5 with mix()

vec3 rainbowMap(float t) {
    vec3 blue   = vec3(0.0, 0.0, 1.0);
    vec3 cyan   = vec3(0.0, 1.0, 1.0);
    vec3 green  = vec3(0.0, 1.0, 0.0);
    vec3 yellow = vec3(1.0, 1.0, 0.0);
    vec3 red    = vec3(1.0, 0.0, 0.0);

    if (t < 0.25) {
        return mix(blue, cyan, t / 0.25);
    } else if (t < 0.5) {
        return mix(cyan, green, (t - 0.25) / 0.25);
    } else if (t < 0.75) {
        return mix(green, yellow, (t - 0.5) / 0.25);
    } else {
        return mix(yellow, red, (t - 0.75) / 0.25);
    }
}


vec3 coolWarmMap(float t) {
    vec3 blue  = vec3(0.0, 0.0, 1.0);
    vec3 white = vec3(1.0, 1.0, 1.0);
    vec3 red   = vec3(1.0, 0.0, 0.0);

    if (t < 0.5) {
        return mix(blue, white, t / 0.5);
    } else {
        return mix(white, red, (t - 0.5) / 0.5);
    }
}



void main() {
    // TODO: apply colormap to the scalar field texture value
    // Hint: sample the texture to get a scalar value in [0,1],
    //   use it to look up the colormap color, then blend between
    //   grayscale and the mapped color using the blend factor.
    //   When colormap mode is off (0), use the original behavior below.
    //   Make sure overlays (glyphs, streamlines) still work by
    //   setting colormapMode to 0 from C++ before drawing them.

    float t = texture(txtr, texCoord).r;

    vec3 grayscale = vec3(t);
    vec3 finalRGB;

    if (colormapMode == 1) {
        vec3 colormapped = rainbowMap(t);
        finalRGB = mix(grayscale, colormapped, blendFactor);
    } else 
    if (colormapMode == 2) {
        vec3 colormapped = coolWarmMap(t);
        finalRGB = mix(grayscale, colormapped, blendFactor);
    } else 
    {

        finalRGB = grayscale;
    }

    fragColor = vertexColor + vec4(finalRGB, 1.0);

   // fragColor = vertexColor + texture(txtr, texCoord);
}
