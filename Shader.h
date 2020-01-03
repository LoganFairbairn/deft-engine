#pragma once
#include <string>
#include <GL/glew.h>

namespace Deft
{
	//Every Shader program has a vertex shader and a fragment shader component.
	class Shader
	{
	public:
		Shader();
		~Shader();

		void CompileShader(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);
		void LinkShaders();
		void AddAttribute(const std::string& attributeName);

		void Use();
		void UnUse();

		//Setters.
		void SetID(unsigned int shaderID) { m_ID = shaderID; }
		void SetName(const std::string& shaderName) { m_Name = shaderName; }

		//Getters.
		GLint GetAttributeLocation(const std::string& attributeName);
		GLint GetUniformLocation(const std::string& uniformName);
		unsigned int GetID() { return m_ID; }
		std::string GetName() { return m_Name; }

	private:
		void LoadShader(const std::string& filePath, GLuint shaderID);
		
		std::string m_Name;
		unsigned int m_ID;					//This is the number used to identify this shader.
		int m_NumberOfAttributes;			//The numer of attributes this shader program has.
		GLuint m_ProgramID;					//This is the programs ID number used by OpenGL.
		GLuint m_VertexShaderID;			//This is the Vertex Shaders ID number used by OpenGL.
		GLuint m_FragmentShaderID;			//This is the Fragment Shaders ID number used by OpenGL.
	};
}

