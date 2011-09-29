
/*
  OpenGL Fixed function
*/

#ifdef NGI_RENDER_API_GLES1
#include <GLES/gl.h>
#define RENDER_GLFF
#endif

#ifdef NGI_RENDER_API_OPENGL

#ifdef __APPLE__
#include <OpenGL/gl.h>
#define RENDER_GLFF
#else
#include <GL/gl.h>
#define RENDER_GLFF
#endif

#endif

#ifdef NGI_WINDOW_WIN32
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <GL/gl.h>
#define RENDER_GLFF
#endif

int render_glff_is_supported() {
    #ifdef RENDER_GLFF
    return 1;
    #else
    return 0;
    #endif
}

#ifdef RENDER_GLFF

void render_glff_init()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void render_glff_resize(int w, int h) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    #ifdef NGI_RENDER_API_GLES1
    glOrthof(0,w,0,h,-1,1);
    #else
    glOrtho(0,w,0,h,-1,1);
    #endif
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0,0,w,h);
}

void render_glff_clear() {
    glClearColor(0.2, 0.3f, 0.4f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void render_glff_quad(float x, float y, float w, float h, const unsigned char color[4]) {
    
    float verts[] = {
        x,y,
        x,y+h,
        x+w,y+h,
        x+w,y+h,
        x+w,y,
        x,y
    };
    
    glColor4ub(color[0], color[1], color[2], color[3] );

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, verts);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);

    
    
}


void render_glff_text(float x, float y, const char* str, ...) {
    (void)x;
    (void)y;
    (void)str;
    
}


#else

void render_glff_init()
{
}

void render_glff_resize(int w, int h) {
    (void)w;
    (void)h;
}

void render_glff_clear() {
}

void render_glff_quad(float x, float y, float w, float h, const unsigned char color[4]) {
    (void)x;
    (void)y;
    (void)w;
    (void)h;
    (void)color;
}


void render_glff_text(float x, float y, const char* str, ...) {
    (void)x;
    (void)y;
    (void)str;
}



#endif


extern int dummy;
