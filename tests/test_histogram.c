#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "image.h"
#include "Histogram/histogram.h"

int main(void)
{
    /* Création d'une image 3x3 sur 8 bits */
    Image *img = image_create(3, 3, 8);

    assert(img != NULL);

    /* Remplissage de l'image */
    pixel_t pixels[3][3] = {
        {10, 20, 10},
        {30, 20, 10},
        {40, 30, 20}
    };

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            image_set(img, i, j, pixels[i][j]);
        }
    }

    /* Calcul de l'histogramme */
    size_t *hist = histogram_compute(img);

    assert(hist != NULL);

    /* Vérification des effectifs */
    assert(hist[10] == 3);
    assert(hist[20] == 3);
    assert(hist[30] == 2);
    assert(hist[40] == 1);

    /* Vérification des autres niveaux */
    assert(hist[0] == 0);
    assert(hist[50] == 0);
    assert(hist[255] == 0);

    /* Vérification du nombre total de pixels */
    size_t total = 0;

    for (int i = 0; i < 256; i++) {
        total += hist[i];
    }

    assert(total == 9);

    /* Libération de la mémoire */
    free(hist);
    image_free(img);

    printf("test_histogram OK\n");

    return 0;
}