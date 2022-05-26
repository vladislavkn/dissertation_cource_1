#include "Application.h"

int main()
{
    Application app(nullptr);
    app.build_objects_tree();
    
    return app.start();
}
