#version 330

in vec2 TexCoord0;
in vec4 LightIntensity0;

out vec4 FragColor;

uniform sampler2D gTextureSampler;
uniform vec4 g_colorVec;
void main()
{
    FragColor = g_colorVec * LightIntensity0;

}