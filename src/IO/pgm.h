#ifndef PGM_H
#define PGM_H

#include "image.h"

/*
 * LECTURE / ÉCRITURE DE FICHIERS PGM (niveaux de gris)
 *
 * Format PGM : un en-tête texte (type, largeur, hauteur, valeur maximale),
 * suivi des pixels. Le type "P2" stocke les pixels en texte (ASCII), le
 * type "P5" les stocke en binaire.
 *
 * LECTURE
 * Entrée : le chemin d'un fichier PGM.
 * Sortie : une nouvelle image contenant les pixels du fichier, ou un
 *          indicateur d'échec si le fichier est absent ou invalide.
 * À respecter :
 *   - lire les types P2 et P5 ;
 *   - ignorer les commentaires (lignes commençant par '#') de l'en-tête ;
 *   - déduire K de la valeur maximale du fichier ;
 *   - refuser proprement un fichier invalide ou tronqué, sans fuite de
 *     mémoire.
 *
 * ÉCRITURE
 * Entrée : une image et un chemin de fichier.
 * Sortie : le fichier PGM créé sur le disque, et un indicateur de
 *          réussite ou d'échec.
 * À respecter :
 *   - écrire en P5 ;
 *   - la valeur maximale écrite dans l'en-tête est 2^K - 1 ;
 *   - le fichier produit doit pouvoir être relu par la fonction de lecture
 *     et ouvert par un visionneur d'images standard.
 *
 * Périmètre actuel : images 8 bits maximum (K <= 8). Les fichiers dont la
 * valeur maximale dépasse 255 sont refusés.
 */

/* TODO : déclarations des fonctions ici */

#endif /* PGM_H */