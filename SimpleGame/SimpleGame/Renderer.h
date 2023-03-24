#pragma once

#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include "Dependencies\glew.h"

class Renderer
{
public:
	Renderer(int windowSizeX, int windowSizeY);
	~Renderer();

	bool IsInitialized();
	void DrawSolidRect(float x, float y, float z, float size, float r, float g, float b, float a);
	void Class0310_Render();
	void DrawParticleEffect();

private:
	void Initialize(int windowSizeX, int windowSizeY);
	bool ReadFile(char* filename, std::string *target);
	void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);
	GLuint CompileShaders(char* filenameVS, char* filenameFS);
	void CreateVertexBufferObjects();
	void GetGLPosition(float x, float y, float *newX, float *newY);

	void Class0310();

	// particle VBO
	void CreateParticle(uint32_t numParticle);
	GLuint m_ParticleVBO = -1;
	GLuint m_ParticleVelVBO = -1;	// Velocity(속도)
	GLuint m_ParticleEmitTimeVBO = -1;	// EmitTime(생성 시작 시간)
	GLuint m_ParticleLifeTimeVBO = -1;	// LifeTime(생성 지속 시간)
	GLuint m_ParticleShader = -1;
	GLuint m_ParticleVerticesCount = -1;	// 파티클을 그리는데 필요한 버텍스 갯수

	bool m_Initialized = false;
	
	unsigned int m_WindowSizeX = 0;
	unsigned int m_WindowSizeY = 0;

	GLuint m_VBORect = 0;
	GLuint m_SolidRectShader = 0;

	GLuint m_testVBO = 0;
	GLuint m_testVBO1 = 0;
	GLuint m_testVBOColor = 0;
};

