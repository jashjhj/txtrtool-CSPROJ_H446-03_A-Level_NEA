#include "shader.h"

Shader::Shader()
{
}

Shader::Shader(const char* vertex_path, const char* fragment_path)
{
    
    // 0. init extra values


    // 1. Read shader files

    std::string vert_code;
    std::string frag_code;
    std::ifstream vert_shader_file;
    std::ifstream frag_shader_file;

    // lets the files throw exceptions if errors - as compiles @ runtime
    vert_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    frag_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        // open files
        vert_shader_file.open(vertex_path);
        frag_shader_file.open(fragment_path);

        // read file's buffer contents into streams
        std::stringstream vert_shader_stream, frag_shader_stream;
        vert_shader_stream << vert_shader_file.rdbuf();
        frag_shader_stream << frag_shader_file.rdbuf();

        // close files
        vert_shader_file.close();
        frag_shader_file.close();

        // convert stream into string
        vert_code = vert_shader_stream.str();
        frag_code = frag_shader_stream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl; // if it all goes horribly wrong - send error message
    }

    const char* vert_shader_code = vert_code.c_str();
    const char* frag_shader_code = frag_code.c_str();


    // 2. Attempt to compile shaders

    unsigned int vertex, fragment;
    int success;
    char info_log[512];
    
    // 2.a. vertex

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vert_shader_code, NULL);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success) // if failed to compile
    {
        glGetShaderInfoLog(vertex, 512, NULL, info_log);
        std::cout << "ERROR! Vertex Shader failed to compile!\n" << info_log << std::endl << std::endl; // return error logs for debug
    }

    //2.b. fragment
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &frag_shader_code, NULL);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, info_log);
        std::cout << "ERROR! Fragment Shader failed to compile!\n" << info_log << std::endl << std::endl; // again, return error logs for debug
    }


    // 3. Assemble the shaders into one program

    this->ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(ID, 512, NULL, info_log);
        std::cout << "ERROR! Shader Program failed to compile!\n" << info_log << std::endl << std::endl; // one last time, return error logs for debug
    }
    else {
        std::cout << "Shader successfully loaded!\n";
    }



    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);


}


Shader::~Shader() {
    glDeleteProgram(this->ID);
}

void Shader::use()
{
    glUseProgram(this->ID);
}

void Shader::set_bool(const std::string& name, bool value)
{
    this->use();
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::set_int(const std::string& name, int value)
{
    this->use();
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::set_float(const std::string& name, float value)
{
    this->use();
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::set_2d_float(const std::string& name, float value_1, float value_2)
{
    this->use();
    glUniform2f(glGetUniformLocation(ID, name.c_str()), value_1, value_2);
}

