// tests/test_interpolation.c
#include <assert.h>
#include <stdio.h>
#include "image.h"
#include "Interpolation/interpolation.h"

typedef Image *(*interp_fn)(const Image *, int, int);

#define NB_METHODS 3
static const interp_fn METHODS[NB_METHODS] = {
    interpolation_ppv,
    interpolation_bilin,
    interpolation_bicub
};

/* Construit une image M x N sur K bits à partir d'un tableau (ligne par ligne). */
static Image *make_image(int M, int N, int K, const int *vals) {
    Image *img = image_create(M, N, K);
    assert(img != NULL);
    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
            image_set(img, i, j, (pixel_t)vals[i * N + j]);
    return img;
}

/* 1 si les deux images ont les mêmes dimensions, le même K et les mêmes pixels. */
static int same_image(const Image *a, const Image *b) {
    if (a->M != b->M || a->N != b->N || a->K != b->K)
        return 0;
    for (int i = 0; i < a->M; i++)
        for (int j = 0; j < a->N; j++)
            if (image_get(a, i, j) != image_get(b, i, j))
                return 0;
    return 1;
}

/* Vérifie que tous les pixels de img valent v. */
static void assert_uniform(const Image *img, int v) {
    for (int i = 0; i < img->M; i++)
        for (int j = 0; j < img->N; j++)
            assert(image_get(img, i, j) == v);
}

/* ---------- Tests communs aux trois méthodes ---------- */

/* Même taille en sortie : on doit retrouver exactement l'image d'origine. */
static void test_identity(void) {
    const int vals[12] = { 0, 10, 20, 30,
                           200, 190, 180, 170,
                           5, 255, 5, 255 };
    Image *in = make_image(3, 4, 8, vals);
    for (int m = 0; m < NB_METHODS; m++) {
        Image *out = METHODS[m](in, 3, 4);
        assert(out != NULL);
        assert(same_image(in, out));
        image_free(out);
    }
    image_free(in);
}

/* Une image uniforme reste uniforme, quelle que soit la taille de sortie. */
static void test_constant(void) {
    const int vals[9] = { 77, 77, 77, 77, 77, 77, 77, 77, 77 };
    Image *in = make_image(3, 3, 8, vals);
    for (int m = 0; m < NB_METHODS; m++) {
        Image *up = METHODS[m](in, 7, 5);       /* zoom */
        assert(up != NULL && up->M == 7 && up->N == 5 && up->K == 8);
        assert_uniform(up, 77);
        image_free(up);

        Image *down = METHODS[m](in, 2, 2);     /* réduction */
        assert(down != NULL && down->M == 2 && down->N == 2);
        assert_uniform(down, 77);
        image_free(down);
    }
    image_free(in);
}

/* Une image 1x1 donne une image uniforme de la taille demandée. */
static void test_one_pixel(void) {
    const int vals[1] = { 42 };
    Image *in = make_image(1, 1, 8, vals);
    for (int m = 0; m < NB_METHODS; m++) {
        Image *out = METHODS[m](in, 3, 4);
        assert(out != NULL && out->M == 3 && out->N == 4);
        assert_uniform(out, 42);
        image_free(out);
    }
    image_free(in);
}

/* Réduction 4x4 -> 2x2 : les coins de l'image d'origine sont conservés
 * (les pixels de bord sont alignés sur les bords de l'image d'origine). */
static void test_downscale_corners(void) {
    int vals[16];
    for (int k = 0; k < 16; k++)
        vals[k] = 10 * k;
    Image *in = make_image(4, 4, 8, vals);
    for (int m = 0; m < NB_METHODS; m++) {
        Image *out = METHODS[m](in, 2, 2);
        assert(out != NULL && out->M == 2 && out->N == 2 && out->K == 8);
        assert(image_get(out, 0, 0) == 0);
        assert(image_get(out, 0, 1) == 30);
        assert(image_get(out, 1, 0) == 120);
        assert(image_get(out, 1, 1) == 150);
        image_free(out);
    }
    image_free(in);
}

/* L'image d'entrée n'est jamais modifiée, et la sortie est un nouvel objet. */
static void test_input_not_modified(void) {
    const int vals[6] = { 10, 200, 30, 40, 50, 250 };
    Image *in = make_image(2, 3, 8, vals);
    Image *copy = image_clone(in);
    assert(copy != NULL);
    for (int m = 0; m < NB_METHODS; m++) {
        Image *out = METHODS[m](in, 5, 7);
        assert(out != NULL && out != in && out->data != in->data);
        assert(same_image(in, copy));
        image_free(out);
    }
    image_free(in);
    image_free(copy);
}

/* K est conservé, y compris pour des valeurs de K différentes de 8. */
static void test_k_preserved(void) {
    const int vals[4] = { 0, 1, 1, 0 };
    Image *in = make_image(2, 2, 1, vals);
    for (int m = 0; m < NB_METHODS; m++) {
        Image *out = METHODS[m](in, 4, 4);
        assert(out != NULL && out->K == 1);
        for (int i = 0; i < out->M; i++)
            for (int j = 0; j < out->N; j++)
                assert(image_get(out, i, j) <= 1);
        image_free(out);
    }
    image_free(in);
}

/* Paramètres invalides : NULL attendu, sans planter. */
static void test_invalid(void) {
    const int vals[4] = { 1, 2, 3, 4 };
    Image *in = make_image(2, 2, 8, vals);
    for (int m = 0; m < NB_METHODS; m++) {
        assert(METHODS[m](NULL, 4, 4) == NULL);
        assert(METHODS[m](in, 0, 4) == NULL);
        assert(METHODS[m](in, 4, 0) == NULL);
        assert(METHODS[m](in, -1, 4) == NULL);
        assert(METHODS[m](in, 4, -3) == NULL);
    }
    image_free(in);
}

/* ---------- Tests propres à chaque méthode ---------- */

/* Plus proche voisin : agrandissement 2x2 -> 4x4.
 * pas = 1/3, donc les indices source sont round(0, 1/3, 2/3, 1) = 0, 0, 1, 1 :
 * chaque pixel est copié sur deux lignes et deux colonnes. */
static void test_ppv_upscale(void) {
    const int vals[4] = { 10, 20, 30, 40 };
    const int expected[16] = { 10, 10, 20, 20,
                               10, 10, 20, 20,
                               30, 30, 40, 40,
                               30, 30, 40, 40 };
    Image *in = make_image(2, 2, 8, vals);
    Image *out = interpolation_ppv(in, 4, 4);
    assert(out != NULL && out->M == 4 && out->N == 4);
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            assert(image_get(out, i, j) == expected[i * 4 + j]);
    image_free(in);
    image_free(out);
}

/* Bilinéaire 1D : [0, 100] étiré sur 5 colonnes -> 0, 25, 50, 75, 100. */
static void test_bilinear_1d(void) {
    const int vals[2] = { 0, 100 };
    const int expected[5] = { 0, 25, 50, 75, 100 };
    Image *in = make_image(1, 2, 8, vals);
    Image *out = interpolation_bilin(in, 1, 5);
    assert(out != NULL && out->M == 1 && out->N == 5);
    for (int j = 0; j < 5; j++)
        assert(image_get(out, 0, j) == expected[j]);
    image_free(in);
    image_free(out);
}

/* Bilinéaire 2D : f(i,j) = 100 * (i + j) est exactement bilinéaire.
 * En 3x3 (pas = 0.5), on doit donc obtenir 50 * (i + j). */
static void test_bilinear_2d(void) {
    const int vals[4] = { 0, 100, 100, 200 };
    Image *in = make_image(2, 2, 8, vals);
    Image *out = interpolation_bilin(in, 3, 3);
    assert(out != NULL && out->M == 3 && out->N == 3);
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            assert(image_get(out, i, j) == 50 * (i + j));
    image_free(in);
    image_free(out);
}

/* Bilinéaire sur des données croissantes : la sortie doit rester croissante. */
static void test_bilinear_monotone(void) {
    const int vals[4] = { 0, 0, 255, 255 };
    Image *in = make_image(1, 4, 8, vals);
    Image *out = interpolation_bilin(in, 1, 13);
    assert(out != NULL);
    for (int j = 1; j < out->N; j++)
        assert(image_get(out, 0, j) >= image_get(out, 0, j - 1));
    image_free(in);
    image_free(out);
}

/* Bicubique : sur un front 0 -> 255, le noyau déborde de [0, L-1].
 * Le résultat doit être saturé (sinon l'assert de image_set plante),
 * et les extrémités doivent être conservées. */
static void test_bicubic_saturation(void) {
    const int vals[4] = { 0, 0, 255, 255 };
    Image *in = make_image(1, 4, 8, vals);
    Image *out = interpolation_bicub(in, 1, 13);
    assert(out != NULL && out->N == 13);
    assert(image_get(out, 0, 0) == 0);
    assert(image_get(out, 0, 12) == 255);
    for (int j = 0; j < out->N; j++)
        assert(image_get(out, 0, j) <= 255);
    image_free(in);
    image_free(out);
}

int main(void) {
    test_identity();
    test_constant();
    test_one_pixel();
    test_downscale_corners();
    test_input_not_modified();
    test_k_preserved();
    test_invalid();

    test_ppv_upscale();
    test_bilinear_1d();
    test_bilinear_2d();
    test_bilinear_monotone();
    test_bicubic_saturation();

    printf("test_interpolation OK\n");
    return 0;
}