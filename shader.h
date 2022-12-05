#ifndef SHADER_H
#define SHADER_H

#include <QOpenGLFunctions>
#include <QString>

enum ShaderType
{
    kVertex,
    kFragment
};

class Shader
{
public:
    Shader(QString path, ShaderType type);
    ~Shader();

    ShaderType Type() const { return type_; }
    unsigned int Id() const { return shader_id_; }

    bool Compile();
    std::unique_ptr<std::string> Log() { return std::move(log_); }

protected:

    std::unique_ptr<std::string> ReadSource_();
    std::unique_ptr<std::string> FetchLog_() const;

    unsigned int shader_id_;
    ShaderType type_;
    std::string file_path_;

    std::unique_ptr<std::string> log_ = nullptr;

    QOpenGLFunctions* gl_functions_ = nullptr;
};

class ShaderProgram
{
public:
    ShaderProgram(std::shared_ptr<Shader> vert, std::shared_ptr<Shader> frag);
    ~ShaderProgram();

    bool Link();

    std::unique_ptr<std::string> Log() { return std::move(log_); }
    unsigned int Id() const { return program_id_; }

private:
    std::unique_ptr<std::string> FetchLog_() const;

    unsigned int program_id_;

    std::shared_ptr<Shader> vertex_;
    std::shared_ptr<Shader> fragment_;

    std::unique_ptr<std::string> log_ = nullptr;

    QOpenGLFunctions* gl_functions_ = nullptr;
};

#endif // SHADER_H
