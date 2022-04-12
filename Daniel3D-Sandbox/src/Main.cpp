#include <SampleApplication.h>
#include <memory>
#include <future>

int main()
{
    auto app = std::make_unique<dg3d::SampleApplication>("Adventure Game", 1024, 768);
    app->Run();

    exit(EXIT_SUCCESS);
}