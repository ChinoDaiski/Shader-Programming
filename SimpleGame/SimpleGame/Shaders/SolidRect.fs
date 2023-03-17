#version 330

layout(location=0) out vec4 FragColor;		// fragement shader에 왜 layout이 있을까?

uniform vec4 u_Color;
in vec4 v_Color;

void main()
{
	//FragColor = vec4(u_Color.r, u_Color.g, u_Color.b, u_Color.a);
	FragColor = vec4(v_Color.r, v_Color.g, v_Color.b, v_Color.a);
}
