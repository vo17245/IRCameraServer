#pragma once
#include <string>
#include <vector>
class Image
{
public:
	Image(const std::string& path);
	Image(int width, int height, int channels);
	Image(const char* data,int len);
	~Image();
	int SaveAsPNG(const std::string& path);
	unsigned char* toPNG(int* pngSize);
	void Free(unsigned char* png);
	inline unsigned char* At(int i, int j) { return &m_Data[(i * m_Width + j) * m_Channels]; }
	inline int& GetWidth() { return m_Width; }
	inline int& GetHeight() { return m_Height; }
	inline int& GetChannels() { return m_Channels; }
	inline unsigned char* GetData() { return m_Data; }
private:
	int m_Width;
	int m_Height;
	int m_Channels;
	unsigned char* m_Data;
private:
	/*
	* 0 -> created by stb
	* 1 -> a normal memory created by new
	*/
	int m_DataType;

};

class DNGImage
{
public:
	DNGImage(const std::string& path);
    DNGImage(const char* data,size_t size);
	~DNGImage();
	inline int& GetWidth() { return m_Width; }
	inline int& GetHeight() { return m_Height; }
	inline int& GetBitsPerPixel() { return m_BitsPerPixel; }
	inline int** GetPattern() { return (int**)m_Pattern; }
	inline unsigned char* GetData() { return m_Data.data(); }
	inline unsigned char* At(int i, int j) { return &(m_Data.data()[(i * m_Width + j) * m_BitsPerPixel / 8]); }
	int ColorAt(int i,int j);
private:
	int m_Width;
	int m_Height;
	int m_BitsPerPixel;
	int m_Pattern[2][2];
	std::vector<unsigned char> m_Data;
};