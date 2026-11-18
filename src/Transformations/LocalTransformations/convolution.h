#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include "image.h"
#include "Transformations/LocalTransformations/filter.h"

/*
 * Règles communes :
 *  - aucune fonction ne modifie l'image d'entrée ;
 *  - chacune retourne une NOUVELLE image, de mêmes dimensions (M x N) et de
 *    même K, que l'appelant libère avec image_free() ;
 *  - NULL si un paramètre est invalide ou si l'allocation échoue.
 *
 * GESTION DES BORDS (à décider en groupe, puis à écrire ici)
 * Le cours propose : mettre à zéro, convolution partielle (sur une portion
 * du noyau), ou miroir de l'image ("pas de solution miracle"). Le groupe
 * choisit UNE méthode, valable pour la convolution ET pour le médian, et la
 * documente ici.
 *   Choix retenu : ...
 */

/*
 * CONVOLUTION
 * Entrée : une image et un filtre (créé avec filter_mean, filter_gaussian...).
 * Sortie : l'image résultat du produit de convolution :
 *              I'(i,j) = somme sur (u,v) de I(i-u, j-v) * filtre(u,v)
 *          (une sommation de multiplications).
 * À respecter :
 *   - calculer dans un type signé large (double), jamais en pixel_t ;
 *   - ramener chaque résultat dans [0, 2^K - 1] avant image_set (arrondi
 *     puis saturation), comme pour les autres opérations.
 *
 * RÉF : (Cours 03 : "Convolution numérique discrète", "Convolution numérique")
 */
Image *convolution(const Image *img, const Filter *f);

/*
 * FILTRE MÉDIAN (non linéaire)
 * Entrée : une image et la taille du voisinage NxN (impaire).
 * Sortie : l'image où chaque pixel est remplacé par la valeur médiane de son
 *          voisinage NxN.
 * Ce n'est pas un produit de convolution : le médian ne s'écrit pas avec des
 * coefficients, donc il n'utilise pas Filter. Il élimine bien le bruit
 * « poivre et sel », là où le moyenneur et le gaussien échouent.
 *
 * RÉF : (Cours 03 : "Filtre médian (non-linéaire)")
 */
Image *convolution_median(const Image *img, int size);

#endif /* CONVOLUTION_H */