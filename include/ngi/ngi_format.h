#ifndef NGI_FORMAT_H
#define NGI_FORMAT_H

#ifdef __cplusplus
extern "C" {
#endif


typedef struct ngi_format_tag {
    
    union {
        struct {
            void* format;
        } cocoa;
        struct {
            void* fbc;
            void* visual;
        } xlib;
        struct {
            unsigned int pixelformat;
            char pfd[40];
        } wgl;
    } platform;
    
    
} ngi_format;




int ngi_format_init(ngi_application* app, ngi_format* format, ngi_config *config);
int ngi_format_deinit(ngi_format* format);

int ngi_format_init_cocoa(ngi_application* app, ngi_format* format, ngi_config *config);
int ngi_format_deinit_cocoa(ngi_format* format);

int ngi_format_init_glx(ngi_application* app, ngi_format* format, ngi_config *config);

int ngi_format_init_win32(ngi_application* app, ngi_format* format, ngi_config *config);

int ngi_format_init_ios(ngi_application* app, ngi_format* format, ngi_config *config);

#ifdef __cplusplus
}
#endif



#endif
