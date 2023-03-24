#version 330

in vec3 a_Position;	// ATTRIBUTE (VS INPUT)
in vec3 a_Vel;
in float a_EmitTime;
in float a_lifeTime;

uniform float u_Time;

const vec3 c_Gravity = vec3(0.0f, -0.8f, 0.0f);
const float c_lifeTime = 2.0f;
const vec3 c_Vel = vec3(0.1f, 0.f, 0.f);

#define PI 3.141592

void main()
{
	vec4 newPosition = vec4(0.f, 0.f, 0.f, 1.f);
	
	float t = fract(u_Time / 10.f) * 10.f;	// 0 ~ 10초 사이의 값
	newPosition.x = a_Position.x + t * c_Vel.x;

	float yTime = (t / 10.f) * 2.f * PI * 2.f;
	newPosition.y = a_Position.y + sin(yTime);
	newPosition.z = 0.f; //a_Position.z + t * c_Vel.z;

	gl_Position = newPosition;



	/*
	float time = u_Time - a_EmitTime;

	vec4 newPosition = vec4(0.f, 0.f, 0.f, 1.f);

	// 생성시간이 0이하라면 생성이 되지 않아야 한다.
	if(time < 0.f)
	{

	}
	else
	{
		float t = a_lifeTime * fract(time / a_lifeTime);	// fract : 0 ~ 1 사이로 값을 자르는 함수

		newPosition.xyz = a_Position
							+ a_Vel * t
							+ 0.5f * c_Gravity * t * t;
		
		//float t = c_lifeTime * fract(time / c_lifeTime);	// fract : 0 ~ 1 사이로 값을 자르는 함수
		//
		//newPosition.xyz = a_Position
		//					+ a_Vel * t
		//					+ 0.5f * c_Gravity * t * t;
		
	}


	*/

	// newPosition.w = 1.f;
}
