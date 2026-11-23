#include "logicOps.h"

static int image_is_valid(const Image *img) {
    int i;
    int j;
    pixel_t max_level;

    if (!img || !img->data || img->M <= 0 || img->N <= 0 ||
        img->K < 1 || img->K > IMAGE_MAX_BITS) {
        return 0;
    }

    max_level = image_max_level(img);
    for (i = 0; i < img->M; i++) {
        for (j = 0; j < img->N; j++) {
            if (image_get(img, i, j) > max_level) {
                return 0;
            }
        }
    }

    return 1;
}

static int images_are_compatible(const Image *a, const Image *b) {
    return image_is_valid(a) && image_is_valid(b) &&
           a->M == b->M && a->N == b->N && a->K == b->K;
}

Image *image_and(const Image *a, const Image *b) {
    Image *result;
    int i;
    int j;

    if (!images_are_compatible(a, b)) {
        return NULL;
    }

    result = image_create(a->M, a->N, a->K);
    if (!result) {
        return NULL;
    }

    for (i = 0; i < a->M; i++) {
        for (j = 0; j < a->N; j++) {
            image_set(result, i, j, image_get(a, i, j) & image_get(b, i, j));
        }
    }

    return result;
}

Image *image_or(const Image *a, const Image *b) {
    Image *result;
    int i;
    int j;

    if (!images_are_compatible(a, b)) {
        return NULL;
    }

    result = image_create(a->M, a->N, a->K);
    if (!result) {
        return NULL;
    }

    for (i = 0; i < a->M; i++) {
        for (j = 0; j < a->N; j++) {
            image_set(result, i, j, image_get(a, i, j) | image_get(b, i, j));
        }
    }

    return result;
}

Image *image_not(const Image *img) {
    Image *result;
    pixel_t max_level;
    int i;
    int j;

    if (!image_is_valid(img)) {
        return NULL;
    }

    result = image_create(img->M, img->N, img->K);
    if (!result) {
        return NULL;
    }

    max_level = image_max_level(img);
    for (i = 0; i < img->M; i++) {
        for (j = 0; j < img->N; j++) {
            image_set(result, i, j, (pixel_t)(~image_get(img, i, j)) & max_level);
        }
    }

    return result;
}

Image *image_xor(const Image *a, const Image *b) {
    Image *result;
    int i;
    int j;

    if (!images_are_compatible(a, b)) {
        return NULL;
    }

    result = image_create(a->M, a->N, a->K);
    if (!result) {
        return NULL;
    }

    for (i = 0; i < a->M; i++) {
        for (j = 0; j < a->N; j++) {
            image_set(result, i, j, image_get(a, i, j) ^ image_get(b, i, j));
        }
    }

    return result;
}

Image *image_nand(const Image *a, const Image *b) {
    Image *result;
    pixel_t max_level;
    int i;
    int j;

    if (!images_are_compatible(a, b)) {
        return NULL;
    }

    result = image_create(a->M, a->N, a->K);
    if (!result) {
        return NULL;
    }

    max_level = image_max_level(a);
    for (i = 0; i < a->M; i++) {
        for (j = 0; j < a->N; j++) {
            image_set(result, i, j,
                      (pixel_t)(~(image_get(a, i, j) & image_get(b, i, j))) & max_level);
        }
    }

    return result;
}
