#version 450

layout(location = 0) in vec3 fragTexCoord;
layout(location = 0) out vec4 outColor;

void main() {
    float tileSize = 100.0;  // 控制格子大小
    vec2 tiledUV = fragTexCoord.xy * tileSize;
    float checker = mod(floor(tiledUV.x) + floor(tiledUV.y), 2.0);
       // 使用柔和的灰色替代纯黑/纯白
    vec3 color = mix(vec3(0.8), vec3(0.95), checker); // 浅灰到更浅灰
    outColor = vec4(color, 1.0);
}