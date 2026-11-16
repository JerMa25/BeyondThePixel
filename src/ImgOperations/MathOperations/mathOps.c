/*
 * mathOps.c – Opérations mathématiques sur les images
 *
 * Implémente : addition, soustraction, multiplication par scalaire.
 * Toutes les opérations respectent la convention du projet :
 *   1. Lire le pixel et le convertir en int.
 *   2. Effectuer le calcul en int.
 *   3. Ramener le résultat dans [0, L-1] (clamp).
 *   4. Reconvertir en pixel_t et écrire via image_set.
 *
 * RÉF : Cours 02 – « Opérations sur les images »
 */

#include "ImgOperations/MathOperations/mathOps.h"

#include <math.h>   /* round() */

/* -------------------------------------------------------------------------
 * Utilitaire interne : clamp un entier dans [0, max_val].
 * ------------------------------------------------------------------------- */
static int clamp(int v, int max_val) {
    if (v < 0)        return 0;
    if (v > max_val)  return max_val;
    return v;
}

/* -------------------------------------------------------------------------
 * Vérifie que deux images sont compatibles (mêmes dimensions et même K).
 * Retourne 1 si OK, 0 sinon.
 * ------------------------------------------------------------------------- */
static int compatible(const Image *a, const Image *b) {
    return a && b && a->M == b->M && a->N == b->N && a->K == b->K;
}

/* =========================================================================
 * image_add – Addition pixel à pixel
 *   résultat(i,j) = Min( f(i,j) + g(i,j), L-1 )
 * ========================================================================= */
Image *image_add(const Image *img_a, const Image *img_b) {
    if (!compatible(img_a, img_b)) {
        return NULL;
    }

    Image *result = image_create(img_a->M, img_a->N, img_a->K);
    if (!result) {
        return NULL;
    }

    int L_minus1 = (int)image_max_level(img_a);

    for (int i = 0; i < img_a->M; i++) {
        for (int j = 0; j < img_a->N; j++) {
            int f = (int)image_get(img_a, i, j);
            int g = (int)image_get(img_b, i, j);
            int val = clamp(f + g, L_minus1);
            image_set(result, i, j, (pixel_t)val);
        }
    }

    return result;
}

/* =========================================================================
 * image_sub – Soustraction pixel à pixel
 *   résultat(i,j) = Max( f(i,j) - g(i,j), 0 )
 * ========================================================================= */
Image *image_sub(const Image *img_a, const Image *img_b) {
    if (!compatible(img_a, img_b)) {
        return NULL;
    }

    Image *result = image_create(img_a->M, img_a->N, img_a->K);
    if (!result) {
        return NULL;
    }

    int L_minus1 = (int)image_max_level(img_a);

    for (int i = 0; i < img_a->M; i++) {
        for (int j = 0; j < img_a->N; j++) {
            int f = (int)image_get(img_a, i, j);
            int g = (int)image_get(img_b, i, j);
            int val = clamp(f - g, L_minus1);
            image_set(result, i, j, (pixel_t)val);
        }
    }

    return result;
}

/* =========================================================================
 * image_scale – Multiplication par un scalaire
 *   résultat(i,j) = Min( Max( round(alpha * f(i,j)), 0 ), L-1 )
 * ========================================================================= */
Image *image_scale(const Image *img, double alpha) {
    if (!img) {
        return NULL;
    }

    Image *result = image_create(img->M, img->N, img->K);
    if (!result) {
        return NULL;
    }

    int L_minus1 = (int)image_max_level(img);

    for (int i = 0; i < img->M; i++) {
        for (int j = 0; j < img->N; j++) {
            int f   = (int)image_get(img, i, j);
            int val = (int)round(alpha * f);
            val = clamp(val, L_minus1);
            image_set(result, i, j, (pixel_t)val);
        }
    }

    return result;
}
