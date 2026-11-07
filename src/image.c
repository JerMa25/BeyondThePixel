#include "image.h"

#include <stdlib.h>
#include <string.h>

Image *image_create(int M, int N, int K) {
    if (M <= 0 || N <= 0 || K < 1 || K > IMAGE_MAX_BITS) {
        return NULL;
    }

    Image *img = malloc(sizeof *img);
    if (!img) {
        return NULL;
    }

    img->M = M;
    img->N = N;
    img->K = K;
    img->data = calloc(M * N, sizeof(pixel_t));
    if (!img->data) {
        free(img);
        return NULL;
    }

    return img;
}

void image_free(Image *img) {
    if (!img) 
        return;
    free(img->data);
    free(img);
}

Image *image_clone(const Image *img) {
    if (!img) {
        return NULL;
    }

    Image *copy = image_create(img->M, img->N, img->K);
    if (!copy) {
        return NULL;
    }

    memcpy(copy->data, img->data, (size_t)img->M * (size_t)img->N * sizeof(pixel_t));
    return copy;
}