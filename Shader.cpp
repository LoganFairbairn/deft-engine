#include "Shader.h"
#include "FatalErrors.h"
#include <fstream>
#include <vector>
#include <iostream>

namespace Deft
{
	Shader::Shader() : m_NumberOfAttributes(0), m_ProgramID(0), m_VertexShaderID(0), m_FragmentShaderID(0)
	{
	}


	Shader::~Shader()
	{
		//Delete the shader program.
		if (m_ProgramID != -1)
		{
			glDeleteProgram(m_ProgramID);
		}
	}

	//Complies both the vertex and the fragment shaders by reading through their respected text files.
	void Shader::CompileShader(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath)
	{
		//Create a shader program and store it's ID. This program needs to be created before adding shader attributes to it, otherwise attributes are not added properly to the shader program.
		m_ProgramID = glCreateProgram();

		//Create a vertex shader and store it's ID.
		m_VertexShaderID = glCreateShader(GL_VERTEX_SHADER);

		//Check to make sure that a vertex shader ID was created.
		if (m_VertexShaderID == 0)
		{
			FatalError("Vertex shader failed to be created!");
		}

		//Create a fragment shader and store it's ID.
		m_FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

		//Check to make sure that a fragment shader ID was created.
		if (m_FragmentShaderID == 0)
		{
			FatalError("Fragment shader failed to be created!");
		}

		//Loads both the vertex and the fragment shaders and compiles them.
		LoadShader(vertexShaderFilePath, m_VertexShaderID);
		LoadShader(fragmentShaderFilePath, m_FragmentShaderID);
	}

	//Links both the fragment and the vertex shaders into one program.
	void Shader::LinkShaders()
	{
		//Attatch our shaders to the program.
		glAttachShader(m_ProgramID, m_VertexShaderID);
		glAttachShader(m_ProgramID, m_FragmentShaderID);

		//Link our program.
		glLinkProgram(m_ProgramID);

		//Check for any errors caused by linking shaders.
		GLint isLinked = 0;
		glGetProgramiv(m_ProgramID, GL_LINK_STATUS, (int *)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_ProgramID, GL_INFO_LOG_LENGTH, &maxLength);

			//The max length includes the NULL character.
			std::vector<char> infoLog(maxLength);
			glGetProgramInfoLog(m_ProgramID, maxLength, &maxLength, &infoLog[0]);

			//Delete the program to avoid a memory leak.
			glDeleteProgram(m_ProgramID);

			//Delete both of the shaders to avoid a memory leak.
			glDeleteShader(m_VertexShaderID);
			glDeleteShader(m_FragmentShaderID);

			//Display the error.
			std::printf("%s\n", &(infoLog[0]));
			FatalError("Shaders failed to link!");

			//Leave after displaying an error.
			return;
		}

		//Detach shaders after a successful link.
		glDetachShader(m_ProgramID, m_VertexShaderID);
		glDetachShader(m_ProgramID, m_FragmentShaderID);

		//We no longer need the vertex or fragment shaders since they are linked into one program now.
		//Delete both the fragment and the vertex shader.
		glDeleteShader(m_VertexShaderID);
		glDeleteShader(m_FragmentShaderID);
	}

	//Adds an attribute (such as color, position, and textures) to the shader program in the next available OpenGL shader attribute slot.
	void Shader::AddAttribute(const std::string& attributeName)
	{
		//Adds a new shader attribute to our program in the next available slot.
		glBindAttribLocation(m_ProgramID, m_NumberOfAttributes, attributeName.c_str());

		//Increase the number of attributes, so next time AddAttribute is called, the attribute will be added in the next attribute slot.
		m_NumberOfAttributes++;
	}

	GLint Shader::GetAttributeLocation(const std::string& attributeName)
	{
		GLint location = glGetAttribLocation(m_ProgramID, attributeName.c_str());

		return location;
	}

	//Gets the integer ID number of a uniform variable from a shader.
	GLint Shader::GetUniformLocation(const std::string& uniformName)
	{
		GLint location = glGetUniformLocation(m_ProgramID, uniformName.c_str());

		//Check to make sure the uniform ID that was returned by the shader correctly corresponds to a valid uniform in the shader, and return an error if it doesn't.
		if (location == GL_INVALID_INDEX)
		{
			FatalError("Uniform " + uniformName + " not found in shader!");
		}

		return location;
	}

	//Call this function to use this shader program before drawing.
	//Uses the shader program, and enables any attributes this shader requires.
	void Shader::Use()
	{
		//Tells OpenGL to use this shader program.
		glUseProgram(m_ProgramID);

		//When you start to use the shader program, enable all shader attributes.
		//Cycle through all attributes this shader program has and enable them.
		for (int i = 0; i < m_NumberOfAttributes; i++)
		{
			glEnableVertexAttribArray(i);
		}
	}

	//Call this function when you are finished with using this shader program.
	void Shader::UnUse()
	{
		//Tells OpenGL to not use any shader programs
		glUseProgram(0);

		//After using the shader program, disable all shader attributes.
		//Cycle through all attributes this shader has and disable them.
		//Not disabling these attributes can cause a lot of drawing issues depending on the graphics card being used.
		for (int i = 0; i < m_NumberOfAttributes; i++)
		{
			glDisableVertexAttribArray(i);
		}
	}

	//Loads a shader program from a file path and then compiles is using OpenGL.
	void Shader::LoadShader(const std::string& filePath, GLuint shaderID)
	{
		//Open the vertex shader text file, and check to make sure it opened properly.
		std::ifstream file(filePath);
		if (file.fail())
		{
			perror(filePath.c_str());
			FatalError("Failed to open " + filePath + ".");
		}

		std::string fileContents = "";
		std::string line;

		//Read through the entire vertex shader text file and store it in one string.
		while (std::getline(file, line))
		{
			fileContents += line + "\n";
		}

		//Close the file after reading is complete.
		file.close();

		//Replace the source code in the shader object with the contents of the shader file that was just read.
		const char* contentsPtr = fileContents.c_str();
		glShaderSource(shaderID, 1, &contentsPtr, nullptr);

		//Use OpenGL to compile the shader.
		glCompileShader(shaderID);

		//Check for errors in the vertex shader that was just compiled.
		GLint success = 0;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

		//If there is an error compiling the shader, an error message is triggered.
		if (success == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

			//The maxLength includes the NULL character.
			std::vector<char> errorLog(maxLength);
			glGetShaderInfoLog(shaderID, maxLength, &maxLength, &errorLog[0]);

			//Delete the shader to avoid a memory leak.
			glDeleteShader(shaderID);

			//Display the shader error.
			std::printf("%s\n", &(errorLog[0]));
			FatalError("Shader " + filePath + " failed to compile!");
		}

		else
		{
			std::cout << "\nShader " + filePath + " compiled successfully.";
		}
	}
}