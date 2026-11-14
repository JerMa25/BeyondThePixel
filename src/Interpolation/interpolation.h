#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include "image.h"

/*
 * INTERPOLATION D'IMAGES (changement d'échelle)
 *
 * Méthodes à implémenter :
 *   - plus proche voisin
 *   - bilinéaire
 *   - bicubique
 *
 * Entrée : une image et la taille souhaitée pour le résultat (ou un
 *          facteur d'échelle).
 * Sortie : l'image redimensionnée avec la méthode choisie.
 */

Image *interpolation_ppv(const Image *img, int new_M, int new_N);
Image *interpolation_bilin(const Image *img, int new_M, int new_N);
Image *interpolation_bicub(const Image *img, int new_M, int new_N);

#endif /* INTERPOLATION_H */