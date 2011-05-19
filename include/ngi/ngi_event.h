#ifndef NGI_EVENT_H
#define NGI_EVENT_H

typedef enum ngi_event_type_t {
    ngi_key_event,
    ngi_character_event,
    ngi_mouse_move_event,
    ngi_mouse_button_event,
    ngi_touch_event,
    ngi_scroll_event,
    ngi_resize_event,
    ngi_focus_event,
    ngi_redraw_event,
    ngi_lifecycle_event,
    ngi_orientation_event,
    ngi_custom_event,
} ngi_event_type;


typedef struct ngi_key_t {
    ngi_event_type type;
    double timestamp;

    int down;
    const char* keycode;
    int modifiers;
} ngi_key;

typedef struct ngi_character_t {
    ngi_event_type type;
    double timestamp;

    int codepoint;
    int repeat;
    int modifiers;
} ngi_character;

typedef struct ngi_mouse_move_t {
    ngi_event_type type;
    double timestamp;

    float x,y;
    float dx,dy;
} ngi_mouse_move;

typedef struct ngi_mouse_button_t {
    ngi_event_type type;
    double timestamp;

    float x,y;
    int button;
    int down;
    int repeats;
} ngi_mouse_button;
#if 0
typedef struct ngi_touch_t {
    ngi_event_type type;
    double timestamp;

    float x, y;
    float dx,dy;
    int tid;
} ngi_touch;

typedef struct ngi_scroll_t {
    ngi_event_type type;
    double timestamp;

    float dx,dy,dz;
} ngi_scroll;

typedef struct ngi_resize_t {
    ngi_event_type type;
    double timestamp;

    int width, height;
} ngi_resize;

typedef struct ngi_focus_t {
    ngi_event_type type;
    double timestamp;

    int gain;
} ngi_focus;


typedef struct ngi_custom_t {
    ngi_event_type type;
    double timestamp;

    const char* type;
    void* data;
} ngi_custom;


typedef enum ngi_lifecycle_state_t {
    ngi_lifecycle_quit_request,
    ngi_lifecycle_terminate,
    ngi_lifecycle_pause,
    ngi_lifecycle_resume,
} ngi_lifecycle_state;

typedef struct ngi_lifecycle_event_t {
    ngi_event_type type;
    double timestamp;
    
    ngi_lifecycle_state state;
    
} ngi_lifecycle;

typedef struct ngi_orientation_t {
    int direction;
} ngi_orientation;
#endif


typedef union ngi_event_tag {

    // As this is an union, this needs to be first on every struct as well.
    ngi_event_type type;
    
    ngi_key key;
    ngi_character character;
    ngi_mouse_move mouse_move;
    ngi_mouse_button mouse_button;
#if 0
    ngi_touch touch;
    ngi_scroll scroll;
    ngi_resize resize;
    ngi_focus focus;
    ngi_lifecycle lifecycle;
    ngi_orientation orientation;
    ngi_custom custom;
#endif
    
} ngi_event;



typedef int (*ngi_event_cb)(ngi_event* ev);


#endif