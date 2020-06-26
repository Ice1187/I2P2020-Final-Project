#include <UltraOOXX/Wrapper/Porting.h>
// #include "AITemplate.h"
#include "DummyAI.h"
#define AI DummyAI

// Do not edit this!
void* getai()
{
    AIInterface *ptr = new AI();
    return ptr;
}
