#ifndef ADD_H
#define ADD_H

#include <CL/cl.h>

struct addparams {
    cl_mem a;
    cl_mem b;
    cl_mem c;
    cl_command_queue q;
};

extern int vect_add_linear(const int *a, const int *b, int *c, int len);

extern int vect_add_linear_multistage(const int *a, const int *b, int *c, int len);

extern int vect_add_create_buffers(struct clparams *clp, struct addparams *addp, int len);

extern int vect_add_parallel(struct clparams *clp, struct addparams *addp,
        const int *a, const int *b, int *c, int len);

extern int vect_add_parallel_multistage(struct clparams *clp, struct addparams *addp,
        const int *a, const int *b, int *c, int len);

extern int vect_add_free(struct addparams *addp);

#endif /* ADD_H */

