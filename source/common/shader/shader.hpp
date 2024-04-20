#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace our {

    class ShaderProgram {

    private:
        //Shader Program Handle (OpenGL object name)
        GLuint program;

    public:
        ShaderProgram(){
            //TODO: (Req 1) Create A shader program
            program = glCreateProgram();
        }
        ~ShaderProgram(){
            //TODO: (Req 1) Delete a shader program
            glDeleteProgram(program);
        }

        bool attach(const std::string &filename, GLenum type) const;

        bool link() const;

        void use() { 
            glUseProgram(program);
        }

        GLuint getUniformLocation(const std::string &name) {
            //TODO: (Req 1) Return the location of the uniform with the given name
            GLuint time_loc = glGetUniformLocation(program, name.c_str());
            return time_loc;
        }

        void set(const std::string &uniform, GLfloat value) {
            //TODO: (Req 1) Send the given float value to the given uniform
            GLuint time_loc = getUniformLocation(uniform);
            glUniform1f(time_loc, value);
        }

        void set(const std::string &uniform, GLuint value) {
            //TODO: (Req 1) Send the given unsigned integer value to the given uniform
            GLuint time_loc = getUniformLocation(uniform);
            glUniform1ui(time_loc, value);
        }

        void set(const std::string &uniform, GLint value) {
            //TODO: (Req 1) Send the given integer value to the given uniform
            GLuint time_loc = getUniformLocation(uniform);
            glUniform1i(time_loc, value);
        }

        void set(const std::string &uniform, glm::vec2 value) {
            //TODO: (Req 1) Send the given 2D vector value to the given uniform
            GLuint time_loc = getUniformLocation(uniform);
            glUniform2f(time_loc, value[0], value[1]);
        }

        void set(const std::string &uniform, glm::vec3 value) {
            //TODO: (Req 1) Send the given 3D vector value to the given uniform
            GLuint time_loc = getUniformLocation(uniform);
            glUniform3f(time_loc, value[0], value[1], value[2]);
        }

        void set(const std::string &uniform, glm::vec4 value) {
            //TODO: (Req 1) Send the given 4D vector value to the given uniform
            GLuint time_loc = getUniformLocation(uniform);
            glUniform4f(time_loc, value[0], value[1], value[2], value[3]);
        }

        void set(const std::string &uniform, glm::mat4 matrix) {
            //TODO: (Req 1) Send the given matrix 4x4 value to the given uniform
            GLuint time_loc = getUniformLocation(uniform);
            glUniformMatrix4fv(time_loc, 1, GL_FALSE, &matrix[0][0]);
        }

        //TODO: (Req 1) Delete the copy constructor and assignment operator.
        ShaderProgram (const ShaderProgram&) = delete;
        ShaderProgram& operator= (const ShaderProgram&) = delete;
        //Question: Why do we delete the copy constructor and assignment operator?
        //Answer: To prevent copying of resource, achieving RAII (Resource Acquisiion Is Initialization) rule in C++.
        //Resource acquisition and release is scope-bounded (to the lifetime of object). Also, according to 3/5 rule in C++,
        //if you write for a class one of a destructor, copy/move constructor, or copy/move assignment operator, then you must write
        //all of them. If we attempt to return resource, for example, by calling a function, the resource would be copied, but then
        //again, it will be destroyed at the end of the function call, meaning we now have an object name (in OpenGL) that has
        //been destroyed. We need to prevent such cases.
    };

}

#endif