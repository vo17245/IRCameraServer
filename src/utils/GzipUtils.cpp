#include "GzipUtils.h"
#include <zlib.h>
#include <string.h>

const int GzipUtils::CHUNK_SIZE=16384;
char* GzipUtils::out=new char[GzipUtils::CHUNK_SIZE];
std::vector<char> GzipUtils::Compress(const std::vector<char>& data)
{
    const char* source = data.data();

    // Setup zlib data structures
    z_stream deflateStream;
    memset(&deflateStream, 0, sizeof(deflateStream));
    deflateStream.zalloc = Z_NULL;
    deflateStream.zfree = Z_NULL;
    deflateStream.opaque = Z_NULL;

    // Initialize zlib deflate stream
    if (deflateInit2(&deflateStream, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 31, 8, Z_DEFAULT_STRATEGY) != Z_OK)
    {
        throw GzipException("Failed to initialize the deflate stream");
    }

    std::vector<char> compressedData;

    // Compress the source string
    deflateStream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(source));
    deflateStream.avail_in = strlen(source);

    int flush = Z_NO_FLUSH;
    int ret;
    
    do
    {
        deflateStream.next_out = reinterpret_cast<Bytef*>(&out[0]);
        deflateStream.avail_out = CHUNK_SIZE;

        ret = deflate(&deflateStream, flush);
        if (ret == Z_STREAM_ERROR)
        {
            deflateEnd(&deflateStream);
            throw GzipException("Failed to compress the data");
        }

        int have = CHUNK_SIZE - deflateStream.avail_out;
        if (have > 0)
        {
            compressedData.insert(compressedData.end(), &out[0], &out[have]);
        }
    } while (deflateStream.avail_out == 0);

    // Flush and finish the deflate stream
    flush = Z_FINISH;
    do
    {
        deflateStream.next_out = reinterpret_cast<Bytef*>(&out[0]);
        deflateStream.avail_out = CHUNK_SIZE;

        ret = deflate(&deflateStream, flush);
        if (ret == Z_STREAM_ERROR)
        {
            deflateEnd(&deflateStream);
            throw GzipException("Failed to finish the deflation process.");
        }

        int have = CHUNK_SIZE - deflateStream.avail_out;
        if (have > 0)
        {
            compressedData.insert(compressedData.end(), &out[0], &out[have]);
        }
    } while (deflateStream.avail_out == 0);

    deflateEnd(&deflateStream);

    return compressedData;
}


Buffer GzipUtils::Compress(const Buffer& data)
{
    const char* source = data.GetData();

    // Setup zlib data structures
    z_stream deflateStream;
    memset(&deflateStream, 0, sizeof(deflateStream));
    deflateStream.zalloc = Z_NULL;
    deflateStream.zfree = Z_NULL;
    deflateStream.opaque = Z_NULL;

    // Initialize zlib deflate stream
    if (deflateInit2(&deflateStream, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 31, 8, Z_DEFAULT_STRATEGY) != Z_OK)
    {
        throw GzipException("Failed to initialize the deflate stream");
    }

    Buffer compressedData;

    // Compress the source string
    deflateStream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(source));
    deflateStream.avail_in = strlen(source);

    int flush = Z_NO_FLUSH;
    int ret;
    
    do
    {
        deflateStream.next_out = reinterpret_cast<Bytef*>(&out[0]);
        deflateStream.avail_out = CHUNK_SIZE;

        ret = deflate(&deflateStream, flush);
        if (ret == Z_STREAM_ERROR)
        {
            deflateEnd(&deflateStream);
            throw GzipException("Failed to compress the data");
        }

        int have = CHUNK_SIZE - deflateStream.avail_out;
        if (have > 0)
        {
            compressedData.Push(have,[have](char* data){
                memcpy(data,GzipUtils::out,have);
            });
        }
    } while (deflateStream.avail_out == 0);

    // Flush and finish the deflate stream
    flush = Z_FINISH;
    do
    {
        deflateStream.next_out = reinterpret_cast<Bytef*>(&out[0]);
        deflateStream.avail_out = CHUNK_SIZE;

        ret = deflate(&deflateStream, flush);
        if (ret == Z_STREAM_ERROR)
        {
            deflateEnd(&deflateStream);
            throw GzipException("Failed to finish the deflation process.");
        }

        int have = CHUNK_SIZE - deflateStream.avail_out;
        if (have > 0)
        {
            compressedData.Push(have,[have](char* data){
                memcpy(data,GzipUtils::out,have);
            });
        }
    } while (deflateStream.avail_out == 0);

    deflateEnd(&deflateStream);

    return compressedData;
}
Buffer GzipUtils::Decompress(const Buffer& data)
{
    throw GzipException("NotImplement: Buffer GzipUtils::Decompress(const Buffer& data) ");
}