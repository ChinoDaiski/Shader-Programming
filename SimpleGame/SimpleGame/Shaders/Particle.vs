#version 330

in vec3 a_Position;	// ATTRIBUTE (VS INPUT)
in vec3 a_Vel;

uniform float u_Time;

const vec3 c_Gravity = vec3(0.0f, -0.8f, 0.0f);

void main()
{
	vec4 newPosition;
	newPosition.xyz = a_Position
						+ a_Vel * u_Time
						+ 0.5f * c_Gravity * u_Time * u_Time;
	newPosition.w = 1.f;
	gl_Position = newPosition;
}
