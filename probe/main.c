
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

#define max_last_events  64
ngi_event last_events[max_last_events];
int event_serials[max_last_events];
int cur_last_event = 0;
int num_last_events = 0;
int event_serial = 0;

void draw(int w, int h) {

    float cy;
    float lh = 16*1.2f;
    float left = lh*2;
    int i = 0;

    rend->resize(w,h);
    rend->clear();

    drawCorners(w,h);

    cy = h-lh*2;

    rend->text(left, cy, "NGI Probe");

    cy-=lh*2;

    for(; i < num_last_events && cy > lh*2; ++i)
    {

        int j = (cur_last_event-i-1 + max_last_events)%max_last_events;

        
        const char* str = ngi_event_name(last_events[j].type);
        rend->text(left, cy, "Event %4d: %s", event_serials[j], str);

        cy-=lh;
    }


    
}


struct render_t* guess_renderer(ngi_context* ctx) {
    
    if(render_glsl.is_supported()) return &render_glsl;
    if(render_glff.is_supported()) return &render_glff;
    
    printf("warning: unsupported graphics api: %s\n", ctx->graphics ? ctx->graphics : "NULL" );
    return NULL;

}

int event(ngi_event* ev) {
    
    last_events[cur_last_event] = *ev;
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
        break;
        
        default: break;
    }

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


        draw(win.width, win.height);
        check( ngi_context_swap(&ctx) );

    }


//    check( ngi_application_run(&app) );
    
    printf("[NGI TEST] end\n");
    
    check( ngi_window_deinit(&win) );
    check( ngi_application_deinit(&app) );

    return 0;
}


