

#ifdef NGI_WINDOW_COCOA
#include <OpenGL/gl.h>
#endif

#ifdef NGI_CONTEXT_EGL
#include <GLES2/gl2.h>
#endif

#ifdef NGI_CONTEXT_GLX
#include <GL/gl.h>
#endif

#ifdef NGI_WINDOW_WIN32
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <GL/gl.h>
#endif




void render_opengl_init()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void render_opengl_resize(int w, int h) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,w,0,h,-1,1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0,0,w,h);
}

void render_opengl_clear() {
    glClearColor(0.2, 0.3f, 0.4f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void render_opengl_quad(float x, float y, float w, float h, const unsigned char color[4]) {
    
    float verts[] = {
        x,y,
        x,y+h,
        x+w,y+h,
        x+w,y+h,
        x+w,y,
        x,y
    };
    
    glColor4ubv(color);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, verts);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);

    
    
}
