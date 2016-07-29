#include "read_bmp.h"
#ifndef H_DRAW_H
#define H_DRAW_H

char get_brightness( unsigned int value ) {

//    printf( "%hu ", value );
    char ret;

    if( value >= 230 ) {

        ret = ' ';
    } else if( value >= 200 ) {

        ret = '.';
    } else if( value >= 180 ) {

        ret = ':';
    } else if( value >= 160 ) {

        ret = 'o';
    } else if( value >= 130 ) {

        ret = '&';
    } else if( value >= 100 ) {

        ret = '*';
    } else if( value >= 70 ) {

        ret = '8';
    } else if( value >= 50 ) {

        ret = '#';
    } else {

        ret = '@';
    }
    return ret;
}

void draw( Image *image ) {

    int avg, k = 0;

//    for( int i=0; i<image->size; i+=3 ) {
//        avg = image->data[i];
//        avg += image->data[i+1];
//        avg += image->data[i+2];
//        gray[k++] = avg / 3;
//    }

    k = 0;
    for( int i=0; i<image->sizeY; i++ ) {
        for( int j=0; j<image->sizeX; j++ ) {
//            printf( "%4d", gray[k] );
            putchar( get_brightness( image->gray[ k++ ] ) );
//            get_brightness( gray[ k++ ] );
        }
        putchar( '\n' );
    }

}

#endif // H_DRAW_H
