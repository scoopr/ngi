#ifndef RENDER_H
#define RENDER_H

#include <math.h>

typedef struct box_tag {
    float left, right;
    float top, bottom;
    float width, height;
} box_t;


typedef int (*render_is_supported)();
typedef void (*render_init)();
typedef void (*render_resize)(int w, int h);
typedef void (*render_clear)();
typedef void (*render_quad)(float coords[2*4], const unsigned char color[4]);
typedef void (*render_text)(float x, float y, const char* str, ...);

struct render_t {
    render_is_supported is_supported;
    render_init init;
    render_resize resize;
    render_clear clear;
    render_quad quad;
    render_text text;
};


inline static void render_rect(struct render_t* r, float x, float y, float w, float h, const unsigned char color[4]) {
    
    float coords[] = {
        x,y,
        x,y+h,
        x+w,y+h,
        x+w,y
    };
    
    r->quad(coords, color);
    
}

inline static void render_box(struct render_t* r, box_t* b, const unsigned char color[4]) {
    
    render_rect(r, b->left, b->bottom, b->width, b->height, color );
    
}

inline static void render_line(struct render_t* r, float x, float y, float x2, float y2, float width, const unsigned char color[4]) {

    float dx = x2-x;
    float dy = y2-y;
    float len = sqrtf(dx*dx + dy*dy);

    float nx = dx/len;
    float ny = dy/len;
    
    float rx = ny;
    float ry = -nx;

    width*=.5f;
    
    float coords[] = {
        x - rx*width, y - ry*width,
        x + rx*width, y + ry*width,
        x2 + rx*width, y2 + ry*width,
        x2 - rx*width, y2 - ry*width
    };
    
    r->quad(coords, color);
    
}




#endif
