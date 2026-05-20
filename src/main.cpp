#include "Core/Application.h"

#include <exception>
#include <iostream>

int main()
{
    try
    {
        ogl::Application app;
        app.Run();
        return 0;
    }
    catch (const std::exception &error)
    {
        std::cerr << "OpenGLRenderer error: " << error.what() << '\n';
        return 1;
    }
}
