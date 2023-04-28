#include "Image.h"
#include <vector>
namespace ircs
{
    class IRRenderer
    {
    public:
        static Image Render(DNGImage& dng);
        static void CreateWindow();
        static void ShowImage(Image& png);
    };
}
