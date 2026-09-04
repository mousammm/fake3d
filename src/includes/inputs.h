#ifndef INPUTS_HEADER
#define INPUTS_HEADER

#include "./vectors.h"

void handle_inputs(
    vec3_t* translation, vec3_t* rotation, float* scale,
    int* render_mode, int render_mode_count,
    float delta_time
    );

#endif
