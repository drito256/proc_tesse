#include "../include/headers/shader.h"

    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
    Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath,
                   const char* tesseControlPath,  const char* tesseEvalPath)
    {
        // 1. retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::string geometryCode;
        std::string tesseControlCode;
        std::string tesseEvalCode;

        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        std::ifstream gShaderFile;
        std::ifstream tCShaderFile;
        std::ifstream tEShaderFile;
        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        gShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        tCShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        tEShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try 
        {
            // open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();		
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();			
            // if geometry shader path is present, also load a geometry shader
            if(geometryPath != nullptr)
            {
                gShaderFile.open(geometryPath);
                std::stringstream gShaderStream;
                gShaderStream << gShaderFile.rdbuf();
                gShaderFile.close();
                geometryCode = gShaderStream.str();
            }
            // tesselation shaders
            if(tesseControlPath != nullptr && tesseEvalPath != nullptr)
            {
                tCShaderFile.open(tesseControlPath);
                std::stringstream gTesseControlStream;
                gTesseControlStream << tCShaderFile.rdbuf();
                tCShaderFile.close();
                tesseControlCode = gTesseControlStream.str();

                tEShaderFile.open(tesseEvalPath);
                std::stringstream gTesseEvalStream;
                gTesseEvalStream << tEShaderFile.rdbuf();
                tEShaderFile.close();
                tesseEvalCode = gTesseEvalStream.str();

            }

        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char * fShaderCode = fragmentCode.c_str();
        // 2. compile shaders
        unsigned int vertex, fragment;
        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");
        
        // fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");

        // if geometry shader is given, compile geometry shader
        unsigned int geometry;
        if(geometryPath != nullptr)
        {
            const char * gShaderCode = geometryCode.c_str();
            geometry = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(geometry, 1, &gShaderCode, NULL);
            glCompileShader(geometry);
            checkCompileErrors(geometry, "GEOMETRY");
        }

        // if tesse shader is given, compile geometry shader
        unsigned int tesseControl, tesseEval;
        if(tesseControlPath != nullptr && tesseEvalPath != nullptr)
        {
            const char * tCShaderCode = tesseControlCode.c_str();
            tesseControl = glCreateShader(GL_TESS_CONTROL_SHADER);
            glShaderSource(tesseControl, 1, &tCShaderCode, NULL);
            glCompileShader(tesseControl);
            checkCompileErrors(tesseControl, "GEOMETRY");

            const char * tEShaderCode = tesseEvalCode.c_str();
            tesseEval = glCreateShader(GL_TESS_EVALUATION_SHADER);
            glShaderSource(tesseEval, 1, &tEShaderCode, NULL);
            glCompileShader(tesseEval);
            checkCompileErrors(tesseEval, "GEOMETRY");
        }
 
 

        // shader Program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        
        glAttachShader(ID, fragment);
        if(geometryPath != nullptr)
            glAttachShader(ID, geometry);
        
        if(tesseControlPath != nullptr && tesseEvalPath!=nullptr){
            glAttachShader(ID, tesseControl);
            glAttachShader(ID, tesseEval);
        }
        glLinkProgram(ID);

        checkCompileErrors(ID, "PROGRAM");
        // delete the shaders as they're linked into our program now and no longer necessery
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        if(geometryPath != nullptr)
            glDeleteShader(geometry);
        
        if(tesseControlPath != nullptr && tesseEvalPath!=nullptr){
            glDeleteShader(tesseControl);
            glDeleteShader(tesseEval);
        }
    }
    // activate the shader
    // ------------------------------------------------------------------------
    void Shader::use() 
    { 
        glUseProgram(ID); 
    }
    // utility uniform functions
    // ------------------------------------------------------------------------
    void Shader::setBool(const std::string &name, bool value) const
    {         
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
    }
    // ------------------------------------------------------------------------
    void Shader::setInt(const std::string &name, int value) const
    { 
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
    }
    // ------------------------------------------------------------------------
    void Shader::setFloat(const std::string &name, float value) const
    { 
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
    }
    // ------------------------------------------------------------------------
    void Shader::setVec2(const std::string &name, const glm::vec2 &value) const
    { 
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }
    void Shader::setVec2(const std::string &name, float x, float y) const
    { 
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y); 
    }
    // ------------------------------------------------------------------------
    void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
    { 
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }
    void Shader::setVec3(const std::string &name, float x, float y, float z) const
    { 
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z); 
    }
    // ------------------------------------------------------------------------
    void Shader::setVec4(const std::string &name, const glm::vec4 &value) const
    { 
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }
    void Shader::setVec4(const std::string &name, float x, float y, float z, float w) const
    { 
        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w); 
    }
    // ------------------------------------------------------------------------
    void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void Shader::checkCompileErrors(GLuint shader, std::string type)
    {
        GLint success;
        GLchar infoLog[1024];
        if(type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if(!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if(!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
