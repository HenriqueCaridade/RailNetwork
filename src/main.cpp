
#include "App.h"
#include <string>
#include <Windows.h>
using namespace std;

int main() {
    //SetConsoleOutputCP(CP_UTF8);
    //setvbuf(stdout, nullptr, _IOFBF, 1000);

    App().start();
    return 0;
}
