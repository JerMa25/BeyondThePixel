#ifndef FILTER_H
#define FILTER_H

/*
 * FILTRES DE CONVOLUTION
 *
 * Un filtre (masque, noyau) est une matrice carrée de coefficients réels,
 * de taille impaire (3x3, 5x5, 7x7...). Les coefficients sont des double,
 * pas des pixels : ils peuvent être fractionnaires (1/9) et, pour d'autres
 * filtres à venir, négatifs.
 *
 * Le type de filtre (moyenneur, gaussien...) est choisi à la CRÉATION, en
 * appelant la fonction correspondante. Ce fichier ne contient que la création
 * et la libération : l'application d'un filtre à une image est dans
 * convolution.h, qui ne connaît que les coefficients.
 *
 * Règles communes :
 *  - chaque fonction de création retourne un nouveau filtre, que l'appelant
 *    libère avec filter_free() ;
 *  - elle retourne NULL si un paramètre est invalide (taille paire ou <= 0,
 *    sigma <= 0) ou si l'allocation échoue ;
 *  - les coefficients sont NORMALISÉS à la création (somme = 1), pour ne
 *    pas modifier la luminance globale de l'image. La convolution n'a donc
 *    pas à diviser le résultat.
 *
 * RÉF : (Cours 03 : "Masque de convolution", "Le filtre moyenneur",
 *        "Le filtre Gaussien")
 */

/* Un filtre = une matrice size x size de coefficients, stockée ligne par
 * ligne : le coefficient (u, v) est coef[u * size + v]. */
typedef struct {
    int size;        /* taille impaire : 3, 5, 7... */
    double *coef;    /* size * size coefficients */
} Filter;

/*
 * FILTRE MOYENNEUR
 * Entrée : la taille du filtre (impaire).
 * Sortie : un filtre dont tous les coefficients sont égaux (1 / size^2).
 * Effet  : lisse l'image, réduit le bruit, rend l'image floue. Plus le
 *          filtre est grand, plus le flou est important.
 */
Filter *filter_mean(int size);

/*
 * FILTRE GAUSSIEN
 * Entrée : la taille du filtre (impaire) et l'écart-type sigma (> 0).
 * Sortie : un filtre dont les coefficients suivent la gaussienne
 *              exp( -(x^2 + y^2) / (2 sigma^2) )
 *          centrée sur le milieu du filtre, puis normalisés.
 * Effet  : lisse mieux et réduit mieux le bruit que le moyenneur.
 */
Filter *filter_gaussian(int size, double sigma);

/* Libère le filtre et ses coefficients. Accepte NULL. */
void filter_free(Filter *f);

#endif /* FILTER_H */