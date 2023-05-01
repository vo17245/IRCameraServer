#pragma once
#include <vector>
#include <exception>
#include <string>
#include "Buffer.h"
class GzipException:public std::exception
{
private:
    std::string m_Msg;
public:
    GzipException(const std::string& msg)
        :m_Msg(msg){}
    GzipException(const char* msg)
        :m_Msg(msg){}
    const char* what()const noexcept{return m_Msg.c_str();}
};
class GzipUtils
{
private:
    static const int CHUNK_SIZE;
    static char* out;
public:
    static std::vector<char> Compress(const std::vector<char>& data);
    static std::vector<char> Decompress(const std::vector<char>& data);
    static Buffer Compress(const Buffer& data);
    static Buffer Decompress(const Buffer& data);
};