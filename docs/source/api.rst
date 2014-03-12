*************
API Reference
*************

.. highlight:: c++

The Tiny Lib api contains a plain functions which are all prefixed with
``rx_`` and some classes. Below we describe the API per category.

String and conversion utils
---------------------------

.. function:: float rx_to_float(const std::string&)

   Convert a string to a float value.

.. function:: int rx_to_int(const std::string&)
   
   Convert a string to a integer value.

.. function:: std::string rx_string_replace(std::string, char from, char to)

   Convert a character in a string into another value

.. function:: std::string rx_string_replace(std::string, std::string from, std::string to)

   Convert a string from one value into another.

.. function:: std::string rx_int_to_string(const int& v)

   Convert an integer value into a string.

.. function:: std::string rx_float_to_string(const float& v)

   Convert a float to a string.

.. function:: std::vector<std::string> rx_split(std::string str, char delim)

   Split a string on the given delimiter and return a ``std::vector<std::string>`` with the
   separate parts.

File utils
----------

.. function:: std::string rx_get_exe_path()
 
   Returns the the full path to the directory where the executable is located.

.. function:: std::string rx_to_data_path(const std::string filename)

   Returns a path to what we call a data path. A data path is a directory where you
   can store things like textures, shaders, fonts, etc. By default this will return 
   a path to your executable with ``data/``.

.. function:: bool rx_is_dir(std::string filepath)
   
   Checks if the given path is an directory

.. function:: bool rx_file_exists(std::string filepath)

   Checks if the given filepath exists and returns true if it does, otherwise it will return 
   false.

.. function:: std::string rx_strip_filename(std::string filepath)
            
   Removes the filename from the given path, returning only the full path.

.. function:: std::string rx_strip_dir(std::string filepath)

   Removes the path from the given filepath keeping only the filename.

.. function:: bool rx_create_dir(std::string path)

   Create the given (sub) directory.

.. function:: bool rx_create_path(std::string filepath)

   Create a complete path. This will create all the given paths that don't exist.

   ::
 
        rx_create_path(rx_to_data_path("2014/01/16/"));


.. function:: std::string rx_get_file_ext(std::string filename)

   This will return the filename extension, like "jpg", "gif", etc..

   ::

        std::string ext = rx_get_file_ext("/file/image.jpg");
        printf("%s\n", ext.c_str()); // will print 'jpg'

.. function:: std::vector<std::string> rx_get_files(std::string path, std::string ext = "")

   Get all the files in the given path. You can specify a file extension filter like "jpg",
   "gif" etc.. 

.. function:: std::string rx_norm_path(std::string path)

   Creates a normalized, cross platform path. Always pass in forward slashes; on windows
   we will convert these to backslahes:

   ::
      
      std::string normpath = rx_norm_path("/path/to/my/dir");
   
.. function:: std::string rx_read_file(std::string filepath)

   Read a file into a string.


Time utils
----------

.. function:: uint64_t rx_hrtime()

   A high resolution timer in nano seconds.

   ::

      // somewhere we have a defined a delay and timeout
      uint64_t delay = 1000ull * 1000ull * 1000ull; // 1 second, 1000 millis
      uint64_t timeout = rx_hrtime() + delay;

      // then somewhere else you can check if this delay has been reached
      uint64_t now = rx_hrtime();
      if(now > timeout) {     
         // Do something every second.
         timeout = rx_hrtime() + delay; // set new delay      
      }

.. function:: float rx_millis()

   Returns the time since the first call to this function in milliseconds.


.. function:: std::string rx_strftime(const std::string fmt)

   Wrapper around ``strftime`` which returns a a time/data.

   :: 
       
       std::string datetime = rx_strftime("%Y/%m/%d");
       printf("%s\n", datetime.c_str()); // prints e.g. 2014/01/16

.. function:: std::string rx_get_year()

   Get the current year with 4 digits, eg. 2014

.. function:: std::string rx_get_month()

   Get the current month with 2 digits, [00-11]

.. function:: std::string rx_get_day()

   Get the current day of the month with 2 digits, [00-031]

.. function:: std::string rx_get_hour()

   Get the current hour with 2 digits, [00-23]

.. function:: std::string rx_get_minute()

   Get the current minutes with 2 digits, [00-60]


OpenGL
------

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
    :param string: The filepath of the shader to load                   



