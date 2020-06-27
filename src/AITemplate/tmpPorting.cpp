#include <UltraOOXX/Wrapper/Porting.h>

#if AITYPE == 1
#include "DummyAI.h"
#define AI DummyAI
#elif AITYPE == 2
#include "RandomAI.h"
#define AI RandomAI
#elif AITYPE == 3
#include "MinMaxAI.h"
#define AI MinMaxAI
#else
#include "AITemplate.h"
#endif

void *getai()
{
    AIInterface *ptr = new AI();
    return ptr;
}
