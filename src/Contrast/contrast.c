/*
 * contrast.c – Luminance et contraste d'une image
 *
 * RÉF : Cours 02 – « Luminance ou brillance d'une image »,
 *       « Contraste d'une image » (diapos 4 à 7)
 */

#include "Contrast/contrast.h"

#include <math.h>   /* sqrt() */

double contrast_luminance(const Image *img)
{
    if (img == NULL || img->data == NULL)
        return -1.0;

    double somme = 0.0;

    for (int i = 0; i < img->M; i++) {
        for (int j = 0; j < img->N; j++) {
            somme += image_get(img, i, j);
        }
    }

    return somme / ((double)img->M * img->N);
}

double contrast_stddev(const Image *img)
{
    if (img == NULL || img->data == NULL)
        return -1.0;

    double mu = contrast_luminance(img);
    double s = 0.0;

    for (int i = 0; i < img->M; i++) {
        for (int j = 0; j < img->N; j++) {
            double d = (double)image_get(img, i, j) - mu;
            s += d * d;
        }
    }

    return sqrt(s / ((double)img->M * img->N));
}

int contrast_range(const Image *img)
{
    if (img == NULL || img->data == NULL)
        return -1;

    int min = image_get(img, 0, 0);
    int max = min;

    for (int i = 0; i < img->M; i++) {
        for (int j = 0; j < img->N; j++) {
            int pixel = image_get(img, i, j);

            if (pixel < min)
                min = pixel;

            if (pixel > max)
                max = pixel;
        }
    }

    return max - min;
}
