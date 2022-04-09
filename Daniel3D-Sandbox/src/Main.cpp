#include <SampleApplication.h>
#include <memory>
#include <future>

int main()
{
    auto app = std::make_unique<dg3d::SampleApplication>("Hello App", 800, 600);
    app->Run();

    exit(EXIT_SUCCESS);
}