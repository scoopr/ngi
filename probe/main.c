
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#define snprintf _snprintf
#endif

#include <stdio.h>
#include <stdlib.h>

#include "ngi/ngi.h"



#include "../../wtf8/wtf8.h"

#include "render.h"
#include "render_glff.h"
#include "render_glsl.h"

#include "typo.h"



box_t screenBox;
box_t timelineBox;
box_t tachoBox;

char* codepointhex(int cp) {
    static char out[4*2];
    unsigned char* in = (unsigned char*)&cp;
    snprintf(out,8,"%2x%2x%2x%2x", in[0], in[1], in[2], in[3]);
    return out;
}

char* codepointutf8(unsigned int cp) {
    static char out[8];

    memset(out,0,8);
    
    wtf8_encode(cp, out);

    return out;
}

void check_(int succ, const char* str) {
    if(succ) {
//        printf("[NGI TEST] %s OK\n", str);
    } else {
        printf("[NGI TEST] %s FAIL\n", str);
        exit(1);
    }
}



#define check(x) check_( (x), #x)
int done;

const unsigned char white[]={255,255,255,255};

struct render_t* rend = NULL;

void drawCorners(int w, int h) {

    float m = 5;
    float sw = 5;
    float lw = 20;

    render_rect(rend, m, m, sw, lw, white);
    render_rect(rend, m, m, lw, sw, white);
    
    render_rect(rend, w-m-sw, m, sw, lw, white);
    render_rect(rend, w-m-lw, m, lw, sw, white);

    render_rect(rend, w-m-sw, h-lw-m, sw, lw, white);
    render_rect(rend, w-m-lw, h-sw-m, lw, sw, white);

    render_rect(rend, m, h-lw-m, sw, lw, white);
    render_rect(rend, m, h-sw-m, lw, sw, white);
    
}

#define max_last_events  256
ngi_event last_events[max_last_events];
int event_serials[max_last_events];
double last_events_time[max_last_events];
int cur_last_event = 0;
int num_last_events = 0;
int event_serial = 0;

#define max_frametimes 256
int cur_frametime = 0;
int last_frametime = 0;
double frametimes[max_frametimes];



const unsigned char line_color[]={255,255,255,255};

void box_update_size(box_t* b) {
    b->width = b->right - b->left;
    b->height = b->top - b->bottom;
}

void box_update_bottom_right(box_t* b) {
    b->bottom = b->top - b->height;
    b->right = b->left + b->width;
}

void box_inside(box_t* outside, box_t* inside, int margin) {
    inside->top = outside->top - margin;
    inside->bottom = outside->bottom + margin;
    inside->left = outside->left + margin;
    inside->right = outside->right - margin;
    box_update_size(inside);
}

double get_spf() {
    return 1.0/ 60.0;
//    return 1.0/51.56;
}


void layout(int w, int h) {
    
    screenBox.top = (float)h;
    screenBox.bottom = 0.0f;
    screenBox.left = 0.0f;
    screenBox.right = (float)w;
    screenBox.width = (float)w;
    screenBox.height = (float)h;

    box_inside(&screenBox, &timelineBox, 50);
    timelineBox.height = 30;
    box_update_bottom_right(&timelineBox);
    
    box_inside(&screenBox, &tachoBox, 50);
    tachoBox.top = timelineBox.bottom - 5;
    tachoBox.left += tachoBox.width/2;
    tachoBox.bottom = tachoBox.top - (tachoBox.right - tachoBox.left);
    box_update_size(&tachoBox);
    
    
}

// FIXME:
double ngi_get_time();


unsigned char event_colors[7][4] = {
    { 230, 128, 128, 255 },
    { 128, 230, 128, 255 },
    { 230, 230, 128, 255 },
    { 230, 128, 230, 255 },
    { 128, 128, 230, 255 },
    { 128, 230, 230, 255 },
    { 230, 230, 230, 255 }
};

unsigned char timeline_bg_color[4] = { 32,32,64 };

unsigned char* event_color(int type) {
    return event_colors[ type % 7 ];
}

void draw_pointers() {
    
    int first = 1;
    
    float lx = 0, ly = 0;
    int i;
    
    for(i=0; i < num_last_events; ++i)
     {
         int j = (cur_last_event-i-1 + max_last_events)%max_last_events;
         float x,y;
         int pointer = 0;
         int type = last_events[j].type;
		 float w;

         if( !last_events[j].common.window ) continue;
         if( type == ngi_event_mouse_move ) {
             pointer = 1;
             x = last_events[j].mouse_move.x;
             y = screenBox.top-last_events[j].mouse_move.y;
         }

 /*        if( type == ngi_event_mouse_button ) {
             x = last_events[j].mouse_button.x;
             y = last_events[j].mouse_button.y;
             render_rect(rend, x-3, y-3, 7, 7, event_color(type));
         }
*/
         if(!pointer) continue;
         
         if(first) {
             lx = x;
             ly = y;
             first = 0;
             continue;
         }


         w = 1.0f + 2*last_events[j].mouse_move.drag;
         render_line(rend, lx, ly, x, y, w, event_color(type));
//         render_rect(rend, x-(w), y-(w), w*2+1, w*2+1, event_color(type));
         
         
         lx = x;
         ly = y;
         
         
     }
}

void draw_timeline() {
    int i;
    
    const double scale = get_spf()*50000.0;
    double spacing = get_spf() * scale;
	double x;

	//    double now = last_events[(cur_last_event-1+max_last_events)%max_last_events].common.timestamp;
//    double now = frametimes[last_frametime];
    double now = ngi_get_time();

	render_rect(rend, timelineBox.left, timelineBox.bottom, 
               timelineBox.width, timelineBox.height, timeline_bg_color);
    render_rect(rend, timelineBox.left, timelineBox.top, timelineBox.width, 1, line_color);
    
    
//    float scale = 1;
    
    for(x = timelineBox.right; x >= timelineBox.left; x-=spacing) {
        render_rect(rend, (float)x, timelineBox.top, 1, -timelineBox.height*.1f, line_color);

    }
    



    for(i=0; i < num_last_events; ++i)
    {

        int j = (cur_last_event-i-1 + max_last_events)%max_last_events;

        unsigned char *col = event_color(last_events[j].type);
        double age = now - last_events[j].common.timestamp;
        
        double latency = age - (now - last_events_time[j]);
        float x = timelineBox.right - (float)(age*scale);
        float h = -timelineBox.height/4 * (last_events[j].type*.3f+1);

		latency*=scale;
        if(latency < 1) latency = 1;
//        const char* str = ngi_event_name(last_events[j].type);
//        rend->text(left, cy, "Event %4d: %s", event_serials[j], str);

        if( x < timelineBox.left ) continue;
//        if(last_events[j].type == ngi_redraw_event) h=-h;
        render_rect(rend, x, timelineBox.top,1, h, col);
 
        render_rect(rend, x, timelineBox.top+h, (float)latency, 4, col);

    }

    
}


void draw_tachometer() {
    

    const unsigned char col1[] = { 180,180,255,255};
    const unsigned char colbg[] = { 16,16,20,255};


    float innerRadius = tachoBox.width * .3f;
    float outerRadius = tachoBox.width * .35f;
    float centerx = tachoBox.left + tachoBox.width/2;
    float centery = tachoBox.bottom + tachoBox.height/2;

//    double spf = 1.0/60.0;
//    double spf = 1.0/51.56 ;
//    double spf = 1.0/10.0;
//    double spf = 1.0/60.0;
    double spf = get_spf();


    
    double offset = frametimes[last_frametime] ; //fmod(,spf);
//    double offset = frametimes[last_frametime]  - fmod(frametimes[last_frametime],spf);

    const int tachoframes = 6;
    double max_age = frametimes[last_frametime] - spf * (tachoframes-1);

	int i;

	render_box(rend, &tachoBox, colbg);

	for(i = 0; i < tachoframes; ++i)
    {
        float angle = -i * 3.14159f * 2.0f / tachoframes ;
        float ax = cosf(angle);
        float ay = sinf(angle);
        render_line(rend, centerx + ax * innerRadius, centery + ay * innerRadius,
                          centerx + ax * outerRadius, centery + ay * outerRadius, 2, col1);
    }


    for(i = 0; i < max_frametimes; ++i)
    {
        int j = (i + cur_frametime) % max_frametimes;
        double t = frametimes[ j ] - offset;

        int lum = i*255/max_frametimes;
        unsigned char col[]= { lum, lum>>1, lum>>1, 255};
        float r = -(1.0f-(i/(float)max_frametimes)) * 15 + 5;
//        if( j <= last_frametime && j > (last_frametime-tachoframes+max_frametimes)%max_frametimes) {
        float angle = (float)(-fmod(t, spf * tachoframes ) / (spf*tachoframes) * 3.14159 * 2.0);
        float ax = cosf(angle);
        float ay = sinf(angle);

        if( frametimes[j] > max_age ) {
            r=15.0f;
        } else {
            lum = lum >>1;
        }
        if(j == last_frametime) memcpy(col, white ,4);;


        render_line(rend, centerx + ax * (outerRadius+r), centery + ay * (outerRadius+r),
                          centerx + ax * (outerRadius+5+r), centery + ay * (outerRadius+5+r), 3, col);
        
    }




    
}



void draw(int w, int h) {

    float cy;
    float lh = 16*1.2f;
    float left = lh*2;
//    int i = 0;

    layout(w,h);
    

    rend->resize(w,h);
    rend->clear();

    draw_pointers();
    draw_timeline();

    draw_tachometer();

    drawCorners(w,h);

    cy = h-lh*2;

    rend->text(left, cy, "NGI Probe");

    cy-=lh*2;
#if 0
    for(; i < num_last_events && cy > lh*2; ++i)
    {

        int j = (cur_last_event-i-1 + max_last_events)%max_last_events;

        
        const char* str = ngi_event_name(last_events[j].type);
        rend->text(left, cy, "Event %4d: %s", event_serials[j], str);

        cy-=lh;
    }
#endif

    
}


struct render_t* guess_renderer(ngi_context* ctx) {
    
    if(render_glsl.is_supported()) return &render_glsl;
    if(render_glff.is_supported()) return &render_glff;
    
    printf("warning: unsupported graphics api: %s\n", ctx->graphics ? ctx->graphics : "NULL" );
    return NULL;

}

struct probe_window
{
    ngi_window win;
    ngi_context ctx;
    ngi_format fmt;
};

struct probe_window main_window;

int event(ngi_event* ev) {

    double now = ngi_get_time();
    
    last_events[cur_last_event] = *ev;
    last_events_time[cur_last_event] = now;
    event_serials[cur_last_event] = event_serial;
    cur_last_event = (cur_last_event+1)%max_last_events;
    num_last_events++;
    event_serial++;
    if(num_last_events>max_last_events) num_last_events = max_last_events;
    
//    printf("[event %s  time:%f]\n", ngi_event_name(ev->type), ev->common.timestamp);
    switch(ev->type) {

        case ngi_event_application_init:
        {
            check( ngi_format_init(ev->application_init.application, &main_window.fmt, NULL) );
            check( ngi_window_init(ev->application_init.application, &main_window.win, &main_window.fmt, NULL) );
            check( ngi_context_init(&main_window.ctx, &main_window.win, &main_window.fmt) );

            ngi_context_set_active(&main_window.ctx);
            rend = guess_renderer(&main_window.ctx);
            rend->init();
            ngi_window_animate(&main_window.win, 1);

        }
        break;

        case ngi_event_key_down:
        case ngi_event_key_up:
//        printf("<key: %f, %s, %d, %s>\n", ev->common.timestamp, ev->key.keycode, ev->key.down, codepointutf8(ev->key.codepoint));
        if(ev->key.codepoint==27) {
            ngi_application_quit();
        }
        break;
        case ngi_event_text:
 //       printf("<text: %f %s \tU+%04X>\n", ev->common.timestamp, codepointutf8(ev->text.codepoint), ev->text.codepoint);
        
        
        break;
        
        case ngi_event_redraw:
        if(!rend) return 0; 
        draw(ev->common.window->width, ev->common.window->height);
        check( ngi_context_swap(ev->common.window->context) );
        frametimes[cur_frametime] = ngi_get_time();

        last_frametime = cur_frametime;
        cur_frametime = (cur_frametime+1)%max_frametimes;
        
        break;
        
        default: break;
    }

    if(ev->common.window) ngi_window_redisplay(ev->common.window);


    return 0;
}



int main(int argc, char* argv[]) {
    return ngi_run(argc, argv, event);
}


