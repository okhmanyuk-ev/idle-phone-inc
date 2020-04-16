#include "application.h"
#include "gameplay.h"

using namespace PhoneInc;

Application::Application() : RichApplication(PROJECT_CODE)
{
	PLATFORM->setTitle(PROJECT_NAME);
	PLATFORM->resize(360, 640);
	PLATFORM->rescale(1.5f);
	RENDERER->setVsync(true);

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
		} },
	//	{ "textures", [this] {
	//		PRECACHE_TEXTURE_ALIAS("textures/ruby.png", "ruby");
	//	} }
	});

	setPayloadWaiting(0.0f);

	std::srand((unsigned int)std::time(nullptr));

	CONSOLE->registerCVar("g_node_editor", { "bool" }, CVAR_GETTER_BOOL(mNodeEditor), CVAR_SETTER_BOOL(mNodeEditor));
	CONSOLE->registerCVar("g_stats", { "bool" }, CVAR_GETTER_BOOL_FUNC(STATS->isEnabled), CVAR_SETTER_BOOL_FUNC(STATS->setEnabled));

	STATS->setAlignment(Shared::StatsSystem::Align::BottomRight);
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
	STATS->setEnabled(false);
	FONT("default")->setCustomVerticalOffset(-4.0f);

#if defined(BUILD_DEVELOPER)
	CONSOLE->execute("hud_show_fps 1");
	CONSOLE->execute("hud_show_drawcalls 1");
#else
	CONSOLE_DEVICE->setEnabled(false);
	STATS->setEnabled(false);
#endif

	Scene::Debug::Font = FONT("default");
	Scene::Sampler::DefaultSampler = Renderer::Sampler::Linear;
	Scene::Sprite::DefaultTextureAddress = Renderer::TextureAddress::Clamp;

	auto root = mGameScene.getRoot();

	mSceneManager = std::make_shared<Shared::SceneManager>();
	root->attach(mSceneManager);

	auto gameplay = std::make_shared<Gameplay>();
	mSceneManager->switchScreen(gameplay);
}

void Application::frame()
{
	mGameScene.frame();

	if (mNodeEditor)
		mSceneEditor.show();

	//ShowCheatsMenu(mSceneManager);
}