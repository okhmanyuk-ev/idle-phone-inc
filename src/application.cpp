#include "application.h"

using namespace PhoneInc;

Application::Application() : RichApplication(PROJECT_CODE)
{
	PLATFORM->setTitle(PROJECT_NAME);
	PLATFORM->resize(360, 640);
	RENDERER->setVsync(true);
#if !defined(PLATFORM_MOBILE)
	PLATFORM->rescale(1.5f);
#endif

	ENGINE->addSystem<Profile>(std::make_shared<Profile>());

	LOCALIZATION->loadDicrionaries("localization");
	LOCALIZATION->setLanguage(Shared::LocalizationSystem::Language::Russian);

	PROFILE->load();

	PLATFORM->initializeBilling({
		//{ "rubies.001", [this] { 
		//	addRubies(500);
		//} }
	});

	addLoadingTasks({
		{ "fonts", [this] {
			PRECACHE_FONT_ALIAS("fonts/rubik/Rubik-Medium.ttf", "default");
			PRECACHE_FONT_ALIAS("fonts/rubik/Rubik-Bold.ttf", "default_bold");
		} },
		{ "preparing", [this] {
			prepare();
		} }
	//	{ "textures", [this] {
	//		PRECACHE_TEXTURE_ALIAS("textures/ruby.png", "ruby");
	//	} }
	});

	setPayloadWaiting(0.0f);

	std::srand((unsigned int)std::time(nullptr));

	CONSOLE->registerCVar("g_editor", { "bool" }, CVAR_GETTER_BOOL_FUNC(mSceneEditor.isEnabled), 
		CVAR_SETTER_BOOL_FUNC(mSceneEditor.setEnabled));
	
	CONSOLE->registerCVar("g_stats", { "bool" }, CVAR_GETTER_BOOL_FUNC(STATS->isEnabled), 
		CVAR_SETTER_BOOL_FUNC(STATS->setEnabled));

	STATS->setAlignment(Shared::StatsSystem::Align::BottomRight);

	mGameScene.setInteractTestCallback([](const auto& pos) {
		return !ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow | ImGuiHoveredFlags_AllowWhenBlockedByPopup);
	});
}

Application::~Application()
{
	PROFILE->save();
}

void Application::loading(const std::string& stage, float progress)
{
	//mSplashScene.updateProgress(progress);
	//mSplashScene.frame();
#if defined BUILD_DEVELOPER
	RichApplication::loading(stage, progress);
#endif
}

void Application::initialize()
{
#if defined(BUILD_DEVELOPER)
	CONSOLE->execute("hud_show_fps 1");
	CONSOLE->execute("hud_show_drawcalls 1");
#else
	CONSOLE_DEVICE->setEnabled(false);
	STATS->setEnabled(false);
#endif

	mSceneManager->switchScreen(mGameplay);
}

void Application::prepare()
{
	FONT("default")->setCustomVerticalOffset(-4.0f);
	FONT("default_bold")->setCustomVerticalOffset(-4.0f);

	Scene::Debug::Font = FONT("default");

	auto root = mGameScene.getRoot();

	mSceneManager = std::make_shared<Shared::SceneManager>();
	root->attach(mSceneManager);

	mGameplay = std::make_shared<Gameplay>();
}

void Application::frame()
{
	mGameScene.frame();
	Cheats::ShowDevMenu(mSceneManager);
}

void Application::event(const Helpers::PushWindowEvent& e)
{
	mSceneManager->pushWindow(e.window);
}

void Application::event(const Helpers::PopWindowEvent& e)
{
	mSceneManager->popWindow();
}

void Application::event(const Profile::ProfileClearedEvent& e)
{
	mSceneManager->switchScreen(nullptr, [this] {
		mGameplay = std::make_shared<Gameplay>();
		mSceneManager->switchScreen(mGameplay);
	});
}