
#define STBTT_malloc(x,u)  malloc(x)
#define STBTT_free(x,u)    free(x)

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

#include "typo.h"

#include <stdio.h>

stbtt_bakedchar chardata[96];

int typo_width, typo_height;

void* typo_init_texture(float pts, int w, int h)
{
    FILE* fp = fopen("res/Jura-Regular.ttf", "rb");

    assert(fp);

    fseek(fp, 0, SEEK_END);
    long sz = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    assert(sz);

    unsigned char* font_data = malloc(sz);
    fread(font_data, 1, sz, fp);
    fclose(fp);

    unsigned char* texdata = malloc(w * h);

    stbtt_BakeFontBitmap(font_data,0, pts, texdata,w,h, 32,96, chardata);

    

    free(font_data);
    free(texdata);
    
    typo_width = w;
    typo_height = h;

    return texdata;
    
}



void typo_get_char(int codepoint, float *x, float *y, float verts[2*3*2*2]) {

    stbtt_aligned_quad q;
    stbtt_GetBakedQuad(chardata, typo_width,typo_height, codepoint-32, x,y,&q,1);

    if(codepoint < 32 || codepoint > 32+96) {
        codepoint = '.';
    }

    int i = 0;
    
    stbtt_bakedchar* bc = chardata+(codepoint-32);

    // We have bottom-left origin, stbtt assumes top-left, fixup.
    int fixup = -(bc->yoff*2 + (bc->y1 - bc->y0));

    verts[i++] = q.x0;
    verts[i++] = q.y0 + fixup;
    verts[i++] = q.s0;
    verts[i++] = q.t1;

    verts[i++] = q.x1;
    verts[i++] = q.y0 + fixup;
    verts[i++] = q.s1;
    verts[i++] = q.t1;

    verts[i++] = q.x1;
    verts[i++] = q.y1 + fixup;
    verts[i++] = q.s1;
    verts[i++] = q.t0;


    verts[i++] = q.x0;
    verts[i++] = q.y0 + fixup;
    verts[i++] = q.s0;
    verts[i++] = q.t1;

    verts[i++] = q.x1;
    verts[i++] = q.y1 + fixup;
    verts[i++] = q.s1;
    verts[i++] = q.t0;

    verts[i++] = q.x0;
    verts[i++] = q.y1 + fixup;
    verts[i++] = q.s0;
    verts[i++] = q.t0;

        
}

