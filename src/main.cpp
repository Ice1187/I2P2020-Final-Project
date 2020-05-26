#include <dlfcn.h>

#include <functional>
#include <iostream>
#include <memory>

#include <UltraOOXX/Game.h>
#include <UltraOOXX/Wrapper/Content.h>

bool load(const char *libpath, Content &table)
{
    table.handle = dlopen(libpath, RTLD_LAZY);
    // void *dlopen(const char* path, int mode)
    //  path: dynamic shared library 的路徑
    //  mode: 外部函式的 binding 方式
    //
    //  從 libpath load dynamic shared library 進來，並回傳一個針對該 library 的 handle。library 中的 function 採用 lazy binding 機制 (RTLD_LAZY)
    //  * lazy binding: external function 只有在第一次被呼叫時，才會 resolve 其位址

    if (!table.handle)
    {
        // dlopen failed
        std::cout << ".so open:" << dlerror() << std::endl;
        return false;
    }

    bool flag = true;
    auto tryLoad = [&](const char *func) -> void * {
        // tryLoad 為一個 lambda 運算式，執行下面的功能
        void *func_ptr = dlsym(table.handle, func);
        // void *dlsym(void* handle, const char* symbol)
        //   handle: 要搜尋的 library/bundle 的 handle
        //   symbol: 要搜尋的 symbol
        //
        // 從 table.handle 綁定的 library 中搜尋 func，並回傳其位址
        if (func_ptr == nullptr)
        {
            // table.handle 中找不到 func
            std::cout << "No sym:" << dlerror() << std::endl;
            flag = false;
            return nullptr;
        }
        return func_ptr;
    };

    table.getai = reinterpret_cast<decltype(table.getai)>(tryLoad("getai"));
    // 回傳 table.handle(line: 30) 中的 getai function 的位址

    return flag;
}

int main()
{
    Content P1, P2;

    if (!load("./a1.so", P1))
    {
        std::cout << "P1 Fail";
        exit(-1);
    }

    if (!load("./a2.so", P2))
    {
        std::cout << "P2 Fail";
        exit(-1);
    }

    TA::UltraOOXX game;

    game.setPlayer1((AIInterface *)P1.getai());
    game.setPlayer2((AIInterface *)P2.getai());

    game.run();

    dlclose(P1.handle);
    dlclose(P2.handle);
    return 0;
}
