#include <stdio.h>
#include <stdlib.h>
#include "read_bmp.h"
#include "draw.h"

big_endian = 1;

int *array;

int main() {

//    freopen( "out.txt", "w+", stdout );

    char filename[127];
    Image img;

//    scanf( "%s", filename );

    if( load_image( "img2.bmp", &img ) ) {

        draw( &img );
        free( img.gray );
    }


    return 0;
}
