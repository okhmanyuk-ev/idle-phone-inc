#include "../src/application.h"
#include <platform/system_ios.h>

@interface AppDelegate : UIResponder <UIApplicationDelegate>

@end

@implementation AppDelegate

- (void)runMain {
    PhoneInc::Application().run();
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    [self performSelectorOnMainThread:@selector(runMain) withObject:nil waitUntilDone:NO];
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application {
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
}

- (void)applicationWillTerminate:(UIApplication *)application {
}

@end

int main(int argc, char * argv[]) {
    @autoreleasepool {
        return UIApplicationMain(argc, argv, nil, NSStringFromClass([AppDelegate class]));
    }
}
