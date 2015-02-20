#include <stdint.h>

typedef struct gwidget_s {

} GWidget_S;

typedef struct gwidget_context_s {
    // focus widget

} GWidgetContext_S;

void widgets_init(uint32_t screen_w, uint32_t screen_h);
void widgets_scale(float scale_factor);
void widgets_update();
