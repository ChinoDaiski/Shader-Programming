#version 330

// layout(location = 0) in vec3 a_Position;	// ATTRIBUTE (VS INPUT)
// in vec3 a_Position;	// ATTRIBUTE (VS INPUT)	// layout 없이 사용이 가능하다.

layout(location = 0) in vec3 a_Position;	// ATTRIBUTE (VS INPUT)
layout(location = 1) in vec3 a_Position1;	// a는 attribute의 약자
uniform vec4 u_Trans;

uniform float u_Scale;	// uniform 전역 변수

void main()
{
	vec4 newPosition;
	//newPosition.xy = a_Position.xy*u_Trans.w + u_Trans.xy;
	//newPosition.xy = a_Position1.xy*u_Trans.w + u_Trans.xy;
	//newPosition.xy = (a_Position.xy + a_Position1.xy) * u_Trans.w + u_Trans.xy;
	newPosition.xy = u_Scale * (a_Position.xy + a_Position1.xy) * u_Trans.w + u_Trans.xy;
	newPosition.z = 0;
	newPosition.w= 1;
	gl_Position = newPosition;
}
