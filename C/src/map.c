/**
 * @file map.c
 * @author Zyno and BlueNZ
 * @brief map structure and functions implementation
 * @version 0.2
 * @date 2024-06-19
 * 
 */

#include <stdio.h>
#include <malloc.h>
#include <time.h>

#include "loadingBar.h"
#include "gradientGrid.h"
#include "layer.h"
#include "chunk.h"
#include "map.h"

double* getMapValue(map* map, int width_idx, int height_idx)
{
    double* map_value = NULL;

    if (map != NULL)
    {
        int width = map->map_width * map->chunk_width;
        int height = map->map_height * map->chunk_height;

        if (width_idx < 0 || width_idx >= width)
        {
            printf("%sERROR : invalid width_idx = %d when reading map value. Should be in range [0, %d]%s\n", RED_COLOR, width_idx, width - 1, DEFAULT_COLOR);
            return NULL;
        }
        
        if (height_idx < 0 || height_idx >= height)
        {
            printf("%sERROR : invalid height_idx = %d when reading map value. Should be in range [0, %d]%s\n", RED_COLOR, height_idx, height - 1, DEFAULT_COLOR);
            return NULL;
        }


        chunk* current_chunk = getChunk(map, width_idx/map->chunk_width, height_idx/map->chunk_height);

        map_value = getChunkValue(current_chunk, width_idx%map->chunk_width, height_idx%map->chunk_height);
    }

    return map_value;
}



chunk* getChunk(map* map, int width_idx, int height_idx)
{
    int width = map->map_width;
    int height = map->map_height;

    if (width_idx < 0 || width_idx >= width)
    {
        return NULL;
    }
    
    if (height_idx < 0 || height_idx >= height)
    {
        return NULL;
    }

    return map->chunks[height_idx * width + width_idx];
}



chunk* getVirtualChunk(map* map, int width_idx, int height_idx)
{
    int width = map->map_width+2;
    int height = map->map_height+2;

    if (height_idx<0 || height_idx>=height || width_idx<0 || width_idx>=width) return NULL;

    if (width_idx==0) return map->virtual_chunks[height_idx];
    else if (width_idx==width-1) return map->virtual_chunks[height+height_idx];
    else if (height_idx==0) return map->virtual_chunks[2*height-1+width_idx];
    else if (height_idx==height-1) return map->virtual_chunks[2*height+width-3+width_idx];

    return NULL;
}





double* getFullMap(map* map)
{
    double* altitude = NULL;

    if (map != NULL)
    {
        int width = map->map_width * map->chunk_width;
        int height = map->map_height * map->chunk_height;

        altitude = calloc(width * height, sizeof(double));

        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                altitude[i*width + j] = *getMapValue(map, j, i);
            }
        }
    }

    return altitude;
}





double interpolate2D(double a1, double a2, double a3, double a4, double x, double y) 
{
    return a1 + (a2 - a1) * smoothstep(x) + (a3 - a1) * smoothstep(y) + (a1 + a4 - a2 - a3) * smoothstep(x) * smoothstep(y);
}



map* addMeanAltitude(map* p_map, unsigned int display_loading) 
{
    if (display_loading != 0)
    {
        indent_print(display_loading - 1, "Adding base altitude on Generated Map...\n");
        display_loading += 2;
    }

    int chunk_width = p_map->chunk_width;
    int chunk_height = p_map->chunk_height;
    int map_width = p_map->map_width;
    int map_height = p_map->map_height;

    // map* res = copyMap(p_map);   //? Uncomment this and comment the next line to make it so that the functions returns a copy
                                    //? of the original map with the correct final altitude values.
    map* res=p_map;

    double altitude[map_width+2][map_height+2];

    clock_t start_time = clock();
    clock_t start_getting_time = clock();

    for (int i=0; i<map_width; i++)
    {
        for (int j=0; j<map_height; j++)
        {
            altitude[i+1][j+1]=getChunk(res,i,j)->base_altitude;
            if (display_loading != 0)
            {
                int nb_indents = display_loading - 1;

                char base_str[100] = "Getting chunks base altitude values...      ";

                predefined_loading_bar(j + i * (map_height), (map_width) * (map_height) - 1, NUMBER_OF_SEGMENTS, base_str, nb_indents, start_getting_time);
            }
        }
    }
    start_getting_time=clock();
    for (int j=0;j<map_height+2;j++) 
    {
        altitude[0][j]=getVirtualChunk(res,0,j)->base_altitude;
        altitude[map_width+1][j]=getVirtualChunk(res,map_width+1,j)->base_altitude;
        if (display_loading != 0)
        {
            int nb_indents = display_loading - 1;

            char base_str[100] = "Getting virtual chunks altitude values...   ";

            predefined_loading_bar(j*2+2, 2*(map_height+2+map_width+2), NUMBER_OF_SEGMENTS, base_str, nb_indents, start_getting_time);
        }
    }
    for (int i=1;i<map_width+1;i++) 
    {
        altitude[i][0]=getVirtualChunk(res,i,0)->base_altitude;
        altitude[i][map_height+1]=getVirtualChunk(res,i,map_height+1)->base_altitude;
        if (display_loading != 0)
        {
            int nb_indents = display_loading - 1;

            char base_str[100] = "Getting virtual chunks altitude values...   ";

            predefined_loading_bar(i*2+4 +2*(map_height+2), 2*(map_height+2+map_width+2), NUMBER_OF_SEGMENTS, base_str, nb_indents, start_getting_time);
        }
    }


    clock_t start_adding_time = clock();
    for (int j=0; j<map_height+1; j++)
    {
        for (int i=0; i<map_width+1; i++)
        {
            double a1=altitude[i][j];
            double a2=altitude[i+1][j];
            double a3=altitude[i][j+1];
            double a4=altitude[i+1][j+1];


            for (int pj=0; pj<chunk_height; pj++)
            {
                for (int pi=0; pi<chunk_width; pi++)
                {
                    if ((i<map_width || pi<chunk_width*0.5) && (j<map_height || pj<chunk_height*0.5)
                            && (i!=0 || pi>=chunk_width*0.5) && (j!=0 || pj>=chunk_height*0.5)) 
                    {
                        int ii=(int)(pi+(i-0.5)*chunk_width);
                        int jj=(int)(pj+(j-0.5)*chunk_height);
                        double x=pi*1./chunk_width;
                        double y=pj*1./chunk_height;
                        double alt = interpolate2D(a1,a2,a3,a4,x,y);
                        *getMapValue(res,ii,jj)+=alt;
                    }
                }
            }
            if (display_loading != 0)
            {
                int nb_indents = display_loading - 1;

                char base_str[100] = "Adding altitude values...                   ";

                predefined_loading_bar(j + i * (map_height+1), (map_width+1) * (map_height+1) - 1,
                                        NUMBER_OF_SEGMENTS, base_str, nb_indents, start_adding_time);
            }
        }
    }

    if (display_loading != 0)
    {
        display_loading-=2;

        
        double total_time = (double) (clock() - start_time)/CLOCKS_PER_SEC;
        char final_string[200] = "";

        snprintf(final_string, sizeof(final_string), "%sSUCCESS :%s base altitude generation took a total of %.4lf second(s) in CPU time.\n",
                                GREEN_COLOR, DEFAULT_COLOR, total_time);
        
        int nb_indents = display_loading-1;
        indent_print(nb_indents, final_string);
    }
    


    // printf("Base altitude from the first square : %lf, %lf, %lf, %lf", altitude[0][0], altitude[1][0], altitude[0][1], altitude[1][1]);
    return res;
}





map* newMapFromChunks(int map_width, int map_height, chunk* chunks[map_width * map_height], chunk* virtual_chunks[(map_height+2+map_width+2)*2-4], unsigned int display_loading)
{
    // clock_t start_time = clock();

    if (chunks != NULL && map_width > 0 && map_height > 0)
    {
        map* new_map = calloc(1, sizeof(map));

        new_map->map_width = map_width;
        new_map->map_height = map_height;

        // Copy chunks list to ensure dynamic allocation
        chunk** chunks_list = calloc(map_width * map_height, sizeof(chunk*));
        for (int i = 0; i < map_width * map_height; i++)
        {
            // chunks_list[i] = copyChunk(chunks[i]);
            // freeChunk(chunks[i]);
            chunks_list[i] = chunks[i];
        }
        new_map->chunks = chunks_list;

        // copy virtual chunks list to ensure dynamic allocation
        chunk** v_chunks_list = calloc((map_height+2+map_width+2)*2-4, sizeof(chunk*));
        for (int i = 0; i < (map_height+2+map_width+2)*2-4; i++)
        {
            // v_chunks_list[i] = copyChunk(virtual_chunks[i]);
            // freeChunk(virtual_chunks[i]);
            v_chunks_list[i] = virtual_chunks[i];
        }
        new_map->virtual_chunks = v_chunks_list;


        // Initialize map parameters
        chunk* firstChunk = getChunk(new_map, 0, 0);
        int chunk_width = firstChunk->width;
        int chunk_height = firstChunk->height;

        new_map->chunk_width = chunk_width;
        new_map->chunk_height = chunk_height;


        // int width = map_width * chunk_width;
        // int height = map_height * chunk_height;
        // double* map_values = calloc(width * height, sizeof(double));

        // new_map->map_values = map_values;

        // // Copy the correct altitude values
        // for (int i = 0; i < height; i++)
        // {
        //     for (int j = 0; j < width; j++)
        //     {
        //         double* value = getMapValue(new_map, j, i);
        //         chunk* current_chunk = getChunk(new_map, j/chunk_width, i/chunk_height);

        //         *value = *getChunkValue(current_chunk, j%chunk_width, i%chunk_height);
                
        //         if (display_loading != 0)
        //         {
        //             int nb_indents = display_loading - 1;

        //             char base_str[100] = "Generating map values...           ";

        //             predefined_loading_bar(j + i * width, width * height - 1, NUMBER_OF_SEGMENTS, base_str, nb_indents, start_time);
        //         }
        //     }
        // }


        new_map = addMeanAltitude(new_map,display_loading);

        return new_map;
    }
    else
    {
        return NULL;
    }
}



map* newMap(int number_of_layers, int gradGrids_width[number_of_layers], int gradGrids_height[number_of_layers],
                 int size_factors[number_of_layers], double layers_factors[number_of_layers], int map_width, int map_height, unsigned int display_loading)
{
    clock_t start_time = clock();

    chunk* chunks[map_width * map_height];
    chunk* virtual_chunks[(map_height+2+map_width+2)*2-4];

    int c_loading = display_loading;
    
    if (display_loading != 0)
    {
        indent_print(display_loading - 1, "Chunks generation before generating map...\n");
        // Chunks loading bars will be indenting twice more.
        c_loading += 2;
    }

    // Generates the chunks
    for (int i = 0; i < map_height; i++)
    {
        for (int j = 0; j < map_width; j++)
        {
            clock_t chunk_start_time = clock();

            chunk* current_chunk = NULL;

            if (display_loading != 0)
            {
                char to_print[200] = "";
                snprintf(to_print, sizeof(to_print), "Generating chunk %d/%d...\n", i * map_width + j + 1, map_width * map_height);

                indent_print(display_loading, to_print);
            }

            if (i == 0 && j == 0)
            {
                // First chunk
                current_chunk = newChunk(number_of_layers, gradGrids_width, gradGrids_height, size_factors, layers_factors, c_loading);
            }
            else
            {
                // Others : adjacent chunks
                chunk* north_chunk = NULL;
                chunk* west_chunk = NULL;

                if (j > 0)
                {
                    west_chunk = chunks[i * map_width + j - 1];
                }
                if (i > 0)
                {
                    north_chunk = chunks[(i - 1) * map_width + j];
                }

                current_chunk = newAdjacentChunk(north_chunk, west_chunk, c_loading);
            }

            chunks[i * map_width + j] = current_chunk;
            
            if (display_loading != 0)
            {
                double total_time = (double) (clock() - chunk_start_time)/CLOCKS_PER_SEC;
                char final_string[200] = "";

                snprintf(final_string, sizeof(final_string), "%sSUCCESS :%s The chunk generation took a total of %.4lf second(s) in CPU time.\n",
                                        GREEN_COLOR, DEFAULT_COLOR, total_time);
                
                int nb_indents = display_loading;
                indent_print(nb_indents, final_string);

                indent_print(display_loading, "\n");
            }
        }
    }    

    clock_t v_start_time=clock();
    for (int j = 0; j < (map_height+2+map_width+2)*2-4; j++)
    {
        chunk* current_chunk = NULL;
        
        if (display_loading != 0)
        {
            int nb_indents = display_loading - 1;

            char base_str[100] = "Generating virtual chunks...       ";

            predefined_loading_bar(j+1,(map_height+2+map_width+2)*2-4, NUMBER_OF_SEGMENTS, base_str, nb_indents, v_start_time);
        }

        current_chunk = newVirtualChunk(number_of_layers, gradGrids_width, gradGrids_height, size_factors, layers_factors);

        virtual_chunks[j] = current_chunk;
    }

    // Generating the map from the new chunks
    map* new_map = newMapFromChunks(map_width, map_height, chunks, virtual_chunks, display_loading);

    if (display_loading == 1)
    {
        double total_time = (double) (clock() - start_time)/CLOCKS_PER_SEC;
        char final_string[200] = "";

        snprintf(final_string, sizeof(final_string), "%sSUCCESS :%s The map generation took a total of %.4lf second(s) in CPU time.\n",
                                GREEN_COLOR, DEFAULT_COLOR, total_time);
        
        int nb_indents = display_loading - 1;
        indent_print(nb_indents, final_string);
    }

    return new_map;
}





map* copyMap(map* p_map) 
{
    map* res = calloc(1, sizeof(map));

    res->map_width=p_map->map_width;
    res->map_height=p_map->map_height;
    res->chunk_width=p_map->chunk_width;
    res->chunk_height=p_map->chunk_height;

    res->chunks = calloc(res->chunk_width*res->chunk_height,sizeof(chunk*));
    for (int i=0; i<res->map_width; i++)
    {
        for (int j=0; j<res->map_height; j++)
        {
            res->chunks[i*res->map_width+j]=copyChunk(p_map->chunks[i*p_map->map_width+j]);
        }
    }

    // int n = res->chunk_width*res->map_width;
    // int m = res->chunk_height*res->map_height;

    // res->map_values = calloc(n*m, sizeof(double));

    // for (int i=0; i<n; i++)
    // {
    //     for (int j=0; j<m; j++)
    //     {
    //         *getMapValue(res,i,j)=*getMapValue(p_map,i,j);
    //     }
    // }

    return res;
}





void writeMapFile(map* map, char path[])
{
    FILE* f = NULL;

    f = fopen(path, "w");

    if (f != NULL)
    {
        fprintf(f, "Map\n");

        // Writing the parameters
        int map_width = map->map_width;
        int map_height = map->map_height;
        int chunk_width = map->chunk_width;
        int chunk_height = map->chunk_height;

        fprintf(f, "map_width=%d\nmap_height=%d\nchunk_width=%d\nchunk_height=%d\n", map_width, map_height, chunk_width, chunk_height);

        int width = map_width * chunk_width;
        int height = map_height * chunk_height;

        // Writing the values
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                double value = *getMapValue(map, j, i);

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



map* readMapFile(char path[]);





void printMap(map* map)
{
    int map_width = map->map_width;
    int map_height = map->map_height;

    int chunk_width = map->chunk_width;
    int chunk_height = map->chunk_height;

    printf("-------------------------------------------\n");
    printf("Printing map of size (%d, %d), with each chunk of size (%d, %d)\n\n", map_height, map_width, chunk_height, chunk_width);

    for (int i = 0; i < map_height * chunk_height; i++)
    {
        for (int j = 0; j < map_width * chunk_width; j++)
        {
            double* value = getMapValue(map, j, i);

            if (j != 0 && j%chunk_width == 0)
            {
                printf(" | ");
            }

            printf("% .4lf   ", *value);
        }
        printf("\n");
        if (i != 0 && i%chunk_height == 0)
        {
            for (int j = 0; j < map_width * chunk_width; j++)
            {
                if (j != 0 && j%chunk_width == 0)
                {
                    printf("   ");
                }

                printf("----------");
            }
            printf("\n");
        }
    }

    printf("-------------------------------------------\n");
}



void freeMap(map* map)
{
    if (map != NULL)
    {
        int map_width = map->map_width;
        int map_height = map->map_height;

        // if (map->map_values != NULL)
        // {
        //     free(map->map_values);
        // }


        if (map->chunks != NULL)
        {
            for (int i = 0; i < map_height; i++)
            {
                for (int j = 0; j < map_width; j++)
                {
                    freeChunk(getChunk(map, j, i));
                }
            }

            free(map->chunks);
        }

        if (map->virtual_chunks != NULL)
        {
            for (int j = 0; j < map_height+2; j++)
            {
                freeChunk(getVirtualChunk(map, 0, j));
                freeChunk(getVirtualChunk(map, map_width+1, j));
            }
            for (int j = 1; j < map_width+1; j++)
            {
                freeChunk(getVirtualChunk(map, j, 0));
                freeChunk(getVirtualChunk(map, j, map_height+1));
            }

            free(map->virtual_chunks);
        }

        free(map);
    }
}
