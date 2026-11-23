#include <assert.h>

#include "ImgOperations/LogicOperations/logicOps.h"

static Image *create_first_image(void) {
    Image *img = image_create(2, 2, 4);

    assert(img != NULL);
    image_set(img, 0, 0, 0xC);
    image_set(img, 0, 1, 0x5);
    image_set(img, 1, 0, 0x0);
    image_set(img, 1, 1, 0xF);
    return img;
}

static Image *create_second_image(void) {
    Image *img = image_create(2, 2, 4);

    assert(img != NULL);
    image_set(img, 0, 0, 0xA);
    image_set(img, 0, 1, 0x3);
    image_set(img, 1, 0, 0xF);
    image_set(img, 1, 1, 0x1);
    return img;
}

static void assert_pixels(const Image *img, pixel_t p00, pixel_t p01,
                          pixel_t p10, pixel_t p11) {
    assert(img != NULL);
    assert(image_get(img, 0, 0) == p00);
    assert(image_get(img, 0, 1) == p01);
    assert(image_get(img, 1, 0) == p10);
    assert(image_get(img, 1, 1) == p11);
}

int main(void) {
    Image *a = create_first_image();
    Image *b = create_second_image();
    Image *incompatible = image_create(1, 2, 4);
    Image *result;

    assert(incompatible != NULL);

    result = image_and(a, b);
    assert_pixels(result, 0x8, 0x1, 0x0, 0x1);
    image_free(result);

    result = image_or(a, b);
    assert_pixels(result, 0xE, 0x7, 0xF, 0xF);
    image_free(result);

    result = image_xor(a, b);
    assert_pixels(result, 0x6, 0x6, 0xF, 0xE);
    image_free(result);

    result = image_not(a);
    assert_pixels(result, 0x3, 0xA, 0xF, 0x0);
    image_free(result);

    result = image_nand(a, b);
    assert_pixels(result, 0x7, 0xE, 0xF, 0xE);
    image_free(result);

    assert(image_and(NULL, b) == NULL);
    assert(image_or(a, NULL) == NULL);
    assert(image_xor(a, incompatible) == NULL);
    assert(image_nand(a, incompatible) == NULL);
    assert(image_not(NULL) == NULL);

    assert_pixels(a, 0xC, 0x5, 0x0, 0xF);
    assert_pixels(b, 0xA, 0x3, 0xF, 0x1);

    image_free(incompatible);
    image_free(b);
    image_free(a);
    return 0;
}
