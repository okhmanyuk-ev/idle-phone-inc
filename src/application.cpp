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
	STYLEBOOK->load("stylebook.json");
	PROFILE->load();

	PLATFORM->initializeBilling({
		//{ "rubies.001", [this] { 
		//	addRubies(500);
		//} }
	});

	addLoadingTasks({
		{ "fonts", [this] {
			PRECACHE_FONT_ALIAS("fonts/rubik/Rubik-Medium.ttf", "default");
		} },
		{ "fonts", [this] {
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

	makeLoadingScene();
}

Application::~Application()
{
	PROFILE->save();
}

void Application::loading(const std::string& stage, float progress)
{
	mProgressbar->setProgress(progress);
	mLoadingScene.frame();
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
	mSceneManager->setScale(Helpers::InvScale);
	mSceneManager->setStretch(Helpers::Scale);
	root->attach(mSceneManager);

	mGameplay = std::make_shared<Gameplay>();
}

void Application::frame()
{
	mGameScene.frame();
	Cheats::ShowDevMenu(mSceneManager);
}

void Application::makeLoadingScene()
{
	auto root = mLoadingScene.getRoot();

	auto bg = std::make_shared<Scene::Sprite>();
	bg->setTexture(TEXTURE("textures/loading.png"));
	bg->setAnchor(0.5f);
	bg->setPivot(0.5f);
	bg->setScale(Helpers::InvScale);
	bg->setSampler(Renderer::Sampler::Linear);
	root->attach(bg);

	mProgressbar = std::make_shared<Helpers::StreetProgressbar>();
	mProgressbar->setAnchor({ 0.5f, 0.8f });
	mProgressbar->setPivot(0.5f);
	mProgressbar->setSize({ 276.0f * Helpers::Scale, 8.0f * Helpers::Scale });
	bg->attach(mProgressbar);
}

void Application::event(const Helpers::PushWindowEvent& e)
{
	mSceneManager->pushWindow(e.window, e.finishCallback);
}

void Application::event(const Helpers::PopWindowEvent& e)
{
	mSceneManager->popWindow();
}

void Application::event(const Profile::ProfileClearedEvent& e)
{
	if (!isInitialized())
		return;

	mGameplay->setEnabled(false);

	mSceneManager->popWindow(mSceneManager->getWindowsCount(), [this] {
		mSceneManager->switchScreen(nullptr, [this] {
			mGameplay = std::make_shared<Gameplay>();
			mSceneManager->switchScreen(mGameplay);
		});
	});
}