#ifndef LOGICOPS_H
#define LOGICOPS_H

#include "image.h"

/*
 * OPÉRATIONS LOGIQUES SUR LES IMAGES
 *
 * À implémenter : AND, OR, NOT, XOR, NAND.
 *
 * Entrée : une ou deux images, selon l'opération (NOT n'en prend qu'une).
 * Sortie : l'image résultant de l'opération appliquée pixel à pixel.
 *
 * RÉF: (Cours 02, "Opérations sur les images")
 */

/* Applique un ET bit à bit entre deux images compatibles.
 * Retourne une nouvelle image, à libérer avec image_free(), ou NULL en erreur. */
Image *image_and(const Image *a, const Image *b);

/* Applique un OU bit à bit entre deux images compatibles.
 * Retourne une nouvelle image, à libérer avec image_free(), ou NULL en erreur. */
Image *image_or(const Image *a, const Image *b);

/* Inverse les bits de chaque pixel d'une image.
 * Retourne une nouvelle image, à libérer avec image_free(), ou NULL en erreur. */
Image *image_not(const Image *img);

/* Applique un OU exclusif bit à bit entre deux images compatibles.
 * Retourne une nouvelle image, à libérer avec image_free(), ou NULL en erreur. */
Image *image_xor(const Image *a, const Image *b);

/* Applique un NON-ET bit à bit entre deux images compatibles.
 * Retourne une nouvelle image, à libérer avec image_free(), ou NULL en erreur. */
Image *image_nand(const Image *a, const Image *b);

#endif /* LOGICOPS_H */
