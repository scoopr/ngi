
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
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <GL/gl.h>
#define alloca _alloca
#define RENDER_GLFF
#endif

#include "typo.h"
#include <math.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include <alloca.h>

int render_glff_is_supported() {
    #ifdef RENDER_GLFF
    return 1;
    #else
    return 0;
    #endif
}

#ifdef RENDER_GLFF

GLuint font_tex;


void render_glff_init()
{
    int font_width = 512;
    int font_height = 512;
    char *fontTex = NULL;
/*    int i;*/

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    
    
    glGenTextures(1, &font_tex);
    glBindTexture(GL_TEXTURE_2D, font_tex);
        
    fontTex = typo_init_texture(14.0, font_width, font_height);


#if 0
    for(i = 0; i < font_width*font_height; ++i)
    {
        int v = ((unsigned char*)fontTex)[i];
        v = (int)( powf(v/255.0f, 1.0f/1.4f)*255.0f );
        if(v>255) v = 255;
        if(v<0) v = 0;
        fontTex[i] = v;
    }
#endif

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_INTENSITY, font_width, font_height, 0, GL_RED, GL_UNSIGNED_BYTE, fontTex);

    free(fontTex);

    
    
    
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
    glClearColor(0.2f, 0.3f, 0.4f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void render_glff_quad(float coords[2*4], const unsigned char color[4]) {
    
    float verts[] = {
        coords[0], coords[1],
        coords[2], coords[3],
        coords[4], coords[5],
        coords[4], coords[5],
        coords[6], coords[7],
        coords[0], coords[1]
/*
        x,y,
        x,y+h,
        x+w,y+h,
        x+w,y+h,
        x+w,y,
        x,y*/
    };
    
    glColor4ub(color[0], color[1], color[2], color[3] );

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, verts);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);

    
    
}


void render_glff_text(float x, float y, const char* str, ...) {
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

      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, font_tex);
      
      glEnableClientState(GL_VERTEX_ARRAY);
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
      glVertexPointer(2, GL_FLOAT, sizeof(float)*4, verts );
      glTexCoordPointer(2, GL_FLOAT, sizeof(float)*4, verts +2);
      glDrawArrays(GL_TRIANGLES, 0, len*6);
      
      glDisable(GL_BLEND);
      glDisable(GL_TEXTURE_2D);
      
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
