#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct ivec2
{
    int x ,y ;
    ivec2 operator*(int i) const { return {x*i,y*i}; }
    ivec2 operator+(const ivec2 &other) const { return {x + other.x, y+other.y} ; }
};

/*
// Original implementation, easier to understand
ivec2 pointOnDiscreteSpiral( int x )
{
    const int n = ceil(sqrtf(4*x+1)*0.5f-1 + 0.5f) ; // 'n' is side-length of L-shaped spiral block 
    const int even = !(n & 0x1) ; // Odd-length blocks are bottom-left, even-length blocks are top-right
    const int lastx = n*(n+1) ; // Which was the last tile covered by block of length n?
    const int firstBlockSection = ( lastx - x < n ) ; // Each block has two sections. First sections are top and bottom, second sections are left and right
    const int sectionIdx = even * 2 + firstBlockSection ; // 0:bottom , 1:left , 2: top , 3 :right 
    const ivec2 startPositions[4] = { {0,0},{-1,0},{0,0},{0,0} } ;
    const ivec2     spreadDirs[4] = { {1,-1},{-1,-1},{-1,1},{1,1} } ;
    const ivec2     directions[4] = { {-1,0},{0,1},{1,0},{0,-1} } ;
    const ivec2 startPos = startPositions[sectionIdx] + spreadDirs[sectionIdx] * (n/2);
    const int offset = x - (lastx - 2*n) - firstBlockSection*n ;
    return startPos + directions[sectionIdx] * offset ;
}
*/

// Compact implementation
ivec2 pointOnDiscreteSpiral( int x )
{
    const int n = ceil(sqrtf(4*x+1)*0.5f-1 + 0.5f) ;
    const int even = !(n & 0x1) ;
    const int lastx = n*(n+1) ; 
    const int firstBlockSection = ( lastx - x < n ) ; 
    const int topLeft = 2*(even ^ firstBlockSection) - 1 ; 
    const ivec2 spreadDir{ -topLeft , 2*even - 1 } ;
    const ivec2 startPos = ivec2{ 0 - (!even && firstBlockSection) ,0} + spreadDir * (n/2) ;
    const ivec2 dir =  ivec2{!firstBlockSection, firstBlockSection} * topLeft ;
    const int offset = x - lastx + 2*n - firstBlockSection * n ;
    return startPos + dir * offset ;
}

int main()
{
    const int RADIUS= 10;
    const int ARR_SIZE = RADIUS*2 ;
    int arr[ARR_SIZE*ARR_SIZE];
    memset(arr , -1 , sizeof(int) * ARR_SIZE*ARR_SIZE) ;

    for( int i = 0 ; i< 300 ; ++i ){
        ivec2 pos = pointOnDiscreteSpiral(i);
        arr[(pos.x+RADIUS) + (pos.y+RADIUS) * ARR_SIZE] = i ;
    }

    for( int y = ARR_SIZE -1 ; y >= 0 ; y-- ) {
        for( int x = 0 ; x < ARR_SIZE ; ++x ) {
            printf( "%3d,%s" , arr[x + y*ARR_SIZE] , x==ARR_SIZE-1?"\n":"" ) ;
        }
    }
    return 0 ;
}

