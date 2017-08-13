#include "GLShaderProgram.h"
#include "GLShader.h"

#include <iostream>
#include <string_view>

/***********************************************************************************/
GLShaderProgram::GLShaderProgram(const std::string_view programName, const std::initializer_list<GLShader> shaders) : m_programName(programName) {

	m_programID = glCreateProgram();

	for (const auto& shader : shaders) {
		shader.AttachShader(m_programID);
	}

	linkAndValidate();

	// Cleanup
	for (const auto& shader : shaders) {
		shader.DetachShader(m_programID);
		shader.DeleteShader();
	}
}

/***********************************************************************************/
GLShaderProgram::~GLShaderProgram() {
	DeleteProgram();
}

/***********************************************************************************/
void GLShaderProgram::Bind() const {
	glUseProgram(m_programID);
}

/***********************************************************************************/
void GLShaderProgram::AddUniforms(const std::initializer_list<std::string_view> uniforms) { 
	
	for (const auto& uniform : uniforms) {
		const auto uniformLoc = glGetUniformLocation(m_programID, uniform.data());

		if (uniformLoc == -1) {
			std::cerr << "Shader Program Error: " << uniform.data() << " uniform not found in shader: " << m_programName.c_str() << std::endl;
		}
		m_uniforms.try_emplace(uniform, uniformLoc);
	}
}

/***********************************************************************************/
void GLShaderProgram::DeleteProgram() const {
	if (m_programID != 0) {
		glDeleteProgram(m_programID);
	}
}

/***********************************************************************************/
GLShaderProgram& GLShaderProgram::SetUniformi(const std::string_view uniformName, const int value) {
	glUniform1i(m_uniforms.at(uniformName), value);

	return *this;
}

/***********************************************************************************/
GLShaderProgram& GLShaderProgram::SetUniformf(const std::string_view uniformName, const float value) {
	glUniform1f(m_uniforms.at(uniformName), value);

	return *this;
}

/***********************************************************************************/
GLShaderProgram& GLShaderProgram::SetUniform(const std::string_view uniformName, const glm::ivec2& value) {
	glUniform2iv(m_uniforms.at(uniformName), 1, &value[0]);

	return *this;
}

/***********************************************************************************/
GLShaderProgram& GLShaderProgram::SetUniform(const std::string_view uniformName, const glm::vec2& value) {
	glUniform2f(m_uniforms.at(uniformName), value.x, value.y);

	return *this;
}

/***********************************************************************************/
GLShaderProgram& GLShaderProgram::SetUniform(const std::string_view uniformName, const glm::vec3& value) {
	glUniform3f(m_uniforms.at(uniformName), value.x, value.y, value.z);

	return *this;
}

/***********************************************************************************/
GLShaderProgram& GLShaderProgram::SetUniform(const std::string_view uniformName, const glm::vec4& value) {
	glUniform4f(m_uniforms.at(uniformName), value.x, value.y, value.z, value.w);

	return *this;
}

/***********************************************************************************/
GLShaderProgram& GLShaderProgram::SetUniform(const std::string_view uniformName, const glm::mat3x3& value) {
	glUniformMatrix3fv(m_uniforms.at(uniformName), 1, GL_FALSE, value_ptr(value));

	return *this;
}

/***********************************************************************************/
GLShaderProgram& GLShaderProgram::SetUniform(const std::string_view uniformName, const glm::mat4x4& value) {
	glUniformMatrix4fv(m_uniforms.at(uniformName), 1, GL_FALSE, value_ptr(value));

	return *this;
}

/***********************************************************************************/
void GLShaderProgram::linkAndValidate() {

	glLinkProgram(m_programID);
	glGetProgramiv(m_programID, GL_LINK_STATUS, &m_success);
	if (!m_success) {
		glGetProgramInfoLog(m_programID, m_infoLog.size(), nullptr, m_infoLog.data());
		std::cerr << "Shader Program Linking Error: " << m_infoLog.data() << std::endl;
	}

	glValidateProgram(m_programID);
	glGetProgramiv(m_programID, GL_VALIDATE_STATUS, &m_success);
	if (!m_success) {
		glGetProgramInfoLog(m_programID, m_infoLog.size(), nullptr, m_infoLog.data());
		std::cerr << "Shader Program Validation Error: " << m_infoLog.data() << std::endl;
	}
}
