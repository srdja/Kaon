#include <stdint.h>

#include "widget.h"

static uint32_t s_width;
static uint32_t s_height;

static float widget_scale; // ui scale

static void render_widget(GWidget_S *widget);

void widgets_init(uint32_t screen_w, uint32_t screen_h)
{
    s_width  = screen_w;
    s_height = screen_h;

    // init widget collections
}


void widgets_update()
{

}

