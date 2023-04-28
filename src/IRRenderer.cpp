#include "IRRenderer.h"
#include "Log.h"
#include <opencv2/opencv.hpp>
#include "opencv2/imgcodecs/legacy/constants_c.h"




namespace ircs
{
    std::mutex IRRenderer::m_Mutex;
    std::list<std::unique_ptr<Buffer>> IRRenderer::m_DngBufferList; 
    unsigned char color[]=
    {0x00,0x00,0x00,
    0x10,0x07,0x07,
    0x20,0x10,0x0E,
    0x30,0x1A,0x15,
    0x40,0x23,0x1C,
    0x50,0x2C,0x23,
    0x60,0x35,0x2A,
    0x70,0x3E,0x31,
    0x80,0x47,0x3A,
    0x90,0x4F,0x41,
    0xA0,0x58,0x48,
    0xB0,0x61,0x4F,
    0xC0,0x6A,0x56,
    0xD0,0x73,0x5D,
    0xE0,0x7C,0x64,
    0xF0,0x85,0x6B,
    0xFF,0x8E,0x72,
    0xFF,0x97,0x7A,
    0xFF,0xA0,0x82,
    0xFF,0xAA,0x8A,
    0xFF,0xB3,0x91,
    0xFF,0xBC,0x99,
    0xFF,0xC5,0xA1,
    0xFF,0xCE,0xA9,
    0xFF,0xD7,0xB0,
    0xFF,0xE0,0xB8,
    0xFF,0xE9,0xC0,
    0xFF,0xF2,0xC8,
    0xFF,0xFB,0xD0,
    0xFF,0xF4,0xD9,
    0xFF,0xEE,0xD2,
    0xFF,0xE8,0xCC,
    0xFF,0xE2,0xC5,
    0xFF,0xDD,0xBC,
    0xFF,0xD7,0xB6,
    0xFF,0xD1,0xAF,
    0xFF,0xCB,0xA9,
    0xFF,0xC5,0xA2,
    0xFF,0xBF,0xA5,
    0xFF,0xB9,0xA9,
    0xFF,0xB3,0xAD,
    0xFF,0xAD,0xA0,
    0xFF,0xA7,0x9D,
    0xFF,0xA1,0x96,
    0xFF,0x9B,0x8F,
    0xFF,0x95,0x89,
    0xFF,0x8F,0x82,
    0xFF,0x89,0x7C,
    0xFF,0x83,0x75,
    0xFF,0x7D,0x6F,
    0xFF,0x77,0x68,
    0xFF,0x71,0x62,
    0xFF,0x6B,0x5B,
    0xFF,0x65,0x55,
    0xFF,0x5F,0x4E,
    0xFF,0x59,0x48,
    0xFF,0x53,0x41,
    0xFF,0x4D,0x3B,
    0xFF,0x47,0x34,
    0xFF,0x41,0x2E,
    0xFF,0x3B,0x27,
    0xFF,0x35,0x1F,
    0xFF,0x2F,0x18,
    0xFF,0x29,0x0D};
    void light2color(unsigned short light,
                    unsigned short min_light,
                    unsigned short max_light,
                    unsigned char& r,
                    unsigned char& g,
                    unsigned char& b)
    {
        float a=light;
        a-=min_light;
        a/=(max_light-min_light);
        a*=63;
        int i=(int)a;
        unsigned char* p=&color[i*3];
        r=p[0];
        g=p[1];
        b=p[2];
        //std::cout<<light<<" "<<i<<" "<<(unsigned)r<<" "<<(unsigned)g<<" "<<(unsigned)b<<std::endl;
        //std::cin.get();
    }
    Image IRRenderer::Render(DNGImage& dng)
    {
        int cols=dng.GetWidth();
        int rows=dng.GetHeight();
        unsigned short max_light=0;
        unsigned short min_light=UINT16_MAX;
        unsigned short light;
        unsigned char r,g,b;
        for(int col=0;col<cols;col++)
        {
            for(int row=0;row<rows;row++)
            {
                light=*(unsigned short*)dng.At(row,col);
                if(light<min_light)
                {
                    min_light=light;
                }
                else if(light>max_light)
                {
                    max_light=light;
                }
            }
        }
    
        ASSERT(dng.GetBitsPerPixel()==16);
        Image png(dng.GetWidth(),dng.GetHeight(),3);

        unsigned char* p;
        for(int col=0;col<cols;col++)
        {
            for(int row=0;row<rows;row++)
            {
                light=*(unsigned short*)dng.At(row,col);
                light2color(light,min_light,max_light,r,g,b);
                p=png.At(row,col);
                p[0]=r;
                p[1]=g;
                p[2]=b;
            }
        }
        return png;
    }
    void IRRenderer::CreateWindow()
    {
        cv::namedWindow("IRRenderer");
        cv::resizeWindow("IRRenderer", 640, 480);
    }
    void IRRenderer::ShowImage(Image& png)
    {
        
        //cv::Mat m(png.GetHeight(),png.GetWidth(),(((0) & ((1 << 3) - 1)) + (((3)-1) << 3)),png.GetData());
        //cv::imshow("IRRenderer",m);
        //cv::waitKey(100);

        cv::Mat m(png.GetHeight(),png.GetWidth(),(((0) & ((1 << 3) - 1)) + (((3)-1) << 3)),png.GetData());
        cv::cvtColor(m, m, cv::COLOR_RGB2BGR);
        cv::imshow("IRRenderer",m);
        cv::waitKey(100);
    }
    void IRRenderer::PushImage(std::unique_ptr<Buffer> dngBuf)
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        m_DngBufferList.push_front(std::move(dngBuf));        
    }
    std::unique_ptr<Buffer> IRRenderer::PopImage()
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        std::unique_ptr<Buffer> buf=std::move(m_DngBufferList.back());
        m_DngBufferList.pop_back();
        return buf;
    }

    Frame::Frame(size_t index,std::unique_ptr<Buffer> dngBuf)
        :m_Index(index),m_DngBuf(std::move(dngBuf))
    {

    }
    
}
