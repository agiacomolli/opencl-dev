#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include "utils.h"

int kernel_create(struct clparams *params, char *source_str, char *kernel_name)
{
    FILE *fp;
    size_t source_size;
    cl_int ret;
    cl_int err;
    cl_uint num_devices;
    cl_uint num_platforms;

    /* Abre o arquivo com o código do kernel. */
    fp = fopen(source_str, "r");
    if (fp == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo fonte do kernel\n");
        return 1;
    }

    /* Lê o conteúdo do arquivo e armazena nas variáveis. */
    source_str = (char *)malloc(1024);
    source_size = fread(source_str, 1, 1024, fp);

    /* Obtém o número de plataformas disponíveis. */
    ret = clGetPlatformIDs(0, NULL, &num_platforms);
    if (ret != CL_SUCCESS) {
        fprintf(stderr, "Erro ao obter os id da plataforma\n");
        return 1;
    }

    if (num_platforms == 0) {
        fprintf(stderr, "Nenhuma plataforma disponivel\n");
        return 1;
    }

    /* Declara um vetor para armazenar os ids das plataformas. */
    cl_platform_id platform[num_platforms];

    /* Obtém novamente as informações das plataformas. */
    ret = clGetPlatformIDs(num_platforms, platform, NULL);
    if (ret != CL_SUCCESS) {
        fprintf(stderr, "Erro ao obter os id da plataforma\n");
        return 1;
    }

    printf("OpenCL plataformas: %d\n", num_platforms);
    for (int i = 0; i < num_platforms; i++) {
        cl_char name[255];
        err = clGetPlatformInfo(platform[i], CL_PLATFORM_NAME, sizeof(name), &name, NULL);
        printf("Platform %d: %s\n", i, name);
    }

    ret = clGetDeviceIDs(platform[0], CL_DEVICE_TYPE_ALL, 1, &params->device_id,
            &num_devices);
    if (ret != CL_SUCCESS) {
        fprintf(stderr, "Erro ao obter o id do dispositivo\n");
        return 1;
    }

    params->context = clCreateContext(NULL, 1, &params->device_id, NULL, NULL, &ret);
    if (ret != CL_SUCCESS) {
        fprintf(stderr, "Erro ao criar um novo contexto\n");
        return 1;
    }

    /* Cria um programa a partir do código do kernel. */
    params->program = clCreateProgramWithSource(params->context, 1,
            (const char **)&source_str, (const size_t *)&source_size, &ret);
    if (ret != CL_SUCCESS) {
        fprintf(stderr, "Erro ao criar o programa do kernel\n");
        return 1;
    }

    /* Compila o programa do kernel. */
    ret = clBuildProgram(params->program, 1, &params->device_id, NULL, NULL, NULL);
    if (ret != CL_SUCCESS) {
        fprintf(stderr, "Erro ao compilar o programa do kernel\n");
        return 1;
    }

    /* Cria o kernel OpenCL. */
    params->kernel = clCreateKernel(params->program, kernel_name, &ret);
    if (ret != CL_SUCCESS) {
        fprintf(stderr, "Erro ao criar o kernel OpenCL\n");
        return 1;
    }
}

void kernel_free(struct clparams *params)
{
    clReleaseKernel(params->kernel);
    clReleaseProgram(params->program);
    clReleaseContext(params->context);
}

