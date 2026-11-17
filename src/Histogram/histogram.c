#include "Histogram/histogram.h"

#include <stdlib.h>

/*
 * Calcule l'histogramme d'une image en niveaux de gris.
 */
size_t *histogram_compute(const Image *img)
{
    /* Vérification de l'image */
    if (img == NULL || img->data == NULL ||
        img->M <= 0 || img->N <= 0 ||
        img->K < 1 || img->K > IMAGE_MAX_BITS) {
        return NULL;
    }

    /* Nombre de niveaux de gris */
    size_t L = (size_t)1 << img->K;

    /* Allocation et initialisation du tableau */
    size_t *histogram = calloc(L, sizeof(size_t));

    if (histogram == NULL) {
        return NULL;
    }

    /* Parcours de tous les pixels */
    for (int i = 0; i < img->M; i++) {
        for (int j = 0; j < img->N; j++) {

            /* Récupération du niveau de gris */
            pixel_t pixel = image_get(img, i, j);

            /* Incrémentation de l'effectif */
            histogram[pixel]++;
        }
    }

    return histogram;
}