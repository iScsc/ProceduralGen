/**
 * @file test_interpreter.c
 * @author BlueNZ
 * @brief a testing script for the interpreter functions
 * @version 0.1
 * @date 2024-07-27
 * 
 */

#include <stdio.h>
#include <malloc.h>

#include "interpreter.h"
#include "gradientGrid.h"
#include "layer.h"
#include "chunk.h"
#include "map.h"
#include "mapGenerator.h"

int main() {
    printf("Testing binary encoding and decoding: \n\n\n");

    __uint8_t d = 1;
    int a=0,e=-5683;
    double b=1.3,c=-0.394383;

    printf("\tEncoding some numbers : %d, %d, %d, %f, %f\n",d,a,e,b,c);

    bytes db=bytesUint8(d);
    bytes ab=bytesInt(a);
    bytes eb=bytesInt(e);
    bytes bb=bytesDouble(b);
    bytes cb=bytesDouble(c);

    printf("\t\tUint8 :  %d, ",d);
    printBytes(db,"","\n");
    printf("\t\tint :    %d, ",a);
    printBytes(ab,"","\n");
    printf("\t\tint :    %d, ",e);
    printBytes(eb,"","\n");
    printf("\t\tdouble : %lf, ",b);
    printBytes(bb,"","\n");
    printf("\t\tdouble : %lf, ",c);
    printBytes(cb,"","\n");

    object od = nextUint8(db).object;
    object oa = nextInt(ab).object;
    object oe = nextInt(eb).object;
    object ob = nextDouble(bb).object;
    object oc = nextDouble(cb).object;

    d = *((__uint8_t*)od);
    a = *((int*)oa);
    e = *((int*)oe);
    b = *((double*)ob);
    c = *((double*)oc);

    printf("\tDecoding numbers : %d, %d, %d, %f, %f\n\n\n",d,a,e,b,c);



    printf("\tTesting for gradients:\n");
    gradientGrid* grid = newRandomGradGrid(1,1,0);
    printGradientGrid(grid);

    bytes gridb = bytesGradientGrid(grid);
    printBytes(gridb, "", "\n");

    gradientGrid* gridd = ((gradientGrid*)nextGradientGrid(gridb).object);
    printGradientGrid(gridd);

    printf("\n\n");


    printf("\tTesting for layers:\n");
    layer* lay = newLayer(2,2,3,0);
    printLayer(lay);

    bytes laybf = bytesLayer(lay,false);
    printBytes(laybf,"","\n");
    bytes laybt = bytesLayer(lay,true);
    printBytes(laybt,"","\n");

    layer* laydff = ((layer*)nextLayer(laybf,false).object);
    printLayer(laydff);
    layer* laydft = ((layer*)nextLayer(laybf,true).object);
    printLayer(laydft);
    layer* laydtf = ((layer*)nextLayer(laybt,false).object);
    printLayer(laydtf);
    layer* laydtt = ((layer*)nextLayer(laybt,true).object);
    printLayer(laydtt);



    printf("\n\n\tTesting for chunks:\n");
    int grid_size[] = {2};
    int size_factor[] = {3};
    double layer_factor[] = {1.};
    chunk* chk = newChunk(1,grid_size,grid_size,size_factor,layer_factor,0);
    printChunk(chk);

    bytes chkb = bytesChunk(chk);
    printBytes(chkb,"","\n");

    chunk* chkd = ((chunk*)nextChunk(chkb).object);
    printChunk(chkd);



    printf("\n\n\tTesting for maps:\n");
    map* m = newMap(1,grid_size,grid_size,size_factor,layer_factor,1,1,0);
    printMap(m);

    bytes mb = bytesMap(m);
    printBytes(mb,"","\n");

    map* md = ((map*)nextMap(mb).object);
    printMap(md);



    printf("\n\n\tTesting for colors:\n");
    color col = {255,0,0};
    printf("color: %u - %u - %u\n", col.red, col.green, col.blue);
    bytes colb = bytesColor(col);
    printBytes(colb,"","\n");
    color* cold = ((color*)nextColor(colb).object);
    printf("color: %u - %u - %u\n", cold->red, cold->green, cold->blue);

    printf("\n\n\tTesting for completeMaps:\n");
    completeMap* cmap = newCompleteMap(1,grid_size,grid_size,size_factor,layer_factor,1,1,0,0);
    printMap(cmap->map);
    bytes cmapb = bytesCompleteMap(cmap);
    printBytes(cmapb,"","\n");
    completeMap* cmapd = ((completeMap*)nextCompleteMap(cmapb).object);
    printMap(cmapd->map);



    printf("\n\n\tTesting file writing and reading...\n");
    writeBytesFile(cmapb, "../saves","test.data");
    bytes cmapbr = readBytesFile("../saves/test.data");
    printBytes(cmapb,"","\n");
    printBytes(cmapbr,"","\n");



    printf("\n\nDeallocating...\n");
    free(od);
    free(oa);
    free(oe);
    free(ob);
    free(oc);
    freeBytes(db);
    freeBytes(ab);
    freeBytes(eb);
    freeBytes(bb);
    freeBytes(cb);


    freeGradGrid(grid);
    freeBytes(gridb);
    freeGradGrid(gridd);


    freeLayer(lay);
    freeBytes(laybf);
    freeBytes(laybt);
    freeLayer(laydff);
    freeLayer(laydft);
    freeLayer(laydtf);
    freeLayer(laydtt);

    freeChunk(chk);
    freeBytes(chkb);
    freeChunk(chkd);

    freeMap(m);
    freeBytes(mb);
    freeMap(md);

    freeBytes(colb);
    free(cold);

    freeCompleteMap(cmap);
    freeBytes(cmapb);
    freeCompleteMap(cmapd);
    freeBytes(cmapbr);

    return 0;
}