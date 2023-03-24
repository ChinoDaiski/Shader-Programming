#include "stdafx.h"
#include "Renderer.h"

Renderer::Renderer(int windowSizeX, int windowSizeY)
{
	Initialize(windowSizeX, windowSizeY);
	//Class0310();

	CreateParticle(1000);
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

	glGenBuffers(1, &m_VBORect);	// 1�� ����
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);





	// �����͸� �غ��ϴ� �κ�

	float vertices[] = { 0,0,0, 1,0,0, 1,1,0 };	// CPU memory

	glGenBuffers(1, &m_testVBO);	// Get Buffer Object ID, 2�� ����
	glBindBuffer(GL_ARRAY_BUFFER, m_testVBO);	// bind tp array buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);	// Data transfer to GPU


	float vertices1[] = { -1,-1,0, 0,-1,0, 0,0,0 };	// CPU memory

	glGenBuffers(1, &m_testVBO1);	// Get Buffer Object ID, 2�� ����
	glBindBuffer(GL_ARRAY_BUFFER, m_testVBO1);	// bind tp array buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);	// Data transfer to GPU



	float verticesColor[] = { 1,0,0,1, 0,1,0,1, 0,0,1,1 };	// CPU memory

	glGenBuffers(1, &m_testVBOColor);	// Get Buffer Object ID, 2�� ����
	glBindBuffer(GL_ARRAY_BUFFER, m_testVBOColor);	// bind tp array buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesColor), verticesColor, GL_STATIC_DRAW);	// Data transfer to GPU
}

void Renderer::AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	//���̴� ������Ʈ ����
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = strlen(pShaderText);
	//���̴� �ڵ带 ���̴� ������Ʈ�� �Ҵ�
	glShaderSource(ShaderObj, 1, p, Lengths);

	//�Ҵ�� ���̴� �ڵ带 ������
	glCompileShader(ShaderObj);

	GLint success;
	// ShaderObj �� ���������� ������ �Ǿ����� Ȯ��
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];

		//OpenGL �� shader log �����͸� ������
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		printf("%s \n", pShaderText);
	}

	// ShaderProgram �� attach!!
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
	GLuint ShaderProgram = glCreateProgram(); //�� ���̴� ���α׷� ����

	if (ShaderProgram == 0) { //���̴� ���α׷��� ����������� Ȯ��
		fprintf(stderr, "Error creating shader program\n");
	}

	std::string vs, fs;

	//shader.vs �� vs ������ �ε���
	if (!ReadFile(filenameVS, &vs)) {
		printf("Error compiling vertex shader\n");
		return -1;
	};

	//shader.fs �� fs ������ �ε���
	if (!ReadFile(filenameFS, &fs)) {
		printf("Error compiling fragment shader\n");
		return -1;
	};

	// ShaderProgram �� vs.c_str() ���ؽ� ���̴��� �������� ����� attach��
	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);

	// ShaderProgram �� fs.c_str() �����׸�Ʈ ���̴��� �������� ����� attach��
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	//Attach �Ϸ�� shaderProgram �� ��ŷ��
	glLinkProgram(ShaderProgram);

	//��ũ�� �����ߴ��� Ȯ��
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

	if (Success == 0) {
		// shader program �α׸� �޾ƿ�
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

	//glUseProgram(ShaderProgram);	// ������ ���� ���ָ� �Ǵ� �ڵ��̴�. ���⼭ �� �ʿ�� ����.
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
	// �������ϴ� �κ�

	//Program select
	glUseProgram(m_SolidRectShader);

	glUniform4f(glGetUniformLocation(m_SolidRectShader, "u_Trans"), 0, 0, 0, 1);
	glUniform4f(glGetUniformLocation(m_SolidRectShader, "u_Color"), 1, 1, 1, 1);	// white color

	//int attribPosition = glGetAttribLocation(m_SolidRectShader, "a_Position");

#pragma region using_layout_in_SolidRect.vs
	//glEnableVertexAttribArray(0);	// SolidRect.vs���� layout = 0�� ���־��� ������ �ش� ���� �����ϴ�.
	//								// �ƴϸ� ���� �̸��� ���� �ش� ���� ���� �� ���� �ִ�.
	//glBindBuffer(GL_ARRAY_BUFFER, m_testVBO);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);		// 3�� �� �а�, stride�� ����.
#pragma endregion

#pragma region no_layout_in_SolidRect.vs
	int attribLoc_Position = -1;
	attribLoc_Position = glGetAttribLocation(m_SolidRectShader, "a_Position");
	glEnableVertexAttribArray(attribLoc_Position);

	glBindBuffer(GL_ARRAY_BUFFER, m_testVBO);
	glVertexAttribPointer(attribLoc_Position, 3, GL_FLOAT, GL_FALSE, 0, 0);		// 3�� �� �а�, stride�� ����.
#pragma endregion

#pragma region vs_Color
	int attribLoc_Color = -1;
	attribLoc_Color = glGetAttribLocation(m_SolidRectShader, "a_Color");
	glEnableVertexAttribArray(attribLoc_Color);

	glBindBuffer(GL_ARRAY_BUFFER, m_testVBOColor);
	glVertexAttribPointer(attribLoc_Color, 4, GL_FLOAT, GL_FALSE, 0, 0);		// 4�� �� �а�, stride�� ����.
#pragma endregion

#pragma region use_layout
	glEnableVertexAttribArray(1);	// ��Ʈ����Ʈ Ȱ��ȭ
	glBindBuffer(GL_ARRAY_BUFFER, m_testVBO1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
#pragma endregion

#pragma region uniform_variable_in_SolidRect.vs
	int uniformLoc_Scale = -1;
	uniformLoc_Scale = glGetUniformLocation(m_SolidRectShader, "u_Scale");
	glUniform1f(uniformLoc_Scale, g_time);	// 1f : 1��, float ���� �̶�� ��
	g_time += 0.016f;
	if (g_time > 1.f)
		g_time = 1.f;	// 0.f
#pragma endregion


	glDrawArrays(GL_TRIANGLES, 0, 3);	 // GL_LINE_STRIP, GL_POINTS � �����ϴ�.

}

void Renderer::DrawParticleEffect()
{
	//Program select
	int Program = m_ParticleShader;

	glUseProgram(Program);

	{
		int attribLoc_Position = -1;
		attribLoc_Position = glGetAttribLocation(Program, "a_Position");
		glEnableVertexAttribArray(attribLoc_Position);

		glBindBuffer(GL_ARRAY_BUFFER, m_ParticleVBO);
		glVertexAttribPointer(attribLoc_Position, 3, GL_FLOAT, GL_FALSE, 0, 0);		// 3�� �� �а�, stride�� ����.
	}



	{
		int attribLoc_Velocity = -1;
		attribLoc_Velocity = glGetAttribLocation(Program, "a_Vel");
		glEnableVertexAttribArray(attribLoc_Velocity);

		glBindBuffer(GL_ARRAY_BUFFER, m_ParticleVelVBO);
		glVertexAttribPointer(attribLoc_Velocity, 3, GL_FLOAT, GL_FALSE, 0, 0);		// 3�� �� �а�, stride�� ����.
	}



	{
		int uniformLoc_Time = -1;
		uniformLoc_Time = glGetUniformLocation(Program, "u_Time");
		glEnableVertexAttribArray(uniformLoc_Time);

		glUniform1f(uniformLoc_Time, g_time);	// g_time�� ���� query_performance_time���� ������� �ð����� �־��൵ �ȴ�.
		g_time += 0.005f;

		/*if (g_time > 10.f)
			g_time = 0.f;*/
	}


	{
		int attribLoc_EmitTime = -1;
		attribLoc_EmitTime = glGetAttribLocation(Program, "a_EmitTime");
		glEnableVertexAttribArray(attribLoc_EmitTime);

		glBindBuffer(GL_ARRAY_BUFFER, m_ParticleEmitTimeVBO);
		glVertexAttribPointer(attribLoc_EmitTime, 1, GL_FLOAT, GL_FALSE, 0, 0);		// 1�� �� �а�, stride�� ����.
	}

	{
		int attribLoc_LifeTime = -1;
		attribLoc_LifeTime = glGetAttribLocation(Program, "a_lifeTime");
		glEnableVertexAttribArray(attribLoc_LifeTime);

		glBindBuffer(GL_ARRAY_BUFFER, m_ParticleLifeTimeVBO);
		glVertexAttribPointer(attribLoc_LifeTime, 1, GL_FLOAT, GL_FALSE, 0, 0);		// 1�� �� �а�, stride�� ����.
	}
	
	glDrawArrays(GL_TRIANGLES, 0, m_ParticleVerticesCount);	 // GL_LINE_STRIP, GL_POINTS � �����ϴ�.
}

void Renderer::GetGLPosition(float x, float y, float *newX, float *newY)
{
	*newX = x * 2.f / m_WindowSizeX;
	*newY = y * 2.f / m_WindowSizeY;
}

void Renderer::Class0310()
{
	//// �����͸� �غ��ϴ� �κ�

	//float vertices[] = { 0,0,0, 1,0,0, 1,1,0 };	// CPU memory
	//
	//glGenBuffers(1, &m_testVBO);	// Get Buffer Object ID, 2�� ����
	//glBindBuffer(GL_ARRAY_BUFFER, m_testVBO);	// bind tp array buffer
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);	// Data transfer to GPU

	//
	//float vertices1[] = { -1,-1,0, 0,-1,0, 0,0,0 };	// CPU memory

	//glGenBuffers(1, &m_testVBO1);	// Get Buffer Object ID, 2�� ����
	//glBindBuffer(GL_ARRAY_BUFFER, m_testVBO1);	// bind tp array buffer
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);	// Data transfer to GPU



	// GL_STATIC_DRAW : GPU�� �ø� �����ʹ� �ٲ� �� ���� ���(static)�� ����� ���̴�.
	// STATIC���� ������ ������	: CPU �ʿ��� �����͸� GPU�� �ø��� ���� ���̻� ���� �������� �ʴ´�.
	// DYNAMIC���� ������ ������	: CPU�� �ʿ� �����͸� �����ϴ� ������ ���� ������, GPU�� ���� �ø���.
	//							  OpenGL���� �ش� ���� �ٲ�ų� ���� �ٲ�°��� �����ϴ� �Լ��� ���� �����Ͱ� �ٲ�� GPU�� �����͸� �ø���.

	// DYNAMIC�� �ַ� ���� ���꿡 �ʿ��� �����͵��� ó���� �� ����ߴ�.

	// GPU�� �����͸� �ø��� �ð��� �����ϴ� ����, ���⿡ chrono�� ����Ͽ� �ð��� �����ϸ� �ʴ� GPU�� �ö󰡴� �������� ���� �� �� �ִ�. 
	//float* temp;
	//int size = 400000000;
	//temp = new float[size];

	//memset(temp, 1, sizeof(float)* size);

	//glBindBuffer(GL_ARRAY_BUFFER, m_testVBO);	// bind tp array buffer
	//glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size, temp, GL_STATIC_DRAW);	// Data transfer to GPU
	//std::cout << "asdf" << std::endl;

}

void Renderer::CreateParticle(uint32_t numParticle)
{
	float centerX, centerY, randomDir;
	float speed = 1.f;
	float size = 0.01f;

	int particleCount = numParticle;
	m_ParticleVerticesCount = particleCount * 6;
	int floatCount = m_ParticleVerticesCount * 3;	// 6 : �簢���� �ʿ��� ���ؽ� ������ ������ 3 * 2 = 6, ���⿡ �����ϳ��� 3���� float���� �̷���� ������ * 3
	int floatCountSingle = m_ParticleVerticesCount;	
	float* vertices = NULL;
	vertices = new float[floatCount];


	// ��ġ�� ���õ� �κ�
	int index = 0;

	for (uint32_t i = 0; i < particleCount; ++i) {
		// ���� ��
		//centerX = ((float)rand() / (float)RAND_MAX) * 2.f - 1.f;
		//centerY = ((float)rand() / (float)RAND_MAX) * 2.f - 1.f;

		// �ʱ� ���� ��ġ�� 0����, �������� �Ѵ�.
		centerX = 0.f;
		centerY = 0.f;
		// ==========================================
		// ù��° �ﰢ��
		// ==========================================
		// ù��° ���ؽ�
		vertices[index++] = centerX - size;
		vertices[index++] = centerY + size;
		vertices[index++] = 0.f;

		// �ι�° ���ý�
		vertices[index++] = centerX - size;
		vertices[index++] = centerY - size;
		vertices[index++] = 0.f;

		// ����° ���ý�
		vertices[index++] = centerX + size;
		vertices[index++] = centerY + size;
		vertices[index++] = 0.f;

		// ==========================================
		// �ι�° �ﰢ��
		// ==========================================
		// ù��° ���ؽ�
		vertices[index++] = centerX + size;
		vertices[index++] = centerY + size;
		vertices[index++] = 0.f;

		// �ι�° ���ý�
		vertices[index++] = centerX - size;
		vertices[index++] = centerY - size;
		vertices[index++] = 0.f;

		// ����° ���ý�
		vertices[index++] = centerX + size;
		vertices[index++] = centerY - size;
		vertices[index++] = 0.f;
	}
	

	glGenBuffers(1, &m_ParticleVBO);

	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleVBO);	// bind tp array buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * floatCount, vertices, GL_STATIC_DRAW);	// Data transfer to GPU





	float* verticesVel = NULL;
	verticesVel = new float[floatCount];
	// �ӵ��� ���õ� �κ�
	index = 0;

	for (uint32_t i = 0; i < particleCount; ++i) {
		//centerX = ((float)rand() / (float)RAND_MAX) * 2.f - 1.f;
		//centerY = ((float)rand() / (float)RAND_MAX) * 2.f - 1.f;
		centerX = ((float)rand() / (float)RAND_MAX) * 2.f - 1.f;
		centerY = ((float)rand() / (float)RAND_MAX) * 2.f;
		// ==========================================
		// ù��° �ﰢ��
		// ==========================================
		// ù��° ���ؽ�
		verticesVel[index++] = centerX;
		verticesVel[index++] = centerY;
		verticesVel[index++] = 0.f;

		// �ι�° ���ý�
		verticesVel[index++] = centerX;
		verticesVel[index++] = centerY;
		verticesVel[index++] = 0.f;

		// ����° ���ý�
		verticesVel[index++] = centerX;
		verticesVel[index++] = centerY;
		verticesVel[index++] = 0.f;

		// ==========================================
		// �ι�° �ﰢ��
		// ==========================================
		// ù��° ���ؽ�
		verticesVel[index++] = centerX;
		verticesVel[index++] = centerY;
		verticesVel[index++] = 0.f;

		// �ι�° ���ý�
		verticesVel[index++] = centerX;
		verticesVel[index++] = centerY;
		verticesVel[index++] = 0.f;

		// ����° ���ý�
		verticesVel[index++] = centerX;
		verticesVel[index++] = centerY;
		verticesVel[index++] = 0.f;
	}

	glGenBuffers(1, &m_ParticleVelVBO);

	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleVelVBO);	// bind tp array buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * floatCount, verticesVel, GL_STATIC_DRAW);	// Data transfer to GPU








	// ��ƼŬ ���� �ð��� ���õ� �κ�
	float* verticesEmitTime = NULL;
	verticesEmitTime = new float[floatCountSingle];
	index = 0;

	for (uint32_t i = 0; i < particleCount; ++i) {
		
		float emitTime = ((float)rand() / (float)RAND_MAX) * 10.f;	// ((float)rand() / (float)RAND_MAX) : 0 ~ 1 ������ ���� ����
		// ==========================================
		// ù��° �ﰢ��
		// ==========================================
		// ù��° ���ؽ�
		verticesEmitTime[index++] = emitTime;

		// �ι�° ���ý�
		verticesEmitTime[index++] = emitTime;

		// ����° ���ý�
		verticesEmitTime[index++] = emitTime;

		// ==========================================
		// �ι�° �ﰢ��
		// ==========================================
		// ù��° ���ؽ�
		verticesEmitTime[index++] = emitTime;

		// �ι�° ���ý�
		verticesEmitTime[index++] = emitTime;

		// ����° ���ý�
		verticesEmitTime[index++] = emitTime;
	}

	glGenBuffers(1, &m_ParticleEmitTimeVBO);

	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleEmitTimeVBO);	// bind tp array buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * floatCountSingle, verticesEmitTime, GL_STATIC_DRAW);	// Data transfer to GPU









	// ��ƼŬ ���� �ð��� ���õ� �κ�
	float* verticesLifeTime = NULL;
	verticesLifeTime = new float[floatCountSingle];
	index = 0;

	for (uint32_t i = 0; i < particleCount; ++i) {

		float lifeTime = ((float)rand() / (float)RAND_MAX) * 5.f;	// ((float)rand() / (float)RAND_MAX) : 0 ~ 1 ������ ���� ����
		// ==========================================
		// ù��° �ﰢ��
		// ==========================================
		// ù��° ���ؽ�
		verticesLifeTime[index++] = lifeTime;

		// �ι�° ���ý�
		verticesLifeTime[index++] = lifeTime;

		// ����° ���ý�
		verticesLifeTime[index++] = lifeTime;

		// ==========================================
		// �ι�° �ﰢ��
		// ==========================================
		// ù��° ���ؽ�
		verticesLifeTime[index++] = lifeTime;

		// �ι�° ���ý�
		verticesLifeTime[index++] = lifeTime;

		// ����° ���ý�
		verticesLifeTime[index++] = lifeTime;
	}

	glGenBuffers(1, &m_ParticleLifeTimeVBO);

	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleLifeTimeVBO);	// bind tp array buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * floatCountSingle, verticesLifeTime, GL_STATIC_DRAW);	// Data transfer to GPU





	delete[] vertices;
	delete[] verticesVel;
	delete[] verticesEmitTime;
	delete[] verticesLifeTime;
}
