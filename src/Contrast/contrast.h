#ifndef CONTRAST_H
#define CONTRAST_H

#include "image.h"

/*
 * LUMINANCE
 * Entrée : une image.
 * Sortie : la luminance (brillance) de l'image, c'est-à-dire la moyenne
 *          de tous ses pixels.
 *
 * CONTRASTE (les deux définitions du cours)
 * Entrée : une image.
 * Sortie : la valeur du contraste.
 * Méthodes à implémenter :
 *   - l'écart-type des niveaux de gris ;
 *   - la variation entre niveaux de gris min et max.
 *
 * AMÉLIORATION DU CONTRASTE
 * Entrée : une image, plus les paramètres propres à chaque méthode.
 * Sortie : l'image dont le contraste a été amélioré.
 * Méthodes à implémenter :
 *   - transformation linéaire                         (fait)
 *   - transformation linéaire avec saturation         (fait)
 *   - transformation linéaire par morceaux            (fait)
 *   - transformation non linéaire : correction gamma  (fait)
 *   - égalisation de l'histogramme
 *
 * RÉF: (Cours 02: "Luminance ou brillance d'une image", "Contraste d'une image", "Amélioration du contraste")
 */

/* TODO : luminance, contraste, égalisation de l'histogramme */

/*
 * Notation : L-1 = image_max_level(img) = 2^K - 1 (255 pour K = 8).
 *
 * Les quatre fonctions ci-dessous :
 *   - ne modifient pas l'image d'entrée ;
 *   - retournent une NOUVELLE image, que l'appelant doit libérer avec image_free() ;
 *   - retournent NULL si un paramètre est invalide ou si l'allocation échoue.
 */

/* TRANSFORMATION LINÉAIRE (Cours 02, diapo 9)
 * Étire la dynamique [min, max] de l'image sur toute la plage [0, L-1] :
 *     I'(i,j) = (L-1) * (I(i,j) - min) / (max - min)
 * Image uniforme (min = max) : on retourne une copie inchangée. */
Image *transformation_linear(const Image *img);

/* TRANSFORMATION LINÉAIRE AVEC SATURATION (Cours 02, diapo 13)
 * Comme la linéaire, mais avec des bornes [smin, smax] choisies :
 *     I(i,j) <= smin        ->  I'(i,j) = 0
 *     I(i,j) >= smax        ->  I'(i,j) = L-1
 *     smin < I(i,j) < smax  ->  I'(i,j) = (L-1) * (I(i,j) - smin) / (smax - smin)
 * Exige 0 <= smin < smax <= L-1. */
Image *transformation_linear_saturation(const Image *img, int smin, int smax);

/* TRANSFORMATION LINÉAIRE PAR MORCEAUX (Cours 02, diapos 14-15)
 * La courbe relie 4 points par des droites : (0,0) -> (r1,s1) -> (r2,s2) -> (L-1,L-1).
 * Exige 0 <= r1 < r2 <= L-1 et 0 <= s1 <= s2 <= L-1. */
Image *transformation_piecewise(const Image *img, int r1, int s1, int r2, int s2);

/* TRANSFORMATION NON LINÉAIRE : CORRECTION GAMMA (Cours 02, diapo 16)
 *     I'(i,j) = (L-1) * (I(i,j) / (L-1)) ^ (1 / gamma)
 * gamma > 1 éclaircit, gamma < 1 assombrit, gamma = 1 ne change rien.
 * Exige gamma > 0. */
Image *transformation_gamma(const Image *img, double gamma);

#endif /* CONTRAST_H */