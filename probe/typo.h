#ifndef PROBE_TYPO_H
#define PROBE_TYPO_H



void* typo_init_texture(float pts, int w, int h);
void typo_get_char(int codepoint, float *x, float *y, float verts[2*3*2*2]);



#endif
