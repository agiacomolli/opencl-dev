#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include "utils.h"
#include "add.h"

int vect_add_linear(const int *a, const int *b, int *c, int len)
{
    int i;

    for (i = 0; i < len; i++)
        c[i] = a[i] + b[i];

    return 0;
}

int vect_add_create_buffers(struct clparams *clp, struct addparams *addp, int len)
{
    cl_int ret;

    /* Cria a fila de comandos. */
    addp->q = clCreateCommandQueue(clp->context, clp->device_id, 0, &ret);
    if (ret != CL_SUCCESS) {
        fprintf(stderr, "Erro ao criar a fila de comandos\n");
        return 1;
    }

    /* Cria os buffers para acesso à memória do dispositivo. */
    addp->a = clCreateBuffer(clp->context, CL_MEM_READ_ONLY,
            len * sizeof(int), NULL, &ret);
    if (ret != CL_SUCCESS) {
        fprintf(stderr, "Erro ao criar o buffer a\n");
        return 1;
    }

    addp->b = clCreateBuffer(clp->context, CL_MEM_READ_ONLY,
            len * sizeof(int), NULL, &ret);
    if (ret != CL_SUCCESS) {
        fprintf(stderr, "Erro ao criar o buffer b\n");
        return 1;
    }

    addp->c = clCreateBuffer(clp->context, CL_MEM_READ_ONLY,
            len * sizeof(int), NULL, &ret);
    if (ret != CL_SUCCESS) {
        fprintf(stderr, "Erro ao criar o buffer c\n");
        return 1;
    }
}

int vect_add_parallel(struct clparams *clp, struct addparams *addp,
        const int *a, const int *b, int *c, int len)
{
    cl_int ret;

    /* Copia os dados dos vetores a e b para os buffers do dispositivo. */
    ret = clEnqueueWriteBuffer(addp->q, addp->a, CL_TRUE, 0,
            len * sizeof(int), a, 0, NULL, NULL);
    if (ret != CL_SUCCESS) {
        fprintf(stderr, "Erro ao copiar os dados para o buffer a\n");
        return 1;
    }

    ret = clEnqueueWriteBuffer(addp->q, addp->b, CL_TRUE, 0,
            len * sizeof(int), b, 0, NULL, NULL);
    if (ret != CL_SUCCESS) {
        fprintf(stderr, "Erro ao copiar os dados para o buffer b\n");
        return 1;
    }

    /* Define os argumentos para o kernel. */
    ret = clSetKernelArg(clp->kernel, 0, sizeof(cl_mem), (void *)&addp->a);
    ret = clSetKernelArg(clp->kernel, 1, sizeof(cl_mem), (void *)&addp->b);
    ret = clSetKernelArg(clp->kernel, 2, sizeof(cl_mem), (void *)&addp->c);

    /* Executa o kernel OpenCL. */
    size_t global_item_size = len;
    size_t local_item_size = 128;
    ret = clEnqueueNDRangeKernel(addp->q, clp->kernel, 1, NULL,
           &global_item_size, &local_item_size, 0, NULL, NULL);
    if (ret != CL_SUCCESS) {
        fprintf(stderr, "Erro ao executar o kernel OpenCL: %d\n", ret);
        return 1;
    }

    /* Lê o resultado da operação, contido no buffer c. */
    ret = clEnqueueReadBuffer(addp->q, addp->c, CL_TRUE, 0,
            len * sizeof(int), c, 0, NULL, NULL);
    if (ret != CL_SUCCESS) {
        fprintf(stderr, "Erro ao ler o resultado da operacao\n");
        return 1;
    }
}

int vect_add_free(struct addparams *addp)
{
    /* Libera memória dos recursos alocados. */
    clFlush(addp->q);
    clFinish(addp->q);
    clReleaseMemObject(addp->a);
    clReleaseMemObject(addp->b);
    clReleaseMemObject(addp->c);
    clReleaseCommandQueue(addp->q);
}

