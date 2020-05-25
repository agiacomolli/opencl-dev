#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <CL/cl.h>
#include "utils.h"
#include "add.h"

#define VECTOR_LEN          256000000

void print_res(int *a, int *b, int *c, int len)
{
    int i;

    for (i = 0; i < len; i++)
        printf("%d + %d = %d\n", a[i], b[i], c[i]);
}

int main(int argc, char **argv)
{
    int i;
    int e;
    int *ma;
    int *mb;
    int *mc;
    clock_t t;
    double elapsed;
    struct clparams clp;
    struct addparams addp;

    /* Aloca memória para os vetores. */
    ma = (int *)malloc(sizeof(int) * VECTOR_LEN);
    mb = (int *)malloc(sizeof(int) * VECTOR_LEN);
    mc = (int *)malloc(sizeof(int) * VECTOR_LEN);

    /* Preenche os vetores com valores iniciais. */
    for (i = 0; i < VECTOR_LEN; i++)
        ma[i] = mb[i] = i;

    /* Cria o kernel. */
    e = kernel_create(&clp, "kernel_vect_add.cl", "vector_add");

    /* Cria os buffers para interação com o kernel. */
    e = vect_add_create_buffers(&clp, &addp, VECTOR_LEN);

    /* Operação linear de adição entre os dois vetores. */
    t = clock();
    e = vect_add_linear(ma, mb, mc, VECTOR_LEN);
    if (e) {
        fprintf(stderr, "vect_add error\n");
        exit(e);
    }
    t = clock() - t;
    elapsed = ((double)t)/CLOCKS_PER_SEC;
    printf("vect_add_linear: \t%f s\n", elapsed);

    /* Exibe o resultado. */
    //print_res(ma, mb, mc, VECTOR_LEN);

    /* Operação paralela de adição entre os dois vetores. */
    for (int i = 0; i < 5; i++) {
    t = clock();
    e = vect_add_parallel(&clp, &addp, ma, mb, mc, VECTOR_LEN);
    if (e) {
        fprintf(stderr, "kernel_vect_add error\n");
        exit(e);
    }
    t = clock() - t;
    elapsed = ((double)t)/CLOCKS_PER_SEC;
    printf("vect_add_parallel: \t%f s\n", elapsed);
    }

    /* Exibe o resultado. */
    //print_res(ma, mb, mc, VECTOR_LEN);

    /* Libera memória dos vetores temporários. */
    free(ma);
    free(mb);
    free(mc);

    /* Libera memória dos buffers. */
    vect_add_free(&addp);

    /* Libera memória do kernel. */
    kernel_free(&clp);

    return 0;
}

