#include "application.h"

#if defined(PLATFORM_IOS)
#import <FBSDKLoginKit/FBSDKLoginKit.h>
#include <platform/system_ios.h>
#endif

using namespace PhoneInc;

Application::Application() : RichApplication(PROJECT_NAME)
{
	PLATFORM->setTitle(PRODUCT_NAME);
	PLATFORM->resize(360, 640);
#if !defined(PLATFORM_MOBILE)
	PLATFORM->rescale(1.5f);
#endif
	RENDERER->setVsync(true);

	GRAPHICS->setSdfSmoothFactor(Helpers::Scale);

	std::srand((unsigned int)std::time(nullptr));

	CONSOLE->registerCVar("g_editor", { "bool" }, CVAR_GETTER_BOOL_FUNC(mSceneEditor.isEnabled), CVAR_SETTER_BOOL_FUNC(mSceneEditor.setEnabled));
	CONSOLE->registerCVar("g_stats", { "bool" }, CVAR_GETTER_BOOL_FUNC(STATS->isEnabled), CVAR_SETTER_BOOL_FUNC(STATS->setEnabled));

	STATS->setAlignment(Shared::StatsSystem::Align::BottomRight);

	mGameScene.setInteractTestCallback([](const auto& pos) {
		return !ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow | ImGuiHoveredFlags_AllowWhenBlockedByPopup);
	});

	LOCALIZATION->loadDicrionaries("localization");
	LOCALIZATION->setLanguage(Shared::LocalizationSystem::Language::English);

	ENGINE->addSystem<Microtasks>(std::make_shared<Microtasks>());
	ENGINE->addSystem<Profile>(std::make_shared<Profile>());
	
	PROFILE->load();
	PROFILE->setNightBackground(!PROFILE->isNightBackground());

	MICROTASKS->checkForCompletion();

	STYLEBOOK->load("stylebook.json");

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
	ENGINE->removeSystem<Shared::SceneManager>();
}

void Application::frame()
{
	adaptToScreen(SCENE_MANAGER->getWindowHolder(), false);
	mGameScene.frame();
	Cheats::ShowDevMenu();
}

void Application::adaptToScreen(std::shared_ptr<Scene::Node> node, bool horizontal_priority)
{
	const glm::vec2 target = { 1080.0f, 1920.0f };

	glm::vec2 size = { (float)PLATFORM->getLogicalWidth(), (float)PLATFORM->getLogicalHeight() };

	auto scale = size / target;

#if defined(PLATFORM_WINDOWS)
	horizontal_priority = false;
#endif

	if (horizontal_priority) 
	{
		node->setScale(scale.x);
	}
	else
	{
		node->setScale(glm::min(scale.x, scale.y));
	}
}

void Application::event(const Profile::ProfileClearedEvent& e)
{
    if (!mSceneInitialized)
        return;

	SCENE_MANAGER->popWindow(SCENE_MANAGER->getWindowsCount(), [this] {
		SCENE_MANAGER->switchScreen(nullptr, [this] {
			auto gameplay_screen = std::make_shared<GameplayScreen>();
			gameplay_screen->runAction(Shared::ActionHelpers::ExecuteInfinite([this, gameplay_screen = std::weak_ptr<GameplayScreen>(gameplay_screen)]{
				adaptToScreen(gameplay_screen.lock(), true);
			}));
			SCENE_MANAGER->switchScreen(gameplay_screen);
		});
	});
}

void Application::initializeScene()
{
    mSceneInitialized = true;

	ENGINE->addSystem<Shared::SceneManager>(std::make_shared<Shared::SceneManager>());
	Scene::Sampler::DefaultSampler = Renderer::Sampler::Linear;

	auto root = mGameScene.getRoot();
	root->attach(SCENE_MANAGER);

	auto loading = std::make_shared<Scene::Actionable<LoadingScreen>>();
	loading->setTasks({
		[] { PRECACHE_FONT_ALIAS("fonts/rubik/Rubik-Medium.ttf", "default"); },
		[] { PRECACHE_FONT_ALIAS("fonts/rubik/Rubik-Bold.ttf", "default_bold"); },
		[this, root] {
			// prepare
			
			FONT("default")->setCustomVerticalOffset(-4.0f);
			FONT("default_bold")->setCustomVerticalOffset(-4.0f);

			// particles holder

			auto particles_holder = std::make_shared<Scene::Actionable<Scene::Node>>();
			particles_holder->runAction(Shared::ActionHelpers::ExecuteInfinite([particles_holder, this] {
				adaptToScreen(particles_holder, false);
			}));
			root->attach(particles_holder);
			Helpers::DollarEmitter::Holder = particles_holder;

			// turor holder

			auto tutor_holder = std::make_shared<TutorHolder>();
			tutor_holder->runAction(Shared::ActionHelpers::ExecuteInfinite([this, tutor_holder] {
				adaptToScreen(tutor_holder, false);
			})); 
			root->attach(tutor_holder);

			ENGINE->addSystem<TutorialSystem>(tutor_holder);

			// gameplay

			auto gameplay_screen = std::make_shared<GameplayScreen>();
			gameplay_screen->runAction(Shared::ActionHelpers::ExecuteInfinite([this, gameplay_screen = std::weak_ptr<GameplayScreen>(gameplay_screen)] {
				adaptToScreen(gameplay_screen.lock(), true);
			}));
			SCENE_MANAGER->switchScreen(gameplay_screen);
		}
	});
	SCENE_MANAGER->switchScreen(loading, [loading] {
		loading->runTasks();
	});

	loading->runAction(Shared::ActionHelpers::ExecuteInfinite([this, loading] {
		adaptToScreen(loading, false);
	}));
}
