#version 330 core
out vec4 FragColor;

uniform sampler2D tex0;

in vec2 fragPos;
in vec4 clip;

uniform vec3 color;

void main()
{
    vec4 texColor = texture(tex0, fragPos);
    if (texColor.r == 0, texColor.g == 0 && texColor.b == 0)
        discard ;
    else
        FragColor = vec4(vec3(0, 1, 0), 1.0);
}
