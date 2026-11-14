// tests/test_contrast_transformation.c
#include <assert.h>
#include <stdio.h>
#include "image.h"
#include "Contrast/contrast.h"

/* Crée une image d'une ligne contenant les valeurs données. */
static Image *ligne(int K, const int *valeurs, int n) {
    Image *img = image_create(1, n, K);
    assert(img != NULL);
    for (int j = 0; j < n; j++) {
        image_set(img, 0, j, valeurs[j]);
    }
    return img;
}

/* Vérifie que la ligne 0 de img contient exactement les valeurs attendues. */
static void verifie(const Image *img, const int *attendu, int n) {
    assert(img != NULL);
    for (int j = 0; j < n; j++) {
        assert(image_get(img, 0, j) == attendu[j]);
    }
}

int main(void) {
    /* LINÉAIRE : min = 50, max = 150  ->  255 * (v - 50) / 100 */
    int v1[] = {50, 100, 150};
    int r1[] = {0, 127, 255};
    Image *img = ligne(8, v1, 3);
    Image *out = transformation_linear(img);
    verifie(out, r1, 3);
    verifie(img, v1, 3);                       /* l'entrée n'a pas changé */
    image_free(out);
    image_free(img);

    /* LINÉAIRE sur une image uniforme : copie inchangée (pas de division par 0) */
    int v2[] = {7, 7};
    img = ligne(8, v2, 2);
    out = transformation_linear(img);
    verifie(out, v2, 2);
    image_free(out);
    image_free(img);

    /* LINÉAIRE en 16 bits : le calcul ne doit pas déborder */
    int v3[] = {1000, 1500, 2000};
    int r3[] = {0, 32767, 65535};
    img = ligne(16, v3, 3);
    out = transformation_linear(img);
    verifie(out, r3, 3);
    image_free(out);
    image_free(img);

    /* SATURATION : smin = 100, smax = 200 */
    int v4[] = {50, 100, 150, 200, 250};
    int r4[] = {0, 0, 127, 255, 255};
    img = ligne(8, v4, 5);
    out = transformation_linear_saturation(img, 100, 200);
    verifie(out, r4, 5);
    image_free(out);
    assert(transformation_linear_saturation(img, 200, 100) == NULL);   /* smin >= smax */
    assert(transformation_linear_saturation(img, -1, 100) == NULL);    /* hors plage */
    assert(transformation_linear_saturation(img, 0, 256) == NULL);     /* hors plage */
    image_free(img);

    /* PAR MORCEAUX : (0,0) -> (64,32) -> (192,224) -> (255,255) */
    int v5[] = {0, 32, 64, 128, 192, 223, 255};
    int r5[] = {0, 16, 32, 128, 224, 239, 255};
    img = ligne(8, v5, 7);
    out = transformation_piecewise(img, 64, 32, 192, 224);
    verifie(out, r5, 7);
    image_free(out);
    assert(transformation_piecewise(img, 192, 32, 64, 224) == NULL);   /* r1 >= r2 */
    assert(transformation_piecewise(img, 64, 224, 192, 32) == NULL);   /* s1 > s2 */
    image_free(img);

    /* PAR MORCEAUX avec r1 = s1 et r2 = s2 : la courbe est l'identité */
    Image *tous = image_create(1, 256, 8);
    for (int j = 0; j < 256; j++) {
        image_set(tous, 0, j, j);
    }
    out = transformation_piecewise(tous, 64, 64, 192, 192);
    for (int j = 0; j < 256; j++) {
        assert(image_get(out, 0, j) == j);
    }
    image_free(out);

    /* GAMMA = 1 : identité */
    out = transformation_gamma(tous, 1.0);
    for (int j = 0; j < 256; j++) {
        assert(image_get(out, 0, j) == j);
    }
    image_free(out);
    image_free(tous);

    /* GAMMA = 2 : racine carrée, 64 -> 255 * sqrt(64/255) = 127,75 -> 128 */
    int v6[] = {0, 64, 255};
    int r6[] = {0, 128, 255};
    img = ligne(8, v6, 3);
    out = transformation_gamma(img, 2.0);
    verifie(out, r6, 3);
    image_free(out);
    assert(transformation_gamma(img, 0.0) == NULL);
    assert(transformation_gamma(img, -1.0) == NULL);
    image_free(img);

    /* image absente */
    assert(transformation_linear(NULL) == NULL);
    assert(transformation_gamma(NULL, 2.0) == NULL);

    printf("test_contrast_transformation OK\n");
    return 0;
}
