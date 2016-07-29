//#include <stdio.h>
//#include <stdlib.h>
#ifndef H_READBMP_H
#define H_READBMP_H

int big_endian;


typedef struct {
    unsigned int sizeX;
    unsigned int sizeY;
    unsigned int paddedX;
    unsigned int size;
    unsigned short int planes;
    unsigned short int bpp;
    unsigned char *gray;
} Image;

unsigned int read_long( FILE *file ) {

    char buff[4];
    int ret;

    if( fread( buff, 1, 4, file ) < 4 ) {
        return 0;
    }

    if( big_endian )
        ret = ( buff[3] << 24 ) | ( buff[2] << 16 ) | ( buff[1] << 8 ) | buff[0];
    else
        ret = ( buff[0] << 24 ) | ( buff[1] << 16 ) | ( buff[2] << 8 ) | buff[3];

    return ret;
}

unsigned short int read_short( FILE *file ) {

    char buff[4];
    short int ret;

    if( fread( buff, 1, 2, file ) < 2 ) {
        return 0;
    }

    if( big_endian )
        ret = ( buff[1] << 8 ) | buff[0];
    else
        ret = ( buff[0] << 8 ) | buff[1];

    return ret;
}

int load_image( char *filename, Image *image ) {

    FILE *file;

    if( ( file = fopen( filename, "rb" ) ) == NULL ) {
        printf( "File Not Found!: %s\n", filename );
        return 0;
    }

    char c1, c2;
    fscanf( file, "%c%c", &c1, &c2 );
    printf( "%c %c\n", c1, c2 );
    fseek( file, 10, SEEK_SET );
    printf( "Offset: %d\n", read_short( file ) );
    fseek( file, 18, SEEK_SET );

    if( !( image->sizeX = read_long( file ) ) ) {
        printf( "Error loading Width: %s\n", filename );
        return 0;
    }

    printf( "Width: %u\n", image->sizeX );

    if( !( image->sizeY = read_long( file ) ) ) {
        printf( "Error loading Height: %s\n", filename );
        return 0;
    }

    printf( "Height: %u\n", image->sizeY );

    image->paddedX = image->sizeX;
    if( image->paddedX % 4 != 0 ) {
        image->paddedX = 4 - ( image->paddedX % 4 );
    }
    image->size = ( image->sizeX + image->paddedX ) * image->sizeY * 3;

    if( !( image->planes = read_short( file ) ) ) {
        printf( "Error reading planes: %s\n", filename );
        return 0;
    }

    if( image->planes != 1 ) {
        printf( "Planes not 1: %s %hu\n", filename, image->planes );
        return 0;
    }

    if( !( image->bpp = read_short( file ) ) ) {
        printf( "Error reading bpp: %s %hu\n", filename, image->bpp );
        return 0;
    }

    if( image->bpp != 24 ) {
        printf( "BPP not 24: %s %hu\n", filename, image->bpp );
        return 0;
    }

    unsigned char *data = (unsigned char*)malloc( image->sizeX * 3  );
    image->gray = (unsigned char*)malloc( image->sizeX * image->sizeY );

    if( data == NULL || image->gray == NULL ) {
        printf( "Error allocating memory for pixel data: %s\n", filename );
        return 0;
    }

    fseek( file, 54, SEEK_SET );

    int k = 0;
    for( int i=0; i<image->sizeY; i++ ) {
        if( fread( data, image->sizeX*3, 1, file ) != 1 ) {
            printf( "%d\n", i );
            printf( "Error reading pixel data: %s\n", filename );
            return 0;
        }
        if( image->sizeX % 4 ) {
            fseek( file, 4 - ( image->sizeX % 4 ), SEEK_CUR );
            fread( data, 4 - ( image->sizeX % 4 ), 1, file );
        }
        for( int j=0; j<image->sizeX*3; j+=3 ) {
            image->gray[k++] = ( data[j] + data[j+1] + data[j+2] ) / 3;
        }
    }
    printf( "Loaded %s successfully \n", filename );
    fclose( file );

    free( data );
    return 1;
}

#endif // H_READBMP_H
