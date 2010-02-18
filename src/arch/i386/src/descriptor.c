
#include <arch/descriptor.h>
#include <arch/misc.h>

/**
 * Initialize the given descriptor as the given type. For data/code segments v
 * sets the access flags for the segment. For 16 and 32 bit call gates, v is the
 * parameter count of the gate. All other types ignore the value.
 */
void init_descriptor(union descriptor* desc, enum desc_type_t type, unsigned char v) {
    /* Zero out entire structure */
    desc->words[0] = 0;
    desc->words[1] = 0;

    switch(type) {
    case DESC_SEGMENT:
        desc->segment._h0 = 1;
        desc->segment.opsize = DESC_32BIT; /* Default to 32 bit code/data */
        desc->segment.type = v;
        break;

    case DESC_TASK_STATE_SEGMENT:
        break;

    case DESC_CALL_GATE16:
    case DESC_CALL_GATE32:
        desc->gate.param_count = v;

    case DESC_TASK_GATE:
    case DESC_TRAP_GATE16:
    case DESC_TRAP_GATE32:
    case DESC_INTERRUPT_GATE16:
    case DESC_INTERRUPT_GATE32:
        /* Gates have a common initialization */
        desc->gate.type = type;
        break;

    default:
        // -
        break;
    }
}
