#include "input_context.h"

void           input_init        ();
void           input_destroy     ();
void           input_poll        ();
void           input_add_context (InputContext *context);
InputContext  *input_get_context (char *context_name);
