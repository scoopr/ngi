#ifndef NGI_FORMAT_H
#define NGI_FORMAT_H



typedef struct ngi_format_tag {
    
    union {
        struct {
            void* format;
        } cocoa;
        struct {
            void* fbc;
            void* visual;
        } xlib;
    } platform;
    
    
} ngi_format;




int ngi_format_init(ngi_application* app, ngi_format* format, ngi_config *config);

int ngi_format_init_cocoa(ngi_application* app, ngi_format* format, ngi_config *config);

int ngi_format_init_glx(ngi_application* app, ngi_format* format, ngi_config *config);



#endif
