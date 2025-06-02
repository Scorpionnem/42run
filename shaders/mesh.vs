#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTexPos;

out vec3 FragColor;
out vec3 FragPos;
out vec3 FragNormal;
out vec3 FragAbsNormal;

out	vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);

	FragColor = aColor;
	FragAbsNormal = abs(aNormal);
	
	FragPos = vec3(model * vec4(aPos, 1.0));
	
	FragNormal = mat3(transpose(inverse(model))) * aNormal;

	texCoord = aTexPos;
}
