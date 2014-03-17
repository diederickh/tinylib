*************
API Reference
*************

.. highlight:: c++

The Tiny Lib api contains a plain functions which are all prefixed with
``rx_`` and some classes. Below we describe the API per category.


Overview
--------

*String and conversion utils*
  -  :cpp:func:`rx_to_float()`       
  -  :cpp:func:`rx_to_int()`
  -  :cpp:func:`rx_int_to_string()`
  -  :cpp:func:`rx_float_to_string()`
  -  :cpp:func:`rx_string_replace()`
  -  :cpp:func:`rx_split()`

*File Utils*
  - :cpp:func:`rx_get_exe_path()`
  - :cpp:func:`rx_to_data_path()`
  - :cpp:func:`rx_is_dir()`
  - :cpp:func:`rx_file_exists()`
  - :cpp:func:`rx_strip_filename()`
  - :cpp:func:`rx_strip_dir()`
  - :cpp:func:`rx_create_dir()`
  - :cpp:func:`rx_create_path()`
  - :cpp:func:`rx_get_file_ext()`
  - :cpp:func:`rx_get_files()`
  - :cpp:func:`rx_norm_path()`
  - :cpp:func:`rx_read_file()`

*Time Utils*
  - :cpp:func:`rx_hrtime()`
  - :cpp:func:`rx_millis()`
  - :cpp:func:`rx_strftime()`
  - :cpp:func:`rx_get_year()`
  - :cpp:func:`rx_get_month()`
  - :cpp:func:`rx_get_day()`
  - :cpp:func:`rx_get_hour()`
  - :cpp:func:`rx_get_minute()`
  - :cpp:func:`rx_get_time_string()`

*Image Utils*
  - :cpp:func:`rx_save_png()`
  - :cpp:func:`rx_load_png()`
  - :cpp:func:`rx_save_jpg()`
  - :cpp:func:`rx_load_jpg()`
   
*OpenGL*
  - :cpp:func:`rx_create_shader()`
  - :cpp:func:`rx_create_shader_from_file()`
  - :cpp:func:`rx_create_program_with_attribs()`
  - :cpp:func:`rx_get_uniform_location()`
  - :cpp:func:`rx_uniform_1i()`
  - :cpp:func:`rx_uniform_1f()`
  - :cpp:func:`rx_uniform_mat4fv()`
  - :cpp:func:`rx_create_png_screenshot()`
  - :cpp:func:`rx_create_jpg_screenshot()`
 
 

String and conversion utils
---------------------------

.. cpp:function:: float rx_to_float(const std::string&)

   Convert a string to a float value.

.. cpp:function:: int rx_to_int(const std::string&)
   
   Convert a string to a integer value.

.. cpp:function:: std::string rx_string_replace(std::string, char from, char to)

   Convert a character in a string into another value

.. cpp:function:: std::string rx_string_replace(std::string, std::string from, std::string to)

   Convert a string from one value into another.

.. cpp:function:: std::string rx_int_to_string(const int& v)

   Convert an integer value into a string.

.. cpp:function:: std::string rx_float_to_string(const float& v)

   Convert a float to a string.

.. cpp:function:: std::vector<std::string> rx_split(std::string str, char delim)

   Split a string on the given delimiter and return a ``std::vector<std::string>`` with the
   separate parts.

File utils
----------

.. cpp:function:: std::string rx_get_exe_path()
 
   Returns the the full path to the directory where the executable is located.

.. cpp:function:: std::string rx_to_data_path(const std::string filename)

   Returns a path to what we call a data path. A data path is a directory where you
   can store things like textures, shaders, fonts, etc. By default this will return 
   a path to your executable with ``data/``.

.. cpp:function:: bool rx_is_dir(std::string filepath)
   
   Checks if the given path is an directory

.. cpp:function:: bool rx_file_exists(std::string filepath)

   Checks if the given filepath exists and returns true if it does, otherwise it
   will return false.

.. cpp:function:: std::string rx_strip_filename(std::string filepath)
            
   Removes the filename from the given path, returning only the full path.

.. cpp:function:: std::string rx_strip_dir(std::string filepath)

   Removes the path from the given filepath keeping only the filename.

.. cpp:function:: bool rx_create_dir(std::string path)

   Create the given (sub) directory.

.. cpp:function:: bool rx_create_path(std::string filepath)

   Create a complete path. This will create all the given paths that don't exist.

   ::
 
      rx_create_path(rx_to_data_path("2014/01/16/"));


.. cpp:function:: std::string rx_get_file_ext(std::string filename)

   This will return the filename extension, like "jpg", "gif", etc..

   ::

      std::string ext = rx_get_file_ext("/file/image.jpg");
      printf("%s\n", ext.c_str()); // will print 'jpg'

.. cpp:function:: std::vector<std::string> rx_get_files(std::string path, std::string ext = "")

   Get all the files in the given path. You can specify a file extension filter like "jpg",
   "gif" etc.. 

.. cpp:function:: std::string rx_norm_path(std::string path)

   Creates a normalized, cross platform path. Always pass in forward slashes; on windows
   we will convert these to backslahes:

   ::
   
       std::string normpath = rx_norm_path("/path/to/my/dir");
   
.. cpp:function:: std::string rx_read_file(std::string filepath)

   Read a file into a string.


Time utils
----------

.. cpp:function:: uint64_t rx_hrtime()

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

.. cpp:function:: float rx_millis()

   Returns the time since the first call to this function in milliseconds.


.. cpp:function:: std::string rx_strftime(const std::string fmt)

   Wrapper around ``strftime`` which returns a a time/date.

   ::

        std::string datetime = rx_strftime("%Y/%m/%d");
        printf("%s\n", datetime.c_str()); // prints e.g. 2014/01/16


.. cpp:function:: std::string rx_get_year()

   Get the current year with 4 digits, eg. 2014

.. cpp:function:: std::string rx_get_month()

   Get the current month with 2 digits, [00-11]

.. cpp:function:: std::string rx_get_day()

   Get the current day of the month with 2 digits, [00-031]

.. cpp:function:: std::string rx_get_hour()

   Get the current hour with 2 digits, [00-23]

.. cpp:function:: std::string rx_get_minute()

   Get the current minutes with 2 digits, [00-60]

.. cpp:function:: std::string rx_get_time_string()

   Returns a string for the current date-time with milli second
   accuracy. This function is handy if you want to create unique
   filenames for example (as long as there is some time between
   each time you call this function to prevent duplicates).

   ::
  
       std::string time_string = rx_get_time_string();
       printf("%s\n", time_string.c_str()); // prints something like: 2014.01.16_19.01.00_328



Image utils
-----------

.. cpp:function:: bool rx_save_png(std::string file, unsigned char* pix, int w, int h, int nchannels, bool flip)

   Save the given pixels to the given file path.
              
   ::
    
        int width = 320;
        int height = 240;
        unsigned char* pix = new unsigned char[width * height * 3];
        
        // some pixel data
        for(int i = 0; i < width; ++i) {
           for(int j = 0; j < height; ++j) {
               int dx = j * width * 3 + i * 3;
               if(i < (width/2)) {
                   pix[dx + 0] = 255;
                   pix[dx + 1] = 255;
                   pix[dx + 2] = 255;
               }
               else {
                   pix[dx + 0] = 0;
                   pix[dx + 1] = 0;
                   pix[dx + 2] = 0;
               }
           }
        }
 
        std::string outfile = rx_to_data_path("test.png");

        if(rx_save_png(outfile, pix, width, height, 3) == false) {
            printf("Error: cannot save PNG: %s\n", outfile.c_str());
            ::exit(EXIT_FAILURE);
        }


   :param string: ``file`` Full file path where to save the image
   :param unsigned char*: ``pix`` Pointer to the raw pixels you want to save
   :param int:  ``w`` The width of the pixel buffer 
   :param int: ``h`` The height of the pixel buffer
   :param int: ``nchannels`` The number of color components (e.g. 1 for grayscale, 3 for RGB)
   :param bool: ``flip`` Set to true if you want to flip the image horizontally (handy when using ``glReadPixels()``)
   :returns: boolean true on success, else false.


.. cpp:function:: bool rx_load_png(std::string file, unsigned char** pix, int& w, int& h, int& nchannels)

   Load a PNG file from the given filepath and create a pixel buffer, set width, height and nchannels.
   
   ::

       int w = 0;
       int h = 0;    
       int channels = 0;
       unsigned char* pix = NULL;

       if(rx_load_png("test.png", pix, w, h, channels) == false) {
          printf("Error: cannot load the png.\n");
          ::exit(EXIT_FAILURE)
       }

       printf("Width: %d\n", w);
       printf("Height: %d\n", h);
       printf("Color Channels: %d\n", channels);


   :param string: ``file`` Load the png from this filepath.
   :param unsigned char*: ``pix`` (out)  We will allocate a ``unsigned char`` buffer for you; you need to delete this buffer yourself!
   :param int&: ``w`` (out) Reference to the width result. We will set the width value of the loaded image to ``w``.
   :param int&: ``h`` (out) Reference to the height result. We will set the height value of the loaded image to ``h``.
   :param int&: ``nchannels`` (out) The number of color channels in the loaded png.
   :returns: true on success, else false


.. cpp:function:: bool rx_load_jpg(std::string file, unsigned char** pix, int& w, int& height, int& nchannels)

   Loads a JPG file, see ``rx_load_png`` for an example as the function works the same, but only loads a JPG.

   :param string: ``file`` Load the jpg from this filepath.
   :param unsigned char*: ``pix`` (out) We will allocate a ``unsigned char`` buffer for you; you need to delete this buffer yourself!
   :param int&: ``w`` (out) Reference to the width result. We will set the width value of the loaded image to ``w``.
   :param int&: ``h`` (out) Reference to the height result. We will set the height value of the loaded image to ``h``.
   :param int&: ``nchannels`` (out) The number of color channels in the loaded jpg.
   :returns: true on success, else false

.. cpp:function:: bool rx_save_jpg(std::string file, unsigned char* pix, int width, int height, int nchannels, int quality = 80, bool flip = false, J_COLOR_SPACE colorSpace, J_DCT_METHOD dctMethod = JDCT_FASTEST)

   :param string: ``file`` Save a jpg to this filepath.
   :param unsigned char*: ``pix`` The pixels you want to save.
   :param int: ``width`` The width of the ``pix`` buffer.
   :param int: ``height`` The height of the ``pix`` buffer.
   :param int: ``nchannels`` The number of color channels. (e.g. 3).
   :param int: ``quality`` The quality (reasonable values are 65-100, 80 is ok)
   :param bool: ``flip`` Flip the given input pixels horizontally (e.g. nice when using ``glReadPixels()``)
   :param J_COLOR_SPACE: ``colorSpace`` The JPEG color space that you pass as `pix`, by default ``JCS_RGB``. Other options ``JCS_GRAYSCALE``, ``JCS_YCbCr``, ``JCS_CMYK``, ``JCS_YCCK``
   :param J_DCT_METHOD: ``dctMethod`` DCT/IDCT algorithms, by default ``JDCT_FASTEST``. Other options ``JDCT_ISLOW``, ``JDCT_IFAST``, ``JDCT_FLOAT``, ``JDCT_SLOWEST``
   :returns: true on success else false

OpenGL
------

.. cpp:function:: GLuint rx_create_shader(GLenum, const char*)

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
   :returns: GLuint, the created shader.

.. cpp:function:: GLuint rx_create_shader_from_file(GLenum, std::string)

   Creates a shader for the given type and filepath.

   ::

         GLuint vert = rx_create_shader_from_file(GL_VERTEX_SHADER, "my_shader.vert");

   :param GLenum: What kind of shader to create ``GL_VERTEX_SHADER``, ``GL_FRAGMENT_SHADER``
   :param string: The filepath of the shader to load                   


.. cpp:function:: GLuint rx_create_program(GLuint vert, GLuint frag, bool link = false)

   Create a shader program from the given vertex and fragment shaders. 
   Set ``link`` to true if you want to link the shader program as well.
   Sometimes, especially when using GLSL < 330, you want to bind the attribute
   locations in your shader. In this case you'll pass ``link = false``. Otherwise, 
   when using version 330 you can use the ``layout( location = 0 )`` directives.

   :param GLuint: ``vert`` The vertex shader.
   :param GLuint: ``frag`` The fragment shader.
   :returns: ``GLuint`` We return the newly created shader program id (not linked). 

.. cpp:function:: GLuint rx_create_program_with_attribs(GLuint vert, GLuint frag, int nattribs, const char** attribs)
      
   This function is similar to ``rx_create_program`` except that it will bind the 
   attribute locations for you. The indices of the given ``attribs`` array are used
   as bind locations. The example below will bind ``a_pos`` at index 0, ``a_tex`` at 1 and
   ``a_col`` at 2. This function will also link the shader.

   ::

       const char* attribs[] = { "a_pos", "a_tex", "a_col" } ;
       GLuint prog = rx_create_program_with_attribs(vert, frag, 3, attribs);

   :param GLuint: ``vert`` The vertex shader.
   :param GLuint: ``frag`` The fragment shader.
   :param int: ``nattribs`` The number of attributes in the ``attribs`` array.
   :param const char**: ``attribs`` The attributes that you want to set.
   :returns: ``GLuint`` A linked program.

.. cpp:function:: GLint rx_get_uniform_location(GLuint prog, std::string name)

   Safe way to retrieve uniform locations. When compiled in debug mode, 
   this function will make sure that the uniform is found and will cause
   an assertion if we cannot find the uniform (which often means it's 
   optimized away and thus not used in the shader). Make sure that your 
   shader is active (``glUseProgram(prog)``).

   :param GLuint: ``prog`` The shader program. 
   :param string: ``name`` The name of the uniform for which you want the location.   
   :returns: ``GLint`` The location of the uniform (-1 on failure).

.. cpp:function:: void rx_uniform_1i(GLuint prog, std::string name, GLint v)

   This function will set the uniform with the given name to ``v``. This 
   function is to be used in your setup routines. It's an easy wrapper for
   e.g. settings uniform locations for your texture samplers.

   :param GLuint: ``prog`` The program that contains the ``name`` uniform.
   :param string: ``name`` The name of the uniform you want to set. 
   :param GLint: ``v`` The value you want to set.
   :returns: void


.. cpp:function:: void rx_uniform_1f(GLuint prog, std::string name, GLfloat v)

   Similar to ``rx_uniform_1i`` ecxept you use this to set float values.

   :param GLuint: ``prog`` The program that contains the ``name`` uniform.
   :param string: ``name`` The name of the uniform you want to set. 
   :param GLfloat: ``v`` The value you want to set.
   :returns: void

.. cpp:function:: void rx_uniform_mat4fv(GLuint prog, std::string name, GLint count, GLboolean transpose, GLfloat* value)

   Wrapper around `glUniformMatrix4fv`. This function will set the given matrix.

   :param GLuint: ``prog`` The program that contains the ``name`` uniform.
   :param string: ``name`` The name of the uniform you want to set. 
   :param GLint: ``count`` Number of matrices to set. 
   :param GLboolean: ``transpose`` Transpose the matrix.
   :param const float*: ``value`` Pointer to the matrix.
   :returns: void


.. cpp:function:: bool rx_create_png_screenshot(std::string filepath)

   Creates a PNG screenshot of the current read buffer with the size of 
   the current viewport. The image is saved to the given ``filepath``. 
   Note that this function allocates some static memory so we don't have to
   allocate every time you create a screenshot. This does mean that we "leak"
   a couple of bytes and that you need to be carefull calling this function from 
   different threads at the same time.

   **NOTE:** This means that both ``ROXLU_USE_OPENGL`` and ``ROXLU_USE_PNG`` must be 
   enabled.
   
   :param string: ``filepath`` The file path where you want to save the screenshot.
   :returns: boolean, true on success else false. 

.. cpp:function:: bool rx_create_jpg_screenshot(std::string filepath, int quality)

   Same as ``rx_create_png_screenshot()`` but this creates a JPG image. Writing 
   JPGs to disk is often faster then writing PNG images. 

   **NOTE:** This means that both ``ROXLU_USE_OPENGL`` and ``ROXLU_USE_PNG`` must be 
   enabled.
   
   :param string: ``filepath`` The file path where you want to save the screenshot.
   :param integer: ``quality`` The quality level for the JPG.
   :returns: boolean, true on success else false. 


