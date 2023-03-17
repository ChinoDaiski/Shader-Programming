#include "stdafx.h"
#include "Renderer.h"

Renderer::Renderer(int windowSizeX, int windowSizeY)
{
	Initialize(windowSizeX, windowSizeY);
	//Class0310();

	CreateParticle();
}


Renderer::~Renderer()
{
}

void Renderer::Initialize(int windowSizeX, int windowSizeY)
{
	//Set window size
	m_WindowSizeX = windowSizeX;
	m_WindowSizeY = windowSizeY;

	//Load shaders
	m_SolidRectShader = CompileShaders("./Shaders/SolidRect.vs", "./Shaders/SolidRect.fs");
	m_ParticleShader = CompileShaders("./Shaders/Particle.vs", "./Shaders/Particle.fs");
	
	//Create VBOs
	CreateVertexBufferObjects();

	if (m_SolidRectShader > 0 && m_VBORect > 0)
	{
		m_Initialized = true;
	}
}

bool Renderer::IsInitialized()
{
	return m_Initialized;
}

void Renderer::CreateVertexBufferObjects()
{
	float rect[]
		=
	{
		-1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f, -1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, 1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, //Triangle1
		-1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f,  1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, 1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f, //Triangle2
	};

	glGenBuffers(1, &m_VBORect);	// 1번 생성
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);





	// 데이터를 준비하는 부분

	float vertices[] = { 0,0,0, 1,0,0, 1,1,0 };	// CPU memory

	glGenBuffers(1, &m_testVBO);	// Get Buffer Object ID, 2번 생성
	glBindBuffer(GL_ARRAY_BUFFER, m_testVBO);	// bind tp array buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);	// Data transfer to GPU


	float vertices1[] = { -1,-1,0, 0,-1,0, 0,0,0 };	// CPU memory

	glGenBuffers(1, &m_testVBO1);	// Get Buffer Object ID, 2번 생성
	glBindBuffer(GL_ARRAY_BUFFER, m_testVBO1);	// bind tp array buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);	// Data transfer to GPU



	float verticesColor[] = { 1,0,0,1, 0,1,0,1, 0,0,1,1 };	// CPU memory

	glGenBuffers(1, &m_testVBOColor);	// Get Buffer Object ID, 2번 생성
	glBindBuffer(GL_ARRAY_BUFFER, m_testVBOColor);	// bind tp array buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesColor), verticesColor, GL_STATIC_DRAW);	// Data transfer to GPU
}

void Renderer::AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	//쉐이더 오브젝트 생성
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = strlen(pShaderText);
	//쉐이더 코드를 쉐이더 오브젝트에 할당
	glShaderSource(ShaderObj, 1, p, Lengths);

	//할당된 쉐이더 코드를 컴파일
	glCompileShader(ShaderObj);

	GLint success;
	// ShaderObj 가 성공적으로 컴파일 되었는지 확인
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];

		//OpenGL 의 shader log 데이터를 가져옴
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		printf("%s \n", pShaderText);
	}

	// ShaderProgram 에 attach!!
	glAttachShader(ShaderProgram, ShaderObj);
}

bool Renderer::ReadFile(char* filename, std::string *target)
{
	std::ifstream file(filename);
	if (file.fail())
	{
		std::cout << filename << " file loading failed.. \n";
		file.close();
		return false;
	}
	std::string line;
	while (getline(file, line)) {
		target->append(line.c_str());
		target->append("\n");
	}
	return true;
}

GLuint Renderer::CompileShaders(char* filenameVS, char* filenameFS)
{
	GLuint ShaderProgram = glCreateProgram(); //빈 쉐이더 프로그램 생성

	if (ShaderProgram == 0) { //쉐이더 프로그램이 만들어졌는지 확인
		fprintf(stderr, "Error creating shader program\n");
	}

	std::string vs, fs;

	//shader.vs 가 vs 안으로 로딩됨
	if (!ReadFile(filenameVS, &vs)) {
		printf("Error compiling vertex shader\n");
		return -1;
	};

	//shader.fs 가 fs 안으로 로딩됨
	if (!ReadFile(filenameFS, &fs)) {
		printf("Error compiling fragment shader\n");
		return -1;
	};

	// ShaderProgram 에 vs.c_str() 버텍스 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);

	// ShaderProgram 에 fs.c_str() 프레그먼트 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	//Attach 완료된 shaderProgram 을 링킹함
	glLinkProgram(ShaderProgram);

	//링크가 성공했는지 확인
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

	if (Success == 0) {
		// shader program 로그를 받아옴
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error linking shader program\n" << ErrorLog;
		return -1;
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error validating shader program\n" << ErrorLog;
		return -1;
	}

	//glUseProgram(ShaderProgram);	// 렌더링 전에 해주면 되는 코드이다. 여기서 할 필요는 없다.
	std::cout << filenameVS << ", " << filenameFS << " Shader compiling is done.";

	return ShaderProgram;
}

void Renderer::DrawSolidRect(float x, float y, float z, float size, float r, float g, float b, float a)
{
	float newX, newY;

	GetGLPosition(x, y, &newX, &newY);

	//Program select
	glUseProgram(m_SolidRectShader);

	glUniform4f(glGetUniformLocation(m_SolidRectShader, "u_Trans"), newX, newY, 0, size);
	glUniform4f(glGetUniformLocation(m_SolidRectShader, "u_Color"), r, g, b, a);

	int attribPosition = glGetAttribLocation(m_SolidRectShader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

float g_time = 0.f;

void Renderer::Class0310_Render()
{
	// 렌더링하는 부분

	//Program select
	glUseProgram(m_SolidRectShader);

	glUniform4f(glGetUniformLocation(m_SolidRectShader, "u_Trans"), 0, 0, 0, 1);
	glUniform4f(glGetUniformLocation(m_SolidRectShader, "u_Color"), 1, 1, 1, 1);	// white color

	//int attribPosition = glGetAttribLocation(m_SolidRectShader, "a_Position");

#pragma region using_layout_in_SolidRect.vs
	//glEnableVertexAttribArray(0);	// SolidRect.vs에서 layout = 0을 해주었기 때문에 해당 값은 안전하다.
	//								// 아니면 변수 이름을 통해 해당 숫자 값을 알 수도 있다.
	//glBindBuffer(GL_ARRAY_BUFFER, m_testVBO);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);		// 3개 씩 읽고, stride는 없다.
#pragma endregion

#pragma region no_layout_in_SolidRect.vs
	int attribLoc_Position = -1;
	attribLoc_Position = glGetAttribLocation(m_SolidRectShader, "a_Position");
	glEnableVertexAttribArray(attribLoc_Position);

	glBindBuffer(GL_ARRAY_BUFFER, m_testVBO);
	glVertexAttribPointer(attribLoc_Position, 3, GL_FLOAT, GL_FALSE, 0, 0);		// 3개 씩 읽고, stride는 없다.
#pragma endregion

#pragma region vs_Color
	int attribLoc_Color = -1;
	attribLoc_Color = glGetAttribLocation(m_SolidRectShader, "a_Color");
	glEnableVertexAttribArray(attribLoc_Color);

	glBindBuffer(GL_ARRAY_BUFFER, m_testVBOColor);
	glVertexAttribPointer(attribLoc_Color, 4, GL_FLOAT, GL_FALSE, 0, 0);		// 4개 씩 읽고, stride는 없다.
#pragma endregion

#pragma region use_layout
	glEnableVertexAttribArray(1);	// 어트리뷰트 활성화
	glBindBuffer(GL_ARRAY_BUFFER, m_testVBO1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
#pragma endregion

#pragma region uniform_variable_in_SolidRect.vs
	int uniformLoc_Scale = -1;
	uniformLoc_Scale = glGetUniformLocation(m_SolidRectShader, "u_Scale");
	glUniform1f(uniformLoc_Scale, g_time);	// 1f : 1개, float 형식 이라는 뜻
	g_time += 0.016f;
	if (g_time > 1.f)
		g_time = 1.f;	// 0.f
#pragma endregion


	glDrawArrays(GL_TRIANGLES, 0, 3);	 // GL_LINE_STRIP, GL_POINTS 등도 가능하다.

}

void Renderer::DrawParticleEffect()
{
	//Program select
	int shaderProgram = m_ParticleShader;

	glUseProgram(shaderProgram);

	int attribLoc_Position = -1;
	attribLoc_Position = glGetAttribLocation(shaderProgram, "a_Position");
	glEnableVertexAttribArray(attribLoc_Position);

	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleVBO);
	glVertexAttribPointer(attribLoc_Position, 3, GL_FLOAT, GL_FALSE, 0, 0);		// 3개 씩 읽고, stride는 없다.

	glDrawArrays(GL_TRIANGLES, 0, m_ParticleVerticesCount);	 // GL_LINE_STRIP, GL_POINTS 등도 가능하다.
}

void Renderer::GetGLPosition(float x, float y, float *newX, float *newY)
{
	*newX = x * 2.f / m_WindowSizeX;
	*newY = y * 2.f / m_WindowSizeY;
}

void Renderer::Class0310()
{
	//// 데이터를 준비하는 부분

	//float vertices[] = { 0,0,0, 1,0,0, 1,1,0 };	// CPU memory
	//
	//glGenBuffers(1, &m_testVBO);	// Get Buffer Object ID, 2번 생성
	//glBindBuffer(GL_ARRAY_BUFFER, m_testVBO);	// bind tp array buffer
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);	// Data transfer to GPU

	//
	//float vertices1[] = { -1,-1,0, 0,-1,0, 0,0,0 };	// CPU memory

	//glGenBuffers(1, &m_testVBO1);	// Get Buffer Object ID, 2번 생성
	//glBindBuffer(GL_ARRAY_BUFFER, m_testVBO1);	// bind tp array buffer
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);	// Data transfer to GPU



	// GL_STATIC_DRAW : GPU에 올릴 데이터는 바꿀 수 없는 상수(static)로 사용할 것이다.
	// STATIC으로 선언한 데이터	: CPU 쪽에선 데이터를 GPU에 올리고 나선 더이상 값을 저장하지 않는다.
	// DYNAMIC으로 선언한 데이터	: CPU한 쪽에 데이터를 저장하는 공간을 만들어서 보내고, GPU에 값을 올린다.
	//							  OpenGL에선 해당 값이 바뀌거나 값이 바뀌는것을 감지하는 함수를 만들어서 데이터가 바뀔시 GPU에 데이터를 올린다.

	// DYNAMIC은 주로 물리 연산에 필요한 데이터들을 처리할 때 사용했다.

	// GPU에 데이터를 올리는 시간을 측정하는 구간, 여기에 chrono를 사용하여 시간을 측정하면 초당 GPU에 올라가는 데이터의 양을 알 수 있다. 
	//float* temp;
	//int size = 400000000;
	//temp = new float[size];

	//memset(temp, 1, sizeof(float)* size);

	//glBindBuffer(GL_ARRAY_BUFFER, m_testVBO);	// bind tp array buffer
	//glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size, temp, GL_STATIC_DRAW);	// Data transfer to GPU
	//std::cout << "asdf" << std::endl;

}

void Renderer::CreateParticle()
{
	float centerX, centerY;
	centerX = 0;
	centerY = 0;
	float size = 0.5f;

	int particleCount = 1;
	m_ParticleVerticesCount = particleCount * 6;
	int floatCount = m_ParticleVerticesCount * 3;	// 6 : 사각형에 필요한 버텍스 정점의 갯수는 3 * 2 = 6, 여기에 정점하나는 3개의 float으로 이루어져 있으니 * 3
	float* vertices = NULL;
	vertices = new float[floatCount];

	int index = 0;
	// ==========================================
	// 첫번째 삼각형
	// ==========================================
	// 첫번째 버텍스
	vertices[index++] = centerX - size;
	vertices[index++] = centerY + size;
	vertices[index++] = 0.f;

	// 두번째 버택스
	vertices[index++] = centerX - size;
	vertices[index++] = centerY - size;
	vertices[index++] = 0.f;

	// 세번째 버택스
	vertices[index++] = centerX + size;
	vertices[index++] = centerY + size;
	vertices[index++] = 0.f;

	// ==========================================
	// 두번째 삼각형
	// ==========================================
	// 첫번째 버텍스
	vertices[index++] = centerX + size;
	vertices[index++] = centerY + size;
	vertices[index++] = 0.f;

	// 두번째 버택스
	vertices[index++] = centerX - size;
	vertices[index++] = centerY - size;
	vertices[index++] = 0.f;

	// 세번째 버택스
	vertices[index++] = centerX + size;
	vertices[index++] = centerY - size;
	vertices[index++] = 0.f;

	

	glGenBuffers(1, &m_ParticleVBO);

	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleVBO);	// bind tp array buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * floatCount, vertices, GL_STATIC_DRAW);	// Data transfer to GPU
}
