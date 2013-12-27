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

  Usage:
  ------
  #define ROXLU_USE_ALL              - to include all code
  #define ROXLU_USE_OPENGL           - to use the opengl code
  #define ROXLU_USE_PNG              - to use the png loader and saver (needs libpng)
  #define ROXLU_USE_MATH             - to use the vec2, vec3, vec4, mat4 classes


  SHADER - define `ROXLU_USE_OPENGL` before including
  ===================================================================================
  vert = rx_create_shader(GL_VERTEX_SHADER, source_char_p);    - create a shader, pass type
  prog = rx_create_program(vert, frag);                        - create a problem - DOES NOT LINK
  rx_print_shader_link_info(prog)                              - print the program link info
  rx_print_shader_compile_info(vert)                           - print the shader compile info


  IMAGES - define `ROXLU_USE_PNG` before including
  ===================================================================================
  rx_save_png("filename.png", pixels, 640, 480, 3);            - writes a png using lib png
  rx_load_png("filepath.png", &pix, width, height, nchannels)  - load the pixels, width, height and nchannels for the given filepath. make sure to delete pix (which is unsigned char*)


  UTILS
  ===================================================================================
  rx_get_exe_path();                  - returns the path to the exe 
  rx_read_file("filepath.txt");       - returns the contents of the filepath.
  rx_to_float("0.15");                - convert a string to float 
  rx_to_int("10");                    - convert a string to integer
  rx_to_data_path("filename.txt")     - convert the given filename to the data dir
  rx_is_dir("path")                   - returns true when the path is a dir


  MATH - define `ROXLU_USE_MATH` before including. 
  ===================================================================================

      utils
      ------------------------------------------------------------------------------
      float rx_random(max)      - generate a random value but limit to max
      float rx_random(min, max) - generate a random value between min and max
      
      vec2, vec3, vec4
      ------------------------------------------------------------------------------
      float length(v)         - get the length of the vector
      float dot(a, b)         - get the dot product aka squared root
      vec2 max(a)             - get the biggest component value
      vec2 min(a)             - get the lowest component value
      vec2 max(a,b)           - get the biggest vector
      vec2 nmin(a,b)          - get the smallest vector
      vec2 floor(a)           - floor the components
      vec2 ceil(a)            - ceil the compoments
      vec2 fract(a)           - get the decimal part
      vec2 normalized(v)      - get the normalized vector
      void print()            - print the x and y 
      vec3 cross(a,b)         - cross product (vec3)

      vec3
      -----------------------------------------------------------------------------
      vec3 perpendicular(a)   - get a perpendicular vector from the given vec, this vector doesn't have to be normalized!, based on http://lolengine.net/blog/2013/09/21/picking-orthogonal-vector-combing-coconuts 
    
      mat4
      ------------------------------------------------------------------------------
      mat4& mat4.rotateX(rad)
      mat4& mat4.rotateY(rad)
      mat4& mat4.rotateZ(rad)
      mat4& mat4.rotate(rad, x, y, z)
      mat4& mat4.scale(x, y, z)
      mat4& mat4.translate(x, y, z)
      mat4& mat4.translate(vec3 v)
      mat4& mat4.ortho(l, r, b, t, n , f)
      mat4& mat4.frustum(l, r, b, t, n, f)
      mat4& mat4.perspective(fov, aspect, near, far)
      void  mat4.print()
      float* mat4.ptr()                                    - get a pointer to the data
    
      <examples>
      // create an ortho matrix with 0,0 at the top left
      mat4 m;
      m.ortho(0.0f, w, h, 0, 0.0f, 100.0f);
      </examples>

*/

#ifndef ROXLU_TINYLIB_H
#define ROXLU_TINYLIB_H

#if defined(ROXLU_USE_ALL)
#  define ROXLU_USE_OPENGL
#  define ROXLU_USE_PNG
#  define ROXLU_USE_MATH
#endif

#include <assert.h>
#include <iostream>
#include <cmath>
#include <iterator>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>

#if defined(_WIN32)
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
#elif defined(__linux)
#  include <string.h>                               /* strlen() */
#  include <dirent.h>                               /* stat() */
#  include <errno.h>                                /* errno */
#  include <unistd.h>                               /* readlink(), getcwd() */
#  include <sys/time.h>                             /* timeofday() */
#  include <libgen.h>                               /* dirname() */
#  include <sys/stat.h>
#  include <stdarg.h>
#  include <GLXW/glxw.h>
#  define MAX_PATH 4096
#endif

#include <math.h>

#if defined(ROXLU_USE_PNG)
#  include <png.h>
#endif

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

#ifndef MIN
#  define MIN(x,y) (((x) < (y)) ? (x) : (y))
#endif

#ifndef MAX
#  define MAX(x,y) (((x) > (y)) ? (x) : (y))
#endif

#ifndef CLAMP
#  define CLAMP(val,min,max) (MAX(MIN(val,max),min))
#endif

#ifndef ABS
#  define ABS(x) (((x) < 0) ? -(x) : (x))
#endif

#ifndef DX
#  define DX(i,j,w)((j)*(w))+(i)
#endif

#if defined(ROXLU_USE_OPENGL)
static void rx_print_shader_link_info(GLuint shader) {
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
  
static void rx_print_shader_compile_info(GLuint shader) {
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
  
static GLuint rx_create_program(GLuint vert, GLuint frag) {
  GLuint prog = glCreateProgram();
  glAttachShader(prog, vert);
  glAttachShader(prog, frag);
  return prog;
}
  
static GLuint rx_create_shader(GLenum type, const char* src) {
  GLuint s = glCreateShader(type);
  glShaderSource(s, 1, &src,  NULL);
  glCompileShader(s);
  rx_print_shader_compile_info(s);
  return s;
}
#endif // ROXLU_USE_OPENGL

// UTILS
// ---------------------------------------------------------------------------
#if defined(_WIN32) // rx_get_exe_path()
static std::string rx_get_exe_path() {
  char buffer[MAX_PATH];

  // Try to get the executable path with a buffer of MAX_PATH characters.
  DWORD result = ::GetModuleFileNameA(nullptr, buffer, static_cast<DWORD>(MAX_PATH));
  if(result == 0) {
    return "";
  }

  std::string::size_type pos = std::string(buffer).find_last_of( "\\/" );

  return std::string(buffer).substr(0, pos) +"\\";
}
#elif defined(__APPLE__) // rx_get_exe_path()
static std::string rx_get_exe_path() {
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
#elif defined(__linux) // rx_get_exe_path()
static std::string rx_get_exe_path() {
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
#endif // rx_get_exe_path()

static bool rx_is_dir(std::string filepath) {
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

static std::string rx_to_data_path(const std::string filename) {
  std::string exepath = rx_get_exe_path();

#if defined(__APPLE__)
  if(rx_is_dir(exepath +"data")) {
    exepath += "data/" +filename;
  }
  else if(rx_is_dir(exepath +"../MacOS")) {
    exepath += "../../../data/" +filename;
  }
#else 
  exepath += "data/" +filename;
#endif  

  return exepath;
}

static int rx_to_int(const std::string& v) {
  int r = 0;
  std::stringstream ss(v);
  ss >> r;
  return r;
}

static float rx_to_float(const std::string& v) {
  float r = 0.0f;
  std::stringstream ss(v);
  ss >> r;
  return r;
}


#if defined(ROXLU_USE_PNG)
// write an w*h array of pixels to a png file
static bool rx_save_png(std::string filepath, unsigned char* pixels, int w, int h, int channels = 3) {
  
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
  
static bool rx_load_png(std::string filepath, 
                        unsigned char** pixels,
                        uint32_t& width,
                        uint32_t& height,
                        uint32_t& nchannels
)
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
  
 static std::string rx_read_file(std::string filepath) {
  std::ifstream ifs(filepath.c_str(), std::ios::in);
  if(!ifs.is_open()) {
  return "";
}
  std::string str((std::istreambuf_iterator<char>(ifs)) , std::istreambuf_iterator<char>());
  return str;
}
#endif // ROXLU_USE_PNG

 // MATH
 // -------------------------------------------------------------------------
#if defined(ROXLU_USE_MATH)

 template<class T>
   class Vec2 {

 public:
 Vec2() : x(), y() {}
 Vec2(T x, T y) : x(x), y(y) {}
 Vec2(const Vec2<T>& o) : x(o.x), y(o.y) {}
 Vec2(T f) : x(f), y(f) {}

  void set(T vx, T vy) { x = vx; y = vy; } 
  T* ptr() { return &x; }
  T& operator [](const unsigned int dx) { return *(&x + dx); } 
  
  Vec2<T> operator + () const { return Vec2<T>(+x, +y); };
  Vec2<T> operator - () const { return Vec2<T>(-x, -y); };
  Vec2<T> operator + (const Vec2<T>& o) const { return Vec2<T>(x + o.x, y + o.y); } 
  Vec2<T> operator - (const Vec2<T>& o) const { return Vec2<T>(x - o.x, y - o.y); } 
  Vec2<T> operator * (const Vec2<T>& o) const { return Vec2<T>(x * o.x, y * o.y); } 
  Vec2<T> operator / (const Vec2<T>& o) const { return Vec2<T>(x / o.x, y / o.y); } 
  Vec2<T> operator + (float s) const { return Vec2<T>(x + s, y + s); } 
  Vec2<T> operator - (float s) const { return Vec2<T>(x - s, y - s); } 
  Vec2<T> operator * (float s) const { return Vec2<T>(x * s, y * s); } 
  Vec2<T> operator / (float s) const { return Vec2<T>(x / s, y / s); } 

  friend Vec2<T> operator + (float s, const Vec2<T>& o) { return Vec2<T>(s + o.x, s + o.y); } 
  friend Vec2<T> operator - (float s, const Vec2<T>& o) { return Vec2<T>(s - o.x, s - o.y); } 
  friend Vec2<T> operator * (float s, const Vec2<T>& o) { return Vec2<T>(s * o.x, s * o.y); } 
  friend Vec2<T> operator / (float s, const Vec2<T>& o) { return Vec2<T>(s / o.x, s / o.y); } 

  Vec2<T>& operator += (const Vec2<T>& o) { return *this = *this + o; }
  Vec2<T>& operator -= (const Vec2<T>& o) { return *this = *this - o; }
  Vec2<T>& operator *= (const Vec2<T>& o) { return *this = *this * o; }
  Vec2<T>& operator /= (const Vec2<T>& o) { return *this = *this / o; }
  Vec2<T>& operator += (float s) { return *this = *this + s; } 
  Vec2<T>& operator -= (float s) { return *this = *this - s; } 
  Vec2<T>& operator *= (float s) { return *this = *this * s; } 
  Vec2<T>& operator /= (float s) { return *this = *this / s; } 

  bool operator == (const Vec2<T>& o) const { return x == o.x && y == o.y; } 
  bool operator != (const Vec2<T>& o) const { return x != o.x || y == o.y; } 

  friend float length(const Vec2<T>& o) { return sqrtf(o.x * o.x + o.y * o.y); } 
  friend float dot(const Vec2<T> &a, const Vec2<T> &b) { return a.x * b.x + a.y * b.y; }
  friend float max(const Vec2<T> &v) { return fmaxf(v.x, v.y); }
  friend float min(const Vec2<T> &v) { return fminf(v.x, v.y); }
  friend Vec2<T> max(const Vec2<T> &a, const Vec2<T> &b) { return Vec2<T>(fmaxf(a.x, b.x), fmaxf(a.y, b.y)); }
  friend Vec2<T> min(const Vec2<T> &a, const Vec2<T> &b) { return Vec2<T>(fminf(a.x, b.x), fminf(a.y, b.y)); }
  friend Vec2<T> floor(const Vec2<T> &v) { return Vec2<T>(floorf(v.x), floorf(v.y)); }
  friend Vec2<T> ceil(const Vec2<T> &v) { return Vec2<T>(ceilf(v.x), ceilf(v.y)); }
  friend Vec2<T> abs(const Vec2<T> &v) { return Vec2<T>(fabsf(v.x), fabsf(v.y)); }
  friend Vec2<T> fract(const Vec2<T> &v) { return v - floor(v); }
  friend Vec2<T> normalized(const Vec2<T> &v) { return v / length(v); }

  void print() { printf("x: %f, y: %f\n", x, y); } 

 public:
  T x, y;
 };


 // ----------------------------------------------------------------------------------

 template<class T>
   class Vec3 {

 public:
 Vec3() : x(), y(), z() {}
 Vec3(T x, T y, T z) : x(x), y(y), z(z) {}
 Vec3(const Vec3<T>& o) : x(o.x), y(o.y), z(o.z) {}
 Vec3(T f) : x(f), y(f), z(f) {}

   void set(const float xv, const float yv, const float zv) { x = xv; y = yv; z = zv; }
   T* ptr() { return &x; }
   T& operator [](const unsigned int dx) { return *(&x + dx); } 
  
   Vec3<T> operator + () const { return Vec3<T>(+x, +y, +z); };
   Vec3<T> operator - () const { return Vec3<T>(-x, -y, -z); };
   Vec3<T> operator + (const Vec3<T>& o) const { return Vec3<T>(x + o.x, y + o.y, z + o.z); } 
   Vec3<T> operator - (const Vec3<T>& o) const { return Vec3<T>(x - o.x, y - o.y, z - o.z); } 
   Vec3<T> operator * (const Vec3<T>& o) const { return Vec3<T>(x * o.x, y * o.y, z * o.z); } 
   Vec3<T> operator / (const Vec3<T>& o) const { return Vec3<T>(x / o.x, y / o.y, z / o.z); } 
   Vec3<T> operator + (float s) const { return Vec3<T>(x + s, y + s, z + s); } 
   Vec3<T> operator - (float s) const { return Vec3<T>(x - s, y - s, z - s); } 
   Vec3<T> operator * (float s) const { return Vec3<T>(x * s, y * s, z * s); } 
   Vec3<T> operator / (float s) const { return Vec3<T>(x / s, y / s, z / s); } 

   friend Vec3<T> operator + (float s, const Vec3<T>& o) { return Vec3<T>(s + o.x, s + o.y, s + o.z); } 
   friend Vec3<T> operator - (float s, const Vec3<T>& o) { return Vec3<T>(s - o.x, s - o.y, s - o.z); } 
   friend Vec3<T> operator * (float s, const Vec3<T>& o) { return Vec3<T>(s * o.x, s * o.y, s * o.z); } 
   friend Vec3<T> operator / (float s, const Vec3<T>& o) { return Vec3<T>(s / o.x, s / o.y, s / o.z); } 

   Vec3<T>& operator += (const Vec3<T>& o) { return *this = *this + o; }
   Vec3<T>& operator -= (const Vec3<T>& o) { return *this = *this - o; }
   Vec3<T>& operator *= (const Vec3<T>& o) { return *this = *this * o; }
   Vec3<T>& operator /= (const Vec3<T>& o) { return *this = *this / o; }
   Vec3<T>& operator += (float s) { return *this = *this + s; } 
   Vec3<T>& operator -= (float s) { return *this = *this - s; } 
   Vec3<T>& operator *= (float s) { return *this = *this * s; } 
   Vec3<T>& operator /= (float s) { return *this = *this / s; } 

   bool operator == (const Vec3<T>& o) const { return x == o.x && y == o.y && z == o.z; } 
   bool operator != (const Vec3<T>& o) const { return x != o.x || y == o.y || z == o.z; } 

   friend float length(const Vec3<T>& o) { return sqrtf(o.x * o.x + o.y * o.y + o.z * o.z); } 
   friend float dot(const Vec3<T> &a, const Vec3<T> &b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
   friend float max(const Vec3<T> &v) { return fmaxf(fmaxf(v.x, v.y), v.z); }
   friend float min(const Vec3<T> &v) { return fminf(fminf(v.x, v.y), v.z); }
   friend Vec3<T> max(const Vec3<T> &a, const Vec3<T> &b) { return Vec3<T>(fmaxf(a.x, b.x), fmaxf(a.y, b.y), fmaxf(a.z, b.z)); }
   friend Vec3<T> min(const Vec3<T> &a, const Vec3<T> &b) { return Vec3<T>(fminf(a.x, b.x), fminf(a.y, b.y), fminf(a.z, b.z)); }
   friend Vec3<T> floor(const Vec3<T> &v) { return Vec3<T>(floorf(v.x), floorf(v.y), floorf(v.z)); }
   friend Vec3<T> ceil(const Vec3<T> &v) { return Vec3<T>(ceilf(v.x), ceilf(v.y), ceilf(v.z)); }
   friend Vec3<T> abs(const Vec3<T> &v) { return Vec3<T>(fabsf(v.x), fabsf(v.y), fabsf(v.z)); }
   friend Vec3<T> fract(const Vec3<T> &v) { return v - floor(v); }
   friend Vec3<T> normalized(const Vec3<T> &v) { return v / length(v); }
   friend Vec3<T> cross(const Vec3<T> &a, const Vec3<T> &b) { return Vec3<T>(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); }
   friend Vec3<T> perpendicular(const Vec3<T>& v) {  return abs(v.x) > abs(v.z) ? Vec3(-v.y, v.x, 0.0) : Vec3(0.0, -v.z, v.y); }

   void print() { printf("x: %f, y: %f, z: %f\n", x, y, z); } 

 public:
   T x, y, z;
 };

 // ----------------------------------------------------------------------------------

 template<class T>
   class Vec4 {

 public:
 Vec4() : x(), y(), z(), w() {}
 Vec4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
 Vec4(const Vec4<T>& o) : x(o.x), y(o.y), z(o.z), w(o.w) {}
 Vec4(T f) : x(f), y(f), z(f), w(f) {}

   T* ptr() { return &x; }
   T& operator [](const unsigned int dx) { return *(&x + dx); } 
  
   Vec4<T> operator + () const { return Vec4<T>(+x, +y, +z, +w); };
   Vec4<T> operator - () const { return Vec4<T>(-x, -y, -z, -w); };
   Vec4<T> operator + (const Vec4<T>& o) const { return Vec4<T>(x + o.x, y + o.y, z + o.z, w + o.w); } 
   Vec4<T> operator - (const Vec4<T>& o) const { return Vec4<T>(x - o.x, y - o.y, z - o.z, w - o.w); } 
   Vec4<T> operator * (const Vec4<T>& o) const { return Vec4<T>(x * o.x, y * o.y, z * o.z, w * o.w); } 
   Vec4<T> operator / (const Vec4<T>& o) const { return Vec4<T>(x / o.x, y / o.y, z / o.z, w / o.w); } 
   Vec4<T> operator + (float s) const { return Vec4<T>(x + s, y + s, z + s, w + s); } 
   Vec4<T> operator - (float s) const { return Vec4<T>(x - s, y - s, z - s, w - s); } 
   Vec4<T> operator * (float s) const { return Vec4<T>(x * s, y * s, z * s, w * s); } 
   Vec4<T> operator / (float s) const { return Vec4<T>(x / s, y / s, z / s, w / s); } 

   friend Vec4<T> operator + (float s, const Vec4<T>& o) { return Vec4<T>(s + o.x, s + o.y, s + o.z, s + o.w); } 
   friend Vec4<T> operator - (float s, const Vec4<T>& o) { return Vec4<T>(s - o.x, s - o.y, s - o.z, s - o.w); } 
   friend Vec4<T> operator * (float s, const Vec4<T>& o) { return Vec4<T>(s * o.x, s * o.y, s * o.z, s * o.w); } 
   friend Vec4<T> operator / (float s, const Vec4<T>& o) { return Vec4<T>(s / o.x, s / o.y, s / o.z, s / o.w); } 

   Vec4<T>& operator += (const Vec4<T>& o) { return *this = *this + o; }
   Vec4<T>& operator -= (const Vec4<T>& o) { return *this = *this - o; }
   Vec4<T>& operator *= (const Vec4<T>& o) { return *this = *this * o; }
   Vec4<T>& operator /= (const Vec4<T>& o) { return *this = *this / o; }
   Vec4<T>& operator += (float s) { return *this = *this + s; } 
   Vec4<T>& operator -= (float s) { return *this = *this - s; } 
   Vec4<T>& operator *= (float s) { return *this = *this * s; } 
   Vec4<T>& operator /= (float s) { return *this = *this / s; } 

   bool operator == (const Vec4<T>& o) const { return x == o.x && y == o.y && z == o.z && w == o.w; } 
   bool operator != (const Vec4<T>& o) const { return x != o.x || y == o.y || z == o.z || w == o.w; } 

   friend float length(const Vec4<T>& o) { return sqrtf(o.x * o.x + o.y * o.y + o.z * o.z + o.w * o.w); } 
   friend float dot(const Vec4<T> &a, const Vec4<T> &b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * a.w; }
   friend float max(const Vec4<T> &v) { return fmaxf(fmaxf(v.x, v.y), fmaxf(v.z, v.w)); }
   friend float min(const Vec4<T> &v) { return fminf(fminf(v.x, v.y), fminf(v.z, v.w)); }
   friend Vec4<T> max(const Vec4<T> &a, const Vec4<T> &b) { return Vec4<T>(fmaxf(a.x, b.x), fmaxf(a.y, b.y), fmaxf(a.z, b.z), fmaxf(a.w, b.w)); }
   friend Vec4<T> min(const Vec4<T> &a, const Vec4<T> &b) { return Vec4<T>(fminf(a.x, b.x), fminf(a.y, b.y), fminf(a.z, b.z), fminf(a.w, b.w)); }
   friend Vec4<T> floor(const Vec4<T> &v) { return Vec4<T>(floorf(v.x), floorf(v.y), floorf(v.z), floorf(v.w)); }
   friend Vec4<T> ceil(const Vec4<T> &v) { return Vec4<T>(ceilf(v.x), ceilf(v.y), ceilf(v.z), ceilf(v.w)); }
   friend Vec4<T> abs(const Vec4<T> &v) { return Vec4<T>(fabsf(v.x), fabsf(v.y), fabsf(v.z), fabsf(v.w)); }
   friend Vec4<T> fract(const Vec4<T> &v) { return v - floor(v); }
   friend Vec4<T> normalized(const Vec4<T> &v) { return v / length(v); }

   void print() { printf("x: %f, y: %f, z: %f, w: %f\n", x, y, z, w); } 

 public:
   T x, y, z, w;
 };

 // ----------------------------------------------------------------------------------

 template<class T>
   class Matrix4 {
 public:
   Matrix4();

   Matrix4<T>& rotateX(T rad);
   Matrix4<T>& rotateY(T rad);
   Matrix4<T>& rotateZ(T rad);
   Matrix4<T>& rotate(T rad, T x, T y, T z);
   Matrix4<T>& rotate(T rad, const Vec3<T>& v) { return rotate(rad, v.x, v.y, v.z); } 
   Matrix4<T>& translate(T x, T y, T z);
   Matrix4<T>& translate(const Vec3<T>& v) { return translate(v.x, v.y, v.z); } 
   Matrix4<T>& scale(T x, T y, T z);
   Matrix4<T>& perspective(T fovDegrees, T aspect, T n, T f);
   Matrix4<T>& ortho(T l, T r, T b, T t, T n, T f);
   Matrix4<T>& frustum(T l, T r, T b, T t, T n, T f);
   Matrix4<T>& identity();
   Matrix4<T>& lookAt(Vec3<T> pos, Vec3<T> target, Vec3<T> up);
   T* ptr() { return &m[0]; } 

   static Matrix4<T> rotation(T rad, T x, T y, T z);

   Matrix4<T>& operator *=(const Matrix4<T>& o);
   Matrix4<T> operator * (const Matrix4<T>& o) const;
   T& operator [] (const unsigned int dx) { return m[dx]; } 

   void print();
 public:
   T m[16];
 };

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
   Matrix4<T>& Matrix4<T>::scale(T x, T y, T z) {
   m[0] *= x; m[4] *= y;  m[8]  *= z;
   m[1] *= x; m[5] *= y;  m[9]  *= z;
   m[2] *= x; m[6] *= y;  m[10] *= z;
   m[3] *= x; m[6] *= y;  m[11] *= z;
   return *this;
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
Matrix4<T>& Matrix4<T>::lookAt(Vec3<T> pos, Vec3<T> target, Vec3<T> up) {

  Vec3<T> ax_z = normalized(pos - target);
  Vec3<T> ax_x = normalized(cross(ax_z, up));
  Vec3<T> ax_y = cross(ax_z, ax_x);

  m[0] = ax_x.x;  m[4] = ax_x.y;  m[8] = ax_x.z;   
  m[1] = ax_y.x;  m[5] = ax_y.y;  m[9] = ax_y.z;   
  m[2] = ax_z.x;  m[6] = ax_z.y;  m[10] = ax_z.z;
  
  translate(-pos);
  return *this ;
}

 typedef Matrix4<float> mat4;
 typedef Vec4<float> vec4;
 typedef Vec3<float> vec3;
 typedef Vec2<float> vec2;


 static float rx_random(float max) {
   return max * rand() / (RAND_MAX + 1.0f);
 }

 static float rx_random(float x, float y) {
   float high = 0;
   float low = 0;
   float result = 0;
        
   high = std::max<float>(x,y);
   low = std::min<float>(x,y);
   result = low + ((high-low) * rand()/(RAND_MAX + 1.0));
   return result;
 }

 // PERLIN
 // ----------------------------------------------------------------------------
 /*

  # Perlin

  Perlin noise, thanks to Ken P. 
  This class is based on: http://www.flipcode.com/archives/Perlin_Noise_Class.shtml 
  with some super tiny changes. Thanks guys!

  octaves: use a value between 1 - 16, 1 = smooth, 16 = noisy, values between 4 - 8 give conventional noise results
  freq:    use a value between 1 - 8, which will give reasanoble results (you can use any value you want)
  ampl:    a value of 1, will result in values between -1 and 1
  seed:    random seed, eg. 94

 */

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
  
};

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

#define PERLIN_SETUP(i, b0, b1, r0, r1) \
  t = vec[i] + PERLIN_N; \
  b0 = ((int)t) & PERLIN_BM; \
  b1 = (b0+1) & PERLIN_BM; \
  r0 = t - (int)t; \
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


#endif // ROXLU_USE_MATH


 // ----------------------------------------------------------------------------
#endif
