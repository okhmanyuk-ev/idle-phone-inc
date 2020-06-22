#include "application.h"

#if defined(PLATFORM_IOS)
#import <FBSDKLoginKit/FBSDKLoginKit.h>
#include <platform/system_ios.h>
#endif

using namespace PhoneInc;

Application::Application() : RichApplication(PROJECT_CODE)
{
	PLATFORM->setTitle(PROJECT_NAME);
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

	ENGINE->addSystem<Profile>(std::make_shared<Profile>());

	PROFILE->load();
	PROFILE->setNightBackground(!PROFILE->isNightBackground());

	STYLEBOOK->load("stylebook.json");

	PLATFORM->initializeBilling({
		//{ "rubies.001", [this] { 
		//	addRubies(500);
		//} }
	});

	initializeScene();
    
#if !defined(BUILD_DEVELOPER)
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

float Application::getScaleFactor(bool horizontal_priority)
{
	const glm::vec2 target = { 1080.0f, 1920.0f };

	glm::vec2 size = { (float)PLATFORM->getLogicalWidth(), (float)PLATFORM->getLogicalHeight() };

	auto scale = size / target;

	if (horizontal_priority)
		return scale.x;
	
	return glm::min(scale.x, scale.y);
}

void Application::adaptToScreen(std::shared_ptr<Scene::Node> node, bool horizontal_priority)
{
	auto scale = getScaleFactor(horizontal_priority);
	node->setScale(scale);
	node->setStretch(1.0f / scale);
}

void Application::event(const Profile::ProfileClearedEvent& e)
{
	SCENE_MANAGER->popWindow(SCENE_MANAGER->getWindowsCount(), [this] {
		SCENE_MANAGER->switchScreen(nullptr, [this] {
			mGameplayScreen = std::make_shared<GameplayScreen>();
			mGameplayScreen->runAction(Shared::ActionHelpers::ExecuteInfinite([this] {
				adaptToScreen(mGameplayScreen, true);
			}));
			SCENE_MANAGER->switchScreen(mGameplayScreen);
		});
	});
}

void Application::initializeScene()
{
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

			mTutorHolder = std::make_shared<TutorHolder>();
			mTutorHolder->runAction(Shared::ActionHelpers::ExecuteInfinite([this] {
				adaptToScreen(mTutorHolder, false);
			})); 
			root->attach(mTutorHolder);

			ENGINE->addSystem<TutorialSystem>(mTutorHolder);

			// gameplay

			mGameplayScreen = std::make_shared<GameplayScreen>();
			mGameplayScreen->runAction(Shared::ActionHelpers::ExecuteInfinite([this] {
				adaptToScreen(mGameplayScreen, true);
			}));
			SCENE_MANAGER->switchScreen(mGameplayScreen);
		}
	});
	SCENE_MANAGER->switchScreen(loading, [loading] {
		loading->runTasks();
	});

	loading->runAction(Shared::ActionHelpers::ExecuteInfinite([this, loading] {
		adaptToScreen(loading, false);
	}));
}