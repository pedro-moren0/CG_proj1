#version 410

layout(location = 0) in vec2 inPosition;

uniform vec2 translation;
uniform vec2 shipPosition;

out vec4 fragColor;

void main() {       /* pos. inic. |   desloc. x  |  desloc. y */
    vec2 newPosition = inPosition + shipPosition + translation;
    gl_PointSize = 5.0;
    gl_Position = vec4(newPosition, 0, 1);
    fragColor = vec4(0, 1, 0, 1);
}
