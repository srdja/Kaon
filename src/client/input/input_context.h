#ifndef __INPUT_CONTEXT_H__
#define __INPUT_CONTEXT_H__

#include <vector.h>
#include <hashtable.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct input_context_s {
    char name[50];

    uint32_t priority;

    bool is_active;
    
    /* If true all key events will be passed to lower priority
       contexts */
    bool fallthrough;
    
    /* If true only unused events will be passed to lower priority
       contexts */
    bool fallthrough_unused;

    /* A table of tables where the outer table maps to
     * specific keys and the inner table maps to the key
     * + mod */
    HashTable *key_maps;

    /* If true, raw keyboard scan codes will be used
       instead of Unicode ones*/
    bool use_key_scan_codes;

    void (*mouse_motion_cb) (int x, int y);
    void (*mouse_wheel_cb) ();
} InputContext;

/**
 * A context in which all available keys are mapped to a single callback.
 */
typedef struct binding_context_s {
    void (*mouse_cb) ();
    void (*key_cp) ();
    void (*controller_cb) ();
} BindingContext;

/**
 * Button action are actions that are triggered by presssing one or a
 *  combination of buttons on a periferal or a combination of periferals
 **/
typedef struct iaction_s {
    char name[50];
    void (*callback) (void); // FIXME for now
    void (*callback_release) (void);
} ButtonAction;


/**
 * An action map maps keyboard keys, mouse buttons, controller buttons to
 * a callback function. The map can be of only one type of input or of
 * some combination of the above.
 *
 * The callback is invoked only when all the keys are pressed down
 */
typedef struct button_map_s {

    uint32_t key;
    uint8_t  key_state;

    uint16_t mod;

    /* If true the callback will be invoked again when
       the keys are released.
     */
    bool invoke_on_release;

    /**
     * 
     */
    bool invoke_on_repeat;

    /* If this is true and the states are not all down,
       it means that one of the keys was released during
       this update cycle*/
    bool combo_was_down;

    /* The timestamp of the most recent update */
    uint32_t last_ts;

    /* Invoked once all keys are down*/
    ButtonAction *action;

} ButtonMap;


ButtonMap    *input_map_new        (uint32_t key,
                                    uint32_t mod,
                                    bool on_release,
                                    bool on_repeat,
                                    ButtonAction *a);

void          input_map_destroy    (ButtonMap *imap);
void          icontext_destroy     (InputContext *cont);
void          icontext_add_key_map (InputContext *c, ButtonMap *map);

InputContext *icontext_new         (char *name,
                                    bool use_scancodes,
                                    uint32_t p,
                                    bool fallthrough,
                                    bool fallthrough_unused);

ButtonMap    *icontext_get_key_map (InputContext *c, 
                                    uint32_t key,
                                    uint32_t mod);
                                   
ButtonMap    *button_map_new       (uint32_t key, 
                                    uint32_t mod, 
                                    bool on_release, 
                                    bool on_repeat, 
                                    ButtonAction *action);




#endif
