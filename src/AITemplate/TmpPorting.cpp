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
#elif AITYPE == 4
#include "FirstWinAI.h"
#define AI FirstWinAI
#elif AITYPE == 5
#include "ManualAI.h"
#define AI ManualAI
#elif AITYPE == 6
#include "FwSmmAI.h" // First-Win and Secone-MinMax AI
#define AI FwSmmAI
#else
#include "AITemplate.h"
#endif

void *getai()
{
    AIInterface *ptr = new AI();
    return ptr;
}
