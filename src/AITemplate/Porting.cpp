#include <UltraOOXX/Wrapper/Porting.h>
// #include "AITemplate.h"
// #include "DummyAI.h"
// #define AI DummyAI
#include "MinMaxAI.h"
#define AI FirstWinAI

// Do not edit this!
void* getai()
{
    AIInterface *ptr = new AI();
    return ptr;
}
