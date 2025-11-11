#version 460 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D u_texture;
uniform float u_exposure;

// ACES tone mapping
vec3 acesToneMapping(vec3 color) {
    const float a = 2.51;
    const float b = 0.03;
    const float c = 2.43;
    const float d = 0.59;
    const float e = 0.14;
    
    return clamp((color * (a * color + b)) / (color * (c * color + d) + e), 0.0, 1.0);
}

// Reinhard tone mapping
vec3 reinhardToneMapping(vec3 color) {
    return color / (color + vec3(1.0));
}

// Gamma correction
vec3 gammaCorrect(vec3 color) {
    return pow(color, vec3(1.0 / 2.2));
}

void main() {
    vec3 hdrColor = texture(u_texture, TexCoord).rgb;
    
    // Exposure
    hdrColor *= u_exposure;
    
    // Tone mapping
    vec3 mapped = acesToneMapping(hdrColor);
    
    // Gamma correction
    mapped = gammaCorrect(mapped);
    
    FragColor = vec4(mapped, 1.0);
}
