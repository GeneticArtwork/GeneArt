////////////////////////////////////////////////////////////////////////////////
// Filename: render.vs
////////////////////////////////////////////////////////////////////////////////

#version 400

/////////////////////
// INPUT VARIABLES //
/////////////////////
in vec3 inputPosition;
in vec3 inputNormal;
in vec3 inputColor;
in vec2 inputUV;

//////////////////////
// OUTPUT VARIABLES //
//////////////////////
out vec3 normal;
out vec3 color;
out vec2 uv;

///////////////////////
// UNIFORM VARIABLES //
///////////////////////
uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
void main(void)
{
	// Calculate the position of the vertex against the world, view, and projection matrices.
	gl_Position = worldMatrix * vec4(inputPosition, 1.0f);
	gl_Position = viewMatrix * gl_Position;
	gl_Position = projectionMatrix * gl_Position;

	// Output variables to the fragment shader.
	normal = inputNormal;
	color = inputColor;
	uv = inputUV;
}
