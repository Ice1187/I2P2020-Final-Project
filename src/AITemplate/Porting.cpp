#include <UltraOOXX/Wrapper/Porting.h>
#include "AITemplate.h"

// Do not edit this!
void* getai()
{
    AIInterface *ptr = new AI();
    return ptr;
}
