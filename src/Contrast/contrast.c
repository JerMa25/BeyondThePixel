#include "Contrast/contrast.h"

#include <math.h>
#include <stdlib.h>

/*
 * Toutes les transformations de ce fichier suivent la même recette
 * (Cours 02, diapo 11 "Utilisation d'une LUT") :
 *   1. remplir une LUT (Look Up Table) : lut[v] = nouvelle valeur du niveau v ;
 *   2. appliquer la LUT à chaque pixel : I'(i,j) = lut[I(i,j)].
 * On fait le calcul L fois (une fois par niveau) au lieu de M*N fois.
 */

/* Alloue une LUT de L = 2^K cases. À libérer avec free(). */
static pixel_t *lut_create(const Image *img) {
    return malloc(((size_t)image_max_level(img) + 1) * sizeof(pixel_t));
}

/* Crée une nouvelle image où chaque pixel v devient lut[v]. */
static Image *lut_apply(const Image *img, const pixel_t *lut) {
    Image *out = image_create(img->M, img->N, img->K);
    if (!out) {
        return NULL;
    }
    for (int i = 0; i < img->M; i++) {
        for (int j = 0; j < img->N; j++) {
            image_set(out, i, j, lut[image_get(img, i, j)]);
        }
    }
    return out;
}

Image *transformation_linear_saturation(const Image *img, int smin, int smax) {
    if (!img || smin < 0 || smin >= smax || smax > image_max_level(img)) {
        return NULL;
    }
    pixel_t *lut = lut_create(img);
    if (!lut) {
        return NULL;
    }

    int max_level = image_max_level(img);
    for (int v = 0; v <= max_level; v++) {
        if (v <= smin) {
            lut[v] = 0;
        } else if (v >= smax) {
            lut[v] = max_level;
        } else {
            /* calcul en long long : (L-1) * (v - smin) dépasse un int pour K = 16 */
            lut[v] = (pixel_t)((long long)max_level * (v - smin) / (smax - smin));
        }
    }

    Image *out = lut_apply(img, lut);
    free(lut);
    return out;
}

Image *transformation_linear(const Image *img) {
    if (!img) {
        return NULL;
    }

    /* recherche du min et du max de l'image */
    int min = image_get(img, 0, 0);
    int max = min;
    for (int i = 0; i < img->M; i++) {
        for (int j = 0; j < img->N; j++) {
            int v = image_get(img, i, j);
            if (v < min) min = v;
            if (v > max) max = v;
        }
    }

    if (min == max) {
        return image_clone(img);   /* image uniforme : rien à étirer, et évite / 0 */
    }
    /* la linéaire est une saturation avec Smin = min et Smax = max (diapo 12) */
    return transformation_linear_saturation(img, min, max);
}

Image *transformation_piecewise(const Image *img, int r1, int s1, int r2, int s2) {
    if (!img) {
        return NULL;
    }
    int max_level = image_max_level(img);
    if (r1 < 0 || r1 >= r2 || r2 > max_level || s1 < 0 || s1 > s2 || s2 > max_level) {
        return NULL;
    }
    pixel_t *lut = lut_create(img);
    if (!lut) {
        return NULL;
    }

    for (int v = 0; v <= max_level; v++) {
        long long r;
        if (v <= r1) {
            /* segment 1 : de (0, 0) à (r1, s1) ; si r1 = 0, seul v = 0 est ici */
            r = (r1 == 0) ? s1 : (long long)s1 * v / r1;
        } else if (v <= r2) {
            /* segment 2 : de (r1, s1) à (r2, s2) */
            r = s1 + (long long)(s2 - s1) * (v - r1) / (r2 - r1);
        } else {
            /* segment 3 : de (r2, s2) à (L-1, L-1) ; ici v > r2 donc r2 < L-1 */
            r = s2 + (long long)(max_level - s2) * (v - r2) / (max_level - r2);
        }
        lut[v] = (pixel_t)r;
    }

    Image *out = lut_apply(img, lut);
    free(lut);
    return out;
}

Image *transformation_gamma(const Image *img, double gamma) {
    if (!img || !(gamma > 0)) {
        return NULL;
    }
    pixel_t *lut = lut_create(img);
    if (!lut) {
        return NULL;
    }

    int max_level = image_max_level(img);
    for (int v = 0; v <= max_level; v++) {
        double n = (double)v / max_level;               /* ramène v dans [0, 1] */
        double r = max_level * pow(n, 1.0 / gamma);     /* n^(1/gamma), remis dans [0, L-1] */
        lut[v] = (pixel_t)(r + 0.5);                    /* + 0.5 : arrondi au plus proche */
    }

    Image *out = lut_apply(img, lut);
    free(lut);
    return out;
}
