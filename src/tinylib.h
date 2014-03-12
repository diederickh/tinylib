/*

--------------------------------------------------------------------------------
      
                                               oooo              
                                               `888              
                oooo d8b  .ooooo.  oooo    ooo  888  oooo  oooo  
                `888""8P d88' `88b  `88b..8P'   888  `888  `888  
                 888     888   888    Y888'     888   888   888  
                 888     888   888  .o8"'88b    888   888   888  
                d888b    `Y8bod8P' o88'   888o o888o  `V88V"V8P' 
                                                                 
                                                  www.roxlu.com
                                             www.apollomedia.nl                                        
              
--------------------------------------------------------------------------------

  Tiny library with a couple of handy functions for opengl based applications.
  Sent changes to: https://github.com/roxlu/tinylib

  TODO:
  -----
  I quickly ported this file to windows, but it needs some more work and testing:
  - we're using GLXW to include GL headers now, because default windows headers are GL 1

  Usage:
  ------
  The tinylib.h file contains the function definitions and declarations. In your
  header files you only use ROXLU_USE_{ALL, OPENGL, PNG, etc..} but in your main.cpp
  file you use all the defines that you use in your app (e.g. ROXLU_USE_ALL) +
  ROXLU_IMPLEMENTATION. When you add ROXLU_IMPLEMENTATION, which is only allowed in a
  .cpp file, all the source code will be added to so your application can link with it.

  #define ROXLU_USE_ALL              - to include all code
  #define ROXLU_USE_OPENGL           - to use the opengl code
  #define ROXLU_USE_PNG              - to use the png loader and saver (needs libpng)
  #define ROXLU_USE_JPG              - add support for jpg loading
  #define ROXLU_USE_MATH             - to use the vec2, vec3, vec4, mat4 classes
  #define ROXLU_USE_CURL             - enable some curl helpers


  OPENGL - define `ROXLU_USE_OPENGL` before including
  ===================================================================================
  rx_create_shader(GL_VERTEX_SHADER, source_char_p);                        - create a shader, pass type
  rx_create_shader_from_file(GL_VERTEX_SHADER, path);                       - create a shader from a file, give a full path, returns -1 on error
  rx_create_program(vert, frag);                                            - create a program - DOES NOT LINK
  rx_create_program_with_attribs(vert, frag, 2, attr);                      - create a program with attribs + LINKS THE PROGRAM
  rx_print_shader_link_info(prog)                                           - print the program link info
  rx_print_shader_compile_info(vert)                                        - print the shader compile info
  rx_create_texture(filepath)                                               - loads a png and creates a texture (only when png is enabled)  
  rx_get_uniform_location(prog, name)                                       - returns the location of the uniform or returns -1 and logs something in debug mode
  rx_uniform_1i(prog, name, value)                                          - set a 1i value
  rx_uniform_1f(prog, name, value)                                          - set a 1f value
  rx_uniform_mat4fv(prog, name, count, trans, ptr)                          - set a mat4fv

  Shader                                                                    - represents a GL shader; only works with files! 
  Shader.load(type, filepath)                                               - load the source for the shader from file 
  Shader.compile()                                                          - compiles the shader

  Program                                                                   - represents a GL program 
  Program.add(Shader)                                                       - add a already compiled shader to the program
  Program.create(type, filepath, extra)                                     - create a Shader for the given filepath. extra is extra source code that gets prepended. 
  Program.link(nattribs, char** attribs, int nfrags, const char** frags)    - link + bind the attrib locations and/or fragment locations
  Program.recompile()                                                       - recompiles the shader and links the program again

  VertexP                                                                   - vertex type for position data
  VertexPC                                                                  - vertex type for position and color (4) data
  VertexPT                                                                  - vertex type for position and texture coord data
  VertexPT3                                                                 - vertex type for position and texture coord with 3 elements (a q for projective mapping)
  VertexPTN                                                                 - vertex type for position, texture and normal
  VertexPN                                                                  - vertex type for position and normals

  OBJ                                                                       - class to load OBJ files
  OBJ.load(filepath)                                                        - load the .obj file, returns boolean
  OBJ.hasNormals()                                                          - returns true if the loaded obj has normals
  OBJ.hasTexCoords()                                                        - returns true if the loaded obj had texcoords
  OBJ.copy(std::vector<VertexPT>&)                                          - copy the loaded vertices

  Painter                                                                   - simple helper to draw lines, circles, rectangles, textures with GL 3.x
  Painter.clear()                                                           - clear all added elements, resets the canvas
  Painter.draw()                                                            - draw the added shapes
  Painter.rect(x, y, w, h)                                                  - draw a rectangle
  Painter.circle(x, y, radius)                                              - draw a circle at x/y with radius 
  Painter.resolution(6)                                                     - sets the circle resolution
  Painter.line(x0, y0, x1, y1)                                              - draw one line
  Painter.texture(texid, x, y, w, h)                                        - draw a texture
  Painter.color(r,g,b,a)                                                    - set the draw color
  Painter.fill()                                                            - draw filled shapes
  Painter.nofill()                                                          - draw only outlines
  -
  Painter.resize(w,h)                                                       - call this when the viewport size changes
  Painter.width()                                                           - return the last set or queried viewport width
  Painter.height()                                                          - return the last set of queried viewport height
  -
  Painter.begin(GL_LINE_STRIP)                                              - begin drawing a type
  Painter.vertex(x, y)                                                      - add a vertex to the "begin/end" section
  Painter.end()                                                             - flush added vertices and make sure their drawn
  -
  

  IMAGES - define `ROXLU_USE_PNG` before including
  ===================================================================================
  rx_save_png("filename.png", pixels, 640, 480, 3);                         - writes a png using lib png
  rx_load_png("filepath.png", &pix, width, height, nchannels)               - load the pixels, width, height and nchannels for the given filepath. make sure to delete pix (which is unsigned char*)
  rx_load_jpg("filepath.jpg", &pix, width, height, nchannels)               - loads an jpg file

  UTILS
  ===================================================================================
  rx_to_float("0.15");                                                     - convert a string to float 
  rx_to_int("10");                                                         - convert a string to integer
  rx_int_to_string(15);                                                    - convert an integer to string
  rx_float_to_string(5.5);                                                 - convert a float to string
                                                                           
  rx_get_exe_path();                                                       - returns the path to the exe 
  rx_read_file("filepath.txt");                                            - returns the contents of the filepath.
  rx_to_data_path("filename.txt")                                          - convert the given filename to the data dir
  rx_is_dir("path")                                                        - returns true when the path is a dir
  rx_strip_filename("/path/filename")                                      - removes the filename from the given path
  rx_strip_dir("/path/filename")                                           - removes the path from the given path leaving only the filename
  rx_create_dir("/path/to")                                                - creates the given directory
  rx_create_path("/a/b/c")                                                 - creates the path. all subdirectories too)
  rx_norm_path("/some/path/")                                              - creates a normalized, cross platform path. always pass in forward slashes; on windows you'll get backslashes
  rx_get_files("/path/", ".jpg")                                           - returns a std::vector<std::string> with the files found in the given dir which have the given extension.
  rx_get_file_ext("/file/path.jpg")                                        - returns the extension of a file ("jpg", "gif", etc..)
                                                                           
  rx_split("string", '/')                                                  - splits a string on the given character returning std::vector<std::string>
  rx_string_replace("string", "replace", "with")                           - replaces characters or string from one to another
                                                                           
  rx_hrtime()                                                              - high resolution timer (time in nano sec)
  rx_millis()                                                              - returns the elapsed millis since the first call as float, 1000 millis returns 1.0
  rx_strftime("%Y/%m%d/")                                                  - strftime wrapper
  rx_get_year()                                                            - get the current year as int, e.g. 2014
  rx_get_month()                                                           - get the current month as int [00-11]
  rx_get_day()                                                             - get the day of the month [00-31]
  rx_get_hour()                                                            - get the hour of day [00-23]
  rx_get_minute()                                                          - get the minuts of the hours, [00-59]
                                                                           
  rx_rgb_to_hsv(r,g,b,h,s,v)                                               - convert rgb in range 0-1 to hsv in the same range. h,s,v are references
  rx_rgb_to_hsv(rgb, hsv)                                                  - convert given vector, hsv will be set (reference)
  rx_rgb_to_hsv(rgb, float*)                                               - "", different typed parameters
  rx_rgb_to_hsv(float*, float*)                                            - "", ""
                                                                           
  rx_hsv_to_rgb(h,s,v,r,g,b)                                               - convert rgb to hsv all in 0-1 range. r,g,b will be set, are references
  rx_hsv_to_rgb(hsv, rgb)                                                  - convert rgb to hsv all in 0-1 rnage. rgb will be set, is a reference
  rx_hsv_to_rgb(hsv, float*)                                               - "", "" 
  rx_hsv_to_rgb(float,* float*)                                            - "", ""
 

  MATH - define `ROXLU_USE_MATH` before including. 
  ===================================================================================

  utils
  ------------------------------------------------------------------------------
  float rx_random(max)                                                     - generate a random value but limit to max
  float rx_random(min, max)                                                - generate a random value between min and max
  
  vec2, vec3, vec4
  ------------------------------------------------------------------------------
  float length(v)                                                          - get the length of the vector
  float dot(a, b)                                                          - get the dot product aka squared root
  vec2 max(a)                                                              - get the biggest component value
  vec2 min(a)                                                              - get the lowest component value
  vec2 max(a,b)                                                            - get the biggest vector
  vec2 nmin(a,b)                                                           - get the smallest vector
  vec2 floor(a)                                                            - floor the components
  vec2 ceil(a)                                                             - ceil the compoments
  vec2 fract(a)                                                            - get the decimal part
  vec2 normalized(v)                                                       - get the normalized vector
  void print()                                                             - print the x and y 
  vec3 cross(a,b)                                                          - cross product (vec3)

  vec3
  -----------------------------------------------------------------------------
  vec3 perpendicular(a)                                                    - get a perpendicular vector from the given vec, this vector doesn't have to be normalized!, based on http://lolengine.net/blog/2013/09/21/picking-orthogonal-vector-combing-coconuts 
  bool intersect(a,b,c,d, &result)                                         - checks if two lines intersect line (b-a) and (d-c). resturns true when they intersect and it will set result to the intersection point
  
  mat4
  ------------------------------------------------------------------------------
  mat4& mat4.rotateX(rad)
  mat4& mat4.rotateY(rad)
  mat4& mat4.rotateZ(rad)
  mat4& mat4.rotate(rad, x, y, z)
  mat4& mat4.scale(x, y, z)
  mat4& mat4.scale(s)
  mat4& mat4.translate(x, y, z)
  mat4& mat4.translate(vec3 v)
  mat4& mat4.ortho(l, r, b, t, n , f)
  mat4& mat4.frustum(l, r, b, t, n, f)
  mat4& mat4.perspective(fov, aspect, near, far)
  mat4& mat4.lookat(eye, pos, up)
  void  mat4.print()
  float* mat4.ptr()                                                        - get a pointer to the data
  
  
  <example>
  // create an ortho matrix with 0,0 at the top left
  mat4 m;
  m.ortho(0.0f, w, h, 0, 0.0f, 100.0f);
  </example>


  Spline<T>  - catmull rom interpolation (MAKE SURE TO USE AT LEAST 4 POINTS!)
  ------------------------------------------------------------------------------

  Spline<T>.size()                                                       - returns the number of elements added
  Spline<T>.clear()                                                      - removes all added elements
  Spline<T>.push_back(T)                                                 - add an element
  Spline<T>.assign(begin, end)                                           - assign multiple values
  Spline<T>.at(float t)                                                  - get the interpolated value at this point

  <example>
  Spline<float> spline;
  spline.push_back(1.0);
  spline.push_back(3.0);
  spline.push_back(6.0);
  spline.push_back(5.0);
       
  int num = 10;
  for(int i = 0; i <= num; ++i) {
    float p = float(i)/num;
    printf("%d: %f (perc: %f)\n",i, spline.at(p), p);
  }
  </example>

  Perlin
  ------------------------------------------------------------------------------
  Perlin noise, thanks to Ken P.
  The class is based on: http://www.flipcode.com/archives/Perlin_Noise_Class.shtml
  with some super tiny changes. Thanks guys!
 
  octaves: use a value between 1 - 16, 1 = smooth, 16 = noisy, values between 4 - 8 give conventional noise results
  freq:    use a value between 1 - 8, which will give reasanoble results (you can use any value you want)
  ampl:    a value of 1, will result in values between -1 and 1
  seed:    random seed, eg. 94

  Perlin(octaves, freq, amplitude, seed)                             - constructor, see values in the description above
  Perlin.get(x)                                                      - get the value for this `x` range 
  Perlin.get(x, y)                                                   - 2d perlin


  CURL - define `ROXLU_USE_CURL`
  ===================================================================================
  
  rx_fetch_url(std::string url, std::string& result)                 - downloads an url into the given result.
  rx_download_file(std::string url, std::string filepath)            - downloads a file to the given filepath.

*/

#if defined(ROXLU_USE_ALL)
#  define ROXLU_USE_OPENGL
#  define ROXLU_USE_PNG
#  define ROXLU_USE_MATH
#  define ROXLU_USE_FONT
#  define ROXLU_USE_CURL
#endif

// ------------------------------------------------------------------------------------
//                              T  I  N  Y  L  I  B 
// ------------------------------------------------------------------------------------

#include <assert.h>
#include <iostream>
#include <cmath>
#include <iterator>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map> 

#if defined(_WIN32)
#  include <direct.h>                              /* _mkdir */
#  include <Shlwapi.h>
#  include <stdint.h>
#  include <time.h>
#  if defined(ROXLU_USE_OPENGL)
#    include <GLXW/glxw.h>
#  endif
#elif defined(__APPLE__)
#  if defined(ROXLU_USE_OPENGL)
#    include <OpenGL/gl3.h>
#  endif
#  include <libgen.h>                               /* dirname */
#  include <CoreFoundation/CFRunLoop.h>
#  include <mach/mach.h>
#  include <mach/mach_time.h>
#  include <mach-o/dyld.h>                          /* _NSGetExecutablePath */
#  include <sys/resource.h>
#  include <sys/sysctl.h>
#  include <sys/stat.h>                             /* stat() */
#  include <unistd.h>                               /* sysconf */
#  include <dirent.h>                               /* DIR */
#elif defined(__linux)
#  include <string.h>                               /* strlen() */
#  include <dirent.h>                               /* stat() */
#  include <errno.h>                                /* errno */
#  include <unistd.h>                               /* readlink(), getcwd() */
#  include <sys/time.h>                             /* timeofday() */
#  include <libgen.h>                               /* dirname() */
#  include <stdint.h>                               /* uint*_t types */
#  include <sys/stat.h>
#  include <stdarg.h>
#  if defined(ROXLU_USE_OPENGL)
#     include <GLXW/glxw.h>
#  endif
#  define MAX_PATH 4096
#endif

#if defined(ROXLU_USE_PNG)
#  include <png.h>
#endif

#ifndef ROXLU_TINYLIB_H
#define ROXLU_TINYLIB_H
#ifndef PI
#  define PI 3.14159265358979323846
#endif

#ifndef TWO_PI
#  define TWO_PI 6.28318530717958647693
#endif

#ifndef M_TWO_PI
#  define M_TWO_PI 6.28318530717958647693
#endif

#ifndef FOUR_PI
#define FOUR_PI 12.56637061435917295385
#endif

#ifndef HALF_PI
#  define HALF_PI 1.57079632679489661923
#endif

#ifndef DEG_TO_RAD
#  define DEG_TO_RAD (PI/180.0)
#endif

#ifndef RAD_TO_DEG
#  define RAD_TO_DEG (180.0/PI)
#endif

#ifndef LOWEST
#  define LOWEST(x,y) (((x) < (y)) ? (x) : (y))
#endif

#ifndef HEIGHEST
#  define HEIGHEST(x,y) (((x) > (y)) ? (x) : (y))
#endif

#ifndef CLAMP
#  define CLAMP(val,min,max) (HEIGHEST(LOWEST(val,max),min))
#endif

#ifndef ABS
#  define ABS(x) (((x) < 0) ? -(x) : (x))
#endif

#ifndef DX
#  define DX(i,j,w)((j)*(w))+(i)
#endif

/* file utils */
extern std::string rx_get_exe_path();
extern std::string rx_to_data_path(const std::string filename);
extern bool rx_is_dir(std::string filepath);
extern bool rx_file_exists(std::string filepath);
extern std::string rx_strip_filename(std::string path);
extern std::string rx_strip_dir(std::string path);
extern bool rx_create_dir(std::string path);
extern bool rx_create_path(std::string path);
extern std::string rx_get_file_ext(std::string filename);
extern std::vector<std::string> rx_get_files(std::string path, std::string ext = "");
extern std::string rx_norm_path(std::string path);

/* string and conversion utils */
extern std::string rx_string_replace(std::string, char from, char to);
extern std::string rx_string_replace(std::string, std::string from, std::string to);
extern int rx_to_int(const std::string& v);
extern float rx_to_float(const std::string& v);
extern std::string rx_int_to_string(const int& v);
extern std::string rx_float_to_string(const float& v);
extern std::vector<std::string> rx_split(std::string str, char delim);

/* time utils */
extern uint64_t rx_hrtime();
extern float rx_millis();
extern std::string rx_strftime(const std::string fmt);
extern std::string rx_read_file(std::string filepath);
extern int rx_get_year();
extern int rx_get_month();
extern int rx_get_day();
extern int rx_get_hour();
extern int rx_get_minute();

#endif // ROXLU_TINYLIB_H

// ------------------------------------------------------------------------------------
//                              R O X L U _ U S E _ M A T H 
// ------------------------------------------------------------------------------------

#if defined(ROXLU_USE_MATH)
#  ifndef ROXLU_USE_MATH_H
#  define ROXLU_USE_MATH_H

template<class T>
class Vec2 {
    
 public:
  Vec2();
  Vec2(T x, T y);
  Vec2(const Vec2<T>& o);
  Vec2(T f);
    
  void set(T vx, T vy);
  T* ptr();
  T& operator [](const unsigned int dx);
    
  Vec2<T> operator + () const;
  Vec2<T> operator - () const;
  Vec2<T> operator + (const Vec2<T>& o) const;
  Vec2<T> operator - (const Vec2<T>& o) const;
  Vec2<T> operator * (const Vec2<T>& o) const;
  Vec2<T> operator / (const Vec2<T>& o) const;
  Vec2<T> operator + (float s) const;
  Vec2<T> operator - (float s) const;
  Vec2<T> operator * (float s) const;
  Vec2<T> operator / (float s) const;
    
  Vec2<T>& operator += (const Vec2<T>& o);
  Vec2<T>& operator -= (const Vec2<T>& o);
  Vec2<T>& operator *= (const Vec2<T>& o);
  Vec2<T>& operator /= (const Vec2<T>& o);
  Vec2<T>& operator += (float s);
  Vec2<T>& operator -= (float s);
  Vec2<T>& operator *= (float s);
  Vec2<T>& operator /= (float s);
    
  bool operator == (const Vec2<T>& o) const;
  bool operator != (const Vec2<T>& o) const;
  void print();
    
 public:
  T x;
  T y;
}; // Vec2<T>

template<class T> inline Vec2<T>::Vec2() : x(), y() {}
template<class T> inline Vec2<T>::Vec2(T x, T y) : x(x), y(y) {}
template<class T> inline Vec2<T>::Vec2(const Vec2<T>& o) : x(o.x), y(o.y) {}
template<class T> inline Vec2<T>::Vec2(T f) : x(f), y(f) {}
template<class T> inline void Vec2<T>::set(T vx, T vy) { x = vx; y = vy; }
template<class T> inline T* Vec2<T>::ptr() { return &x; }
template<class T> inline T& Vec2<T>::operator [](const unsigned int dx) { return *(&x + dx); }
template<class T> inline Vec2<T> Vec2<T>::operator + () const { return Vec2<T>(+x, +y); };
template<class T> inline Vec2<T> Vec2<T>::operator - () const { return Vec2<T>(-x, -y); };
template<class T> inline Vec2<T> Vec2<T>::operator + (const Vec2<T>& o) const { return Vec2<T>(x + o.x, y + o.y); }
template<class T> inline Vec2<T> Vec2<T>::operator - (const Vec2<T>& o) const { return Vec2<T>(x - o.x, y - o.y); }
template<class T> inline Vec2<T> Vec2<T>::operator * (const Vec2<T>& o) const { return Vec2<T>(x * o.x, y * o.y); }
template<class T> inline Vec2<T> Vec2<T>::operator / (const Vec2<T>& o) const { return Vec2<T>(x / o.x, y / o.y); }
template<class T> inline Vec2<T> Vec2<T>::operator + (float s) const { return Vec2<T>(x + s, y + s); }
template<class T> inline Vec2<T> Vec2<T>::operator - (float s) const { return Vec2<T>(x - s, y - s); }
template<class T> inline Vec2<T> Vec2<T>::operator * (float s) const { return Vec2<T>(x * s, y * s); }
template<class T> inline Vec2<T> Vec2<T>::operator / (float s) const { return Vec2<T>(x / s, y / s); }
template<class T> inline Vec2<T> operator + (float s, const Vec2<T>& o) { return Vec2<T>(s + o.x, s + o.y); }
template<class T> inline Vec2<T> operator - (float s, const Vec2<T>& o) { return Vec2<T>(s - o.x, s - o.y); }
template<class T> inline Vec2<T> operator * (float s, const Vec2<T>& o) { return Vec2<T>(s * o.x, s * o.y); }
template<class T> inline Vec2<T> operator / (float s, const Vec2<T>& o) { return Vec2<T>(s / o.x, s / o.y); }
template<class T> inline Vec2<T>& Vec2<T>::operator += (const Vec2<T>& o) { return *this = *this + o; }
template<class T> inline Vec2<T>& Vec2<T>::operator -= (const Vec2<T>& o) { return *this = *this - o; }
template<class T> inline Vec2<T>& Vec2<T>::operator *= (const Vec2<T>& o) { return *this = *this * o; }
template<class T> inline Vec2<T>& Vec2<T>::operator /= (const Vec2<T>& o) { return *this = *this / o; }
template<class T> inline Vec2<T>& Vec2<T>::operator += (float s) { return *this = *this + s; }
template<class T> inline Vec2<T>& Vec2<T>::operator -= (float s) { return *this = *this - s; }
template<class T> inline Vec2<T>& Vec2<T>::operator *= (float s) { return *this = *this * s; }
template<class T> inline Vec2<T>& Vec2<T>::operator /= (float s) { return *this = *this / s; }
template<class T> inline bool Vec2<T>::operator == (const Vec2<T>& o) const { return x == o.x && y == o.y; }
template<class T> inline bool Vec2<T>::operator != (const Vec2<T>& o) const { return x != o.x || y == o.y; }
template<class T> inline float length(const Vec2<T>& o) { return sqrtf(o.x * o.x + o.y * o.y); }
template<class T> inline float dot(const Vec2<T> &a, const Vec2<T> &b) { return a.x * b.x + a.y * b.y; }
template<class T> inline float heighest(const Vec2<T> &v) { return fmaxf(v.x, v.y); }
template<class T> inline float lowest(const Vec2<T> &v) { return fminf(v.x, v.y); }
template<class T> inline Vec2<T> lowest(const Vec2<T> &a, const Vec2<T> &b) { return Vec2<T>(fmaxf(a.x, b.x), fmaxf(a.y, b.y)); }
template<class T> inline Vec2<T> heighest(const Vec2<T> &a, const Vec2<T> &b) { return Vec2<T>(fminf(a.x, b.x), fminf(a.y, b.y)); }
template<class T> inline Vec2<T> floor(const Vec2<T> &v) { return Vec2<T>(floorf(v.x), floorf(v.y)); }
template<class T> inline Vec2<T> ceil(const Vec2<T> &v) { return Vec2<T>(ceilf(v.x), ceilf(v.y)); }
template<class T> inline Vec2<T> abs(const Vec2<T> &v) { return Vec2<T>(fabsf(v.x), fabsf(v.y)); }
template<class T> inline Vec2<T> fract(const Vec2<T> &v) { return v - floor(v); }
template<class T> inline Vec2<T> normalized(const Vec2<T> &v) { T l = length(v); if(!l) { return T(0); } else return v / l; }
template<class T> inline void Vec2<T>::print() { printf("x: %f, y: %f\n", x, y); }

template<class T>
class Vec3 {
    
 public:
  Vec3();;
  Vec3(T x, T y, T z);
  Vec3(const Vec3<T>& o);
  Vec3(T f);
    
  void set(const float xv, const float yv, const float zv);
  T* ptr();
  T& operator [](const unsigned int dx);
    
  Vec3<T> operator + () const;
  Vec3<T> operator - () const;
  Vec3<T> operator + (const Vec3<T>& o) const;
  Vec3<T> operator - (const Vec3<T>& o) const;
  Vec3<T> operator * (const Vec3<T>& o) const;
  Vec3<T> operator / (const Vec3<T>& o) const;
  Vec3<T> operator + (float s) const;
  Vec3<T> operator - (float s) const;
  Vec3<T> operator * (float s) const;
  Vec3<T> operator / (float s) const;
    
  Vec3<T>& operator += (const Vec3<T>& o);
  Vec3<T>& operator -= (const Vec3<T>& o);
  Vec3<T>& operator *= (const Vec3<T>& o);
  Vec3<T>& operator /= (const Vec3<T>& o);
  Vec3<T>& operator += (float s);
  Vec3<T>& operator -= (float s);
  Vec3<T>& operator *= (float s);
  Vec3<T>& operator /= (float s);
    
  bool operator == (const Vec3<T>& o) const;
  bool operator != (const Vec3<T>& o) const;
    
  void print();
    
 public:
  T x, y, z;
}; // Vec3<T>

template<class T> inline Vec3<T>::Vec3() : x(), y(), z() {}
template<class T> inline Vec3<T>::Vec3(T x, T y, T z) : x(x), y(y), z(z) {}
template<class T> inline Vec3<T>::Vec3(const Vec3<T>& o) : x(o.x), y(o.y), z(o.z) {}
template<class T> inline Vec3<T>::Vec3(T f) : x(f), y(f), z(f) {}
template<class T> inline void Vec3<T>::set(const float xv, const float yv, const float zv) { x = xv; y = yv; z = zv; }
template<class T> inline T* Vec3<T>::ptr() { return &x; }
template<class T> inline T& Vec3<T>::operator [](const unsigned int dx) { return *(&x + dx); }
template<class T> inline Vec3<T> Vec3<T>::operator + () const { return Vec3<T>(+x, +y, +z); };
template<class T> inline Vec3<T> Vec3<T>::operator - () const { return Vec3<T>(-x, -y, -z); };
template<class T> inline Vec3<T> Vec3<T>::operator + (const Vec3<T>& o) const { return Vec3<T>(x + o.x, y + o.y, z + o.z); }
template<class T> inline Vec3<T> Vec3<T>::operator - (const Vec3<T>& o) const { return Vec3<T>(x - o.x, y - o.y, z - o.z); }
template<class T> inline Vec3<T> Vec3<T>::operator * (const Vec3<T>& o) const { return Vec3<T>(x * o.x, y * o.y, z * o.z); }
template<class T> inline Vec3<T> Vec3<T>::operator / (const Vec3<T>& o) const { return Vec3<T>(x / o.x, y / o.y, z / o.z); }
template<class T> inline Vec3<T> Vec3<T>::operator + (float s) const { return Vec3<T>(x + s, y + s, z + s); }
template<class T> inline Vec3<T> Vec3<T>::operator - (float s) const { return Vec3<T>(x - s, y - s, z - s); }
template<class T> inline Vec3<T> Vec3<T>::operator * (float s) const { return Vec3<T>(x * s, y * s, z * s); }
template<class T> inline Vec3<T> Vec3<T>::operator / (float s) const { return Vec3<T>(x / s, y / s, z / s); }
template<class T> inline Vec3<T> operator + (float s, const Vec3<T>& o) { return Vec3<T>(s + o.x, s + o.y, s + o.z); }
template<class T> inline Vec3<T> operator - (float s, const Vec3<T>& o) { return Vec3<T>(s - o.x, s - o.y, s - o.z); }
template<class T> inline Vec3<T> operator * (float s, const Vec3<T>& o) { return Vec3<T>(s * o.x, s * o.y, s * o.z); }
template<class T> inline Vec3<T> operator / (float s, const Vec3<T>& o) { return Vec3<T>(s / o.x, s / o.y, s / o.z); }
template<class T> inline Vec3<T>& Vec3<T>::operator += (const Vec3<T>& o) { return *this = *this + o; }
template<class T> inline Vec3<T>& Vec3<T>::operator -= (const Vec3<T>& o) { return *this = *this - o; }
template<class T> inline Vec3<T>& Vec3<T>::operator *= (const Vec3<T>& o) { return *this = *this * o; }
template<class T> inline Vec3<T>& Vec3<T>::operator /= (const Vec3<T>& o) { return *this = *this / o; }
template<class T> inline Vec3<T>& Vec3<T>::operator += (float s) { return *this = *this + s; }
template<class T> inline Vec3<T>& Vec3<T>::operator -= (float s) { return *this = *this - s; }
template<class T> inline Vec3<T>& Vec3<T>::operator *= (float s) { return *this = *this * s; }
template<class T> inline Vec3<T>& Vec3<T>::operator /= (float s) { return *this = *this / s; }
template<class T> inline bool Vec3<T>::operator == (const Vec3<T>& o) const { return x == o.x && y == o.y && z == o.z; }
template<class T> inline bool Vec3<T>::operator != (const Vec3<T>& o) const { return x != o.x || y == o.y || z == o.z; }
template<class T> inline float length(const Vec3<T>& o) { return sqrtf(o.x * o.x + o.y * o.y + o.z * o.z); }
template<class T> inline float dot(const Vec3<T> &a, const Vec3<T> &b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
template<class T> inline float heighest(const Vec3<T> &v) { return fmaxf(fmaxf(v.x, v.y), v.z); }
template<class T> inline float lowest(const Vec3<T> &v) { return fminf(fminf(v.x, v.y), v.z); }
template<class T> inline Vec3<T> heighest(const Vec3<T> &a, const Vec3<T> &b) { return Vec3<T>(fmaxf(a.x, b.x), fmaxf(a.y, b.y), fmaxf(a.z, b.z)); }
template<class T> inline Vec3<T> lowest(const Vec3<T> &a, const Vec3<T> &b) { return Vec3<T>(fminf(a.x, b.x), fminf(a.y, b.y), fminf(a.z, b.z)); }
template<class T> inline Vec3<T> floor(const Vec3<T> &v) { return Vec3<T>(floorf(v.x), floorf(v.y), floorf(v.z)); }
template<class T> inline Vec3<T> ceil(const Vec3<T> &v) { return Vec3<T>(ceilf(v.x), ceilf(v.y), ceilf(v.z)); }
template<class T> inline Vec3<T> abs(const Vec3<T> &v) { return Vec3<T>(fabsf(v.x), fabsf(v.y), fabsf(v.z)); }
template<class T> inline Vec3<T> fract(const Vec3<T> &v) { return v - floor(v); }
template<class T> inline Vec3<T> normalized(const Vec3<T> &v) { T l = length(v); if(!l) { return T(0); } else return v / l; }
template<class T> inline Vec3<T> cross(const Vec3<T> &a, const Vec3<T> &b) { return Vec3<T>(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); }
template<class T> inline Vec3<T> perpendicular(const Vec3<T>& v) {  return abs(v.x) > abs(v.z) ? Vec3<T>(-v.y, v.x, 0.0) : Vec3<T>(0.0, -v.z, v.y); }
template<class T> inline void Vec3<T>::print() { printf("x: %f, y: %f, z: %f\n", x, y, z); }

// lines must be in the xy-plane; only done in 2d
template<class T>
inline bool intersect(const Vec3<T>& p0, const Vec3<T>& p1, const Vec3<T>& p2, const Vec3<T>& p3, Vec3<T>& result) {
  Vec3<T> s1 = p1 - p0;
  Vec3<T> s2 = p3 - p2;
    
  float s, t;
  s = (-s1.y * (p0.x - p2.x) + s1.x * (p0.y - p2.y)) / (-s2.x * s1.y + s1.x * s2.y);
  t = ( s2.x * (p0.y - p2.y) - s2.y * (p0.x - p2.x)) / (-s2.x * s1.y + s1.x * s2.y);
    
  if(s >= 0.0f && s <= 1.0f && t >= 0.0f && t <= 1.0f) {
    result.x = p0.x + (t * s1.x);
    result.y = p0.y + (t * s1.y);
    return true;
  }
    
  return false;
}

template<class T>
class Vec4 {
    
 public:
  Vec4();
  Vec4(T x, T y, T z, T w);
  Vec4(const Vec4<T>& o);
  Vec4(T f);
    
  T* ptr();
  T& operator [](const unsigned int dx);
    
  Vec4<T> operator + () const;
  Vec4<T> operator - () const;
  Vec4<T> operator + (const Vec4<T>& o) const;
  Vec4<T> operator - (const Vec4<T>& o) const;
  Vec4<T> operator * (const Vec4<T>& o) const;
  Vec4<T> operator / (const Vec4<T>& o) const;
  Vec4<T> operator + (float s) const;
  Vec4<T> operator - (float s) const;
  Vec4<T> operator * (float s) const;
  Vec4<T> operator / (float s) const;
    
  Vec4<T>& operator += (const Vec4<T>& o);
  Vec4<T>& operator -= (const Vec4<T>& o);
  Vec4<T>& operator *= (const Vec4<T>& o);
  Vec4<T>& operator /= (const Vec4<T>& o);
  Vec4<T>& operator += (float s);
  Vec4<T>& operator -= (float s);
  Vec4<T>& operator *= (float s);
  Vec4<T>& operator /= (float s);
    
  bool operator == (const Vec4<T>& o) const;
  bool operator != (const Vec4<T>& o) const;
    
  void print();
    
 public:
  T x, y, z, w;
}; // Vec4<T>

template<class T> inline Vec4<T>::Vec4() : x(), y(), z(), w() {}
template<class T> inline Vec4<T>::Vec4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
template<class T> inline Vec4<T>::Vec4(const Vec4<T>& o) : x(o.x), y(o.y), z(o.z), w(o.w) {}
template<class T> inline Vec4<T>::Vec4(T f) : x(f), y(f), z(f), w(f) {}
template<class T> inline T* Vec4<T>::ptr() { return &x; }
template<class T> inline T& Vec4<T>::operator [](const unsigned int dx) { return *(&x + dx); }
template<class T> inline Vec4<T> Vec4<T>::operator + () const { return Vec4<T>(+x, +y, +z, +w); };
template<class T> inline Vec4<T> Vec4<T>::operator - () const { return Vec4<T>(-x, -y, -z, -w); };
template<class T> inline Vec4<T> Vec4<T>::operator + (const Vec4<T>& o) const { return Vec4<T>(x + o.x, y + o.y, z + o.z, w + o.w); }
template<class T> inline Vec4<T> Vec4<T>::operator - (const Vec4<T>& o) const { return Vec4<T>(x - o.x, y - o.y, z - o.z, w - o.w); }
template<class T> inline Vec4<T> Vec4<T>::operator * (const Vec4<T>& o) const { return Vec4<T>(x * o.x, y * o.y, z * o.z, w * o.w); }
template<class T> inline Vec4<T> Vec4<T>::operator / (const Vec4<T>& o) const { return Vec4<T>(x / o.x, y / o.y, z / o.z, w / o.w); }
template<class T> inline Vec4<T> Vec4<T>::operator + (float s) const { return Vec4<T>(x + s, y + s, z + s, w + s); }
template<class T> inline Vec4<T> Vec4<T>::operator - (float s) const { return Vec4<T>(x - s, y - s, z - s, w - s); }
template<class T> inline Vec4<T> Vec4<T>::operator * (float s) const { return Vec4<T>(x * s, y * s, z * s, w * s); }
template<class T> inline Vec4<T> Vec4<T>::operator / (float s) const { return Vec4<T>(x / s, y / s, z / s, w / s); }
template<class T> inline Vec4<T> operator + (float s, const Vec4<T>& o) { return Vec4<T>(s + o.x, s + o.y, s + o.z, s + o.w); }
template<class T> inline Vec4<T> operator - (float s, const Vec4<T>& o) { return Vec4<T>(s - o.x, s - o.y, s - o.z, s - o.w); }
template<class T> inline Vec4<T> operator * (float s, const Vec4<T>& o) { return Vec4<T>(s * o.x, s * o.y, s * o.z, s * o.w); }
template<class T> inline Vec4<T> operator / (float s, const Vec4<T>& o) { return Vec4<T>(s / o.x, s / o.y, s / o.z, s / o.w); }
template<class T> inline Vec4<T>& Vec4<T>::operator += (const Vec4<T>& o) { return *this = *this + o; }
template<class T> inline Vec4<T>& Vec4<T>::operator -= (const Vec4<T>& o) { return *this = *this - o; }
template<class T> inline Vec4<T>& Vec4<T>::operator *= (const Vec4<T>& o) { return *this = *this * o; }
template<class T> inline Vec4<T>& Vec4<T>::operator /= (const Vec4<T>& o) { return *this = *this / o; }
template<class T> inline Vec4<T>& Vec4<T>::operator += (float s) { return *this = *this + s; }
template<class T> inline Vec4<T>& Vec4<T>::operator -= (float s) { return *this = *this - s; }
template<class T> inline Vec4<T>& Vec4<T>::operator *= (float s) { return *this = *this * s; }
template<class T> inline Vec4<T>& Vec4<T>::operator /= (float s) { return *this = *this / s; }
template<class T> inline bool Vec4<T>::operator == (const Vec4<T>& o) const { return x == o.x && y == o.y && z == o.z && w == o.w; }
template<class T> inline bool Vec4<T>::operator != (const Vec4<T>& o) const { return x != o.x || y == o.y || z == o.z || w == o.w; }
template<class T> inline float length(const Vec4<T>& o) { return sqrtf(o.x * o.x + o.y * o.y + o.z * o.z + o.w * o.w); }
template<class T> inline float dot(const Vec4<T> &a, const Vec4<T> &b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * a.w; }
template<class T> inline float heighest(const Vec4<T> &v) { return fmaxf(fmaxf(v.x, v.y), fmaxf(v.z, v.w)); }
template<class T> inline float lowest(const Vec4<T> &v) { return fminf(fminf(v.x, v.y), fminf(v.z, v.w)); }
template<class T> inline Vec4<T> heighest(const Vec4<T> &a, const Vec4<T> &b) { return Vec4<T>(fmaxf(a.x, b.x), fmaxf(a.y, b.y), fmaxf(a.z, b.z), fmaxf(a.w, b.w)); }
template<class T> inline Vec4<T> lowest(const Vec4<T> &a, const Vec4<T> &b) { return Vec4<T>(fminf(a.x, b.x), fminf(a.y, b.y), fminf(a.z, b.z), fminf(a.w, b.w)); }
template<class T> inline Vec4<T> floor(const Vec4<T> &v) { return Vec4<T>(floorf(v.x), floorf(v.y), floorf(v.z), floorf(v.w)); }
template<class T> inline Vec4<T> ceil(const Vec4<T> &v) { return Vec4<T>(ceilf(v.x), ceilf(v.y), ceilf(v.z), ceilf(v.w)); }
template<class T> inline Vec4<T> abs(const Vec4<T> &v) { return Vec4<T>(fabsf(v.x), fabsf(v.y), fabsf(v.z), fabsf(v.w)); }
template<class T> inline Vec4<T> fract(const Vec4<T> &v) { return v - floor(v); }
template<class T> inline Vec4<T> normalized(const Vec4<T> &v) { return v / length(v); }
template<class T> inline void Vec4<T>::print() { printf("x: %f, y: %f, z: %f, w: %f\n", x, y, z, w); }

template<class T>
class Matrix4 {
 public:
  Matrix4();
    
  Matrix4<T>& rotateX(T rad);
  Matrix4<T>& rotateY(T rad);
  Matrix4<T>& rotateZ(T rad);
  Matrix4<T>& rotate(T rad, T x, T y, T z);
  Matrix4<T>& rotate(T rad, const Vec3<T>& v);
  Matrix4<T>& translate(T x, T y, T z);
  Matrix4<T>& translate(const Vec3<T>& v);
  Matrix4<T>& scale(T x, T y, T z);
  Matrix4<T>& scale(T s);
  Matrix4<T>& perspective(T fovDegrees, T aspect, T n, T f);
  Matrix4<T>& ortho(T l, T r, T b, T t, T n, T f);
  Matrix4<T>& frustum(T l, T r, T b, T t, T n, T f);
  Matrix4<T>& identity();
  Matrix4<T>& lookat(Vec3<T> pos, Vec3<T> target, Vec3<T> up);
    
  T* ptr() { return &m[0]; }
    
  Matrix4<T> rotation(T rad, T x, T y, T z);
    
  Matrix4<T>& operator *=(const Matrix4<T>& o);
  Matrix4<T> operator * (const Matrix4<T>& o) const;
  T& operator [] (const unsigned int dx) { return m[dx]; }
    
  void print();
    
 public:
  T m[16];
}; // Matrix4<T>

template<class T>
Matrix4<T>::Matrix4() {
  identity();
}

template<class T>
Matrix4<T>& Matrix4<T>::identity() {
  memset(m, 0x00, sizeof(T) * 16);
  m[0] = (T)1.0f;
  m[5] = (T)1.0f;
  m[10] =(T)1.0f;
  m[15] = (T)1.0f;
  return *this;
}

template<class T>
Matrix4<T>& Matrix4<T>::frustum(T l, T r, T b, T t, T n, T f) {
  m[1]  = T(0);
  m[2]  = T(0);
  m[3]  = T(0);
  m[4]  = T(0);
  m[6]  = T(0);
  m[7]  = T(0);
  m[12] = T(0);
  m[13] = T(0);
    
  m[0]  = T(2) * n / (r-l);
  m[5]  = T(2) * n / (t-b);
  m[8]  = (r+l) / (r-l);
  m[9]  = (t+b) / (t-b);
  m[10] = - (f+n) / (f-n);
  m[11] = - T(1);
  m[14] = - T(2) * f * n / (f-n);
  m[15] = T(0);
  return *this;
}

template<class T>
Matrix4<T>& Matrix4<T>::ortho(T l, T r, T b, T t, T n, T f) {
  m[1]  = T(0);
  m[2]  = T(0);
  m[3]  = T(0);
  m[4]  = T(0);
  m[6]  = T(0);
  m[7]  = T(0);
  m[8]  = T(0);
  m[9]  = T(0);
  m[11] = T(0);
  m[15] = T(1);
    
  float rml = r - l;
  float fmn = f - n;
  float tmb = t - b;
  m[0]  = T(2) / rml;
  m[5]  = T(2) / tmb;
  m[10] = -T(2) / fmn;
  m[12] = -(r+l)/rml;
  m[13] = -(t+b)/tmb;
  m[14] = -(f+n)/fmn;
  return *this;
}


template<class T>
Matrix4<T>& Matrix4<T>::perspective(T fovDegrees, T aspect, T n, T f) {
  T tan_hfov = tan( (fovDegrees * DEG_TO_RAD) * T(0.5) );
  m[1]  = T(0);
  m[2]  = T(0);
  m[3]  = T(0);
  m[4]  = T(0);
  m[6]  = T(0);
  m[7]  = T(0);
  m[12] = T(0);
  m[13] = T(0);
  m[15] = T(0);
  m[0] = T(1) / (aspect * tan_hfov);
  m[5] = T(1) / (tan_hfov);
  m[10] = - (f + n) / (f - n);
  m[11] = - T(1);
  m[14] = - (T(2) * f * n) / (f - n);
  return *this;
}

template<class T>
void Matrix4<T>::print() {
  printf("%f, %f, %f, %f\n", m[0], m[4], m[8], m[12]);
  printf("%f, %f, %f, %f\n", m[1], m[5], m[9], m[13]);
  printf("%f, %f, %f, %f\n", m[2], m[6], m[10], m[14]);
  printf("%f, %f, %f, %f\n", m[3], m[7], m[11], m[15]);
}

template<class T>
Matrix4<T>& Matrix4<T>::rotate(T rad, T x, T y, T z) {
  Matrix4<T> rot = rotation(rad, x, y, z);
  *this *= rot;
  return *this;
}

template<class T>
Matrix4<T>& Matrix4<T>::rotateX(T rad) {
  return rotate(rad, T(1), T(0), T(0));
}

template<class T>
Matrix4<T>& Matrix4<T>::rotateY(T rad) {
  return rotate(rad, T(0), T(1), T(0));
}

template<class T>
Matrix4<T>& Matrix4<T>::rotateZ(T rad) {
  return rotate(rad, T(0), T(0), T(1));
}

template<class T>
Matrix4<T>& Matrix4<T>::rotate(T rad, const Vec3<T>& v) {
  return rotate(rad, v.x, v.y, v.z);
}

template<class T>
Matrix4<T> Matrix4<T>::rotation(T rad, T x, T y, T z) {
    
  Matrix4<T> mat;
    
  float c = cos(rad);
  float s = sin(rad);
  float t = 1.0f - c;
    
  Vec3<T> ax(x,y,z);
  ax = normalized(ax);
    
  float tx = t * ax.x;
  float ty = t * ax.y;
  float tz = t * ax.z;
    
  float sx = s * ax.x;
  float sy = s * ax.y;
  float sz = s * ax.z;
    
  float txy = tx * ax.y;
  float tyz = tx * ax.z;
  float txz = tx * ax.z;
    
  mat.m[0]  = tx * ax.x + c;
  mat.m[4]  = txy - sz;
  mat.m[8]  = txz + sy;
  mat.m[12] = 0.0f;
    
  mat.m[1]  = txy + sz;
  mat.m[5]  = ty * ax.y + c;
  mat.m[9]  = tyz - sx;
  mat.m[13] = 0.0f;
    
  mat.m[2]  = txz - sy;
  mat.m[6]  = tyz + sx;
  mat.m[10] = tz * ax.z + c;
  mat.m[14] = 0.0f;
    
  mat.m[3]  = 0.0f;
  mat.m[7]  = 0.0f;
  mat.m[11] = 0.0f;
  mat.m[15] = 1.0f;
    
  return mat;
}

template<class T>
Matrix4<T>& Matrix4<T>::scale(T s) {
  return scale(s, s, s);
}

template<class T>
Matrix4<T>& Matrix4<T>::scale(T x, T y, T z) {
  m[0] *= x; m[4] *= y;  m[8]  *= z;
  m[1] *= x; m[5] *= y;  m[9]  *= z;
  m[2] *= x; m[6] *= y;  m[10] *= z;
  m[3] *= x; m[6] *= y;  m[11] *= z;
  return *this;
}

template<class T>
Matrix4<T>& Matrix4<T>::translate(const Vec3<T>& v) {
  return translate(v.x, v.y, v.z);
}

template<class T>
Matrix4<T>& Matrix4<T>::translate(T x, T y, T z) {
  m[12] += m[0] * x + m[4] * y + m[8] * z;
  m[13] += m[1] * x + m[5] * y + m[9] * z;
  m[14] += m[2] * x + m[6] * y + m[10] * z;
  m[15] += m[3] * x + m[7] * y + m[11] * z;
  return *this;
}

template<class T>
Matrix4<T>& Matrix4<T>::operator *= (const Matrix4<T>& o) {
  Matrix4<T> r;
    
  r.m[0]  =  m[0] * o.m[0]  +  m[4] * o.m[1]  +  m[8]  * o.m[2]  +  m[12] * o.m[3];
  r.m[1]  =  m[1] * o.m[0]  +  m[5] * o.m[1]  +  m[9]  * o.m[2]  +  m[13] * o.m[3];
  r.m[2]  =  m[2] * o.m[0]  +  m[6] * o.m[1]  +  m[10] * o.m[2]  +  m[14] * o.m[3];
  r.m[3]  =  m[3] * o.m[0]  +  m[7] * o.m[1]  +  m[11] * o.m[2]  +  m[15] * o.m[3];
    
  r.m[4]  =  m[0] * o.m[4]  +  m[4] * o.m[5]  +  m[8]  * o.m[6]  +  m[12] * o.m[7];
  r.m[5]  =  m[1] * o.m[4]  +  m[5] * o.m[5]  +  m[9]  * o.m[6]  +  m[13] * o.m[7];
  r.m[6]  =  m[2] * o.m[4]  +  m[6] * o.m[5]  +  m[10] * o.m[6]  +  m[14] * o.m[7];
  r.m[7]  =  m[3] * o.m[4]  +  m[7] * o.m[5]  +  m[11] * o.m[6]  +  m[15] * o.m[7];
    
  r.m[8]  =  m[0] * o.m[8]  +  m[4] * o.m[9]  +  m[8]  * o.m[10] +  m[12] * o.m[11];
  r.m[9]  =  m[1] * o.m[8]  +  m[5] * o.m[9]  +  m[9]  * o.m[10] +  m[13] * o.m[11];
  r.m[10] =  m[2] * o.m[8]  +  m[6] * o.m[9]  +  m[10] * o.m[10] +  m[14] * o.m[11];
  r.m[11] =  m[3] * o.m[8]  +  m[7] * o.m[9]  +  m[11] * o.m[10] +  m[15] * o.m[11];
    
  r.m[12] =  m[0] * o.m[12] +  m[4] * o.m[13] +  m[8]  * o.m[14] +  m[12] * o.m[15];
  r.m[13] =  m[1] * o.m[12] +  m[5] * o.m[13] +  m[9]  * o.m[14] +  m[13] * o.m[15];
  r.m[14] =  m[2] * o.m[12] +  m[6] * o.m[13] +  m[10] * o.m[14] +  m[14] * o.m[15];
  r.m[15] =  m[3] * o.m[12] +  m[7] * o.m[13] +  m[11] * o.m[14] +  m[15] * o.m[15];
    
  std::copy(&r.m[0], &r.m[15], &m[0]);
  return *this;
}

template<class T>
Matrix4<T> Matrix4<T>::operator * (const Matrix4<T>& o) const {
  Matrix4<T> r;
    
  r.m[0]  =  m[0] * o.m[0]  +  m[4] * o.m[1]  +  m[8]  * o.m[2]  +  m[12] * o.m[3];
  r.m[1]  =  m[1] * o.m[0]  +  m[5] * o.m[1]  +  m[9]  * o.m[2]  +  m[13] * o.m[3];
  r.m[2]  =  m[2] * o.m[0]  +  m[6] * o.m[1]  +  m[10] * o.m[2]  +  m[14] * o.m[3];
  r.m[3]  =  m[3] * o.m[0]  +  m[7] * o.m[1]  +  m[11] * o.m[2]  +  m[15] * o.m[3];
    
  r.m[4]  =  m[0] * o.m[4]  +  m[4] * o.m[5]  +  m[8]  * o.m[6]  +  m[12] * o.m[7];
  r.m[5]  =  m[1] * o.m[4]  +  m[5] * o.m[5]  +  m[9]  * o.m[6]  +  m[13] * o.m[7];
  r.m[6]  =  m[2] * o.m[4]  +  m[6] * o.m[5]  +  m[10] * o.m[6]  +  m[14] * o.m[7];
  r.m[7]  =  m[3] * o.m[4]  +  m[7] * o.m[5]  +  m[11] * o.m[6]  +  m[15] * o.m[7];
    
  r.m[8]  =  m[0] * o.m[8]  +  m[4] * o.m[9]  +  m[8]  * o.m[10] +  m[12] * o.m[11];
  r.m[9]  =  m[1] * o.m[8]  +  m[5] * o.m[9]  +  m[9]  * o.m[10] +  m[13] * o.m[11];
  r.m[10] =  m[2] * o.m[8]  +  m[6] * o.m[9]  +  m[10] * o.m[10] +  m[14] * o.m[11];
  r.m[11] =  m[3] * o.m[8]  +  m[7] * o.m[9]  +  m[11] * o.m[10] +  m[15] * o.m[11];
    
  r.m[12] =  m[0] * o.m[12] +  m[4] * o.m[13] +  m[8]  * o.m[14] +  m[12] * o.m[15];
  r.m[13] =  m[1] * o.m[12] +  m[5] * o.m[13] +  m[9]  * o.m[14] +  m[13] * o.m[15];
  r.m[14] =  m[2] * o.m[12] +  m[6] * o.m[13] +  m[10] * o.m[14] +  m[14] * o.m[15];
  r.m[15] =  m[3] * o.m[12] +  m[7] * o.m[13] +  m[11] * o.m[14] +  m[15] * o.m[15];
  return r;
}

template<class T>
Matrix4<T>& Matrix4<T>::lookat(Vec3<T> pos, Vec3<T> target, Vec3<T> up) {
    
  Vec3<T> f = normalized(target - pos);
  Vec3<T> u = normalized(up);
  Vec3<T> s = normalized(cross(f, u));
  u = cross(s, f);
    
  m[0] =  s.x;  m[4] =  s.y;  m[8] = s.z;
  m[1] =  u.x;  m[5] =  u.y;  m[9] = u.z;
  m[2] = -f.x;  m[6] = -f.y;  m[10] = -f.z;
    
  translate(-pos);
    
  return *this ;
}

template<class T>
struct Spline {
  size_t size();                                 /* the number of points */
  void clear();                                  /* remove all points */
  T at(float t);                                 /* interpolate using catmull rom */
  void push_back(const T point);                 /* add a point to the class */
  template<class I> void assign(I begin, I end); /* assign multiple values; just like std::vector<T>::assign() */
  T& operator[](const unsigned int);
  std::vector<T> points;
}; // Spline<T>

template<class T>
T& Spline<T>::operator[](const unsigned int dx) {
  return points[dx];
}

template<class T>
inline size_t Spline<T>::size() {
  return points.size();
}

template<class T>
inline void Spline<T>::clear() {
  return points.clear();
}

template<class T>
inline void Spline<T>::push_back(const T p) {
  points.push_back(p);
}

template<class T>
inline T Spline<T>::at(float t) {
  if(points.size() < 4) {
    return T();
  }
  if(t > 0.999f) {
    t = 0.99f;
  }
  else if(t < 0) {
    t = 0;
  }
    
  T result;
    
  // get local "t" (also mu)
  float curve_p = t * (points.size()-1);
  int curve_num = curve_p;
  t = curve_p - curve_num; // local t
    
  // get the 4 points
  int b = curve_num;
  int a = b - 1;
  int c = b + 1;
  int d = c + 1;
  if(a < 0) {
    a = 0;
  }
  if(d >= points.size()) {
    d = points.size()-1;
  }
    
  T& p0 = points[a]; // a
  T& p1 = points[b]; // b
  T& p2 = points[c]; // c
  T& p3 = points[d]; // d
    
  float t2 = t*t;
  float t3 = t*t*t;
    
  result = 0.5 * ((2 * p1) + (-p0 + p2) * t + (2 * p0 - 5 * p1 + 4 * p2 - p3) * t2 + (-p0 + 3 * p1 - 3 * p2 + p3) * t3);
    
  return result;
}

typedef Matrix4<float> mat4;
typedef Vec4<float> vec4;
typedef Vec3<float> vec3;
typedef Vec2<float> vec2;

extern float rx_random(float max);
extern float rx_random(float x, float y);
extern void rx_rgb_to_hsv(float r, float g, float b, float& h, float& s, float& v);
extern void rx_rgb_to_hsv(vec3 rgb, vec3& hsv);
extern void rx_rgb_to_hsv(float* rgb, float* hsv);
extern void rx_rgb_to_hsv(vec3 rgb, float* hsv);
extern void rx_hsv_to_rgb(float h, float s, float v, float& r, float& g, float& b);
extern void rx_hsv_to_rgb(vec3 hsv, float* rgb);
extern void rx_hsv_to_rgb(vec3 hsv, float* rgb);
extern void rx_hsv_to_rgb(float* hsv, float* rgb);

#define PERLIN_SIZE 1024

class Perlin {
    
 public:
  Perlin(int octaves, float freq, float amp, int seed);
  float get(float x);
  float get(float x, float y);
    
 private:
  void initPerlin(int n, float p);
  void init();
  float noise1(float arg);
  float noise2(float vec[2]);
  float noise3(float vec[3]);
  void normalize2(float v[2]);
  void normalize3(float v[3]);
  float noise2D(float vec[2]);
    
 private:
  int octaves;
  float freq;
  float amp;
  int seed;
    
  int p[PERLIN_SIZE + PERLIN_SIZE + 2];
  float g3[PERLIN_SIZE + PERLIN_SIZE + 2][3];
  float g2[PERLIN_SIZE + PERLIN_SIZE + 2][2];
  float g1[PERLIN_SIZE + PERLIN_SIZE + 2];
  bool start;
}; // Perlin

inline float Perlin::get(float x) {
  float vec[2] = {x, 0.0f};
  return noise2D(vec);
}

inline float Perlin::get(float x, float y) {
  float vec[2] = {x, y};
  return noise2D(vec);
}

#define PERLIN_B PERLIN_SIZE
#define PERLIN_BM (PERLIN_SIZE - 1)
#define PERLIN_N 0x1000
#define PERLIN_NP 12 /* 2^N */
#define PERLIN_NM 0xFFF
#define PERLIN_CURVE(t)  ( t * t * (3.0f - 2.0f * t) )
#define PERLIN_LERP(t, a, b) ( a + t * (b - a) )

#define PERLIN_SETUP(i, b0, b1, r0, r1)         \
  t = vec[i] + PERLIN_N;                        \
  b0 = ((int)t) & PERLIN_BM;                    \
  b1 = (b0+1) & PERLIN_BM;                      \
  r0 = t - (int)t;                              \
  r1 = r0 - 1.0f;

inline Perlin::Perlin(int octaves, float freq, float amp, int seed)
    :octaves(octaves)
    ,freq(freq)
    ,amp(amp)
    ,seed(seed)
    ,start(true)
{
}

inline float Perlin::noise1(float arg) {
  int bx0, bx1;
  float rx0, rx1, sx, t, u , v, vec[1];
  vec[0] = arg;
    
  if(start) {
    srand(seed);
    start = false;
    init();
  }
    
  PERLIN_SETUP(0, bx0, bx1, rx0, rx1);
  sx = PERLIN_CURVE(rx0);
  u = rx0 * g1[ p[bx0] ];
  v = rx1 * g1[ p[bx1] ];
  return PERLIN_LERP(sx, u, v);
}

inline float Perlin::noise2(float vec[2]) {
  int bx0, bx1, by0, by1, b00, b10, b01, b11;
  float rx0, rx1, ry0, ry1, *q, sx, sy, a, b, t, u, v;
  int i, j;
    
  if(start) {
    srand(seed);
    start = false;
    init();
  }
    
  PERLIN_SETUP(0, bx0, bx1, rx0, rx1);
  PERLIN_SETUP(1, by0, by1, ry0, ry1);
    
  i = p[bx0];
  j = p[bx1];
    
  b00 = p[i + by0];
  b10 = p[j + by0];
  b01 = p[i + by1];
  b11 = p[j + by1];
    
  sx = PERLIN_CURVE(rx0);
  sy = PERLIN_CURVE(ry0);
    
#define at2(rx, ry) (rx * q[0] + ry * q[1])
    
  q = g2[b00];
  u = at2(rx0, ry0);
  q = g2[b10];
  v = at2(rx1, ry0);
  a = PERLIN_LERP(sx, u, v);
    
  q = g2[b01];
  u = at2(rx0, ry1);
  q = g2[b11];
  v = at2(rx1, ry1);
  b = PERLIN_LERP(sx, u, v);
    
  return PERLIN_LERP(sy, a, b);
}

inline float Perlin::noise3(float vec[3]) {
  int bx0, bx1, by0, by1, bz0, bz1, b00, b10, b01, b11;
  float rx0, rx1, ry0, ry1, rz0, rz1, *q, sy, sz, a, b, c, d, t, u, v;
  int i, j;
    
  if(start) {
    srand(seed);
    start = false;
    init();
  }
    
  PERLIN_SETUP(0, bx0, bx1, rx0, rx1);
  PERLIN_SETUP(1, by0, by1, ry0, ry1);
  PERLIN_SETUP(2, bz0, bz1, rz0, rz1);
    
  i = p[bx0];
  j = p[bx1];
    
  b00 = p[i + by0];
  b10 = p[j + by0];
  b01 = p[i + by1];
  b11 = p[j + by1];
    
  t  = PERLIN_CURVE(rx0);
  sy = PERLIN_CURVE(ry0);
  sz = PERLIN_CURVE(rz0);
    
#define at3(rx,ry,rz) ( rx * q[0] + ry * q[1] + rz * q[2] )
    
  q = g3[b00 + bz0];
  u = at3(rx0,ry0,rz0);
  q = g3[b10 + bz0];
  v = at3(rx1,ry0,rz0);
  a = PERLIN_LERP(t, u, v);
    
  q = g3[b01 + bz0];
  u = at3(rx0,ry1,rz0);
  q = g3[b11 + bz0];
  v = at3(rx1,ry1,rz0);
  b = PERLIN_LERP(t, u, v);
    
  c = PERLIN_LERP(sy, a, b);
    
  q = g3[b00 + bz1];
  u = at3(rx0,ry0,rz1);
  q = g3[b10 + bz1];
  v = at3(rx1,ry0,rz1);
  a = PERLIN_LERP(t, u, v);
    
  q = g3[ b01 + bz1 ];
  u = at3(rx0,ry1,rz1);
  q = g3[ b11 + bz1 ];
  v = at3(rx1,ry1,rz1);
  b = PERLIN_LERP(t, u, v);
    
  d = PERLIN_LERP(sy, a, b);
    
  return PERLIN_LERP(sz, c, d);
}

inline void Perlin::normalize2(float v[2]) {
  float s;
    
  s = (float)sqrt(v[0] * v[0] + v[1] * v[1]);
  s = 1.0f/s;
  v[0] = v[0] * s;
  v[1] = v[1] * s;
}

inline void Perlin::normalize3(float v[3]) {
  float s;
    
  s = (float)sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
  s = 1.0f/s;
    
  v[0] = v[0] * s;
  v[1] = v[1] * s;
  v[2] = v[2] * s;
}

inline void Perlin::init(void) {
  int i, j, k;
    
  for(i = 0 ; i < PERLIN_B ; i++) {
    p[i] = i;
    g1[i] = (float)((rand() % (PERLIN_B + PERLIN_B)) - PERLIN_B) / PERLIN_B;
        
    for(j = 0 ; j < 2 ; j++) {
      g2[i][j] = (float)((rand() % (PERLIN_B + PERLIN_B)) - PERLIN_B) / PERLIN_B;
    }
        
    normalize2(g2[i]);
        
    for(j = 0 ; j < 3 ; j++) {
      g3[i][j] = (float)((rand() % (PERLIN_B + PERLIN_B)) - PERLIN_B) / PERLIN_B;
    }
        
    normalize3(g3[i]);
  }
    
  while(--i) {
    k = p[i];
    p[i] = p[j = rand() % PERLIN_B];
    p[j] = k;
  }
    
  for(i = 0 ; i < PERLIN_B + 2 ; i++) {
        
    p[PERLIN_B + i] = p[i];
    g1[PERLIN_B + i] = g1[i];
        
    for (j = 0 ; j < 2 ; j++) {
      g2[PERLIN_B + i][j] = g2[i][j];
    }
        
    for(j = 0 ; j < 3 ; j++){
      g3[PERLIN_B + i][j] = g3[i][j];
    }
        
  }
}

inline float Perlin::noise2D(float vec[2]) {
    
  float result = 0.0f;
  float amplitude = amp;
    
  vec[0] *= freq;
  vec[1] *= freq;
    
  for( int i = 0; i < octaves; i++ ) {
    result += noise2(vec) * amplitude;
    vec[0] *= 2.0f;
    vec[1] *= 2.0f;
    amplitude *= 0.5f;
  }
    
  return result;
}

#  endif // ROXLU_USE_MATH_H
#endif // ROXLU_USE_MATH

// ------------------------------------------------------------------------------------
//                              R O X L U _ U S E _ C U R L
// ------------------------------------------------------------------------------------

#if defined(ROXLU_USE_CURL)
#  ifndef ROXLU_USE_CURL_H
#  define ROXLU_USE_CURL_H
#  include <curl/curl.h>
#  define RX_CHECK_CURLCODE(c, str) { if(c != CURLE_OK) { printf("Error: %s\n", str); return false; } } 

size_t rx_curl_write_string_data(void* ptr, size_t size, size_t nmemb, void* str);
size_t rx_curl_write_file_data(void* ptr, size_t size, size_t nmemb, void* fpptr);

bool rx_fetch_url(std::string url, std::string& result);
bool rx_download_file(std::string url, std::string filepath);

#  endif
#endif

// ------------------------------------------------------------------------------------
//                              R O X L U _ U S E _ P N G
// ------------------------------------------------------------------------------------

#if defined(ROXLU_USE_PNG)
#  ifndef ROXLU_USE_PNG_H
#  define ROXLU_USE_PNG_H

extern bool rx_load_png(std::string filepath, unsigned char** pixels, int& w, int& h, int& nchannels);
extern bool rx_save_png(std::string filepath, unsigned char* pixels, int w, int h, int channels);

#  endif // ROXLU_USE_PNG_H
#endif //  defined(ROXLU_USE_PNG)

// ------------------------------------------------------------------------------------
//                              R O X L U _ U S E _ J P G
// ------------------------------------------------------------------------------------

#if defined(ROXLU_USE_JPG)
#  ifndef ROXLU_USE_JPG_H
#  define ROXLU_USE_JPG_H

#if defined(_WIN32)
#  define XMD_H
#  include <jpeglib.h>
#  undef XMD_H
#else 
#  include <jpeglib.h>
#endif

extern bool rx_load_jpg(std::string filepath, unsigned char** pix, int& width, int& height, int& nchannels);

#  endif // ROXLU_USE_JPG_H
#endif //  defined(ROXLU_USE_JPG)

// ------------------------------------------------------------------------------------
//                              R O X L U _ U S E _ O P E N G L
// ------------------------------------------------------------------------------------

#if defined(ROXLU_USE_OPENGL)
#  ifndef ROXLU_USE_OPENGL_H
#  define ROXLU_USE_OPENGL_H

extern void rx_print_shader_link_info(GLuint shader);
extern void rx_print_shader_compile_into(GLuint shader);
extern GLuint rx_create_program(GLuint vert, GLuint frag, bool link = false);
extern GLuint rx_create_program_with_attribs(GLuint vert, GLuint frag, int nattribs, const char** attribs);
extern GLuint rx_create_shader(GLenum type, const char* src);
extern GLuint rx_create_shader_from_file(GLenum type, std::string filepath);
extern GLint rx_get_uniform_location(GLuint prog, std::string name);
extern void rx_uniform_1i(GLuint prog, std::string name, GLint v);
extern void rx_uniform_1f(GLuint prog, std::string name, GLfloat v);
extern void rx_uniform_mat4fv(GLuint prog, std::string name, GLsizei count, GLboolean transpose, const GLfloat* value);

/* 
   As we so often use fullscreen vertex shaders we defined this one. Use this to create
   a fullscreen vertex shader that has a v_texcoord out varying member. 
*/

static const char* ROXLU_OPENGL_FULLSCREEN_VS = ""
  "#version 330\n"
  "const vec2 verts[4] = vec2[] ("
  "  vec2(-1.0, 1.0), "
  "  vec2(-1.0, -1.0), "
  "  vec2(1.0, 1.0), "
  "  vec2(1.0, -1.0) "
  ");"
  "const vec2 texcoords[4] = vec2[] ("
  "  vec2(0.0, 0.0), "
  "  vec2(0.0, 1.0), "
  "  vec2(1.0, 0.0), "
  "  vec2(1.0, 1.0) "
  ");"
  "out vec2 v_texcoord;"
  "void main() {"                               
  "  gl_Position = vec4(verts[gl_VertexID], 0.0, 1.0);"
  "  v_texcoord = texcoords[gl_VertexID];"
  "}"
  "";

class Shader {                                                                                               /* represents a GL shader - works only with shaders loaded from file */

 public:      
  Shader():type(-1),id(-1){}
  ~Shader() {}                                                                                               /* @todo - we should clean the resources here */
  Shader& load(GLenum type, std::string filepath, std::string extra = "");                                   /* load a shader file for the given type, you can pass extra data to the shader which is prepended */
  Shader& reload();                                                                                          /* reload the previously loaded file */
  Shader& compile();                                                                                         /* compile the shader */

 public:
  std::string filepath;                                                                                       /* filepath to the shader */
  std::string file_source;                                                                                    /* the source we loaded() */
  std::string extra_source;                                                                                   /* source of the shader that is loaded */
  int type;                                                                                                   /* the shader type, eg. GL_VERTEX_SHADER */
  int id;                                                                                                     /* the ID of the shader */
}; // Shader

class Program {

 public:
  Program():id(-1){};
  ~Program();                                                                                                 /* @todo - we should clean the resources here */
  Program& add(Shader* s);                                                                                    /* add a shader when you want to have ownership; you don't need to compile() it yourself */
  Program& create(GLenum type, std::string filepath, std::string extra = "");                                 /* create a shader and add it, we take ownership you can pass extra data to the shader, which is prepended */
  Program& link(int nattribs = 0, const char** attribs = NULL, int nfrags = 0, const char** frags = NULL);    /* compiles + links the shaders, if you want to bind attrib or frag locations pass them */
  Program& recompile();                                                                                       /* recompiles the shaders  + LINKS it for you! */

 public:
  std::vector<std::string> attribs;                                                                           /* any attributes added to link() */
  std::vector<std::string> frags;                                                                             /* output fragment locations */
  std::vector<Shader*> shaders;                                                                               /* the added shaders */
  std::vector<Shader*> created_shaders;                                                                       /* allocated shaders; we have ownership, see create() */
  int id;
}; // Program

#  endif // ROXLU_USE_OPENGL_H
#endif // defined(ROXLU_USE_OPENGL)

// ------------------------------------------------------------------------------------
//                              R O X L U _ U S E _ O P E N G L
//                              R O X L U _ U S E _ M A T H 
// ------------------------------------------------------------------------------------

#if defined(ROXLU_USE_OPENGL) && defined(ROXLU_USE_MATH)
#  ifndef ROXLU_USE_OPENGL_MATH_H
#  define ROXLU_USE_OPENGL_MATH_H

struct VertexP {
  VertexP(){};
  VertexP(vec3 p):pos(p){}
  void set(vec3 p) { pos = p; } 
  float* ptr() { return pos.ptr(); } 
  vec3 pos;
};

struct VertexPC {
  VertexPC(){}
  VertexPC(vec3 pos, vec4 col):pos(pos),col(col){}
  void set(vec3 p, vec4 c) { pos = p; col = c; } 
  float* ptr() { return pos.ptr(); } 
  vec3 pos;
  vec4 col;
};

struct VertexPT {
  VertexPT(){}
  VertexPT(vec3 p, vec2 t):pos(p),tex(t){}
  void set(vec3 p, vec2 t) { pos = p; tex = t; } 
  float* ptr() { return pos.ptr(); } 
  void print() { printf("x: %f, y: %f, z: %f, u: %f, v: %f\n", pos.x, pos.y, pos.z, tex.x, tex.y); } 
  vec3 pos;
  vec2 tex;
};

struct VertexPT3 {
  VertexPT3(){}
  VertexPT3(vec3 p, vec3 t):pos(p),tex(t){}
  void set(vec3 p, vec3 t) { pos = p; tex = t; } 
  float* ptr() { return pos.ptr(); } 
  void print() { printf("x: %f, y: %f, z: %f, u: %f, v: %f, q: %f\n", pos.x, pos.y, pos.z, tex.x, tex.y, tex.z); } 
  vec3 pos;
  vec3 tex;
};

struct VertexPTN {
  VertexPTN() {}
  VertexPTN(vec3 p, vec2 t, vec3 n):pos(p),tex(t),norm(n){}
  void set(vec3 p, vec2 t, vec3 n) { pos = p; tex = t; norm = n; } 
  float* ptr() { return pos.ptr(); } 
  vec3 pos;
  vec2 tex;
  vec3 norm;
};

struct VertexPN {
  VertexPN();
  VertexPN(vec3 p, vec3 n):pos(p),norm(n){}
  void set(vec3 p, vec3 n) { pos = p; norm = n; }
  float* ptr() { return pos.ptr(); }
  vec3 pos;
  vec3 norm;
};

class OBJ {
 public:
  struct TRI { int v, t, n; };
  struct FACE { TRI a, b, c; };
  struct XYZ {  float x, y, z; };
  struct TEXCOORD { float s, t; };

  bool load(std::string filepath);

  bool hasNormals();
  bool hasTexCoords();

  template<class T>
    bool copy(T& result);

  void push_back(vec3 vert, vec3 norm, vec2 tc, std::vector<VertexP>& verts);
  void push_back(vec3 vert, vec3 norm, vec2 tc, std::vector<VertexPTN>& verts);
  void push_back(vec3 vert, vec3 norm, vec2 tc, std::vector<VertexPT>& verts);
  void push_back(vec3 vert, vec3 norm, vec2 tc, std::vector<VertexPN>& verts);

 public:
  std::vector<vec3> vertices;
  std::vector<vec3> normals;
  std::vector<vec2> tex_coords;
  std::vector<OBJ::FACE> faces;
  std::vector<int> indices;
  bool has_texcoords;
  bool has_normals;
}; // OBJ

template<class T>
inline bool OBJ::copy(T& result) {
  for(std::vector<FACE>::iterator it = faces.begin(); it != faces.end(); ++it) {
    FACE& f = *it;
    push_back(vertices[f.a.v], normals[f.a.n], tex_coords[f.a.t], result) ;
    push_back(vertices[f.b.v], normals[f.b.n], tex_coords[f.b.t], result) ;
    push_back(vertices[f.c.v], normals[f.c.n], tex_coords[f.c.t], result) ;
  }
  return true;
}

inline void OBJ::push_back(vec3 vert, vec3 norm, vec2 tc, std::vector<VertexP>& verts) {
  verts.push_back(VertexP(vert));
}

inline void OBJ::push_back(vec3 vert, vec3 norm, vec2 tc, std::vector<VertexPTN>& verts) {
  verts.push_back(VertexPTN(vert, tc, norm));
}

inline void OBJ::push_back(vec3 vert, vec3 norm, vec2 tc, std::vector<VertexPT>& verts) {
  verts.push_back(VertexPT(vert, tc));
}

inline void OBJ::push_back(vec3 vert, vec3 norm, vec2 tc, std::vector<VertexPN>& verts) {
  verts.push_back(VertexPN(vert, norm));
}

inline bool OBJ::hasNormals() {
  return has_normals;
}
 
inline bool OBJ::hasTexCoords() {
  return has_texcoords;
}

inline bool OBJ::load(std::string filepath) {

  // are unset below
  has_normals = true;
  has_texcoords = true;

  std::ifstream ifs;
  ifs.open(filepath.c_str());
  if(!ifs.is_open()) {
    printf("Error: Cannot find .obj file: %s\n", filepath.c_str());
    return false;
  }

  char c;
  std::string line;
  while(std::getline(ifs, line)) {
    std::stringstream ss(line);
    ss >> c;
    if(c == 'v') {
      if(line[1] == ' ') {
        vec3 p;
        ss >> p.x >> p.y >> p.z;
        vertices.push_back(p);
      }
      else if(line[1] == 'n') {
        vec3 p;
        ss >> c;
        ss >> p.x >> p.y >> p.z;
        normals.push_back(p);
      }
      else if (line[1] == 't') {
        vec2 t;
        ss >> c;
        ss >> t.x >> t.y;
        t.y = 1.0f - t.y;
        tex_coords.push_back(t);
      }
    }
    else if(c == 'f') {
      std::string part;
      std::vector<OBJ::TRI> tris;
      while(ss >> part) {
        std::stringstream fss;
        std::string indices[3];
        int dx = 0;
        for(int i = 0; i < part.size(); ++i) {
          if(part[i] == '/') {
            dx++;
            continue;
          }
          indices[dx].push_back(part[i]);
        }
        TRI tri;
        tri.v = atoi(indices[0].c_str()) - 1;
        tri.t = atoi(indices[1].c_str()) - 1;
        tri.n = atoi(indices[2].c_str()) - 1;
        tris.push_back(tri);
      }
      if(tris.size() == 3) {
        OBJ::FACE face;
        face.a = tris[0];
        face.b = tris[1];
        face.c = tris[2];
        faces.push_back(face);
      }
      else {
        printf("Error: wrong face indices.\n");
      }
    }
  }

  // create empty texcoords/normals when not found so we just return invalid values but wont crash
  if(!normals.size()) {
    normals.assign(vertices.size(), vec3());
    has_normals = false;
  }
  if(!tex_coords.size()) {
    tex_coords.assign(vertices.size(), vec2());
    has_texcoords = false;
  }
    
  return true;
} // OBJ::load


/*
  Painter
  --------
  
  The Painter class is a simple wrapper that mimics intermediate 
  mode of openGL draw calls. This Painter class is not meant to be
  used in super optimized applications as using this approach is 
  is just not the correct one for optimized draw calls. Though this
  class is probably quite fast as we will use vbo's. Goal of is class
  is to provide a simple way to draw lines, rects, circles etc.. when 
  you just want to quickly draw something.

  Contexts
  --------
  The painter is build around the concept of contexts, where a 
  context takes care of drawing particular vertex data. At this time
  we only have a VertexPC renderer, which can draw vertices with color.
  When ready, we will also have a VertexPT context that can render 
  textures.

  TODO
  ----
  At this point the API might change...

 */

#define PAINTER_CONTEXT_TYPE_PC 1   /* a context that can draw position + colors */
#define PAINTER_CONTEXT_TYPE_PT 2   /* a context that can draw position + texcoords (not yet implemented) */

#define PAINTER_STATE_NONE 0x0000   /* default state; no-fill */
#define PAINTER_STATE_FILL 0x0001   /* draw everything filled */

// -----------------------------------------------------

static const char* PAINTER_VERTEX_PC_VS  = ""
  "#version 330\n"
  ""
  "layout( std140 ) uniform Shared { "
  "  mat4 u_pm; " 
  "};"
  ""
  "layout( location = 0 ) in vec4 a_pos; " 
  "layout( location = 1 ) in vec4 a_col; "
  "out vec4 v_col;"
  ""
  "void main() {"
  "  gl_Position = u_pm * a_pos; " 
  "  v_col = a_col; "
  "}"
  "";

static const char* PAINTER_VERTEX_PC_FS = ""
  "#version 330\n"
  "in vec4 v_col; " 
  "layout (location = 0) out vec4 fragcolor; " 
  ""
  "void main() {"
  " fragcolor = v_col; " 
  "}"
  "";

// -----------------------------------------------------

static const char* PAINTER_VERTEX_PT_VS = ""
  "#version 330\n"
  ""
  "layout( std140 ) uniform Shared { "
  "  mat4 u_pm; "
  "};"
  ""
  "layout( location = 0 ) in vec4 a_pos; " 
  "layout( location = 1 ) in vec2 a_tex; "
  "out vec2 v_tex;"
  ""
  "void main() {"
  "   gl_Position = u_pm * a_pos; "
  "   v_tex = a_tex; " 
  "}"
  "";

static const char* PAINTER_VERTEX_PT_SAMPLER2D_FS = ""
  "#version 330\n"
  "uniform sampler2D u_tex; "
  "in vec2 v_tex; " 
  "layout( location = 0 ) out vec4 fragcolor; " 
  ""
  "void main() {"
  "  fragcolor = texture(u_tex, v_tex);"
  "}"
  "";

// -----------------------------------------------------

class PainterCommand {
 public:
  GLenum type;
  int offset;
  int count;
  GLuint tex; /* the texture, in case we need to draw a texture */
};

// -----------------------------------------------------

class Painter;

class PainterContextPC {

public:
  PainterContextPC(Painter& painter);                             /* a PainterContext is used to render specific vertex data. This context renders only colors */
  void clear();                                                   /* clear all vertices */
  void update();                                                  /* updates the vbo if necessary */
  void draw();                                                    /* draws the buffers and commands to screen */
  void rect(float x, float y, float w, float h);                  /* create a rectangular shape */
  void circle(float x, float y, float radius);                    /* create a circle */
  void line(float x0, float y0, float x1, float y1);              /* create a line */
  void command(GLenum cmd, std::vector<VertexPC>& vertices);      /* adds a command (Painter calls this when using begin()/end() */

public:
  Painter& painter;                                               /* reference to the main Painter object */
  std::vector<PainterCommand> commands;                           /* the command that we render */
  std::vector<VertexPC> vertices;                                 /* the vertices... also stored in vbo */
  size_t allocated;                                               /* number of bytes allocated in our vbo */
  bool needs_update;                                              /* is set to true whenever our vbo needs to be updated */
  GLuint vao;                                                     /* the vao */
  GLuint vbo;                                                     /* vbo that holds our data on the gpu */
  GLuint vert;                                                    /* our vertex shader, see PAINTER_VERTEX_PC_VS */
  GLuint frag;                                                    /* our fragment shader, see PAINTER_VERTEX_PC_FS */
  GLuint prog;                                                    /* the shader program, used to render our vbo */
};

// -----------------------------------------------------

class PainterContextPT {

 public:
  PainterContextPT(Painter& painter);                             /* a PainterContextPT is used to render textures */
  void clear();                                                   /* clear all vertices */
  void update();                                                  /* updates the vbo with VertexPT data */
  void draw();                                                    /* draws the texture */
  void texture(GLuint tex, float x, float y, float w, float h);   /* add anther texture that should be drawn */

 public:
  Painter& painter;                                               /* reference to the main Painter object */
  std::vector<PainterCommand> commands;                           /* the commands/texture draw calls. the type field contains the texture type; at this moment of writing only GL_TEXTURE_2D is supported */
  std::vector<VertexPT> vertices;                                 /* the vertices we draw */
  size_t allocated;                                               /* number of bytes we've allocated in the vbo */
  bool needs_update;                                              /* is set to true whenever we need to udpate the vbo; is set in texture() */
  GLuint vao;                                                     /* our vao, that contains the vertex info */
  GLuint vbo;                                                     /* reference to our gpu vbo */
  GLuint vert;                                                    /* vertex shader for vertexpt data */  
  GLuint frag;                                                    /* fragment shader for the vertexpt data */
  GLuint prog;                                                    /* the shader program that draws the texture */
};

// -----------------------------------------------------

// Shared GL data
struct PainterShared {
  mat4 pm;
};

// -----------------------------------------------------

class Painter {

 public:
  Painter();
  void clear();                                                                     /* clear all vertices */
  void draw();                                                                      /* draw all the added lines, circles, textures etc.. */
  void rect(float x, float y, float w, float h);                                    /* draw a rectangle */
  void circle(float x, float y, float radius);                                      /* draw a circle, see resolution() to change the resolution of the circle */
  void line(float x0, float y0, float x1, float y1);                                /* draw a single line, see begin()/end() if you want to draw line strips */
  void texture(GLuint tex, float x, float y, float w, float h);                     /* draw a texture, at the time of writing only GL_TEXTURE_2D is supported */

  void begin(GLenum type);                                                          /* begin a batch of vertices, just like the old days  */
  void vertex(float x, float y);                                                    /* add a vertex to the current batch */
  void end();                                                                       /* end the current batch; this will flush the added vertices into the correct context */

  void color(float r = 1.0, float g = 1.0, float b = 1.0, float a = 1.0);           /* set the color of lines, circles, rectangles */
  void fill();                                                                      /* enable fill mode */
  void nofill();                                                                    /* disable fill mode */
  void resize(int w, int h);                                                        /* whenever your viewport changes, call this so we can recalculate the projection matrix */
  void resolution(int n);                                                           /* set the circle resolution */

  int width();                                                                      /* returns the last set/calculated viewport width */
  int height();                                                                     /* returns the last set/calculated viewport height */

 public:
  PainterContextPC context_pc;                                                      /* context used to draw VertexPC vertices (color) */
  PainterContextPT context_pt;                                                      /* context used to draw VertexPT vertices (textures) */
  int circle_resolution;                                                            /* the last set circle resolution */
  vec4 col;                                                                         /* the color we use to draw with */
  int state;                                                                        /* keeps state of the painter; e.g. fill/nofill */
  GLuint ubo;                                                                       /* shared uniform buffer object with matrice(s) */
  PainterShared ubo_data;                                                           /* the actual data for our ubo */
  std::vector<vec2> circle_data;                                                    /* pre-calculated sin/cos values for our circle; just a tiny bit of optimization */
  int win_w;                                                                        /* the last calculated or set viewport width */
  int win_h;                                                                        /* the last calculated or set viewport height */

  /* begin() - end() handling */
  GLenum command_type;                                                              /* when begin() is called we store the command type. the next call to vertex defines what PainterContext we will use; for now only default is used */
  int context_type;                                                                 /* when begin()/end() is used, a call to one of the overloaded vertex() function will define what context will be used. in end() we add the added vertices to the correct context */
  std::vector<VertexPC> vertices_pc;                                                /* VertexPC vertices for our PC context */ 
};

#  endif // ROXLU_USE_OPENGL_MATH_H
#endif // defined(ROXLU_USE_OPENGL) && defined(ROXLU_USE_MATH)

// ------------------------------------------------------------------------------------
//                              R O X L U _ U S E _ O P E N G L 
//                              R O X L U _ U S E _ P N G 
// ------------------------------------------------------------------------------------

#if defined(ROXLU_USE_OPENGL) && defined(ROXLU_USE_PNG)
#  ifndef ROXLU_USE_OPENGL_PNG_H
#  define ROXLU_USE_OPENGL_PNG_H

extern GLuint rx_create_texture(std::string filepath, int internalFormat = -1, int format = -1, int type = -1);

#  endif // ROXLU_USE_OPENGL_PNG_H
#endif // defined(ROXLU_USE_OPENGL) && defined(ROXLU_USE_PNG)


// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//                          I M P L E M E N T A T I O N
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// ====================================================================================
//                              T I N Y L I B 
// ====================================================================================

#if defined(ROXLU_IMPLEMENTATION)

#if defined(_WIN32) 
extern std::string rx_get_exe_path() {
  char buffer[MAX_PATH];

  // Try to get the executable path with a buffer of MAX_PATH characters.
  DWORD result = ::GetModuleFileNameA(nullptr, buffer, static_cast<DWORD>(MAX_PATH));
  if(result == 0) {
    return "";
  }

  std::string::size_type pos = std::string(buffer).find_last_of( "\\/" );

  return std::string(buffer).substr(0, pos) +"\\";
}
#elif defined(__APPLE__) 
extern std::string rx_get_exe_path() {
  char buffer[1024];
  uint32_t usize = sizeof(buffer);;

  int result;
  char* path;
  char* fullpath;

  result = _NSGetExecutablePath(buffer, &usize);
  if (result) {
    return "";
  }

  path = (char*)malloc(2 * PATH_MAX);
  fullpath = realpath(buffer, path);

  if (fullpath == NULL) {
    free(path);
    return "";
  }
  strncpy(buffer, fullpath, usize);

  const char* dn = dirname(buffer);
  usize = strlen(dn);
  std::string ret(dn, usize) ;
  ret.push_back('/');

  free(path);
  return ret;
}
#elif defined(__linux) 
extern std::string rx_get_exe_path() {
  char buffer[MAX_PATH];
  size_t size = MAX_PATH;
  ssize_t n = readlink("/proc/self/exe", buffer, size - 1);
  if (n <= 0) {
    return "";
  }
  buffer[n] = '\0';


  const char* dn = dirname(buffer);
  size = strlen(dn);
  std::string ret(dn, size) ;
  ret.push_back('/');
  return ret;
}
#endif // rx_get_exe_path() / win32

extern bool rx_file_exists(std::string filepath) {

#if defined(_WIN32)
  char* lptr = (char*)filepath.c_str();
  DWORD dwattrib = GetFileAttributes(lptr);
  return (dwattrib != INVALID_FILE_ATTRIBUTES && !(dwattrib & FILE_ATTRIBUTE_DIRECTORY));

#elif defined(__APPLE__)
  int res = access(filepath.c_str(), R_OK);
  if(res < 0) {
    return false;
  }
#else
#  error Need to implement rx_file_exists on linux
#endif

  return true;
}

#if !defined(WIN32) 
extern bool rx_is_dir(std::string filepath) {
  struct stat st;
  int result = stat(filepath.c_str(), &st);

  if(result < 0) {
    if(errno == EACCES) {
      printf("EACCESS: no permission for: %s", filepath.c_str());
    }
    else if(errno == EFAULT) {
      printf("EFAULT: bad address, for: %s", filepath.c_str());
    }
    else if(errno == ELOOP) {
      printf("ELOOP: too many links, for: %s", filepath.c_str());
    }
    else if(errno == ENAMETOOLONG) {
      printf("ENAMETOOLONG: for: %s", filepath.c_str());
    }
    else if(errno == ENOENT) {
      // we expect this when the dir doesn't exist
      return false;
    }
    else if(errno == ENOMEM) {
      printf("ENOMEM: for: %s", filepath.c_str());
    }
    else if(errno == ENOTDIR) {
      printf("ENOTDIR: for: %s", filepath.c_str());
    }
    else if(errno == EOVERFLOW) {
      printf("EOVERFLOW: for: %s", filepath.c_str());
    }

    return false;
  }

#if defined(__APPLE__) or defined(__linux__)
  return S_ISDIR(st.st_mode);
#else 
  return result == 0;
#endif  
}
#endif // !defined(WIN32) for rx_is_dir()

extern std::string rx_to_data_path(const std::string filename) {
  std::string exepath = rx_get_exe_path();

#if defined(__APPLE__)
  if(rx_is_dir(exepath +"data")) {
    exepath += "data/" +filename;
  }
  else if(rx_is_dir(exepath +"../MacOS")) {
    exepath += "../../../data/" +filename;
  }
  else {
    exepath += filename;
  }
#else 
  exepath += "data/" +filename;
#endif  

  return exepath;
}

extern std::string rx_string_replace(std::string str, std::string from, std::string to) {
  size_t start_pos = str.find(from);
  if(start_pos == std::string::npos) {
    return str;
  }
  str.replace(start_pos, from.length(), to);
  return str;
}

extern std::string rx_string_replace(std::string str, char from, char to) {
  std::replace(str.begin(), str.end(), from, to);
  return str;
}

extern std::string rx_strip_filename(std::string path) {
  std::string directory;
  path = rx_string_replace(path, '\\', '/');
  path = rx_string_replace(path, "//", "/");
  const size_t last_slash_idx = path.rfind('/');

  if(std::string::npos != last_slash_idx) {
    directory = path.substr(0, last_slash_idx + 1);
  }

#if defined(_WIN32)
  directory = rx_string_replace(directory, '/', '\\');
#endif

  return directory;
}

extern std::string rx_strip_dir(std::string path) {
  std::string filename;;
  path = rx_string_replace(path, '\\', '/');
  path = rx_string_replace(path, "//", "/");

  const size_t last_slash_idx = path.rfind('/');
  if(last_slash_idx == std::string::npos) {
    return path;
  }

  if(std::string::npos != last_slash_idx) {
    filename = path.substr(last_slash_idx + 1, path.size());
  }

  return filename;
}

extern bool rx_create_dir(std::string path) {
#ifdef _WIN32
  if(_mkdir(path.c_str()) != 0) {
    if(errno == ENOENT) { 
      printf("Cannot create directory: %s (ENOENT)\n", path.c_str());
      return false;
    }
    else if(errno == EEXIST) {
      printf("Cannot create directory: %s (EEXIST)\n", path.c_str());
    }
  }
  return true;

#else
  if(mkdir(path.c_str(), 0777) != 0) {
    return false;
  }
  return true;
#endif
}

extern std::string rx_get_file_ext(std::string filepath) {
  size_t pos = filepath.rfind(".");
  std::string result = "";

  if(pos == std::string::npos) {
    return result;
  }

  std::string ext = filepath.substr(pos + 1, (filepath.size() - pos));
  return ext;
} // rx_get_file_ext()

extern bool rx_create_path(std::string path) {

#ifdef _WIN32
  std::string drive;
  for(int i = 0; i < path.size()-1; ++i) {
    if(path[i] == ':' && path[i + 1] == '\\') {
      break;
    }
    drive.push_back(path[i]);
  }
  path = path.substr(drive.size() + 2);
  drive = drive + ":";
#endif

  std::vector<std::string> dirs;
  while(path.length() > 0) {

     
#ifdef _WIN32
    int index = path.find('\\'); // win 
#else
    int index = path.find('/'); // posix
#endif
    std::string dir = (index == -1 ) ? path : path.substr(0, index);

    if(dir.length() > 0) {
      dirs.push_back(dir);
    }
    if(index + 1 >= path.length() || index == -1) {
      break;
    }
    path = path.substr(index + 1);
  }
    
  struct stat s;
  std::string dir_path;

#ifdef _WIN32
  dir_path = drive;
#endif

  for(unsigned int i = 0; i < dirs.size(); i++) {

#ifdef _WIN32
    dir_path += "\\";
#else
    dir_path += "/";
#endif

#if !defined(_WIN32)
    dir_path += dirs[i];
    if(stat(dir_path.c_str(), &s) != 0) {
      if(!rx_create_dir(dir_path.c_str())) {
        printf("ERROR: cannot create directory: %s\n", dir_path.c_str());
        return false;
      }
    }
#endif
  }

#if defined(_WIN32)
   printf("WARNING: We have not yet implemented rx_create_path() on windows.\n");
#endif

  return true;
} // rx_create_path()

extern std::vector<std::string> rx_get_files(std::string path, std::string ext) { 
  std::vector<std::string> result;
#if !defined(_WIN32)
  DIR* dir;
  struct dirent* ent;
  if((dir = opendir(path.c_str())) != NULL) {
    while((ent = readdir(dir)) != NULL) {
      if(ent->d_type == DT_REG) {
        std::string file_path = path +"/" +ent->d_name;

        if(ext.size()) {
          std::string file_ext = rx_get_file_ext(file_path);
          if(file_ext != ext) {
            continue;
          }
        }

        result.push_back(file_path);
      }
    }
    closedir(dir);
  }
#else
  printf("WARNING: We have not yet implemented rx_get_files on windows.\n");
#endif
  return result;
} // rx_get_files()

extern std::string rx_norm_path(std::string path) {
#if defined(_WIN32)
  std::string from = "/";
  std::string to = "\\";
  size_t start_pos = 0;
  while((start_pos = path.find(from, start_pos)) != std::string::npos) {
    path.replace(start_pos, from.length(), to);
    start_pos += to.length();
  }
  return path;
#else
  return path;
#endif
} // rx_norm_path()


extern int rx_to_int(const std::string& v) {
  int r = 0;
  std::stringstream ss(v);
  ss >> r;
  return r;
}

extern float rx_to_float(const std::string& v) {
  float r = 0.0f;
  std::stringstream ss(v);
  ss >> r;
  return r;
}

extern std::string rx_int_to_string(const int& v) {
  std::stringstream ss;
  ss << v;
  return ss.str();
}

extern std::string rx_float_to_string(const float& v) {
  std::stringstream ss;
  ss << v;
  return ss.str();
}

extern std::vector<std::string> rx_split(std::string str, char delim) {
  std::string line;
  std::stringstream ss(str);
  std::vector<std::string> result;
  while(std::getline(ss, line, delim)) {
    result.push_back(line);
  }
  return result;
}

extern uint64_t rx_hrtime() {
#if defined(__APPLE__) 
  mach_timebase_info_data_t info;
  if(mach_timebase_info(&info) != KERN_SUCCESS) {
    abort();
  }
  return mach_absolute_time() * info.numer / info.denom;

#elif defined(__linux)
  static clock_t fast_clock_id = -1;
  struct timespec t;
  clock_t clock_id;

  if(fast_clock_id == -1) {
    if(clock_getres(CLOCK_MONOTONIC_COARSE, &t) == 0 && t.tv_nsec <= 1 * 1000 * 1000LLU) {
      fast_clock_id = CLOCK_MONOTONIC_COARSE;
    }
    else {
      fast_clock_id = CLOCK_MONOTONIC;
    }
  }

  clock_id =  CLOCK_MONOTONIC;
  if(clock_gettime(clock_id, &t)) {
    return 0; 
  }

  return t.tv_sec * (uint64_t)1e9 +t.tv_nsec;

#elif defined(_WIN32)
  LARGE_INTEGER timer_freq;
  LARGE_INTEGER timer_time;
  QueryPerformanceCounter(&timer_time);
  QueryPerformanceFrequency(&timer_freq);
  static double freq = (double)timer_freq.QuadPart/(double)1000000000;
  return (uint64_t)((double)timer_time.QuadPart / freq);
#endif
};

extern float rx_millis() {
  static uint64_t start = rx_hrtime();
  int64_t d = (rx_hrtime() - start);
  return d / 1000000000.0;
}

extern std::string rx_read_file(std::string filepath) {
  std::ifstream ifs(filepath.c_str(), std::ios::in);
  if(!ifs.is_open()) {
    return "";
  }
  std::string str((std::istreambuf_iterator<char>(ifs)) , std::istreambuf_iterator<char>());
  return str;
}

extern std::string rx_strftime(const std::string fmt) {
  time_t t;
  struct tm* info;
  char buf[4096]; // must be enough..
  time(&t);
  info = localtime(&t);
  strftime(buf, 4096, fmt.c_str(), info);
  std::string result(buf);
  return result;
}

extern int rx_get_year() {
  return rx_to_int(rx_strftime("%Y"));
}

extern int rx_get_month() {
  return rx_to_int(rx_strftime("%m"));
}

extern int rx_get_day() {
  return rx_to_int(rx_strftime("%d"));
}

extern int rx_get_hour() {
  return rx_to_int(rx_strftime("%H"));
}

extern int rx_get_minute() {
  return rx_to_int(rx_strftime("%M"));
}

#endif // defined(ROXLU_IMPLEMENTATION)

// ====================================================================================
//                              R O X L U _ U S E _ M A T H
// ====================================================================================
#if defined(ROXLU_USE_CURL) && defined(ROXLU_IMPLEMENTATION) 

size_t rx_curl_write_string_data(void* ptr, size_t size, size_t nmemb, void* str) {
  std::string* s = static_cast<std::string*>(str);
  std::copy((char*)ptr, (char*)ptr + (size * nmemb), std::back_inserter(*s));
  return size * nmemb;
}

size_t rx_curl_write_file_data(void* ptr, size_t size, size_t nmemb, void* fpptr) {
  FILE* fp = static_cast<FILE*>(fpptr);
  size_t written = fwrite(ptr, size, nmemb, fp);
  return written;
}

bool rx_fetch_url(std::string url, std::string& result) {
  
  CURL* curl = NULL;
  CURLcode res;

  curl = curl_easy_init();
  if(!curl) {
    printf("Error: cannot intialize curl.\n");
    return false;
  }

  res = curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  RX_CHECK_CURLCODE(res, "Cannot set url");

  res = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
  RX_CHECK_CURLCODE(res, "Cannot set follow location");

  res = curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
  RX_CHECK_CURLCODE(res, "Cannot set write data");
  
  res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, rx_curl_write_string_data);
  RX_CHECK_CURLCODE(res, "Cannot set write function");

  res = curl_easy_perform(curl);
  RX_CHECK_CURLCODE(res, "Cannot easy perform");

  curl_easy_cleanup(curl);
  curl = NULL;
  
  return true;
}

bool rx_download_file(std::string url, std::string filepath) {

  FILE* fp = NULL;
  CURL* curl = NULL;
  CURLcode res;

  fp = fopen(filepath.c_str(), "wb");
  if(!fp) {
    printf("Error: cannot open file: %s\n", filepath.c_str());
    return false;
  }
  
  curl = curl_easy_init();
  if(!curl) {
    printf("Error: cannot initialize cur.\n");
    return false;
  }

  res = curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  RX_CHECK_CURLCODE(res, "Cannot set url");

  res = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
  RX_CHECK_CURLCODE(res, "Cannot set follow location");

  res = curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
  RX_CHECK_CURLCODE(res, "Cannot set write data");
  
  res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, rx_curl_write_file_data);
  RX_CHECK_CURLCODE(res, "Cannot set write function");

  res = curl_easy_perform(curl);
  RX_CHECK_CURLCODE(res, "Cannot easy perform");

  curl_easy_cleanup(curl);
  curl = NULL;

  fclose(fp);
  fp = NULL;

  return true;
}

#endif

// ====================================================================================
//                              R O X L U _ U S E _ M A T H
// ====================================================================================
#if defined(ROXLU_USE_MATH) && defined(ROXLU_IMPLEMENTATION) 

extern float rx_random(float max) {
  return max * rand() / (RAND_MAX + 1.0f);
}

extern float rx_random(float x, float y) {
  float high = 0;
  float low = 0;
  float result = 0;
        
  high = std::max<float>(x,y);
  low = std::min<float>(x,y);
  result = low + ((high-low) * rand()/(RAND_MAX + 1.0));
  return result;
}

// all in range 0 - 1 
extern void rx_rgb_to_hsv(float r, float g, float b, float& h, float& s, float& v) {
  float K = 0.f;

  if(g < b) {
    std::swap(g, b);
    K = -1.f;
  }

  if(r < g) {
    std::swap(r, g);
    K = -2.f / 6.f - K;
  }

  float chroma = r - std::min<float>(g, b);
  h = fabs(K + (g - b) / (6.f * chroma + 1e-20f));
  s = chroma / (r + 1e-20f);
  v = r;
}

extern void rx_rgb_to_hsv(vec3 rgb, vec3& hsv) {
  rx_rgb_to_hsv(rgb.x, rgb.y, rgb.z, hsv.x, hsv.y, hsv.z);
}

extern void rx_rgb_to_hsv(vec3 rgb, float* hsv) {
  rx_rgb_to_hsv(rgb.x, rgb.y, rgb.z, hsv[0], hsv[1], hsv[2]);
}

extern void rx_rgb_to_hsv(float* rgb, float* hsv) {
  rx_rgb_to_hsv(rgb[0], rgb[1], rgb[2], hsv[0], hsv[1], hsv[2]);
}

extern void rx_hsv_to_rgb(float h, float s, float v, float& r, float& g, float& b) {
  float tmp_r = CLAMP((-1.0f + fabs(6.0f * h - 3.0f)), 0,1);
  float tmp_g = CLAMP(( 2.0f - fabs(6.0f * h - 2.0f)), 0,1);      
  float tmp_b = CLAMP(( 2.0f - fabs(6.0f * h - 4.0f)), 0,1);  
  float p = 1.0f - s;  
  r = v * (p + tmp_r * s);  
  g = v * (p + tmp_g * s);    
  b = v * (p + tmp_b * s);        
}

extern void rx_hsv_to_rgb(vec3 hsv, vec3& rgb) {
  rx_hsv_to_rgb(hsv.x, hsv.y, hsv.z, rgb.x, rgb.y, rgb.z);
}

extern void rx_hsv_to_rgb(vec3 hsv, float* rgb) {
  rx_hsv_to_rgb(hsv.x, hsv.y, hsv.z, rgb[0], rgb[1], rgb[2]);
}

extern void rx_hsv_to_rgb(float* hsv, float* rgb) {
  rx_hsv_to_rgb(hsv[0], hsv[1], hsv[2], rgb[0], rgb[1], rgb[2]);
}


#endif // defined(ROXLU_USE_MATH) && defined(ROXLU_IMPLEMENTATON) 

// ====================================================================================
//                              R O X L U _ U S E _ J P G
// ====================================================================================

#if defined(ROXLU_USE_JPG) && defined(ROXLU_IMPLEMENTATION)

bool rx_load_jpg(std::string filepath, unsigned char** pix, int& width, int& height, int& nchannels) {
  
  struct jpeg_error_mgr jerr;
  struct jpeg_decompress_struct cinfo;
  FILE* fp;
  JSAMPARRAY buffer;
  int stride = 0;
  int num_bytes = 0;
  unsigned char* pixels = NULL;
  
  if( (fp = fopen(filepath.c_str(), "rb")) == NULL ) {
    printf("Error: cannot load %s\n", filepath.c_str());
    return false;
  }

  cinfo.err = jpeg_std_error(&jerr);

  jpeg_create_decompress(&cinfo);
  jpeg_stdio_src(&cinfo, fp);
  jpeg_read_header(&cinfo, TRUE);
  jpeg_start_decompress(&cinfo);

  stride = cinfo.output_width * cinfo.output_components;
  nchannels = cinfo.output_components;
  width = cinfo.output_width;
  height = cinfo.output_height;
  num_bytes = width * height * nchannels;

  pixels = new unsigned char[num_bytes];
  if(!pixels) {
    printf("Error: cannot allocate pixel buffer for jpg.\n");
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(fp);
    return false;
  }

  size_t dest_row = 0;
  buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, stride, 1);
  while(cinfo.output_scanline < cinfo.output_height) {
    jpeg_read_scanlines(&cinfo, buffer, 1);
    memcpy(pixels + (dest_row * stride), buffer[0], stride);
    dest_row++;
  }

  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
  fclose(fp);
  *pix = pixels;
  return true;
}

#endif // defined(ROXLU_USE_JPG) && defined(ROXLU_IMPLEMENTATON) 


// ====================================================================================
//                              R O X L U _ U S E _ P N G
// ====================================================================================

#if defined(ROXLU_USE_PNG) && defined(ROXLU_IMPLEMENTATION)

extern bool rx_save_png(std::string filepath, 
                        unsigned char* pixels, 
                        int w, 
                        int h, 
                        int channels = 3) 
{
  
  if(!w || !h) {
    printf("error: cannot save png because the given width and height are invalid: %d x %d\n", w, h);
    return false;
  }
  
  if(!channels || channels > 4) {
    printf("error: cannot save png because the number of color channels is invalid: %d\n", channels);
    return false;
  }
  
  if(!pixels) {
    printf("error: cannot save png because we got an invalid pixels array: %p\n", pixels);
    return false;
  }
  
  if(!filepath.size()) {
    printf("error: cannot save png because the given filepath is invalid.\n");
    return false;
  }
  
  png_structp png_ptr; 
  png_infop info_ptr;
  
  FILE* fp = fopen(filepath.c_str(), "wb");
  if(!fp) {
    printf("error: canont save png because we cannot open the filepath: %s\n", filepath.c_str());
    fp = NULL;
    return false;
  }
  
  png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if(png_ptr == NULL) {
    printf("error: cannot save png because we cannot create a png write struct.\n");
    fclose(fp);
    fp = NULL;
    return false;
  }
  
  info_ptr = png_create_info_struct(png_ptr);
  if(info_ptr == NULL) {
    printf("error: cannot save png brecause we cannot create a png info struct.\n");
    fclose(fp);
    fp = NULL;
    return false;
  }
  
  if(setjmp(png_jmpbuf(png_ptr))) {
    printf("error: cannot save png because we cannot set the jump pointer.\n");
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);
    fp = NULL;
    return false;
  }
  
  png_uint_32 color_type;
  switch(channels) {
    case 1: {
      color_type = PNG_COLOR_TYPE_GRAY; 
      break;
    }
    case 3: {
      color_type = PNG_COLOR_TYPE_RGB;
      break;
    }
    case 4: {
      color_type = PNG_COLOR_TYPE_RGB_ALPHA;
      break;
    }
    default: {
      printf("error: cannot save png because we cannot detect the color type based on the number of channels.\n");
      fclose(fp);
      fp = NULL;
      return false;
    }
  };
  
  png_set_IHDR(png_ptr, 
               info_ptr, 
               w, 
               h, 
               8, 
               color_type, 
               PNG_INTERLACE_NONE, 
               PNG_COMPRESSION_TYPE_DEFAULT, 
               PNG_FILTER_TYPE_DEFAULT);
  
  png_bytep* row_ptrs = new png_bytep[h];
  for(size_t j = 0; j < h; ++j) {
    row_ptrs[j] = pixels + (j * (w * channels));
  }
   
  png_init_io(png_ptr, fp);
  png_set_rows(png_ptr, info_ptr, row_ptrs);
  png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
  
  png_destroy_write_struct(&png_ptr, &info_ptr);
  
  delete[] row_ptrs;
  
  fclose(fp);
  
  return true;
}
  
extern bool rx_load_png(std::string filepath, 
                        unsigned char** pixels,
                        int& width,
                        int& height,
                        int& nchannels)
{
  png_structp png_ptr;
  png_infop info_ptr; 
  
  FILE* fp = fopen(filepath.c_str(), "rb");
  if(!fp) {
    printf("Error: cannot load the png file: %s\n", filepath.c_str());
    fp = NULL;
    return false;
  }
  
  unsigned char sig[8];
  size_t r = 0;
  r = fread(sig, 1, 8, fp);
  if(r != 8) {
    printf("Error: invalid png signature (not enough bytes read) in: %s.\n", filepath.c_str());
    fclose(fp);
    fp = NULL;
    return  false;
  }
  
  if(!png_check_sig(sig, 8)) {
    printf("Error: invalid png signature (wrong siganture) in: %s.\n", filepath.c_str());
    fclose(fp);
    fp = NULL;
    return false;
  }
    
  png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if(!png_ptr) {
    printf("Error: cannot create png read struct: %s\n", filepath.c_str());
    fclose(fp);
    fp = NULL;
    return false;
  }
  
  info_ptr = png_create_info_struct(png_ptr);
  if(!info_ptr) {
    png_destroy_read_struct(&png_ptr, NULL, NULL);
    printf("Error: cannot create png info struct for: %s\n", filepath.c_str());
    fclose(fp);
    fp = NULL;
    return false;
  }
  
#if !defined(_WIN32)
  if(setjmp(png_jmpbuf(png_ptr))) {
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
    fp = NULL;
    return false;
  }
#endif
  
  // @TODO - add option to rescale to 8bit color info or 16bit
  // @TODO - add option to strip the alpha (not recommended in the example)
  png_init_io(png_ptr, fp);
  png_set_sig_bytes(png_ptr, 8);
  png_read_info(png_ptr, info_ptr);
  
  uint32_t stride = 0;
  uint32_t num_bytes = 0;
  uint32_t bit_depth = png_get_bit_depth(png_ptr, info_ptr);
  uint32_t color_type = png_get_color_type(png_ptr, info_ptr);
  width = png_get_image_width(png_ptr, info_ptr);
  height = png_get_image_height(png_ptr, info_ptr);
  nchannels = png_get_channels(png_ptr, info_ptr);
    
  if(width == 0 || height == 0) {
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
    fp = NULL;
    return false;
  }
  
  // @TODO - add option to allow input colors/gray values to be not converted
  switch(color_type) {
    case PNG_COLOR_TYPE_PALETTE: {
      png_set_palette_to_rgb(png_ptr);
      nchannels = 3;
      break;
    }
    case PNG_COLOR_TYPE_GRAY: {
      if(bit_depth < 8) {
        png_set_expand_gray_1_2_4_to_8(png_ptr);
        bit_depth = 8;
      }
      break;
    }
    default:break;
  };
    
  // When transparency is set convert it to a full alpha channel
  if(png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
    png_set_tRNS_to_alpha(png_ptr);
    nchannels += 1;
  }
  
  stride = width * bit_depth * nchannels / 8;  
  num_bytes = width * height * bit_depth * nchannels / 8;
  
  *pixels = new unsigned char[num_bytes];
  if(!pixels) {
    printf("Error: image is to big: %s\n", filepath.c_str());
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
    fp = NULL;
    pixels = NULL;
    return false;
  }
  
  png_bytep* row_ptrs = new png_bytep[height];
  if(!row_ptrs) {
    printf("Error: image is to big: %s\n", filepath.c_str());
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
    fp = NULL;
    delete[] *pixels;
    pixels = 0;
    return false;
  }
  
  for(size_t i = 0; i < height; ++i) {
    row_ptrs[i] = (png_bytep)(*pixels) +(i * stride);
  }
  
  png_read_image(png_ptr, row_ptrs);
  
  delete[] row_ptrs;
  row_ptrs = NULL;
  png_destroy_read_struct(&png_ptr, &info_ptr, 0);
  fclose(fp);
  return true;
}

#endif //  defined(ROXLU_USE_PNG) && defined(ROXLU_IMPLEMENATION)

// ====================================================================================
//                              R O X L U _ U S E _ O P E N G L 
// ====================================================================================

#if defined(ROXLU_USE_OPENGL) && defined(ROXLU_IMPLEMENTATION)

extern void rx_print_shader_link_info(GLuint shader) {
  GLint status = 0;
  GLint count = 0;
  GLchar* error = NULL;
  GLsizei nchars = 0;
  glGetProgramiv(shader, GL_LINK_STATUS, &status);
  if(!status) {
    glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &count);
    if(count > 0) {
      error = new GLchar[count];
      glGetProgramInfoLog(shader, 2048, &nchars, error);
      printf("------\n");
      printf("%s\n", error);
      printf("------\n");
      delete[] error;
      error = NULL;
      assert(0);
    }
  }
}

extern void rx_print_shader_compile_info(GLuint shader) {
  GLint status = 0;
  GLint count = 0;
  GLchar* error = NULL;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if(!status) {
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &count);
    if(count > 0) {
      error = new GLchar[count];
      glGetShaderInfoLog(shader, count, NULL, error);
      printf("------\n");
      printf("%s\n", error);
      printf("------\n");
      delete[] error;
      error = NULL;
      assert(0);
    }
  }
}
  
extern GLuint rx_create_program(GLuint vert, GLuint frag, bool link) {
  GLuint prog = glCreateProgram();
  glAttachShader(prog, vert);
  glAttachShader(prog, frag);

  if(link) {
    glLinkProgram(prog);
    rx_print_shader_link_info(prog);
  }
  return prog;
}
  
extern GLuint rx_create_shader(GLenum type, const char* src) {
  GLuint s = glCreateShader(type);
  glShaderSource(s, 1, &src,  NULL);
  glCompileShader(s);
  rx_print_shader_compile_info(s);
  return s;
}


extern GLuint rx_create_shader_from_file(GLenum type, std::string filepath) {

  std::string source = rx_read_file(filepath);
  if(!source.size()) {
    printf("Error: cannot open the shader source: %s\n", filepath.c_str());
    return -1;
  }
  
  return rx_create_shader(type, source.c_str());
}

extern GLuint rx_create_program_with_attribs(GLuint vert, 
                                             GLuint frag, 
                                             int nattribs, 
                                             const char** attribs) 
{
  GLuint prog = rx_create_program(vert, frag);
  
  if(attribs) {
    for(int i = 0; i < nattribs; ++i) {
      glBindAttribLocation(prog, i, attribs[i]);
    }
  }

  glLinkProgram(prog);
  rx_print_shader_link_info(prog);

  return prog;
}

extern GLint rx_get_uniform_location(GLuint prog, std::string name) {
#if !defined(NDEBUG)
  GLint loc = glGetUniformLocation(prog, name.c_str());
  if(loc < 0) {
    printf("Error: cannot find the uniform: %s\n", name.c_str());
  }
#else
  GLint loc = glGetUniformLocation(prog, name.c_str());
#endif
  
  return loc;
}

extern void rx_uniform_1i(GLuint prog, std::string name, GLint v) {
  glUniform1i(rx_get_uniform_location(prog, name), v);
}

extern void rx_uniform_1f(GLuint prog, std::string name, GLfloat v) {
  glUniform1f(rx_get_uniform_location(prog, name), v);
}

extern void rx_uniform_mat4fv(GLuint prog, 
                              std::string name, 
                              GLsizei count, 
                              GLboolean transpose, 
                              const GLfloat* value) 
{
  glUniformMatrix4fv(rx_get_uniform_location(prog, name), count, transpose, value);
}

Program::~Program() {
  for(std::vector<Shader*>::iterator it = created_shaders.begin(); it != created_shaders.end(); ++it) {
    delete *it;
  }
  shaders.clear();
  created_shaders.clear();
}

Program& Program::add(Shader* s) {

  if(id < 0) {
    id = glCreateProgram();
  }

  s->compile();

  if(s->id < 0) {
    printf("Error: a shader is not compiled yet!\n");
    ::exit(EXIT_FAILURE);
  }

  glAttachShader(id, s->id);
  
  shaders.push_back(s);
  return *this;
}

Program& Program::create(GLenum type, 
                         std::string filepath, 
                         std::string extraSource) 
{
  Shader* s = new Shader();
  s->load(type, filepath, extraSource);
  created_shaders.push_back(s);
  return add(s);
}

Program& Program::link(int nattribs, 
                       const char** atts, 
                       int nfrags, 
                       const char** fraglocs) 
{

  if(id < 0) {
    printf("Error: first add a shader before linking.\n");
    ::exit(EXIT_FAILURE);
  }

  if(nattribs) {
    attribs.clear();
    
    for(int i = 0; i < nattribs; ++i) {
      glBindAttribLocation(id, i, atts[i]);
      attribs.push_back(atts[i]);
    }
  }

  if(nfrags) {
    frags.clear();
    for(int i = 0; i < nfrags; ++i) {
      glBindFragDataLocation(id, i, fraglocs[i]);
      frags.push_back(fraglocs[i]);
    }
  }
  
  glLinkProgram(id);
  rx_print_shader_link_info(id);

  return *this;
}

Program& Program::recompile() {

  if(id < 0) {
    printf("Error: cannot recompile the program because we've not been created yet.\n");
    ::exit(EXIT_FAILURE);
  }

  for(size_t i = 0; i < shaders.size(); ++i) {
    shaders[i]->reload();
    shaders[i]->compile();
  }

  for(size_t i = 0; i < attribs.size(); ++i) {
    glBindAttribLocation(id, i, attribs[i].c_str());
  }

  for(size_t i = 0; i < frags.size(); ++i) {
    glBindFragDataLocation(id, i, frags[i].c_str());
  }

  glLinkProgram(id);
  rx_print_shader_link_info(id); 

  return *this;
}

Shader& Shader::load(GLenum t, 
                     std::string fp, 
                     std::string extraSource)
{

  file_source = rx_read_file(fp);

  if(!file_source.size()) {
    printf("Error: cannot get contents for shader file: %s\n", fp.c_str());
    ::exit(EXIT_FAILURE);
  }

  extra_source = extraSource;
  filepath = fp;
  type = t;
  return *this;
}

Shader& Shader::compile() {

  if(id < 0) {
    id = glCreateShader(type);
  }
  
  if(!file_source.size()) {
    printf("Error: The source of the shader is empty, cannot compile.\n");
    ::exit(EXIT_FAILURE);
  }

  std::string combined_source = extra_source +"\n" +file_source;
  const char* src = combined_source.c_str();

  glShaderSource(id, 1, &src, NULL);
  glCompileShader(id);
  rx_print_shader_compile_info(id);
  return *this;
}

Shader& Shader::reload() {
  return load(type, filepath, extra_source);
}

#endif //  defined(ROXLU_USE_OPENGL) && defined(ROXLU_IMPLEMENTATION)

// ====================================================================================
//                              R O X L U _ U S E _ O P E N G L 
//                              R O X L U _ U S E _ M A T H
// ====================================================================================
#if defined(ROXLU_USE_OPENGL) && defined(ROXLU_USE_MATH) && defined(ROXLU_IMPLEMENTATION)

PainterContextPT::PainterContextPT(Painter& painter)
  :painter(painter)
  ,vao(0)
  ,vbo(0)
  ,vert(0)
  ,frag(0)
  ,prog(0)
  ,allocated(0)
  ,needs_update(false)
{
  vert = rx_create_shader(GL_VERTEX_SHADER, PAINTER_VERTEX_PT_VS);
  frag = rx_create_shader(GL_FRAGMENT_SHADER, PAINTER_VERTEX_PT_SAMPLER2D_FS);
  prog = rx_create_program(vert, frag);
  glLinkProgram(prog);

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  
  glEnableVertexAttribArray(0); // pos
  glEnableVertexAttribArray(1); // tex
  
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPT), (GLvoid*) 0); // pos
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPT), (GLvoid*) 12); // tex

  // bind ubo to binding 0
  glUseProgram(prog);
  GLint block_dx = glGetUniformBlockIndex(prog, "Shared");
  printf("block: %d\n", block_dx);
  glUniformBlockBinding(prog, block_dx, 0);
  glUniform1i(glGetUniformLocation(prog, "u_tex"), 0);
  printf(">> %d\n", glGetUniformLocation(prog, "u_tex"));
}

void PainterContextPT::texture(GLuint tex, float x, float y, float w, float h) {
  
  PainterCommand cmd;
  cmd.type = GL_TEXTURE_2D;
  cmd.count = 6;
  cmd.offset = vertices.size();
  cmd.tex = tex;
  commands.push_back(cmd);
  
  VertexPT a(vec3(x,     y + h, 0.0), vec2(0.0, 1.0)); // bottom left
  VertexPT b(vec3(x + w, y + h, 0.0), vec2(1.0, 1.0)); // bottom right
  VertexPT c(vec3(x + w,     y, 0.0), vec2(1.0, 0.0)); // top right
  VertexPT d(vec3(x,         y, 0.0), vec2(0.0, 0.0)); // top left

  vertices.push_back(a);
  vertices.push_back(b);
  vertices.push_back(c);
  vertices.push_back(a);
  vertices.push_back(c);
  vertices.push_back(d);

  needs_update = true;
}

void PainterContextPT::clear() {
  vertices.clear();
  commands.clear();
}

void PainterContextPT::update() {
  if(!needs_update) {
    return;
  }

  size_t needed = sizeof(VertexPT) * vertices.size();
  if(needed == 0) {
    return;
  }

  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  if(needed > allocated) {
    allocated = needed;
    glBufferData(GL_ARRAY_BUFFER, needed, vertices[0].ptr(), GL_STREAM_DRAW);
  }
  else {
    glBufferSubData(GL_ARRAY_BUFFER, 0, needed, vertices[0].ptr());
  }

  needs_update = false;
}

void PainterContextPT::draw() {

  if(vertices.size() == 0) {
    return;
  }

  glDisable(GL_DEPTH_TEST);
  glUseProgram(prog);
  glBindVertexArray(vao);

  glActiveTexture(GL_TEXTURE0);

  for(std::vector<PainterCommand>::iterator it = commands.begin(); it != commands.end(); ++it) {
    PainterCommand& cmd = *it;
    glBindTexture(cmd.type, cmd.tex);
    glDrawArrays(GL_TRIANGLES, cmd.offset, cmd.count);
  }
}

// -----------------------------------------------------

PainterContextPC::PainterContextPC(Painter& painter) 
  :vao(0)
  ,vbo(0)
  ,vert(0)
  ,frag(0)
  ,prog(0)
  ,painter(painter)
  ,needs_update(true)
  ,allocated(0)
{
  vert = rx_create_shader(GL_VERTEX_SHADER, PAINTER_VERTEX_PC_VS);
  frag = rx_create_shader(GL_FRAGMENT_SHADER, PAINTER_VERTEX_PC_FS);
  prog = rx_create_program(vert, frag);
  glLinkProgram(prog);

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  
  glEnableVertexAttribArray(0); // pos
  glEnableVertexAttribArray(1); // col
  
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPC), (GLvoid*) 0); // pos
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexPC), (GLvoid*) 12); // col

  // bind ubo to binding 0
  glUseProgram(prog);
  GLint block_dx = glGetUniformBlockIndex(prog, "Shared");
  glUniformBlockBinding(prog, block_dx, 0);
}

void PainterContextPC::rect(float x, float y, float w, float h) {

  VertexPC a(vec3(x,     y + h, 0.0), painter.col); // bottom left
  VertexPC b(vec3(x + w, y + h, 0.0), painter.col); // bottom right
  VertexPC c(vec3(x + w,     y, 0.0), painter.col); // top right
  VertexPC d(vec3(x,         y, 0.0), painter.col); // top left

  PainterCommand cmd;
  cmd.offset = vertices.size();

  if(painter.state & PAINTER_STATE_FILL) {

    cmd.type = GL_TRIANGLES;

    vertices.push_back(a);
    vertices.push_back(b);
    vertices.push_back(c);
    vertices.push_back(a);
    vertices.push_back(c);
    vertices.push_back(d);
  }
  else {

    cmd.type = GL_LINE_LOOP;
    vertices.push_back(a);
    vertices.push_back(b);
    vertices.push_back(c);
    vertices.push_back(d);
  }

  cmd.count = vertices.size() - cmd.offset;
  commands.push_back(cmd);
  needs_update = true;
}

void PainterContextPC::clear() {
  vertices.clear();
  commands.clear();
}

void PainterContextPC::circle(float x, float y, float radius) {
  
  PainterCommand cmd;
  cmd.offset = vertices.size();

  if(painter.state & PAINTER_STATE_FILL) {
    cmd.type = GL_TRIANGLE_FAN;
    vertices.push_back(VertexPC(vec3(x, y, 0), painter.col));
  }
  else {
    cmd.type = GL_LINE_STRIP;
  }

  cmd.offset = vertices.size();

  for(std::vector<vec2>::iterator it = painter.circle_data.begin(); it != painter.circle_data.end(); ++it) {
    vec2& v= *it;
    vertices.push_back(VertexPC(vec3(x + v.x * radius, y + v.y * radius, 0), painter.col));
  }

  cmd.count = vertices.size() - cmd.offset;

  commands.push_back(cmd);
  needs_update = true;
}

void PainterContextPC::line(float x0, float y0, float x1, float y1) {
  
  PainterCommand cmd;
  cmd.type = GL_LINES;
  cmd.offset = vertices.size();
  cmd.count = 2;
  commands.push_back(cmd);

  VertexPC a(vec3(x0, y0, 0), painter.col);
  VertexPC b(vec3(x1, y1, 0), painter.col);
  vertices.push_back(a);
  vertices.push_back(b);

  needs_update = true;
}

void PainterContextPC::command(GLenum type, std::vector<VertexPC>& v) {
  
  PainterCommand command;
  command.type = type;
  command.offset = vertices.size();
  command.count = v.size();
  
  std::copy(v.begin(), v.end(), std::back_inserter(vertices));

  commands.push_back(command);

  needs_update = true;
}

void PainterContextPC::update() {

  if(!needs_update) {
    return;
  }

  size_t needed = sizeof(VertexPC) * vertices.size();
  if(needed == 0) {
    return;
  }

  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  if(needed > allocated) {
    allocated = needed;
    glBufferData(GL_ARRAY_BUFFER, needed, vertices[0].ptr(), GL_STREAM_DRAW);
  }
  else {
    glBufferSubData(GL_ARRAY_BUFFER, 0, needed, vertices[0].ptr());
  }

  needs_update = false;
}

void PainterContextPC::draw() {
  
  if(vertices.size() == 0) {
    return;
  }

  glUseProgram(prog);
  glBindVertexArray(vao);

  for(std::vector<PainterCommand>::iterator it = commands.begin(); it != commands.end(); ++it) {
    PainterCommand& cmd = *it;
    glDrawArrays(cmd.type, cmd.offset, cmd.count);
  }
}

// -----------------------------------------------------

Painter::Painter() 
  :context_pc(*this)
  ,context_pt(*this)
  ,state(PAINTER_STATE_NONE)
  ,circle_resolution(8)
  ,ubo(0)
{

  col[0] = 1.0f;
  col[1] = 0.0f;
  col[2] = 0.0f;
  col[3] = 1.0f;

  glGenBuffers(1, &ubo);
  glBindBuffer(GL_UNIFORM_BUFFER, ubo);
  glBufferData(GL_UNIFORM_BUFFER, sizeof(PainterShared), NULL, GL_DYNAMIC_DRAW);
  glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo);

  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);
  resize(viewport[2], viewport[3]);

  resolution(circle_resolution);
}

void Painter::clear() {
  context_pc.clear();
  context_pt.clear();
}

void Painter::resolution(int n) {
  
  float a = TWO_PI / float(n);
  for(int i = 0; i <= n; ++i) {
    circle_data.push_back(vec2(cosf(a * i), sinf(a * i)));
  }
}

void Painter::rect(float x, float y, float w, float h) {
  context_pc.rect(x, y, w, h);
}

void Painter::circle(float x, float y, float radius) {
  context_pc.circle(x, y, radius);
}

void Painter::line(float x0, float y0, float x1, float y1) {
  context_pc.line(x0, y0, x1, y1);
}

void Painter::texture(GLuint tex, float x, float y, float w, float h) {
  context_pt.texture(tex, x, y, w, h);
}

void Painter::color(float r, float g, float b, float a) {

  col[0] = r;
  col[1] = g;
  col[2] = b;
  col[3] = a;
}

void Painter::draw() {

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  context_pc.update();
  context_pc.draw();

  context_pt.update();
  context_pt.draw();
}

void Painter::resize(int w, int h) {

  ubo_data.pm.ortho(0, w, h, 0, 0.0f, 100.0f);

  glBindBuffer(GL_UNIFORM_BUFFER, ubo);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(PainterShared), ubo_data.pm.ptr());

  win_w = w;
  win_h = h;
}

void Painter::fill() {
  state |= PAINTER_STATE_FILL;
}

void Painter::nofill() {
  state &= ~PAINTER_STATE_FILL;
}

void Painter::begin(GLenum type) {
  command_type = type;
}

void Painter::vertex(float x, float y) {
  context_type = PAINTER_CONTEXT_TYPE_PC;
  vertices_pc.push_back(VertexPC(vec3(x, y, 0), col));
}

void Painter::end() {

  if(context_type == PAINTER_CONTEXT_TYPE_PC) {
    context_pc.command(command_type, vertices_pc);
    vertices_pc.clear();
  }
}

int Painter::width() {
  return win_w;
}

int Painter::height() {
  return win_h;
}

#endif // defined(ROXLU_USE_OPENGL) && defined(ROXLU_USE_MATH) && defined(ROXLU_IMPLEMENTATION)

// ====================================================================================
//                              R O X L U _ U S E _ O P E N G L 
//                              R O X L U _ U S E _ P N G
// ====================================================================================

#if defined(ROXLU_USE_OPENGL) && defined(ROXLU_USE_PNG) && defined(ROXLU_IMPLEMENTATION)

extern GLuint rx_create_texture(std::string filepath, 
                                int internalFormat, 
                                int format, 
                                int type)
{

  int w, h, n;
  unsigned char* pix;
 
  if(!rx_load_png(filepath, &pix, w, h, n)) {
    printf("Error: cannot find: %s\n", filepath.c_str());
    ::exit(EXIT_FAILURE);
  }
 
  if(format == -1) {
    switch(n) { 
      case 1: format = GL_RED;  break;
      case 2: format = GL_RG;   break;
      case 3: format = GL_RGB;  break;
      case 4: format = GL_RGBA; break;
      default: {
        printf("Unhandled number of channels for texture :%d\n", n); 
        ::exit(EXIT_FAILURE);
      }
    }
  }

  if(internalFormat == -1) {
    switch(n) { 
      case 1: internalFormat = GL_R8;    break;
      case 2: internalFormat = GL_RG8;   break;
      case 3: internalFormat = GL_RGB8;  break;
      case 4: internalFormat = GL_RGBA8; break;
      default: {
        printf("Unhandled number of channels for texture :%d\n", n); 
        ::exit(EXIT_FAILURE);
      }
    }
  }

  if(type == -1) {
    type = GL_UNSIGNED_BYTE;
  }

  GLuint tex;
  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);
  glTexImage2D(GL_TEXTURE_2D, 0, (GLenum)internalFormat, w, h, 0, (GLenum)format, (GLenum)type, pix);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 
  delete[] pix;
  pix = NULL;
  return tex;
}

#endif // defined(ROXLU_USE_OPENGL) && defined(ROXLU_USE_PNG) && defined(ROXLU_IMPLEMENATION)

#undef ROXLU_IMPLEMENTATION
