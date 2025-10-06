#pragma once

#include <glad/glad.h>

#include "../io/file_reader.h"
#include <unordered_map>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../core.h"
#include "../logging/log.h"


class Shader
{
public:
	Shader() {}
	Shader(const std::string& vertSrcFile, const std::string& fragSrcFile);
	~Shader();

	Shader(Shader&& other);
	Shader& operator=(Shader&& other);

	bool Compile(const std::string& vertSrcFile, const std::string& fragSrcFile);

public:
	static void Init();

public:

	void Bind();
	void Unbind();

	void UploadUniformFloat(const std::string& name, float val);
	void UploadUniformFloat2(const std::string& name, const glm::vec2& vec);
	void UploadUniformFloat3(const std::string& name, const glm::vec3& vec);
	void UploadUniformFloat4(const std::string& name, const glm::vec4& vec);

	void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
	void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

	void UploadUniformInt(const std::string& name, int val);
	void UploadUniformInt2(const std::string& name, const glm::ivec2& vec);
	void UploadUniformInt3(const std::string& name, const glm::ivec3& vec);
	void UploadUniformInt4(const std::string& name, const glm::ivec4& vec);

	void UploadUniformUInt(const std::string& name, unsigned int val);


	void UploadUniformBool(const std::string& name, bool val);

	void UploadUniformFloatArray(const std::string& name, float* val, int count);
	void UploadUniformFloat2Array(const std::string& name, float* vec, int count);
	void UploadUniformFloat3Array(const std::string& name, float* vec, int count);
	void UploadUniformFloat4Array(const std::string& name, float* vec, int count);

	void UploadUniformMat3Array(const std::string& name, float* matrix, int count);
	void UploadUniformMat4Array(const std::string& name, float* matrix, int count);

	void UploadUniformIntArray(const std::string& name, int* val, int count);
	void UploadUniformInt2Array(const std::string& name, int* vec, int count);
	void UploadUniformInt3Array(const std::string& name, int* vec, int count);
	void UploadUniformInt4Array(const std::string& name, int* vec, int count);

	void UploadUniformBoolArray(const std::string& name, bool* val, int count);

public:
	static void CreateShader(const std::string& vertFilePath, const std::string& fragFilePath, const std::string& shaderName);
	static Shader* GetShader(const std::string& name);


private:
	GLuint m_ShaderID;
	bool m_Bound;

private:
	static std::unordered_map<std::string, Shader> s_LoadedShaders;
};