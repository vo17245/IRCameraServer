#include <iostream>
#define ASSERT(x) if(!(x)){std::cout<<"assert failed in "<<__FILE__<<":"<<__LINE__<<std::endl;exit(-1);}
int main()
{
    FILE* fp;
    fp=fopen("res/suiginton.png","rb");
    ASSERT(fp!=nullptr);
    fclose(fp);
    return 0;
}