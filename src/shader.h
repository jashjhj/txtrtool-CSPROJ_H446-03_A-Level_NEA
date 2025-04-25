#pragma once

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>


class Shader
{
public:


    // constructor reads and builds the shader
    Shader(); // defualt constuctor - do not use
    Shader(const char*, const char* fragment_filepath);

    //shader(); // null constructor - do not use
    ~Shader();

    
    /// <summary>
    /// Sets shader to current for rendering
    /// </summary>
    void use();

    // utility uniform functions
    void set_bool(const std::string &name, bool);
    void set_int(const std::string &name, int);
    void set_float(const std::string& name, float);

    void set_2d_float(const std::string& name, float, float);


private:
    unsigned int ID;
};

