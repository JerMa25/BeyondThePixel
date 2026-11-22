/* tests/test_contrast.c
 *
 * Tests unitaires pour la luminance et le contraste d'une image.
 * Chaque valeur attendue est calculée à la main.
 *
 * Lancement (depuis la racine du projet) :
 *   make run-contrast
 */

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include "image.h"
#include "Contrast/contrast.h"
#include "Contrast/contrast_transformation.h"

/* -------------------------------------------------------------------------
 * Helpers
 * ------------------------------------------------------------------------- */

/* Les résultats sont des double : on ne les compare pas avec ==,
 * on vérifie qu'ils sont égaux à une très petite erreur près. */
static int proche(double a, double b) {
    return fabs(a - b) < 1e-9;
}

/* Construit une image M x N sur K bits à partir d'un tableau (ligne par ligne). */
static Image *make_image(int M, int N, int K, const int *vals) {
    Image *img = image_create(M, N, K);
    assert(img != NULL);
    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
            image_set(img, i, j, (pixel_t)vals[i * N + j]);
    return img;
}

/* Crée une image M x N sur K bits dont tous les pixels valent v. */
static Image *uniforme(int M, int N, int K, int v) {
    Image *img = image_create(M, N, K);
    assert(img != NULL);
    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
            image_set(img, i, j, (pixel_t)v);
    return img;
}

/* =========================================================================
 * Exemple calculé à la main : [2 4 4 4 ; 5 5 7 9]
 *   somme = 40, Nbp = 8                       -> mu = 5
 *   écarts² = 9+1+1+1+0+0+4+16 = 32, 32/8 = 4 -> sigma = 2
 *   max - min = 9 - 2                         -> 7
 * ========================================================================= */
static void test_exemple_cours(void) {
    const int vals[8] = { 2, 4, 4, 4,
                          5, 5, 7, 9 };
    Image *img = make_image(2, 4, 8, vals);

    assert(proche(contrast_luminance(img), 5.0));
    assert(proche(contrast_stddev(img), 2.0));
    assert(contrast_range(img) == 7);

    image_free(img);
}

/* Image uniforme : la moyenne est la valeur commune, aucun contraste. */
static void test_uniforme(void) {
    Image *img = uniforme(3, 3, 8, 77);

    assert(proche(contrast_luminance(img), 77.0));
    assert(proche(contrast_stddev(img), 0.0));
    assert(contrast_range(img) == 0);

    image_free(img);
}

/* Un seul pixel : même résultat qu'une image uniforme. */
static void test_un_pixel(void) {
    const int vals[1] = { 42 };
    Image *img = make_image(1, 1, 8, vals);

    assert(proche(contrast_luminance(img), 42.0));
    assert(proche(contrast_stddev(img), 0.0));
    assert(contrast_range(img) == 0);

    image_free(img);
}

/* 16 bits, valeurs extrêmes [0, 65535] :
 *   mu = 65535 / 2 = 32767,5
 *   écarts = +-32767,5, donc sigma = 32767,5
 *   max - min = 65535 */
static void test_16_bits(void) {
    const int vals[2] = { 0, 65535 };
    Image *img = make_image(1, 2, 16, vals);

    assert(proche(contrast_luminance(img), 32767.5));
    assert(proche(contrast_stddev(img), 32767.5));
    assert(contrast_range(img) == 65535);

    image_free(img);
}

/* Débordement : 2000 x 2000 pixels à 65535.
 * La somme vaut 4 000 000 x 65535 = 262 140 000 000, ce qui dépasse
 * la capacité d'un int (environ 2,1 milliards). Si la somme était
 * calculée en int, la moyenne serait fausse. */
static void test_debordement(void) {
    Image *img = uniforme(2000, 2000, 16, 65535);

    assert(proche(contrast_luminance(img), 65535.0));
    assert(proche(contrast_stddev(img), 0.0));
    assert(contrast_range(img) == 0);

    image_free(img);
}

/* Les fonctions ne modifient pas l'image d'entrée. */
static void test_entree_inchangee(void) {
    const int vals[6] = { 10, 200, 30, 40, 50, 250 };
    Image *img = make_image(2, 3, 8, vals);
    Image *copie = image_clone(img);
    assert(copie != NULL);

    contrast_luminance(img);
    contrast_stddev(img);
    contrast_range(img);

    for (int i = 0; i < img->M; i++)
        for (int j = 0; j < img->N; j++)
            assert(image_get(img, i, j) == image_get(copie, i, j));

    image_free(img);
    image_free(copie);
}

/* Lien avec le module contrast_transformation :
 * l'étirement linéaire étale la dynamique sur [0, L-1],
 * donc max - min doit valoir L-1 = 255 après transformation. */
static void test_apres_etirement(void) {
    const int vals[3] = { 50, 100, 150 };
    Image *img = make_image(1, 3, 8, vals);
    assert(contrast_range(img) == 100);

    Image *etiree = transformation_linear(img);
    assert(etiree != NULL);
    assert(contrast_range(etiree) == 255);

    image_free(etiree);
    image_free(img);
}

/* Image absente : valeur négative attendue, sans planter. */
static void test_null(void) {
    assert(contrast_luminance(NULL) < 0);
    assert(contrast_stddev(NULL) < 0);
    assert(contrast_range(NULL) < 0);
}

/* =========================================================================
 * main
 * ========================================================================= */
int main(void) {
    test_exemple_cours();
    test_uniforme();
    test_un_pixel();
    test_16_bits();
    test_debordement();
    test_entree_inchangee();
    test_apres_etirement();
    test_null();

    printf("test_contrast OK\n");
    return 0;
}
