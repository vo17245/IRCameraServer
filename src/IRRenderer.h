#include "Image.h"
#include <vector>
#include <mutex>
#include "Buffer.h"
#include <memory>
#include <list>
#include <thread>

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
        static std::mutex s_Mutex;
        static std::list<std::unique_ptr<Buffer>> s_DngBufferList;
        static std::thread* s_WindowThread;
        static const int s_WindowWidth;
        static const int s_WindowHeight;
        static const char* s_WindowName;
        static bool s_isRunning;
    public:
        static Image Render(DNGImage& dng);
        static void CreateWindow();
        static void ShowImage(Image& png);
        static void PushImage(std::unique_ptr<Buffer> dngBuf);
        static bool isDngBufferListEmpty();
        // PopImage() is an unsafe function, check if the s_DngBufferList is empty before calling
        static std::unique_ptr<Buffer> PopImage();
        static void WindowThread();
        static void Run();
        static void Stop();
    };
}
