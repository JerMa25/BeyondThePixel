#ifndef CONTRAST_H
#define CONTRAST_H

#include "image.h"

/*
 * LUMINANCE ET CONTRASTE (Cours 02, diapos 4 à 7)
 *
 * Aucune fonction ne modifie l'image. Elles retournent une valeur
 * négative (-1) si l'image est invalide (NULL), car une luminance,
 * un écart-type ou un écart max-min est toujours >= 0.
 *
 * Notation : Nbp = M * N, le nombre de pixels de l'image.
 */

/* LUMINANCE (ou brillance) : moyenne de tous les pixels
 *     mu = (1 / Nbp) * somme des I(i,j)
 * La somme est calculée en double : pas de débordement, même en 16 bits
 * sur une grande image. */
double contrast_luminance(const Image *img);

/* CONTRASTE 1 : écart-type des niveaux de gris
 *     sigma = sqrt( (1 / Nbp) * somme des (I(i,j) - mu)^2 )
 * Comme dans le cours, on divise par Nbp (et non par Nbp - 1).
 * Image uniforme : sigma = 0. */
double contrast_stddev(const Image *img);

/* CONTRASTE 2 : variation entre niveaux de gris min et max
 *     max - min, dans [0, L-1]
 * Image uniforme : 0. Après transformation_linear sur une image
 * non uniforme : L-1. */
int contrast_range(const Image *img);

#endif /* CONTRAST_H */
