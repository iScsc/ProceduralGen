#include <malloc.h>
#include <time.h>

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





double dotGridGradient(int ix, int iy, double x, double y, gradientGrid* gradient_grid)
{
    double dx = x - (double) ix;
    double dy = y - (double) iy;

    vector* v = getVector(gradient_grid, ix, iy);

    return dx * v->x + dy * v->y;
}



double perlin(double x, double y, gradientGrid* gradient_grid)
{
    int x0 = (int) x;
    int y0 = (int) y;

    int x1 = x0 + 1;
    int y1 = y0 + 1;

    double sx = x - x0;
    double sy = y - y0;

    double n0 = dotGridGradient(x0, y0, x, y, gradient_grid);
    double n1 = dotGridGradient(x1, y0, x, y, gradient_grid);
    double ix0 = interpolate(n0, n1, sx);

    n0 = dotGridGradient(x0, y1, x, y, gradient_grid);
    n1 = dotGridGradient(x1, y1, x, y, gradient_grid);
    double ix1 = interpolate(n0, n1, sx);

    double value = interpolate(ix0, ix1, sy);

    return value;
}





double* getLayerValue(layer* layer, int width_idx, int height_idx)
{
    double* layer_value = NULL;

    if (layer != NULL)
    {
        int width = layer->width;
        int height = layer->height;

        if (width_idx < 0 || width_idx >= width)
        {
            printf("%sERROR : invalid width_idx = %d when reading layer value. Should be in range [0, %d]%s\n", RED_COLOR, width_idx, width - 1, DEFAULT_COLOR);
            return NULL;
        }
        
        if (height_idx < 0 || height_idx >= height)
        {
            printf("%sERROR : invalid height_idx = %d when reading layer value. Should be in range [0, %d]%s\n", RED_COLOR, height_idx, height - 1, DEFAULT_COLOR);
            return NULL;
        }

        layer_value = (layer->values) + height_idx * width + width_idx;
    }

    return layer_value;
}





layer* newLayerFromGradient(gradientGrid* gradient_grid, int size_factor, unsigned int display_loading)
{
    clock_t start_time = clock();

    int gradGridWidth = gradient_grid->width;
    int gradGridHeight = gradient_grid->height;

    // Size layer must be applied **between** gradient grids points.Thus, the layer dimensions are the following:
    int width = (gradGridWidth - 1) * size_factor;
    int height = (gradGridHeight - 1) * size_factor;

    // Initialization
    layer* new_layer = calloc(1, sizeof(layer));
    double* values = calloc(width * height, sizeof(double));

    new_layer->width = width;
    new_layer->height = height;
    new_layer->size_factor = size_factor;

    new_layer->gradient_grid = gradient_grid;

    new_layer->values = values;

    // Setting correct double values
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            double* value = getLayerValue(new_layer, j, i);

            *value = perlin((double) j/size_factor, (double) i/size_factor, gradient_grid);

            if (display_loading != 0)
            {
                int nb_indents = display_loading - 1;

                char base_str[100] = "Generating layer...                ";

                predefined_loading_bar(j + i * width, width * height - 1, NUMBER_OF_SEGMENTS, base_str, nb_indents, start_time);
            }
        }
    }

    return new_layer;
}



layer* newLayer(int gradGrid_width, int gradGrid_height, int size_factor, unsigned int display_loading)
{
    int g_loading = display_loading;

    // if (display_loading != 0)
    // {
    //     indent_print(display_loading - 1, "Generating new layer...\n");
    //     g_loading += 1;
    // }

    // Size Factor should be set to match gradGrid_width - 1 and gradGrid_height - 1
    gradientGrid* gradient_grid = newRandomGradGrid(gradGrid_width, gradGrid_height, g_loading);

    return newLayerFromGradient(gradient_grid, size_factor, g_loading);
}





void writeLayerFile(layer* layer, char path[])
{
    FILE* f = NULL;

    f = fopen(path, "w");

    if (f != NULL)
    {
        fprintf(f, "Layer\n");

        // Writing the parameters
        int width = layer->width;
        int height = layer->height;
        int size_factor = layer->size_factor;

        fprintf(f, "width=%d\nheight=%d\nsize_factor=%d\n", width, height, size_factor);

        // Writing the values
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                double value = *getLayerValue(layer, j, i);

                if (j != width - 1)
                {
                    fprintf(f, "% .8lf\t", value);
                }
                else
                {
                    fprintf(f, "% .8lf\n", value);
                }
            }
        }

        fclose(f);
    }
    else
    {
        printf("%sERROR : could not open file in writing mode at path '%s'%s\n", RED_COLOR, path, DEFAULT_COLOR);
        return;
    }
}



layer* readLayerFile(char path[200]);





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
            double* value = getLayerValue(layer, j, i);

            printf("%lf   ", *value);
        }
        printf("\n");
    }

    printf("-------------------------------------------\n");
}



void freeLayer(layer* layer)
{
    if (layer != NULL)
    {
        if (layer->values != NULL)
        {
            free(layer->values);
        }

        freeGradGrid(layer->gradient_grid);

        free(layer);
    }
}
