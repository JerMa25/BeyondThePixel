/* tests/test_mathOps.c
 *
 * Tests unitaires pour les opérations mathématiques sur les images.
 * Chaque cas est calculé à la main et vérifié par un assert.
 *
 * Compilation (depuis la racine du projet) :
 *   make run-mathOps
 * ou :
 *   make test
 */

#include <assert.h>
#include <stdio.h>
#include "image.h"
#include "ImgOperations/MathOperations/mathOps.h"

/* -------------------------------------------------------------------------
 * Helpers
 * ------------------------------------------------------------------------- */

/* Crée une image 1×n (une ligne) avec les valeurs données. */
static Image *ligne(int K, const int *vals, int n) {
    Image *img = image_create(1, n, K);
    assert(img != NULL);
    for (int j = 0; j < n; j++) {
        image_set(img, 0, j, (pixel_t)vals[j]);
    }
    return img;
}

/* Vérifie que la ligne 0 de img contient exactement les valeurs attendues. */
static void verifie(const Image *img, const int *attendu, int n) {
    assert(img != NULL);
    for (int j = 0; j < n; j++) {
        assert(image_get(img, 0, j) == (pixel_t)attendu[j]);
    }
}

/* =========================================================================
 * Tests – image_add
 * ========================================================================= */
static void test_add(void) {
    /* Cas normal : 100 + 80 = 180 (dans [0, 255]) */
    int va[] = {0,  100, 200};
    int vb[] = {0,   80,  80};
    int ve[] = {0,  180, 255};   /* 200+80=280 -> saturé à 255 */
    Image *a = ligne(8, va, 3);
    Image *b = ligne(8, vb, 3);
    Image *r = image_add(a, b);
    verifie(r, ve, 3);
    /* L'entrée n'a pas été modifiée */
    verifie(a, va, 3);
    verifie(b, vb, 3);
    image_free(r);
    image_free(a);
    image_free(b);

    /* Saturation maximale : 255 + 255 = 510 -> 255 */
    int va2[] = {255, 255};
    int vb2[] = {255,   1};
    int ve2[] = {255, 255};
    a = ligne(8, va2, 2);
    b = ligne(8, vb2, 2);
    r = image_add(a, b);
    verifie(r, ve2, 2);
    image_free(r);
    image_free(a);
    image_free(b);

    /* 16 bits : 60000 + 10000 = 65535 (saturé) */
    int va3[] = {60000, 1000};
    int vb3[] = {10000, 2000};
    int ve3[] = {65535, 3000};
    a = ligne(16, va3, 2);
    b = ligne(16, vb3, 2);
    r = image_add(a, b);
    verifie(r, ve3, 2);
    image_free(r);
    image_free(a);
    image_free(b);

    /* NULL / dimensions incompatibles -> NULL */
    a = image_create(2, 2, 8);
    b = image_create(3, 3, 8);
    assert(image_add(NULL, a) == NULL);
    assert(image_add(a, NULL) == NULL);
    assert(image_add(NULL, NULL) == NULL);
    assert(image_add(a, b) == NULL);   /* 2x2 != 3x3 */
    image_free(a);
    image_free(b);
}

/* =========================================================================
 * Tests – image_sub
 * ========================================================================= */
static void test_sub(void) {
    /* Cas normal : 150 - 50 = 100 */
    int va[] = {150,  50,   0};
    int vb[] = { 50, 100, 100};
    int ve[] = {100,   0,   0};   /* 50-100 < 0 -> 0  ;  0-100 < 0 -> 0 */
    Image *a = ligne(8, va, 3);
    Image *b = ligne(8, vb, 3);
    Image *r = image_sub(a, b);
    verifie(r, ve, 3);
    verifie(a, va, 3);
    verifie(b, vb, 3);
    image_free(r);
    image_free(a);
    image_free(b);

    /* Résultat nul : f == g */
    int v[] = {128, 0, 255};
    a = ligne(8, v, 3);
    b = ligne(8, v, 3);
    r = image_sub(a, b);
    int zeros[] = {0, 0, 0};
    verifie(r, zeros, 3);
    image_free(r);
    image_free(a);
    image_free(b);

    /* 16 bits : 1000 - 2000 -> 0 */
    int va3[] = {50000, 1000};
    int vb3[] = {10000, 2000};
    int ve3[] = {40000,    0};
    a = ligne(16, va3, 2);
    b = ligne(16, vb3, 2);
    r = image_sub(a, b);
    verifie(r, ve3, 2);
    image_free(r);
    image_free(a);
    image_free(b);

    /* NULL / dimensions incompatibles -> NULL */
    a = image_create(2, 2, 8);
    b = image_create(2, 3, 8);
    assert(image_sub(NULL, a) == NULL);
    assert(image_sub(a, NULL) == NULL);
    assert(image_sub(a, b) == NULL);   /* colonnes différentes */
    image_free(a);
    image_free(b);
}

/* =========================================================================
 * Tests – image_scale
 * ========================================================================= */
static void test_scale(void) {
    /* alpha = 2 : doublement, saturation à 255 */
    int v[]  = {0,  50, 100, 200};
    int ve[] = {0, 100, 200, 255};   /* 200*2=400 -> 255 */
    Image *img = ligne(8, v, 4);
    Image *r   = image_scale(img, 2.0);
    verifie(r, ve, 4);
    verifie(img, v, 4);              /* entrée inchangée */
    image_free(r);

    /* alpha = 0.5 : réduction par deux (arrondi) */
    int v2[]  = {0, 100, 255};
    int ve2[] = {0,  50, 128};       /* round(255*0.5) = round(127.5) = 128 */
    r = image_scale(img, 0.5);       /* img contient toujours v */

    /* Recréer avec v2 pour ce cas */
    image_free(r);
    image_free(img);
    img = ligne(8, v2, 3);
    r   = image_scale(img, 0.5);
    verifie(r, ve2, 3);
    image_free(r);
    image_free(img);

    /* alpha = 0 : image noire */
    int v3[]  = {10, 128, 255};
    int ve3[] = {0,    0,   0};
    img = ligne(8, v3, 3);
    r   = image_scale(img, 0.0);
    verifie(r, ve3, 3);
    image_free(r);
    image_free(img);

    /* alpha négatif : image noire (tout clamped à 0) */
    int v4[]  = {10, 100, 200};
    int ve4[] = {0,    0,   0};
    img = ligne(8, v4, 3);
    r   = image_scale(img, -1.5);
    verifie(r, ve4, 3);
    image_free(r);
    image_free(img);

    /* alpha = 1 : identité */
    int v5[] = {0, 64, 128, 192, 255};
    img = ligne(8, v5, 5);
    r   = image_scale(img, 1.0);
    verifie(r, v5, 5);
    image_free(r);
    image_free(img);

    /* 16 bits : alpha = 3, saturation à 65535 */
    int v6[]  = {1000, 20000};
    int ve6[] = {3000, 60000};
    img = ligne(16, v6, 2);
    r   = image_scale(img, 3.0);
    verifie(r, ve6, 2);
    image_free(r);

    /* 20000 * 4 = 80000 > 65535 -> saturation */
    int ve7[] = {4000, 65535};
    r = image_scale(img, 4.0);
    verifie(r, ve7, 2);
    image_free(r);
    image_free(img);

    /* NULL -> NULL */
    assert(image_scale(NULL, 2.0) == NULL);
}

/* =========================================================================
 * main
 * ========================================================================= */
int main(void) {
    test_add();
    test_sub();
    test_scale();

    printf("test_mathOps OK\n");
    return 0;
}
