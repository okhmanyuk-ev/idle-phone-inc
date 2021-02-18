#include "application.h"

#if defined(PLATFORM_IOS)
#import <FBSDKLoginKit/FBSDKLoginKit.h>
#include <platform/system_ios.h>
#endif

using namespace PhoneInc;

Application::Application() : Shared::Application(PROJECT_NAME, { Flag::Audio, Flag::Scene })
{
	PLATFORM->setTitle(PRODUCT_NAME);
	PLATFORM->resize(360, 640);
#if !defined(PLATFORM_MOBILE)
	PLATFORM->rescale(1.5f);
#endif
	RENDERER->setVsync(true);

	GRAPHICS->setSdfSmoothFactor(Helpers::Scale);

	STATS->setAlignment(Shared::StatsSystem::Align::BottomRight);

	ENGINE->addSystem<Microtasks>(std::make_shared<Microtasks>());
	ENGINE->addSystem<Profile>(std::make_shared<Profile>());
	
	PROFILE->load();
	PROFILE->setNightBackground(!PROFILE->isNightBackground());

	MICROTASKS->checkForCompletion();

	PLATFORM->initializeBilling({
		//{ "rubies.001", [this] { 
		//	addRubies(500);
		//} }
	});

	initializeScene();

#if defined(BUILD_DEVELOPER)
	CONSOLE->execute("hud_show_fps 1");
	CONSOLE->execute("hud_show_drawcalls 1");
#else
    CONSOLE_DEVICE->setEnabled(false);
    STATS->setEnabled(false);
#endif

#if defined(PLATFORM_IOS)
    /*auto loginButton = [[FBSDKLoginButton alloc] init];
    auto window = Platform::SystemIos::Window;
    auto rootView = [[window rootViewController] view];
    loginButton.center = rootView.center;
    [rootView addSubview:loginButton];*/
#endif
}

Application::~Application()
{
	PROFILE->save();
	ENGINE->removeSystem<TutorialSystem>();
}

void Application::onFrame()
{
	adaptToScreen(getScene()->getRoot(), { 1080.0f, 1920.0f });
	Cheats::ShowDevMenu();
}

void Application::adaptToScreen(std::shared_ptr<Scene::Node> node, const glm::vec2& dimensions)
{
	glm::vec2 size = { PLATFORM->getLogicalWidth(), PLATFORM->getLogicalHeight() };
	auto scale = size / dimensions;
	node->setScale(glm::min(scale.x, scale.y));
	node->setStretch(1.0f / node->getScale());
}

void Application::onEvent(const Profile::ProfileClearedEvent& e)
{
	SCENE_MANAGER->popWindow(SCENE_MANAGER->getWindowsCount(), [] {
		SCENE_MANAGER->switchScreen(nullptr, [] {
			SCENE_MANAGER->switchScreen(std::make_shared<GameplayScreen>());
		});
	});
}

void Application::initializeScene()
{
	Scene::Sampler::DefaultSampler = Renderer::Sampler::Linear;

	auto loading = std::make_shared<LoadingScreen>();
	loading->setTasks({
		[] { PRECACHE_FONT_ALIAS("fonts/rubik/Rubik-Medium.ttf", "default"); },
		[] { PRECACHE_FONT_ALIAS("fonts/rubik/Rubik-Bold.ttf", "default_bold"); },
		[] { CACHE->makeAtlases(); },
		[] {
			// prepare
			
			FONT("default")->setCustomVerticalOffset(-4.0f);
			FONT("default_bold")->setCustomVerticalOffset(-4.0f);

			// particles holder

			auto particles_holder = std::make_shared<Scene::Node>();
			SCENE_MANAGER->attach(particles_holder);
			Helpers::DollarEmitter::Holder = particles_holder;

			// turor holder

			auto tutor_holder = std::make_shared<TutorHolder>();
			SCENE_MANAGER->attach(tutor_holder);

			ENGINE->addSystem<TutorialSystem>(tutor_holder);

			// gameplay

			SCENE_MANAGER->switchScreen(std::make_shared<GameplayScreen>());
		}
	});
	SCENE_MANAGER->switchScreen(loading, [loading] {
		loading->runTasks();
	});
}
