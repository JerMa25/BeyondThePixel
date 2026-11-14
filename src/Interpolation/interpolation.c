#include "interpolation.h"

#include <stdlib.h>
#include <math.h>

Image *interpolation_ppv(const Image *img, int new_M, int new_N) {
    /* Interpolation par plus proche voisin */

    if (img == NULL || new_M <= 0 || new_N <= 0)
        return NULL;

    // Calcul des pas pour l'interpolation
    double pas_M = (new_M == 1 || img->M == 1) 
                    ? 0.0 : (double)(img->M - 1) / (new_M - 1);
    double pas_N = (new_N == 1 || img->N == 1) 
                    ? 0.0 : (double)(img->N - 1) / (new_N - 1);

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
                    ? 0.0 : (double)(img->M - 1) / (new_M - 1);
    double pas_N = (new_N == 1 || img->N == 1) 
                    ? 0.0 : (double)(img->N - 1) / (new_N - 1);

    // Image de sortie
    Image *new_img = image_create(new_M, new_N, img->K);

    if (new_img == NULL) {
        return NULL;
    }

    for (int i = 0; i < new_M; i++){
        for (int j =0; j < new_N; j++){
            //Position du pixel dans l'image d'origine
            double orig_i = i * pas_M;
            double orig_j = j * pas_N;

            // Indices des pixels voisins dans l'image d'origine
            int i0 = (int)floor(orig_i);
            int j0 = (int)floor(orig_j);
            int i1 = (i0 + 1 < img->M) ? i0 + 1 : i0;
            int j1 = (j0 + 1 < img->N) ? j0 + 1 : j0;

            // Coefficients pour l'interpolation bilinéaire
            double i_frac = orig_i - i0;
            double j_frac = orig_j - j0;

            // Determination des 4 pixels voisins dans l'image d'origine
            pixel_t P1 = image_get(img, i0, j0);
            pixel_t P2 = image_get(img, i1, j0);
            pixel_t P3 = image_get(img, i0, j1);
            pixel_t P4 = image_get(img, i1, j1);

            double pixel_value = (1 - i_frac) * (1 - j_frac) * P1 + i_frac * (1 - j_frac) * P2 + 
                                  (1 - i_frac) * j_frac * P3 + i_frac * j_frac * P4;
            
            if (pixel_value < 0) pixel_value = 0;
            if (pixel_value > image_max_level(img)) pixel_value = image_max_level(img);

            image_set(new_img, i, j, (pixel_t)round(pixel_value));
        }
    }

    return new_img;
}


double bicubic_weight(double x) {
    const double a = -0.5;
    x = fabs(x);

    if (x < 1.0) {
        return (a + 2.0) * x * x * x
             - (a + 3.0) * x * x
             + 1.0;
    }
    else if (x < 2.0) {
        return a * x * x * x
             - 5.0 * a * x * x
             + 8.0 * a * x
             - 4.0 * a;
    }
    else {
        return 0.0;
    }
}

Image *interpolation_bicubique(const Image *img, int new_M, int new_N) {
    /* Interpolation bicubique */

    if (img == NULL || new_M <= 0 || new_N <= 0)
        return NULL;

    // Image de sortie
    Image *new_img = image_create(new_M, new_N, img->K);

    if (new_img == NULL) {
        return NULL;
    }
    
    // Calcul des pas pour l'interpolation
    double pas_M = (new_M == 1 || img->M == 1) 
                    ? 0.0 : (double)(img->M - 1) / (new_M - 1);
    double pas_N = (new_N == 1 || img->N == 1) 
                    ? 0.0 : (double)(img->N - 1) / (new_N - 1);

    for (int i = 0; i < new_M; i++) {
        for (int j = 0; j < new_N; j++) {
            // Position du pixel dans l'image d'origine
            double orig_i = i * pas_M;
            double orig_j = j * pas_N;

            // Indices des pixels voisins dans l'image d'origine
            int i0 = (int)floor(orig_i);
            int j0 = (int)floor(orig_j);

            // Calcul des poids pour l'interpolation bicubique
            double weights[4][4];
            for (int l = 0; l < 4; l++) {
                for (int k = 0; k < 4; k++) {
                    double x = orig_i - (i0 + l - 1);
                    double y = orig_j - (j0 + k - 1);
                    weights[l][k] = bicubic_weight(x) * bicubic_weight(y);
                }
            }

            // Calcul de la valeur du pixel interpolé
            double pixel_value = 0.0;
            for (int l = 0; l < 4; l++) {
                for (int k = 0; k < 4; k++) {
                    int ii = i0 + l - 1;
                    int jj = j0 + k - 1;

                    if (ii < 0) ii = 0;
                    if (ii >= img->M) ii = img->M - 1;
                    if (jj < 0) jj = 0;
                    if (jj >= img->N) jj = img->N - 1;

                    pixel_value += weights[l][k] * image_get(img, ii, jj);
                    
                }
            }

            if (pixel_value < 0) pixel_value = 0;
            if (pixel_value > image_max_level(img)) pixel_value = image_max_level(img);

            image_set(new_img, i, j, (pixel_t)round(pixel_value));
        }
    }
    return new_img;
}