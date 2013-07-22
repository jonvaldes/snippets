#include <stdio.h>

struct Base
{
    virtual void func() = 0 ;
};


struct Child : public Base
{
    virtual void func() { Base::func() ; }
};

void Base::func() { printf("Base says hey!\n"); }

int main()
{
    Child c ;
    c.func() ;
    return 0;
}


