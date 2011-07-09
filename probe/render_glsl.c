
/*
  OpenGL Shader rendering
*/
      

#ifdef NGI_RENDER_API_GLES2
#include <GLES2/gl2.h>
#endif


#ifdef NGI_RENDER_API_OPENGL

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#endif

#include <string.h>
#include <stdio.h>


static const char* vertex_shader=
"                                                \n"
"attribute vec2 pos;                             \n"
"uniform mat4 proj;                              \n"
"void main() {                                   \n"
"   gl_Position = proj * vec4(pos, 0.0, 1.0);    \n"
"}                                               \n";
static const char* fragment_shader=

"                                                \n"
"uniform vec4 color;                             \n"
"                                                \n"
"void main() {                                   \n"
"   gl_FragColor = color;                        \n"
"}                                               \n";


GLuint program;


float projection[16];


void ortho(float left, float right, float bottom, float top, float znear, float zfar) {
    float deltax = right - left;
    float deltay = top - bottom;
    float deltaz = zfar - znear;

    float a = 2.0f / deltax;
    float b = -(right + left) / deltax;
    float c = 2.0f / deltay;
    float d = -(top + bottom) / deltay;
    float e =  -2.0f / deltaz;
    float f = -(zfar + znear) / deltaz;

    memset(projection, 0, sizeof(float)*16);
    projection[0] = a;
    projection[5] = c;
    projection[10] = e;
    projection[12] = b;
    projection[13] = d;
    projection[14] = f;
    projection[15] = 1;
}



void render_glsl_init()
{
    GLuint vsh;
    GLuint fsh;
    char err[1024] = {0};

    vsh = glCreateShader(GL_VERTEX_SHADER);
    fsh = glCreateShader(GL_FRAGMENT_SHADER);

    int vsz = strlen(vertex_shader);
    int fsz = strlen(fragment_shader);
    glShaderSource(vsh, 1, &vertex_shader, &vsz );
    glShaderSource(fsh, 1, &fragment_shader, &fsz );


    glCompileShader(vsh);

    glGetShaderInfoLog(vsh, 1024, NULL, err);        
    if(err[0] != 0) printf("[vertex shader] %s\n", err);
 
    glCompileShader(fsh);

    glGetShaderInfoLog(fsh, 1024, NULL, err);        
    if(err[0] != 0) printf("[fragment shader] %s\n", err);

    program = glCreateProgram();
    
    glAttachShader(program, vsh);
    glAttachShader(program, fsh);


//    glBindAttribLocation(program, 0, "pos");

    glLinkProgram(program);
    
    glValidateProgram(program);
    glGetProgramInfoLog(program, 1024, NULL, err);        
    if(err[0] != 0) printf("[link shader] %s\n", err);
    
}

void render_glsl_resize(int w, int h) {

    ortho(0,w,0,h,-1,1);

    glViewport(0,0,w,h);
}

void render_glsl_clear() {
    glClearColor(0.2, 0.3f, 0.4f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void render_glsl_quad(float x, float y, float w, float h, const unsigned char color[4]) {
    
    
    float verts[] = {
        x,y,
        x,y+h,
        x+w,y+h,
        x+w,y+h,
        x+w,y,
        x,y
    };

    glUseProgram(program);

    float colorf[4] = { color[0]/255.0f, color[1]/255.0f, color[2]/255.0f, color[3]/255.0f };
    glUniform4fv(glGetUniformLocation(program, "color"), 1, colorf);

    glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_FALSE, projection);
    
    int pos_loc = glGetAttribLocation(program, "pos");
    glEnableVertexAttribArray(pos_loc);
    glVertexAttribPointer(pos_loc, 2, GL_FLOAT, GL_FALSE, 0, verts );
    glDrawArrays(GL_TRIANGLES, 0, 6);

    
    
}

