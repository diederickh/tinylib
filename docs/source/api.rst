*************
API Reference
*************

.. highlight:: c++

The Tiny Lib api contains a plain functions which are all prefixed with
``rx_`` and some classes. Below we describe the API per category.

Utils
-----

.. function:: GLuint rx_create_shader(GLenum, const char*)

    Creates a shader for the given type and source.
    
    ::
    
        static const char* MY_VERTEX_SHADER = ""
            "#version 330"
            "uniform mat4 u_pm;"
            "uniform mat4 u_vm;"
            "layout( location = 0 ) in vec4 a_pos;"
            ""
            "void main() {"
            "  gl_Position = u_pm * u_vm * a_pos;"
            "}"
            "";

        GLuint vert = rx_create_shader(GL_VERTEX_SHADER, MY_VERTEX_SHADER);
 

    :param GLenum: What kind of shader to create ``GL_VERTEX_SHADER``, ``GL_FRAGMENT_SHADER``
    :param const char*: Pointer to the shader source
    :rtype: GLuint, the created shader.

.. function:: GLuint rx_create_shader_from_file(GLenum, std::string)

    Creates a shader for the given type and filepath.

    :: 

         GLuint vert = rx_create_shader_from_file(GL_VERTEX_SHADER, "my_shader.vert");

    :param GLenum: What kind of shader to create ``GL_VERTEX_SHADER``, ``GL_FRAGMENT_SHADER``
    :param std::string: The filepath of the shader to load                   



