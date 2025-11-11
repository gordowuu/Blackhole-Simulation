#version 460 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D u_texture;
uniform float u_bloomThreshold;
uniform float u_bloomIntensity;
uniform bool u_horizontal;

// Gaussian blur weights
const float weights[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

vec3 gaussianBlur() {
    vec2 texelSize = 1.0 / textureSize(u_texture, 0);
    vec3 result = texture(u_texture, TexCoord).rgb * weights[0];
    
    if (u_horizontal) {
        for (int i = 1; i < 5; ++i) {
            result += texture(u_texture, TexCoord + vec2(texelSize.x * i, 0.0)).rgb * weights[i];
            result += texture(u_texture, TexCoord - vec2(texelSize.x * i, 0.0)).rgb * weights[i];
        }
    } else {
        for (int i = 1; i < 5; ++i) {
            result += texture(u_texture, TexCoord + vec2(0.0, texelSize.y * i)).rgb * weights[i];
            result += texture(u_texture, TexCoord - vec2(0.0, texelSize.y * i)).rgb * weights[i];
        }
    }
    
    return result;
}

void main() {
    vec3 color = texture(u_texture, TexCoord).rgb;
    
    // Extract bright areas for bloom
    float brightness = dot(color, vec3(0.2126, 0.7152, 0.0722));
    
    if (brightness > u_bloomThreshold) {
        color = gaussianBlur();
        FragColor = vec4(color * u_bloomIntensity, 1.0);
    } else {
        FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
}
