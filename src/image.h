#ifndef IMAGE_H
#define IMAGE_H

#include <assert.h>
#include <stddef.h>
#include <stdint.h>

/* Type d'un pixel : supporte K de 1 à 16 bits. */
typedef uint16_t pixel_t;

#define IMAGE_MAX_BITS 16

/*
 * Image en niveaux de gris.
 *   M    : nombre de lignes   (hauteur)
 *   N    : nombre de colonnes (largeur)
 *   K    : bits par pixel, donc L = 2^K niveaux, valeurs dans [0, 2^K - 1]
 *   data : M*N pixels, stockés ligne par ligne (pixel (i,j) = data[i*N + j])
 */
typedef struct {
    int M;
    int N;
    int K;
    pixel_t *data;
} Image;

/* Crée une image M x N sur K bits, initialisée à 0 (noir).
 * Retourne NULL si les paramètres sont invalides ou si l'allocation échoue.
 * L'appelant doit libérer l'image avec image_free(). */
Image *image_create(int M, int N, int K);

/* Libère l'image et ses pixels. Accepte NULL. */
void image_free(Image *img);

/* Retourne une copie indépendante de l'image (NULL en cas d'échec).
 * L'appelant doit libérer la copie avec image_free(). */
Image *image_clone(const Image *img);

/* Valeur maximale d'un pixel : 2^K - 1 (255 pour K = 8). */
static inline pixel_t image_max_level(const Image *img) {
    return (pixel_t)((1u << img->K) - 1u);
}

/* Lecture du pixel (i, j) : ligne i, colonne j. */
static inline pixel_t image_get(const Image *img, int i, int j) {
    assert(i >= 0 && i < img->M && j >= 0 && j < img->N);
    return img->data[(size_t)i * img->N + j];
}

/* Écriture du pixel (i, j). La valeur doit être dans [0, 2^K - 1] ;
 * pour une valeur calculée, passer par image_clamp(). */
static inline void image_set(Image *img, int i, int j, pixel_t v) {
    assert(i >= 0 && i < img->M && j >= 0 && j < img->N);
    assert(v <= image_max_level(img));
    img->data[(size_t)i * img->N + j] = v;
}

#endif /* IMAGE_H */