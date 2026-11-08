// tests/test_image.c
#include <assert.h>
#include <stdio.h>
#include "image.h"

int main(void) {
    /* création */
    Image *img = image_create(3, 4, 8);
    assert(img != NULL);
    assert(img->M == 3 && img->N == 4 && img->K == 8);
    assert(image_max_level(img) == 255);
    assert(image_get(img, 2, 3) == 0);        /* initialisée à 0 */

    /* lecture / écriture */
    image_set(img, 1, 2, 200);
    assert(image_get(img, 1, 2) == 200);

    /* K différents */
    Image *bin = image_create(2, 2, 1);
    assert(bin != NULL && image_max_level(bin) == 1);
    Image *big = image_create(2, 2, 16);
    assert(big != NULL && image_max_level(big) == 65535);

    /* copie indépendante */
    Image *copy = image_clone(img);
    assert(copy != NULL);
    assert(copy->data != img->data);          /* tableaux distincts */
    assert(image_get(copy, 1, 2) == 200);     /* même contenu */
    image_set(img, 1, 2, 10);
    assert(image_get(copy, 1, 2) == 200);     /* la copie n'a pas bougé */

    /* paramètres invalides */
    assert(image_create(0, 4, 8) == NULL);
    assert(image_create(3, 0, 8) == NULL);
    assert(image_create(3, 4, 0) == NULL);
    assert(image_create(3, 4, 17) == NULL);

    /* free(NULL) doit être permis */
    image_free(NULL);

    image_free(img);
    image_free(copy);
    image_free(bin);
    image_free(big);

    printf("test_image OK\n");
    return 0;
}