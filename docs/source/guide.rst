
******************
Programmers Guide
******************

.. highlight:: c++

In this guide we will describe how to use Tiny Lib in your project.

Introduction to Tiny Lib
------------------------

Tiny Lib is a "one header" library. This means that the declarations (the functions,
types, etc..) and the definitions (the actual source code/implementation) are all in
the same file.  To make sure that the definitions are compiled we need to inject it
somewhere in your project. 

There are two reasons why we store the definitions and declarations in the same file:

- This allows you to use this library in your project w/o any complicated compile scripts
- This allows you to use your own openGL wrapper code that includes the GL headers

How to use Tiny Lib in your project
-----------------------------------

As described above we need to inject the definitions somewhere in your code. To do this
you need to define ``ROXLU_IMPLEMENTATION`` in one file (and only one file!) in your project
before including tinylib.h. A common place to do this is in your main.cpp after you've 
included all the necessary openGL headers (if you want to make use of the tiny lib opengl
features). Besides defining the ``ROXLU_IMPLEMENTATION`` you need to tell tiny lib what
features you want to use. You can use the following defines:

- ``ROXLU_USE_MATH`` To enable mat4, mat3, vec4, vec3, vec2, Perlin, Spline
- ``ROXLU_USE_PNG`` To enable loading and saving of PNG files
- ``ROXLU_USE_OPENGL`` To enable Shader, Program, OBJ, Painter, VertexP, etc..
- ``ROXLU_USE_JPG`` To enable loading of JPG files
- ``ROXLU_USE_CURL`` To enable loading of remote data over http
- ``ROXLU_USE_ALL`` To enable everything

Note that some of these defines enable code that are depending on other libraries see
below in the dependencies section. In the following code examples we show you an example
of how to use the ``ROXLU_IMPLEMENTATION`` in your main.cpp file and how to enable features
in a common header (`MyApplication`).

Example main.cpp::

    #define GLFW_INCLUDE_GLCOREARB
    #include <GLFW/glfw3.h>

    #define ROXLU_USE_MATH
    #define ROXLU_USE_PNG
    #define ROXLU_USE_OPENGL
    #define ROXLU_IMPLEMENTATION
    #include <tinylib.h>


Example MyApplication.h::

   #define ROXLU_USE_OPENGL
   #define ROXLU_USE_MATH
   #include <tinylib.h>

   class MyApplication() {

   } 


Dependencies
------------

When you enable certain features there are some dependencies which are listed below:

- libcurl_: When you use ``ROXLU_USE_CURL``
- libpng_: When you use ``ROXLU_USE_PNG``
- libjpeg_: When you use ``ROXLU_USE_JPG``

.. _libcurl: http://curl.haxx.se/libcurl/
.. _libpng: http://www.libpng.org/pub/png/libpng.html
.. _libjpeg: http://libjpeg.sourceforge.net/
