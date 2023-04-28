#include "Image.h"
#include <vector>
#include <mutex>
#include "Buffer.h"
#include <memory>
#include <list>

namespace ircs
{
    class Frame
    {
    private:
        size_t m_Index;
        std::unique_ptr<Buffer> m_DngBuf;
    public:
        Frame(size_t index,std::unique_ptr<Buffer> dngBuf);

    };
    class IRRenderer
    {
    private:
        static std::mutex m_Mutex;
        static std::list<std::unique_ptr<Buffer>> m_DngBufferList; 
    public:
        static Image Render(DNGImage& dng);
        static void CreateWindow();
        static void ShowImage(Image& png);
        static void PushImage(std::unique_ptr<Buffer> dngBuf);
        static std::unique_ptr<Buffer> PopImage();
    };
}
