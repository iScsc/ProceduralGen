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

int main() {

    __uint8_t d = 1;
    int a=0,e=-5683;
    double b=1.3,c=-0.394383;

    printf("Encoding some numbers : %d, %d, %d, %f, %f\n",d,a,e,b,c);

    bytes db=bytesUint8(d);
    bytes ab=bytesInt(a);
    bytes eb=bytesInt(e);
    bytes bb=bytesDouble(b);
    bytes cb=bytesDouble(c);

    printf("\tUint8 :  %d, ",d);
    printBytes(db,"","\n");
    printf("\tint :    %d, ",a);
    printBytes(ab,"","\n");
    printf("\tint :    %d, ",e);
    printBytes(eb,"","\n");
    printf("\tdouble : %lf, ",b);
    printBytes(bb,"","\n");
    printf("\tdouble : %lf, ",c);
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

    printf("Decoding numbers : %d, %d, %d, %f, %f\n\n\n",d,a,e,b,c);


    gradientGrid* grid = newRandomGradGrid(1,1,0);
    printGradientGrid(grid);

    bytes gridb = bytesGradientGrid(grid);
    printBytes(gridb, "", "\n");

    gradientGrid* gridd = ((gradientGrid*)nextGradientGrid(gridb).object);
    printGradientGrid(gridd);

    printf("\n\n");


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



    int grid_size[] = {2};
    int size_factor[] = {3};
    double layer_factor[] = {1.};
    chunk* chk = newChunk(1,grid_size,grid_size,size_factor,layer_factor,0);
    printChunk(chk);

    bytes chkb = bytesChunk(chk);
    printBytes(chkb,"","\n");

    chunk* chkd = ((chunk*)nextChunk(chkb).object);
    printChunk(chkd);



    map* m = newMap(1,grid_size,grid_size,size_factor,layer_factor,1,1,0);
    printMap(m);

    bytes mb = bytesMap(m);
    printBytes(mb,"","\n");

    map* md = ((map*)nextMap(mb).object);
    printMap(md);



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

    return 0;
}