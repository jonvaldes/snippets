#include <stdio.h>
#include <string.h>
#include <stdint.h>


void Morton_2D_Decode( uint32_t morton, uint32_t * index1, uint32_t * index2 ) 
{
    // unpack 2 16-bit indices from a 32-bit Morton code 
    uint32_t value1 =   morton        & 0x55555555 ; 
    uint32_t value2 = ( morton >> 1 ) & 0x55555555 ;
    value1 |= ( value1 >> 1 ); 
    value2 |= ( value2 >> 1 ); 
    value1 &= 0x33333333; 
    value2 &= 0x33333333; 
    value1 |= ( value1 >> 2 ); 
    value2 |= ( value2 >> 2 ); 
    value1 &= 0x0f0f0f0f; 
    value2 &= 0x0f0f0f0f; 
    value1 |= ( value1 >> 4 ); 
    value2 |= ( value2 >> 4 ); 
    value1 &= 0x00ff00ff; 
    value2 &= 0x00ff00ff; 
    value1 |= ( value1 >> 8 ); 
    value2 |= ( value2 >> 8 ); 
    value1 &= 0x0000ffff; 
    value2 &= 0x0000ffff; 
    *index1 = value1; 
    *index2 = value2; 
}

void InterleavedMorton( uint32_t order , int * x , int * y )
{
    uint32_t index1 , index2 ;
    Morton_2D_Decode( order >> 2 , &index1 , &index2) ;
    *x = int(index1) ;
    *y = int(index2) ;
    if( order & 0x1 ) *x = -1 - int(index1) ;
    if( order & 0x2 ) *y = -1 - int(index2) ;
}

int main()
{
    const int RADIUS= 20;
    const int ARR_SIZE = RADIUS*2 ;
    int arr[ARR_SIZE*ARR_SIZE];
    int x,y;
    memset(arr , 0 , sizeof(int) * ARR_SIZE*ARR_SIZE) ;

    for( int i = 0 ;i < 900 ; ++i )
    {
        InterleavedMorton(i , &x , &y );
        arr[(x+RADIUS) + (y+RADIUS) * ARR_SIZE] = i ;
    }

    for( int y = 0 ; y < ARR_SIZE ; ++y ){
        for( int x = 0 ; x < ARR_SIZE ; ++x ){
            printf( "%3d," , arr[x + y*ARR_SIZE] ) ;
        }
        printf("\n");
    }
    return 0 ;
}

