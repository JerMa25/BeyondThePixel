# BeyondThePixel

Implémentation en C des principales opérations de traitement d'images vues en cours. Le but est de passer de la théorie à la pratique en codant nous-mêmes chaque opération, sans bibliothèque d'images.

> Périmètre actuel : images en **niveaux de gris** uniquement.

## Structure de données

Toutes les opérations manipulent la même structure `Image(M, N, K)` :

| Champ | Rôle |
|-------|------|
| `M`   | nombre de lignes |
| `N`   | nombre de colonnes |
| `K`   | nombre de bits par pixel (L = 2^K niveaux de gris) |
| `data`| matrice de pixels (M x N) |

Définie dans `src/image.h`, elle est implémentée dans `src/image.c`
(création, libération, accès aux pixels).

## Modules

| Dossier | Contenu |
|---------|---------|
| `src/IO` | lecture et écriture de fichiers PGM |
| `src/Histogram` | histogramme, luminance, profil d'intensité |
| `src/Contrast` | contraste, étirement linéaire, saturation, par morceaux, gamma, LUT |
| `src/ImgOperations/LogicOperations` | ET, OU, XOR, NON |
| `src/ImgOperations/MathOperations` | addition, soustraction, multiplication |
| `src/Interpolation` | plus proche voisin, bilinéaire, bicubique |
| `src/Transformations/GlobalTransformations` | transformations utilisant toute l'image |
| `src/Transformations/LocalTransformations` | convolution, filtres moyenneur, gaussien, médian |

## Arborescence

```
BeyondThePixel/
├── Makefile
├── data/       images d'entrée (.pgm)
├── output/     résultats générés (non versionné)
├── build/      objets et exécutables (non versionné)
├── src/        code source, un dossier par module (.c + .h)
└── tests/      un fichier test_<module>.c par module
```

## Compilation et tests

```
make              # compile tous les modules
make test         # compile et lance tous les tests
make run-contrast # lance uniquement tests/test_contrast.c
make clean        # supprime build/ et output/
```

Prérequis : `gcc`, `make`.

## Conventions de travail

- Un module = un dossier = `module.c` + `module.h`, sans `main` dans `src/`.
- Chaque module a son fichier de test dans `tests/`.
- Les `.h` utilisent des gardes d'inclusion et documentent chaque fonction publique.
- Toute fonction qui alloue une image documente qui doit la libérer.
- Compilation sans warning (`-Wall -Wextra`).
- Une branche par fonctionnalité (`feature/<module>`), fusion dans `main` via pull request relue par un autre membre.
- On ne commit jamais `build/` ni `output/`.

## Équipe

| Membre | Modules |
|--------|---------|
|        |         |

## Références

- Cours 01-Introduction, 02-Traitement de base, 03-Convolution
- Gonzalez & Woods, *Digital Image Processing*, Prentice-Hall, 2002