#include "json/json.h"
#include <iostream>
#include <sys/stat.h>
#define ASSERT(x) if(!(x)){std::cout<<"assert failed in "<<__FILE__<<":"<<__LINE__<<std::endl;exit(-1);}
size_t getFileSize(const char* filepath) 
{
	struct stat statbuf;
	stat(filepath, &statbuf);
	return statbuf.st_size;
}

bool isSame(const char* a,const char* b,int len)
{
    for(int i=0;i<len;i++)
    {
        if(a[i]!=b[i])
        {
            return false;
        }
    }
    return true;
}


int main()
{
    char filePath[]="/root/dev/IRCameraServer/src/test/a.json";
    size_t fileSize=getFileSize(filePath);
    char* data=new char[fileSize];
    FILE* fp;
    fp=fopen(filePath,"rb");
    ASSERT(fp!=nullptr);
    fread(data,fileSize,1,fp);
    json_value_s* value= json_parse(data,fileSize);
    json_object_s* object=(json_object_s*)value->payload;
    ASSERT(object->length==2);
    ASSERT(isSame(object->start->name->string,"name",4));
    delete[] data;
    fclose(fp);
    return 0;
}