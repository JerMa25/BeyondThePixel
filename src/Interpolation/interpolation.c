#include "interpolation.h"

#include <stdlib.h>
#include <math.h>

Image *interpolation_ppv(const Image *img, int new_M, int new_N) {
    /* Interpolation par plus proche voisin */

    if (img == NULL || new_M <= 0 || new_N <= 0)
        return NULL;

    // Calcul des pas pour l'interpolation
    double pas_M = (new_M == 1 || img->M == 1) 
                    ? 0.0 : (double)img->M / new_M;
    double pas_N = (new_N == 1 || img->N == 1) 
                    ? 0.0 : (double)img->N / new_N;
    
    // Image de sortie
    Image *new_img = image_create(new_M, new_N, img->K);

    if (new_img == NULL) {
        return NULL;
    }

    for (int i = 0; i < new_M; i++) {
        for (int j = 0; j < new_N; j++) {
            // Calcul des indices du pixel le plus proche dans l'image d'origine
            int orig_i = (int)round(i * pas_M);
            int orig_j = (int)round(j * pas_N);

            // Récupération de la valeur du pixel et assignation à la nouvelle image
            pixel_t pixel_value = image_get(img, orig_i, orig_j);
            image_set(new_img, i, j, pixel_value);
        }
    }
    return new_img;
}

Image *interpolation_bilin(const Image *img, int new_M, int new_N) {
    /* Interpolation bilinéaire */

    if (img == NULL || new_M <= 0 || new_N <= 0)
        return NULL;

    // Calcul des pas pour l'interpolation
    double pas_M = (new_M == 1 || img->M == 1) 
                    ? 0.0 : (double)img->M / new_M;
    double pas_N = (new_N == 1 || img->N == 1) 
                    ? 0.0 : (double)img->N / new_N;

    // Image de sortie
    Image *new_img = image_create(new_M, new_N, img->K);

    if (new_img == NULL) {
        return NULL;
    }

    for (int i = 0; i < new_M; i++){
        for (int j =0; j < new_N; j++){
            // Calcul des indices du pixel le plus proche dans l'image d'origine
            int orig_i = (int)floor(i * pas_M);
            int orig_j = (int)floor(j * pas_N);

            // Determination des 4 pixels voisins dans l'image d'origine
            int P1 = (int)image_get(img, orig_i, orig_j);
            int P2 = (int)image_get(img, orig_i, orig_j);
            int P3 = (int)image_get(img, orig_i, orig_j);
            int P4 = (int)image_get(img, orig_i, orig_j);

            pixel_t pixel_value = (1-i) * (1-j) * P1 + i * (1-j) * P2 + 
                                  (1-i) * j * P3 + i * j * P4;
            image_set(new_img, i, j, pixel_value);
        }
    }

    return new_img;
}

Image *interpolation_bicub(const Image *img, int new_M, int new_N) {
    /* Interpolation bicubique */
    return NULL;
}