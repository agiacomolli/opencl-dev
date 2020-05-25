#ifndef UTILS_H
#define UTILS_H

#include <CL/cl.h>

struct clparams {
    cl_kernel kernel;
    cl_context context;
    cl_program program;
    cl_device_id device_id;
};

extern int kernel_create(struct clparams *params, char *source_strm,
        char *kernel_name);

extern void kernel_free(struct clparams *params);

#endif /* UTILS_H */

