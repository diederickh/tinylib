*************
API Reference
*************

The Tiny Lib api contains a plain functions which are all prefixed with
``rx_`` and some classes. Below we describe the API per category.

Utils
-----

.. function:: GLuint rx_create_shader(GLenum, const char*)

    Creates a shader for the given type and source.              

    :param GLenum: What kind of shader to create ``GL_VERTEX_SHADER``, ``GL_FRAGMENT_SHADER``
    :param const char*: Pointer to the shader source
    :rtype: GLuint, the created shader.
