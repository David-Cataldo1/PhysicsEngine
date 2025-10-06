#include "shader.h"

std::unordered_map<std::string, Shader> Shader::s_LoadedShaders;

Shader::Shader(const std::string& vertSrcFile, const std::string& fragSrcFile)
    : m_ShaderID(0), m_Bound(false)
{
    Compile(vertSrcFile, fragSrcFile);
}

Shader::~Shader()
{
    glDeleteProgram(m_ShaderID);
}

Shader::Shader(Shader&& other)
{
    m_ShaderID = other.m_ShaderID;
    m_Bound = other.m_Bound;

    other.m_ShaderID = 0;
}

Shader& Shader::operator=(Shader&& other)
{
    m_ShaderID = other.m_ShaderID;
    m_Bound = other.m_Bound;

    other.m_ShaderID = 0;

    return *this;
}

bool Shader::Compile(const std::string& vertSrcFile, const std::string& fragSrcFile)
{
    // Create an empty vertex shader handle
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Send the vertex shader source code to GL
    // Note that std::string's .c_str is NULL character terminated.
    std::string code = ReadFile(vertSrcFile);
    const GLchar* source = code.c_str();
    glShaderSource(vertexShader, 1, &source, 0);

    // Compile the vertex shader
    glCompileShader(vertexShader);

    GLint isCompiled = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

        // We don't need the shader anymore.
        glDeleteShader(vertexShader);

        std::cout << infoLog.data() << std::endl;
        std::cout << "Vertex shader compilation failure!" << std::endl;
        return false;
    }

    // Create an empty fragment shader handle
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Send the fragment shader source code to GL
    // Note that std::string's .c_str is NULL character terminated
    code = ReadFile(fragSrcFile);
    source = code.c_str();
    glShaderSource(fragmentShader, 1, &source, 0);

    // Compile the fragment shader
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

        // We don't need the shader anymore.
        glDeleteShader(fragmentShader);
        // Either of them. Don't leak shaders.
        glDeleteShader(vertexShader);

        std::cout << infoLog.data() << std::endl;
        std::cout << "Fragment shader compilation failure!" << std::endl;
        return false;
    }

    // Vertex and fragment shaders are successfully compiled.
    // Now time to link them together into a program.
    // Get a program object.
    m_ShaderID = glCreateProgram();

    // Attach our shaders to our program
    glAttachShader(m_ShaderID, vertexShader);
    glAttachShader(m_ShaderID, fragmentShader);

    // Link our program
    glLinkProgram(m_ShaderID);

    // Note the different functions here: glGetProgram* instead of glGetShader*.
    GLint isLinked = 0;
    glGetProgramiv(m_ShaderID, GL_LINK_STATUS, (int*)&isLinked);
    if (isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(m_ShaderID, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(m_ShaderID, maxLength, &maxLength, &infoLog[0]);

        // We don't need the program anymore.
        glDeleteProgram(m_ShaderID);
        // Don't leak shaders either.
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        std::cout << infoLog.data() << std::endl;
        std::cout << "Shader Link Error" << std::endl;
        return false;
    }

    // Always detach shaders after a successful link.
    glDetachShader(m_ShaderID, vertexShader);
    glDetachShader(m_ShaderID, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return true;
}



void Shader::Init()
{
}


void Shader::Bind()
{
    glUseProgram(m_ShaderID);
    m_Bound = true;
}

void Shader::Unbind()
{
    glUseProgram(0);
    m_Bound = false;
}

void Shader::UploadUniformFloat(const std::string& name, float val) {
    bool unbind = !m_Bound;
    Bind();
    GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
    if (location == -1) {
        PY_TRACE("Uniform location not found: {}", name);
        if (unbind) Unbind();
        return;
    }
    glUniform1f(location, val);
    if (unbind) Unbind();
}

void Shader::UploadUniformFloat2(const std::string& name, const glm::vec2& vec) {
    bool unbind = !m_Bound;
    Bind();
    GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
    if (location == -1) {
        PY_TRACE("Uniform location not found: {}", name);
        if (unbind) Unbind();
        return;
    }
    glUniform2fv(location, 1, glm::value_ptr(vec));
    if (unbind) Unbind();
}

void Shader::UploadUniformFloat3(const std::string& name, const glm::vec3& vec) {
    bool unbind = !m_Bound;
    Bind();
    GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
    if (location == -1) {
        PY_TRACE("Uniform location not found: {}", name);
        if (unbind) Unbind();
        return;
    }
    glUniform3fv(location, 1, glm::value_ptr(vec));
    if (unbind) Unbind();
}

void Shader::UploadUniformFloat4(const std::string& name, const glm::vec4& vec) {
    bool unbind = !m_Bound;
    Bind();
    GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
    if (location == -1) {
        PY_TRACE("Uniform location not found: {}", name);
        if (unbind) Unbind();
        return;
    }
    glUniform4fv(location, 1, glm::value_ptr(vec));
    if (unbind) Unbind();
}

void Shader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix) {
    bool unbind = !m_Bound;
    Bind();
    GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
    if (location == -1) {
        PY_TRACE("Uniform location not found: {}", name);
        if (unbind) Unbind();
        return;
    }
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    if (unbind) Unbind();
}

void Shader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix) {
    bool unbind = !m_Bound;
    Bind();
    GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
    if (location == -1) {
        PY_TRACE("Uniform location not found: {}", name);
        if (unbind) Unbind();
        return;
    }
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    if (unbind) Unbind();
}

void Shader::UploadUniformInt(const std::string& name, int val) {
    bool unbind = !m_Bound;
    Bind();
    GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
    if (location == -1) {
        PY_TRACE("Uniform location not found: {}", name);
        if (unbind) Unbind();
        return;
    }
    glUniform1i(location, val);
    if (unbind) Unbind();
}

void Shader::UploadUniformUInt(const std::string& name, unsigned int val) {
    bool unbind = !m_Bound;
    Bind();
    GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
    if (location == -1) {
        PY_TRACE("Uniform location not found: {}", name);
        if (unbind) Unbind();
        return;
    }
    glUniform1ui(location, val);
    if (unbind) Unbind();
}

void Shader::UploadUniformInt2(const std::string& name, const glm::ivec2& vec) {
    bool unbind = !m_Bound;
    Bind();
    GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
    if (location == -1) {
        PY_TRACE("Uniform location not found: {}", name);
        if (unbind) Unbind();
        return;
    }
    glUniform2iv(location, 1, glm::value_ptr(vec));
    if (unbind) Unbind();
}

void Shader::UploadUniformInt3(const std::string& name, const glm::ivec3& vec) {
    bool unbind = !m_Bound;
    Bind();
    GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
    if (location == -1) {
        PY_TRACE("Uniform location not found: {}", name);
        if (unbind) Unbind();
        return;
    }
    glUniform3iv(location, 1, glm::value_ptr(vec));
    if (unbind) Unbind();
}

void Shader::UploadUniformInt4(const std::string& name, const glm::ivec4& vec) {
    bool unbind = !m_Bound;
    Bind();
    GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
    if (location == -1) {
        PY_TRACE("Uniform location not found: {}", name);
        if (unbind) Unbind();
        return;
    }
    glUniform4iv(location, 1, glm::value_ptr(vec));
    if (unbind) Unbind();
}

void Shader::UploadUniformBool(const std::string& name, bool val) {
    bool unbind = !m_Bound;
    Bind();
    GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
    if (location == -1) {
        PY_TRACE("Uniform location not found: {}", name);
        if (unbind) Unbind();
        return;
    }
    glUniform1i(location, val);
    if (unbind) Unbind();
}

void Shader::UploadUniformFloatArray(const std::string& name, float* val, int count) {
    bool unbind = !m_Bound;
    Bind();
    GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
    if (location == -1) {
        PY_TRACE("Uniform location not found: {}", name);
        if (unbind) Unbind();
        return;
    }
    glUniform1fv(location, count, val);
    if (unbind) Unbind();
}

void Shader::UploadUniformFloat2Array(const std::string& name, float* vec, int count) {
    bool unbind = !m_Bound;
    Bind();
    GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
    if (location == -1) {
        PY_TRACE("Uniform location not found: {}", name);
        if (unbind) Unbind();
        return;
    }
    glUniform2fv(location, count, vec);
    if (unbind) Unbind();
}

void Shader::UploadUniformFloat3Array(const std::string& name, float* vec, int count) {
    bool unbind = !m_Bound;
    Bind();
    GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
    if (location == -1) {
        PY_TRACE("Uniform location not found: {}", name);
        if (unbind) Unbind();
        return;
    }
    glUniform3fv(location, count, vec);
    if (unbind) Unbind();
}

void Shader::UploadUniformFloat4Array(const std::string& name, float* vec, int count) {
    bool unbind = !m_Bound;
    Bind();
    GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
    if (location == -1) {
        PY_TRACE("Uniform location not found: {}", name);
        if (unbind) Unbind();
        return;
    }
    glUniform4fv(location, count, vec);
    if (unbind) Unbind();
}

void Shader::UploadUniformMat3Array(const std::string& name, float* matrix, int count) {
    bool unbind = !m_Bound;
    Bind();
    GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
    if (location == -1) {
        PY_TRACE("Uniform location not found: {}", name);
        if (unbind) Unbind();
        return;
    }
    glUniformMatrix3fv(location, count, GL_FALSE, matrix);
    if (unbind) Unbind();
}

void Shader::UploadUniformMat4Array(const std::string& name, float* matrix, int count) {
    bool unbind = !m_Bound;
    Bind();
    GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
    if (location == -1) {
        PY_TRACE("Uniform location not found: {}", name);
        if (unbind) Unbind();
        return;
    }
    glUniformMatrix4fv(location, count, GL_FALSE, matrix);
    if (unbind) Unbind();
}

void Shader::UploadUniformIntArray(const std::string& name, int* val, int count) {
    bool unbind = !m_Bound;
    Bind();
    GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
    if (location == -1) {
        PY_TRACE("Uniform location not found: {}", name);
        if (unbind) Unbind();
        return;
    }
    glUniform1iv(location, count, val);
    if (unbind) Unbind();
}

void Shader::UploadUniformInt2Array(const std::string& name, int* vec, int count) {
    bool unbind = !m_Bound;
    Bind();
    GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
    if (location == -1) {
        PY_TRACE("Uniform location not found: {}", name);
        if (unbind) Unbind();
        return;
    }
    glUniform2iv(location, count, vec);
    if (unbind) Unbind();
}

void Shader::UploadUniformInt3Array(const std::string& name, int* vec, int count) {
    bool unbind = !m_Bound;
    Bind();
    GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
    if (location == -1) {
        PY_TRACE("Uniform location not found: {}", name);
        if (unbind) Unbind();
        return;
    }
    glUniform3iv(location, count, vec);
    if (unbind) Unbind();
}

void Shader::UploadUniformInt4Array(const std::string& name, int* vec, int count) {
    bool unbind = !m_Bound;
    Bind();
    GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
    if (location == -1) {
        PY_TRACE("Uniform location not found: {}", name);
        if (unbind) Unbind();
        return;
    }
    glUniform4iv(location, count, vec);
    if (unbind) Unbind();
}

void Shader::UploadUniformBoolArray(const std::string& name, bool* val, int count) {
    bool unbind = !m_Bound;
    Bind();
    GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
    if (location == -1) {
        PY_TRACE("Uniform location not found: {}", name);
        if (unbind) Unbind();
        return;
    }
    glUniform1iv(location, count, reinterpret_cast<int*>(val));
    if (unbind) Unbind();
}


void Shader::CreateShader(const std::string& vertFilePath, const std::string& fragFilePath, const std::string& shaderName)
{
    auto [it, inserted] = s_LoadedShaders.try_emplace(shaderName, Shader(vertFilePath, fragFilePath, true));
    if (inserted)
        PY_TRACE("Created Shader: {}", shaderName);
    else
        PY_TRACE("Shader Already Exists: {}", shaderName);
}

Shader* Shader::GetShader(const std::string& name)
{
    if (s_LoadedShaders.contains(name))
        return &s_LoadedShaders[name];
    else
    {
        PY_TRACE("Shader Doesn't Exist: {}", name);
        return nullptr;
    }
}

