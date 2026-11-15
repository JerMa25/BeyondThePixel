#ifndef MATHOPS_H
#define MATHOPS_H

#include "image.h"

/*
 * OPÉRATIONS MATHÉMATIQUES SUR LES IMAGES
 *
 * Chaque opération travaille pixel à pixel et renvoie une nouvelle image
 * allouée sur le tas. L'appelant doit la libérer avec image_free().
 * Retourne NULL si un paramètre est invalide (image NULL, dimensions
 * incompatibles, scalaire hors plage).
 *
 * Convention (cf. README) : on calcule en int, puis on clamp dans [0, L-1]
 * avant d'écrire le résultat (L = 2^K).
 *
 * RÉF : Cours 02 – « Opérations sur les images »
 */

/*
 * image_add – Addition pixel à pixel
 *
 *   résultat(i,j) = Min( f(i,j) + g(i,j), L-1 )
 *
 * Les deux images doivent avoir les mêmes dimensions (M, N) et le même K.
 * Retourne NULL si img_a ou img_b est NULL ou si les dimensions diffèrent.
 * L'appelant est responsable de libérer l'image retournée.
 */
Image *image_add(const Image *img_a, const Image *img_b);

/*
 * image_sub – Soustraction pixel à pixel
 *
 *   résultat(i,j) = Max( f(i,j) - g(i,j), 0 )
 *
 * Les deux images doivent avoir les mêmes dimensions (M, N) et le même K.
 * Retourne NULL si img_a ou img_b est NULL ou si les dimensions diffèrent.
 * L'appelant est responsable de libérer l'image retournée.
 */
Image *image_sub(const Image *img_a, const Image *img_b);

/*
 * image_scale – Multiplication par un scalaire
 *
 *   résultat(i,j) = Min( Max( round(alpha * f(i,j)), 0 ), L-1 )
 *
 * alpha peut être négatif (le Max à 0 ramène alors tout à noir) ou supérieur
 * à 1 (l'image est amplifiée jusqu'à saturation).
 * Retourne NULL si img est NULL.
 * L'appelant est responsable de libérer l'image retournée.
 */
Image *image_scale(const Image *img, double alpha);

#endif /* MATHOPS_H */