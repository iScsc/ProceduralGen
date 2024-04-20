#include <malloc.h>

#include "loadingBar.h"
#include "map2d.h"

double smoothstep(double w)
{
    if (w > 1)
    {
        w = 1;
    }
    else if (w < 0)
    {
        w = 0;
    }

    return w * w * (3.0 - 2.0 * w);
}



double interpolate(double a0, double a1, double w)
{
    return a0 + (a1 - a0) * smoothstep(w);
}





double dotGridGradient(int ix, int iy, double x, double y, gradientGrid* gradGrid)
{
    double dx = x - (double) ix;
    double dy = y - (double) iy;

    vector* v = getVector(gradGrid, ix, iy);

    return dx * v->x + dy * v->y;
}



double perlin(double x, double y, gradientGrid* gradGrid)
{
    int x0 = (int) x;
    int y0 = (int) y;

    int x1 = x0 + 1;
    int y1 = y0 + 1;

    double sx = x - x0;
    double sy = y - y0;

    double n0 = dotGridGradient(x0, y0, x, y, gradGrid);
    double n1 = dotGridGradient(x1, y0, x, y, gradGrid);
    double ix0 = interpolate(n0, n1, sx);

    n0 = dotGridGradient(x0, y1, x, y, gradGrid);
    n1 = dotGridGradient(x1, y1, x, y, gradGrid);
    double ix1 = interpolate(n0, n1, sx);

    double value = interpolate(ix0, ix1, sy);

    return value;
}





map2d* newMap2d(gradientGrid* gradGrid, int sizeFactor, int display_loading)
{
    int gradGridWidth = gradGrid->width;
    int gradGridHeight = gradGrid->height;

    int width = (gradGridWidth - 1) * sizeFactor;
    int height = (gradGridHeight - 1) * sizeFactor;

    // Initialization
    map2d* new_map2d = calloc(1, sizeof(map2d));
    double* values = calloc(width * height, sizeof(double));

    new_map2d->width = width;
    new_map2d->height = height;
    new_map2d->sizeFactor = sizeFactor;
    new_map2d->values = values;

    // Setting correct double values
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            double* value = getValue(new_map2d, j, i);

            *value = perlin((double) j/sizeFactor, (double) i/sizeFactor, gradGrid);

            if (display_loading != 0)
            {
                // max :  (width - 1) + (height - 1) * width  =  width * height - 1
                print_loading_bar(j + i * width, width * height - 1, NUMBER_OF_SEGMENTS,
                "\r   Generating random gradient grid... ", "");
            }
        }
    }

    if (display_loading != 0)
    {
        printf("\n");
    }

    return new_map2d;
}



double* getValue(map2d* map2d, int width_idx, int height_idx)
{
    int width = map2d->width;
    int height = map2d->height;

    if (width_idx < 0 || width_idx >= width)
    {
        return NULL;
    }
    
    if (height_idx < 0 || height_idx >= height)
    {
        return NULL;
    }

    return (map2d->values) + height_idx * width + width_idx;
}



void printMap2d(map2d* map2d)
{
    int width = map2d->width;
    int height = map2d->height;

    printf("-------------------------------------------\n");
    printf("Printing 2d map of size = (%d, %d)\n\n", height, width);

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            double* value = getValue(map2d, j, i);

            printf("%lf   ", *value);
        }
        printf("\n");
    }

    printf("-------------------------------------------\n");
}



void freeMap2d(map2d* map2d)
{
    free(map2d->values);
    free(map2d);
}
