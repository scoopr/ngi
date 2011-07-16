
/*
  OpenGL Shader rendering
*/
      
#ifndef _WIN32

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
#include <stdlib.h>
#include <stdarg.h>

#include <math.h>

#ifndef _WIN32
#include <alloca.h>
#endif

#include "typo.h"

static const char* vertex_shader=
"                                                \n"
"attribute vec2 pos;                             \n"
"                                                \n"
"#ifdef TEX                                      \n"
"attribute vec2 texc;                            \n"
"varying   vec2 tc;                              \n"
"#endif                                          \n"
"                                                \n"
"uniform mat4 proj;                              \n"
"void main() {                                   \n"
"                                                \n"
"    #ifdef TEX                                  \n"
"    tc=texc;                                    \n"
"    #endif                                      \n"
"                                                \n"
"    gl_Position = proj * vec4(pos, 0.0, 1.0);   \n"
"}                                               \n";
static const char* fragment_shader=

"                                                \n"
"#ifdef TEX                                      \n"
"varying   vec2 tc;                              \n"
"uniform sampler2D tex;                          \n"
"#else                                           \n"
"                                                \n"
"uniform vec4 color;                             \n"
"#endif                                          \n"
"                                                \n"
"void main() {                                   \n"
"    #ifdef TEX                                  \n"
"    gl_FragColor = texture2D(tex,tc).aaar;          \n"
"    #else                                       \n"
"    gl_FragColor = color;                       \n"
"    #endif                                      \n"
"}                                               \n";


GLuint solid_program;
GLuint tex_program;


float projection[16];


GLuint font_tex;

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


void compile_shader(GLuint* program, const char* opts, const char* vs, const char* fs) {
    GLuint vsh;
    GLuint fsh;
    char err[1024] = {0};

    vsh = glCreateShader(GL_VERTEX_SHADER);
    fsh = glCreateShader(GL_FRAGMENT_SHADER);

    int optsz = strlen(opts);
    
    int vsz[] = { optsz, strlen(vs) };
    int fsz[] = { optsz, strlen(fs) };
    
    const char* vso[] = { opts, vs };
    const char* fso[] = { opts, fs };
    
    glShaderSource(vsh, 2, vso, vsz );
    glShaderSource(fsh, 2, fso, fsz );


    glCompileShader(vsh);

    glGetShaderInfoLog(vsh, 1024, NULL, err);        
    if(err[0] != 0) printf("[vertex shader] %s\n", err);

    glCompileShader(fsh);

    glGetShaderInfoLog(fsh, 1024, NULL, err);        
    if(err[0] != 0) printf("[fragment shader] %s\n", err);

    *program = glCreateProgram();

    glAttachShader(*program, vsh);
    glAttachShader(*program, fsh);


    //    glBindAttribLocation(program, 0, "pos");

    glLinkProgram(*program);

    glValidateProgram(*program);
    glGetProgramInfoLog(*program, 1024, NULL, err);        
    if(err[0] != 0) printf("[link shader] %s\n", err);

}




void render_glsl_init()
{
    int font_width = 512;
    int font_height = 512;

    compile_shader(&solid_program, "\n", vertex_shader, fragment_shader);
    compile_shader(&tex_program, "#define TEX\n", vertex_shader, fragment_shader);

    glGenTextures(1, &font_tex);
    glBindTexture(GL_TEXTURE_2D, font_tex);
        
    char *fontTex = typo_init_texture(13.0, font_width, font_height);


    for(int i = 0; i < font_width*font_height; ++i)
    {
        int v = ((unsigned char*)fontTex)[i];
        v = powf(v/255.0f, 1.0f/1.4f)*255.0f;
        if(v>255) v = 255;
        if(v<0) v = 0;
        fontTex[i] = v;
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, font_width, font_height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, fontTex);

    free(fontTex);

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


    glDisable(GL_BLEND);

    glUseProgram(solid_program);

    float colorf[4] = { color[0]/255.0f, color[1]/255.0f, color[2]/255.0f, color[3]/255.0f };
    glUniform4fv(glGetUniformLocation(solid_program, "color"), 1, colorf);

    glUniformMatrix4fv(glGetUniformLocation(solid_program, "proj"), 1, GL_FALSE, projection);
    
    int pos_loc = glGetAttribLocation(solid_program, "pos");
    glEnableVertexAttribArray(pos_loc);
    glVertexAttribPointer(pos_loc, 2, GL_FLOAT, GL_FALSE, 0, verts );
    glDrawArrays(GL_TRIANGLES, 0, 6);

    
    
}

void render_glsl_text(float x, float y, const char* str, ...) {
    va_list ap;

    int len;
    float *verts;
    float *v;

    char tmp[255];
    char *tmp2 = tmp;

    va_start(ap, str);
    vsnprintf(tmp2, 255, str, ap);
    va_end(ap);

    len = strlen(tmp);
    verts = alloca(len * 24 * sizeof(float));
    v=verts;

    while(*tmp2) {
        
        typo_get_char(*tmp2, &x, &y, v);

        v+=24;
        tmp2++;
    }
    
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    glUseProgram(tex_program);
    
    int pos_loc = glGetAttribLocation(tex_program, "pos");
    int texc_loc = glGetAttribLocation(tex_program, "texc");

    glUniformMatrix4fv(glGetUniformLocation(tex_program, "proj"), 1, GL_FALSE, projection);

    glEnableVertexAttribArray(pos_loc);
    glVertexAttribPointer(pos_loc, 2, GL_FLOAT, GL_FALSE, sizeof(float)*4, verts );

    glEnableVertexAttribArray(texc_loc);
    glVertexAttribPointer(texc_loc, 2, GL_FLOAT, GL_FALSE, sizeof(float)*4, verts +2);

    glDrawArrays(GL_TRIANGLES, 0, len*6);
    
    
}

#endif

