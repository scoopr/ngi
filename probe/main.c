
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


typedef struct box_tag {
    float left, right;
    float top, bottom;
    float width, height;
} box_t;

box_t screenBox;
box_t timelineBox;

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
        printf("[NGI TEST] %s OK\n", str);
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

    rend->quad(m, m, sw, lw, white);
    rend->quad(m, m, lw, sw, white);
    
    rend->quad(w-m-sw, m, sw, lw, white);
    rend->quad(w-m-lw, m, lw, sw, white);

    rend->quad(w-m-sw, h-lw-m, sw, lw, white);
    rend->quad(w-m-lw, h-sw-m, lw, sw, white);

    rend->quad(m, h-lw-m, sw, lw, white);
    rend->quad(m, h-sw-m, lw, sw, white);
    
}

#define max_last_events  256
ngi_event last_events[max_last_events];
int event_serials[max_last_events];
double last_events_time[max_last_events];
int cur_last_event = 0;
int num_last_events = 0;
int event_serial = 0;

double last_swap = 0;

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


void layout(int w, int h) {
    
    screenBox.top = h;
    screenBox.bottom = 0;
    screenBox.left = 0;
    screenBox.right = w;
    screenBox.width = w;
    screenBox.height = h;

    box_inside(&screenBox, &timelineBox, 50);
    timelineBox.height = 30;
    box_update_bottom_right(&timelineBox);
    
}

// FIXME:
double ngi_get_time();


unsigned char event_colors[4][4] = {
    { 230, 128, 128, 255 },
    { 128, 230, 128, 255 },
    { 128, 128, 230, 255 },
    { 230, 128, 230, 255 }
};

unsigned char timeline_bg_color[4] = { 32,32,64 };

void draw_timeline() {
    int i;
    
    rend->quad(timelineBox.left, timelineBox.bottom, 
               timelineBox.width, timelineBox.height, timeline_bg_color);
    rend->quad(timelineBox.left, timelineBox.top, timelineBox.width, 1, line_color);
    
    
//    float scale = 1;
    const double scale = 1.0/60.0*50000.0;
    double spacing = 1.0/60.0 * scale;
    
    for(float x = timelineBox.right; x >= timelineBox.left; x-=spacing) {
        rend->quad(x,timelineBox.top,1, -timelineBox.height*.1f, line_color);

    }
    

//    double now = last_events[(cur_last_event-1+max_last_events)%max_last_events].common.timestamp;
    double now = ngi_get_time();
//    double now = last_swap;


    for(; i < num_last_events; ++i)
    {

        int j = (cur_last_event-i-1 + max_last_events)%max_last_events;

        unsigned char *col = event_colors[ last_events[j].type % 4 ];
        double age = now - last_events[j].common.timestamp;
        double latency = age - (now - last_events_time[j]);
        latency*=scale;
        if(latency < 1) latency = 1;
//        const char* str = ngi_event_name(last_events[j].type);
//        rend->text(left, cy, "Event %4d: %s", event_serials[j], str);

        float x = timelineBox.right - age*scale;
        if( x < timelineBox.left ) continue;
        float h = -timelineBox.height/4 * (last_events[j].type*.3f+1);
//        if(last_events[j].type == ngi_redraw_event) h=-h;
        rend->quad(x,timelineBox.top,1, h, col);
 
        rend->quad(x,timelineBox.top+h,latency, 4, col);

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

    draw_timeline();

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

int event(ngi_event* ev) {
    
    last_events[cur_last_event] = *ev;
    last_events_time[cur_last_event] = ngi_get_time();
    event_serials[cur_last_event] = event_serial;
    cur_last_event = (cur_last_event+1)%max_last_events;
    num_last_events++;
    event_serial++;
    if(num_last_events>max_last_events) num_last_events = max_last_events;
    
    printf("[event %s  time:%f]\n", ngi_event_name(ev->type), ev->common.timestamp);
    switch(ev->type) {
        case ngi_key_down_event:
        case ngi_key_up_event:
        printf("<key: %f, %s, %d, %s>\n", ev->common.timestamp, ev->key.keycode, ev->key.down, codepointutf8(ev->key.codepoint));
        if(ev->key.codepoint==27) {
            done = 1;
        }
        break;
        case ngi_text_event:
        printf("<text: %f %s \tU+%04X>\n", ev->common.timestamp, codepointutf8(ev->text.codepoint), ev->text.codepoint);
        
        
        break;
        
        case ngi_redraw_event:
        draw(ev->common.window->width, ev->common.window->height);
        check( ngi_context_swap(ev->common.window->context) );
        last_swap = ngi_get_time();
        break;
        
        default: break;
    }

    ngi_window_redisplay(ev->common.window);

    return 0;
}




int main() {

    
    ngi_application app;
    ngi_window win;
    ngi_context ctx;
    
    
    ngi_config config;
    ngi_config_init(&config);
//    ngi_config_set_string(&config, ngi_config_wm_api, ngi_wm_api_cocoa);
    
    memset(last_events,0,sizeof(ngi_event)*max_last_events);

    

    printf("[NGI TEST] start\n");


    // check( ngi_application_init_xlib(&app) );
    check( ngi_application_init(&app) );

    app.event_callback = event;

    printf("[NGI TEST] wm api: %s\n", app.type);
    
    check( ngi_window_init(&app, &win, &config) );
    
//    printf("[NGI TEST] ngi_window_init: %d\n", succ);




    check( ngi_context_init(&ctx, &win) );

    printf("[NGI TEST] ctx api: %s\n", ctx.type);
    printf("[NGI TEST] gfx api: %s\n", ctx.graphics);


    rend = guess_renderer(&ctx);
    if(!rend) return 0;
    
    // printf("[NGI TEST] ngi_context_*_init: %d\n", succ);

    rend->init();

    draw(win.width, win.height);
    check( ngi_context_swap(&ctx) );


    done = 0;
    while(!done) {
        ngi_application_wait_event(&app);
        ngi_window_redisplay(&win);

//        draw(win.width, win.height);
//        check( ngi_context_swap(&ctx) );

    }


//    check( ngi_application_run(&app) );
    
    printf("[NGI TEST] end\n");
    
    check( ngi_window_deinit(&win) );
    check( ngi_application_deinit(&app) );

    return 0;
}


