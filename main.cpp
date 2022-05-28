#include "System.h"

int main()
{
    System app(nullptr);
    app.build_objects_tree();
    
    return app.start();
}
