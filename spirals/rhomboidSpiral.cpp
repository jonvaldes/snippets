#include <stdio.h>
#include <string.h>

struct ivec2
{
    int x ,y ;
    ivec2():x(0),y(0){}
    ivec2(int _x , int _y) : x(_x) , y(_y){}
    ivec2 operator*(int i) const { return ivec2(x*i,y*i) ; }
    ivec2 operator+(const ivec2 &other) const {return ivec2(x + other.x, y+other.y); }
};


int rhomboidRingElements( int radius ) { return radius * 4 ; }
void calcRhomboidRingPos ( int radius , int index , ivec2 * pos)
{
    const int side = index / radius ;
    const int move = index % radius ;
    const ivec2 startPos[4] = {ivec2(0,radius), ivec2(radius,0), ivec2(0,-radius), ivec2(-radius,0)};
    static const ivec2 dirs[4] = {ivec2(1,-1), ivec2(-1,-1), ivec2(-1,1) , ivec2(1,1) } ;
    *pos = startPos[side] + dirs[side] * move ;
}

int abs(int x){ return x>=0?x:-x; }

int main()
{
    const int RADIUS= 10;
    const int ARR_SIZE = RADIUS*2 ;
    int arr[ARR_SIZE*ARR_SIZE];
    memset(arr , 0 , sizeof(int) * ARR_SIZE*ARR_SIZE) ;

    int counter = 0 ;
    for( int radius = 0 ; radius < 20 ; ++radius )
    {
        for( int i = 0 ; i<rhomboidRingElements(radius) ; ++i )
        {
            ivec2 pos ;
            calcRhomboidRingPos(radius , i , &pos );
            if( abs(pos.x) <=RADIUS-1 && abs(pos.y) <= RADIUS -1 )
                arr[(pos.x+RADIUS) + (pos.y+RADIUS) * ARR_SIZE] = ++counter ;
        }
    }

    for( int y = ARR_SIZE -1 ; y >= 0 ; y-- ) {
        for( int x = 0 ; x < ARR_SIZE ; ++x ){
            printf( "%3d," , arr[x + y*ARR_SIZE] ) ;
        }
        printf("\n");
    }
    return 0 ;
}

