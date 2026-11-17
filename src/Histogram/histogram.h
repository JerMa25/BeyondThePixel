#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include "image.h"
#include <stddef.h>

/*
 * HISTOGRAMME
 *
 * Entrée : une image en niveaux de gris.
 * Sortie : pour chaque niveau de gris k de l'intervalle [0, L-1] (L = 2^K),
 *          le nombre de pixels de l'image ayant la valeur k.
 *
 * RÉF: (Cours 02, "Histogramme d'une image")
 */


/*
 * Calcule l'histogramme d'une image en niveaux de gris.
 *
 * Entrée : une image en niveaux de gris.
 *
 * Sortie : un tableau contenant le nombre de pixels
 *          pour chaque niveau de gris.
 *
 * Taille du tableau : 2^K.
 *
 * Retourne NULL en cas d'erreur d'allocation ou d'image invalide.
 *
 * L'appelant doit libérer le tableau avec free().
 */

size_t *histogram_compute(const Image *img);
#endif /* HISTOGRAM_H */