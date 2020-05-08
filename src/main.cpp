#include "application.h"

#if !defined(PLATFORM_IOS)
int main(int argc, char* argv[])
{
    PhoneInc::Application().run();
    return 0;
}
#endif