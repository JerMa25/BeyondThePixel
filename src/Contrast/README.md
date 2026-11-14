# Module Contrast : amélioration du contraste

Référence : **Cours 02 – Traitement de base**, partie « Amélioration du contraste » (diapos 9 à 17).

Fichiers : `contrast.h` (déclarations et documentation), `contrast.c` (code), `tests/test_contrast_transformation.c` (tests).

```
make run-contrast_transformation   # compile et lance les tests de ce module
```

## 1. Les concepts

### Dynamique et contraste
La **dynamique** d'une image est l'intervalle `[min, max]` des niveaux de gris qu'elle utilise vraiment.
Une image de 8 bits *peut* aller de 0 à 255, mais une photo terne n'utilise par exemple que `[60, 180]` :
elle manque de contraste.

**Améliorer le contraste**, c'est appliquer à chaque pixel une fonction `I' = f(I)`
qui écarte les niveaux de gris les uns des autres. La fonction est la même pour tous les pixels :
le résultat ne dépend que de la valeur du pixel, pas de ses voisins.

Notation : `L = 2^K` niveaux, donc les valeurs vont de `0` à `L-1` (255 pour K = 8).
Le code utilise `image_max_level(img)` pour `L-1`, ce qui le rend valable pour tout K de 1 à 16.

### Les 4 transformations

| Transformation | Formule | Paramètres | Effet |
|---|---|---|---|
| **Linéaire** (diapo 9) | `I' = (L-1)·(I − min)/(max − min)` | aucun (min et max sont lus dans l'image) | étire `[min, max]` sur `[0, L-1]` |
| **Linéaire avec saturation** (diapo 13) | `I ≤ Smin → 0` ; `I ≥ Smax → L-1` ; sinon `(L-1)·(I − Smin)/(Smax − Smin)` | `Smin`, `Smax` | étire `[Smin, Smax]` et « écrase » ce qui est en dehors |
| **Linéaire par morceaux** (diapos 14-15) | droites reliant `(0,0) → (r1,s1) → (r2,s2) → (L-1,L-1)` | `r1, s1, r2, s2` | une pente différente selon la zone de gris |
| **Non linéaire : gamma** (diapo 16) | `I' = (L-1)·(I/(L-1))^(1/γ)` | `γ > 0` | `γ > 1` éclaircit les sombres, `γ < 1` assombrit |

Relations utiles à retenir :
- la **linéaire** est une **saturation** où `Smin = min` et `Smax = max` de l'image ;
- la **saturation** est un cas de **par morceaux** où `s1 = 0` et `s2 = L-1` ;
- la **par morceaux** avec `r1 = s1` et `r2 = s2` est l'identité (l'image ne change pas).

### Pourquoi une saturation ?
Dans la linéaire, un seul pixel très clair ou très sombre (du bruit) fixe `max` ou `min` et l'étirement est presque nul.
Avec la saturation, on choisit soi-même l'intervalle utile et on accepte de perdre les extrêmes.

### Pourquoi le gamma ?
Les écrans n'affichent pas la luminosité de façon linéaire (`I_émise = k·n^γ`, γ écran entre 1,3 et 3).
On corrige avec l'inverse `n' = n^(1/γ)`. On travaille sur `n = I/(L-1)` dans `[0, 1]`,
car une puissance de `[0, 1]` reste dans `[0, 1]`, puis on remultiplie par `L-1`.

## 2. L'implémentation

### La LUT (Look Up Table), diapo 11
Le cours précise que calculer la formule pour chaque pixel est « pas optimal ».
Une image de 1000 × 1000 a un million de pixels, mais seulement 256 niveaux de gris possibles.
On calcule donc la formule **une seule fois par niveau** et on range le résultat dans un tableau :

```
lut[v] = f(v)              pour v = 0 .. L-1     (256 calculs)
I'(i,j) = lut[I(i,j)]      pour chaque pixel     (simple lecture)
```

Toutes les fonctions du module suivent la même recette :

1. **vérifier** les paramètres (sinon retourner `NULL`) ;
2. **créer** la LUT (`lut_create`) ;
3. **remplir** la LUT avec la formule de la méthode ;
4. **appliquer** la LUT (`lut_apply`), ce qui crée la nouvelle image ;
5. **libérer** la LUT (`free`) et retourner la nouvelle image.

Seule l'étape 3 change d'une méthode à l'autre.

### Les fonctions

| Fonction | Rôle |
|---|---|
| `lut_create(img)` | *interne* : alloue un tableau de `L` cases |
| `lut_apply(img, lut)` | *interne* : nouvelle image avec `out(i,j) = lut[img(i,j)]` |
| `transformation_linear(img)` | cherche min et max, puis appelle `transformation_linear_saturation(img, min, max)` |
| `transformation_linear_saturation(img, smin, smax)` | formule de la diapo 13 |
| `transformation_piecewise(img, r1, s1, r2, s2)` | 3 segments de droite |
| `transformation_gamma(img, gamma)` | puissance `1/γ`, avec `pow()` de `math.h` |

### Règles respectées (voir le README principal)
- **L'entrée n'est jamais modifiée** : on retourne une nouvelle image, que l'appelant libère avec `image_free()`.
- **Calcul dans un type plus grand, puis reconversion en `pixel_t`** : les produits sont faits en `long long`,
  car `65535 × 65535` dépasse la capacité d'un `int` en 16 bits. Le gamma est calculé en `double`.
- **On ne sort jamais de `[0, L-1]`** : chaque formule donne une valeur dans cet intervalle,
  et `image_set` le vérifie par un `assert`.
- **Pas de division par zéro** :
  - linéaire sur une image uniforme (`min = max`) → copie de l'image ;
  - saturation : on exige `smin < smax` ;
  - par morceaux : on exige `r1 < r2`, et les cas `r1 = 0` et `r2 = L-1` sont traités à part.
- **Arrondi** : les formules entières tronquent, comme le pseudo-code du cours. Le gamma arrondit au plus proche (`+ 0.5`).

### Exemple d'utilisation
```c
Image *img = image_create(512, 512, 8);
/* ... remplir img ... */

Image *etiree = transformation_linear(img);
Image *claire = transformation_gamma(img, 2.2);
if (!etiree || !claire) { /* paramètre invalide ou mémoire insuffisante */ }

image_free(etiree);
image_free(claire);
image_free(img);
```

## 3. Ce qui reste à faire dans ce module
Luminance, contraste (écart-type, min/max) et égalisation de l'histogramme (voir `contrast.h`).
