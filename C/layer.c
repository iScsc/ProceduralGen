#include <malloc.h>

#include "loadingBar.h"
#include "layer.h"

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





layer* newLayer(gradientGrid* gradGrid, int sizeFactor, int display_loading)
{
    int gradGridWidth = gradGrid->width;
    int gradGridHeight = gradGrid->height;

    int width = (gradGridWidth - 1) * sizeFactor;
    int height = (gradGridHeight - 1) * sizeFactor;

    // Initialization
    layer* new_layer = calloc(1, sizeof(layer));
    double* values = calloc(width * height, sizeof(double));

    new_layer->width = width;
    new_layer->height = height;
    new_layer->sizeFactor = sizeFactor;

    new_layer->gradGrid = gradGrid;

    new_layer->values = values;

    // Setting correct double values
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            double* value = getValue(new_layer, j, i);

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

    return new_layer;
}



double* getValue(layer* layer, int width_idx, int height_idx)
{
    int width = layer->width;
    int height = layer->height;

    if (width_idx < 0 || width_idx >= width)
    {
        return NULL;
    }
    
    if (height_idx < 0 || height_idx >= height)
    {
        return NULL;
    }

    return (layer->values) + height_idx * width + width_idx;
}



void printLayer(layer* layer)
{
    int width = layer->width;
    int height = layer->height;

    printf("-------------------------------------------\n");
    printf("Printing layer of size = (%d, %d)\n\n", height, width);

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            double* value = getValue(layer, j, i);

            printf("%lf   ", *value);
        }
        printf("\n");
    }

    printf("-------------------------------------------\n");
}



void freeLayer(layer* layer)
{
    freeGradGrid(layer->gradGrid);

    free(layer->values);
    free(layer);
}
