#include "shader.h"

#include <filesystem>
#include <fstream>
#include <memory>
#include <QOpenGLContext>

Shader::Shader(QString path, ShaderType type) :
    type_(type), file_path_(path.toStdString())
{
    gl_functions_ = QOpenGLContext::currentContext()->functions();
    switch(type_)
    {
    case ShaderType::kVertex:
        shader_id_ = gl_functions_->glCreateShader(GL_VERTEX_SHADER);
        break;
    case ShaderType::kFragment:
        shader_id_ = gl_functions_->glCreateShader(GL_FRAGMENT_SHADER);
        break;
    }
}

Shader::~Shader()
{
    gl_functions_->glDeleteShader(shader_id_);
    gl_functions_ = nullptr;
}


bool Shader::Compile()
{
    bool success = false;

    auto src = ReadSource_();

    GLchar const* shader_src = src->c_str();

    gl_functions_->glShaderSource(shader_id_, 1, &shader_src, nullptr);
    gl_functions_->glCompileShader(shader_id_);

    int status;
    gl_functions_->glGetShaderiv(shader_id_, GL_COMPILE_STATUS, &status);

    if(!status)
    {
        log_ = FetchLog_();
    }
    else
    {
        success = true;
    }

    return success;
}

std::unique_ptr<std::string> Shader::ReadSource_()
{
    std::unique_ptr<std::string> src = nullptr;

    if(std::filesystem::exists(file_path_))
    {
        std::ifstream ifs(file_path_);

        if(ifs.is_open())
        {
            src = std::make_unique<std::string>(
                        std::istreambuf_iterator<char>(ifs),
                        std::istreambuf_iterator<char>());
        }
    }

    return std::move(src);
}

std::unique_ptr<std::string> Shader::FetchLog_() const
{
    std::unique_ptr<std::string> log = nullptr;

    GLsizei log_length = 0;
    gl_functions_->glGetShaderiv(shader_id_, GL_INFO_LOG_LENGTH, &log_length);
    if(log_length > 0)
    {
        char* raw_log = new char[log_length];
        gl_functions_->glGetShaderInfoLog(shader_id_, log_length, nullptr, raw_log);

        log = std::make_unique<std::string>(raw_log);

        delete[] raw_log;
        raw_log = nullptr;
    }

    return std::move(log);
}


ShaderProgram::ShaderProgram(std::shared_ptr<Shader> vert, std::shared_ptr<Shader> frag) :
    vertex_(vert), fragment_(frag)
{
    gl_functions_ = QOpenGLContext::currentContext()->functions();

    program_id_ = gl_functions_->glCreateProgram();
}

ShaderProgram::~ShaderProgram()
{
    gl_functions_->glDeleteProgram(program_id_);
    gl_functions_ = nullptr;
}

bool ShaderProgram::Link()
{
    bool success = false;

    gl_functions_->glAttachShader(program_id_, vertex_->Id());
    gl_functions_->glAttachShader(program_id_, fragment_->Id());

    gl_functions_->glLinkProgram(program_id_);

    int status;
    gl_functions_->glGetProgramiv(program_id_, GL_LINK_STATUS, &status);

    if(!status)
    {
        log_ = FetchLog_();
    }
    else
    {
        success = true;
    }

    return success;
}

std::unique_ptr<std::string> ShaderProgram::FetchLog_() const
{
    std::unique_ptr<std::string> log = nullptr;

    GLsizei log_length = 0;
    gl_functions_->glGetProgramiv(program_id_, GL_INFO_LOG_LENGTH, &log_length);
    if(log_length > 0)
    {
        char* raw_log = new char[log_length];
        gl_functions_->glGetProgramInfoLog(program_id_, log_length, nullptr, raw_log);

        log = std::make_unique<std::string>(raw_log);

        delete[] raw_log;
        raw_log = nullptr;
    }

    return std::move(log);
}
