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
| `src/Contrast` | linéaire, saturation, par morceaux, gamma (LUT) ; à venir : luminance, contraste, égalisation ([détails](src/Contrast/README.md)) |
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
make run-contrast_transformation  # lance uniquement tests/test_contrast_transformation.c
make clean        # supprime build/ et output/
```

Prérequis : `gcc`, `make`.

## Comment tester

Depuis la racine du projet :

```
make test
```

Résultat attendu, un `OK` par fichier de test :

```
== build/tests/test_contrast_transformation
test_contrast_transformation OK
== build/tests/test_image
test_image OK
```

Pour un seul module : `make run-<nom>`, où `<nom>` est le nom du fichier `tests/test_<nom>.c`.
Exemple : `make run-contrast_transformation`.

**Si un test échoue**, le programme s'arrête sur un message qui donne le fichier, la ligne et la condition fausse :

```
test_contrast_transformation: tests/test_contrast_transformation.c:21: verifie: Assertion `image_get(img, 0, j) == attendu[j]' failed.
```

Il suffit d'ouvrir le test à cette ligne pour voir quelle valeur était attendue.

**Vérifier la mémoire** (fuites, accès hors tableau, débordements), une fois par module :

```
gcc -Wall -Wextra -std=c11 -g -fsanitize=address,undefined -Isrc \
    tests/test_contrast_transformation.c src/image.c src/Contrast/contrast.c -o /tmp/t -lm && /tmp/t
```

Si rien d'autre que `... OK` ne s'affiche, il n'y a aucun problème.

**Ajouter un test** : créer `tests/test_<nom>.c` avec un `main` qui utilise `assert` sur des valeurs calculées à la main.
Le Makefile le trouve automatiquement.

## Conventions de travail

- Un module = un dossier = `module.c` + `module.h`, sans `main` dans `src/`.
- Chaque module a son fichier de test dans `tests/`.
- Les `.h` utilisent des gardes d'inclusion et documentent chaque fonction publique.
- Toute fonction qui alloue une image documente qui doit la libérer.
- Compilation sans warning (`-Wall -Wextra`).
- Une branche par fonctionnalité (`feature/<module>`), fusion dans `main` via pull request relue par un autre membre.
- On ne commit jamais `build/` ni `output/`.

## Type des pixels et gestion de la mémoire

Les pixels sont stockés dans le type `pixel_t`, défini comme `uint16_t` (entier non signé sur 16 bits).

### Pourquoi `uint16_t` plutôt que `int` ?

Une image est un grand tableau de pixels. Un `int` occupe généralement 4 octets, un `uint16_t` en occupe 2. Pour une image de 1024 x 1024 :

| Type | Mémoire |
|------|---------|
| `int` | 4 Mo |
| `uint16_t` | 2 Mo |

Ce choix a aussi deux avantages :
- Il exprime le domaine de valeurs : un pixel est dans `[0, 2^K - 1]`, donc jamais négatif.
- Sa taille est identique sur toutes les plateformes, contrairement à `int`.

Il couvre `K` de 1 à 16 bits. En mémoire, chaque pixel occupe toujours 16 bits, quelle que soit la valeur de `K`.

### Attention : dépassement avec un type non signé

Avec un type non signé, une soustraction négative ne donne pas un nombre négatif, elle « boucle » :

```c
pixel_t a = 10, b = 20;
pixel_t r = a - b;   // r = 65526, et non -10 !
```

Même chose pour l'addition, qui peut dépasser `2^K - 1` sans prévenir.

### Convention : calculer en `int`, puis reconvertir

On ne manipule pas directement les `pixel_t` dans les calculs. Pour toute opération pouvant sortir de l'intervalle `[0, 2^K - 1]` (addition, soustraction, multiplication, étirement de contraste, convolution, etc.) :

1. Lire le pixel et le convertir en `int`.
2. Effectuer le calcul en `int`.
3. Ramener le résultat dans `[0, 2^K - 1]`, comme le fait le cours (`Min(f+g, 255)`, `Max(f-g, 0)`).
4. Reconvertir en `pixel_t` et écrire le résultat avec `image_set`.

`image_set` vérifie par un `assert` que la valeur écrite est dans `[0, 2^K - 1]`. Une valeur hors plage fait donc planter le programme en mode debug au lieu de corrompre l'image. Pour que ce filet fonctionne, on ne compile **pas** avec `-DNDEBUG` pendant le développement.

## Références

- Cours 01-Introduction, 02-Traitement de base, 03-Convolution
- Gonzalez & Woods, *Digital Image Processing*, Prentice-Hall, 2002