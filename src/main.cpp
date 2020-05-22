#include <dlfcn.h>

#include <functional>
#include <iostream>
#include <memory>

#include <UltraOOXX/Game.h>
#include <UltraOOXX/Wrapper/Content.h>


bool load(const char *libpath, Content &table)
{
    table.handle = dlopen(libpath, RTLD_LAZY);

    if( !table.handle ) {
        std::cout << ".so open:" << dlerror() << std::endl;
        return false;
    }

    bool flag = true;
    auto tryLoad = [&](const char *func)->void*{
        void *func_ptr = dlsym(table.handle, func);
        if( func_ptr == nullptr )
        {
            std::cout << "No sym:" << dlerror() << std::endl;
            flag = false;
            return nullptr;
        }
        return func_ptr;
    };

    table.getai = reinterpret_cast<decltype(table.getai)>(tryLoad("getai"));

    return flag;
}

int main()
{
    Content P1, P2;

    if( !load("./a1.so", P1) ){
        std::cout<<"P1 Fail";
        exit(-1);
    }

    if( !load("./a2.so", P2) ){
        std::cout<<"P2 Fail";
        exit(-1);
    }

    TA::UltraOOXX game;

    game.setPlayer1( (AIInterface*) P1.getai() );
    game.setPlayer2( (AIInterface*) P2.getai() );

    game.run();

    dlclose(P1.handle);
    dlclose(P2.handle);
    return 0;
}
