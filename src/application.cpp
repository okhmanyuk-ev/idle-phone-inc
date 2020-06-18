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
	LOCALIZATION->setLanguage(Shared::LocalizationSystem::Language::English);
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

    Scene::Sampler::DefaultSampler = Renderer::Sampler::Linear;
    
	makeLoadingScene();

	PROFILE->setNightBackground(!PROFILE->isNightBackground());

	GRAPHICS->setSdfSmoothFactor(Helpers::Scale);

	ENGINE->addSystem<Shared::SceneManager>(std::make_shared<Shared::SceneManager>());
}

Application::~Application()
{
	PROFILE->save();
	ENGINE->removeSystem<TutorialSystem>();
	ENGINE->removeSystem<Shared::SceneManager>();
}

void Application::loading(const std::string& stage, float progress)
{
	updateLoadingScale();
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

	SCENE_MANAGER->switchScreen(mGameplay);
}

void Application::prepare()
{
	FONT("default")->setCustomVerticalOffset(-4.0f);
	FONT("default_bold")->setCustomVerticalOffset(-4.0f);

	auto root = mGameScene.getRoot();

	root->attach(SCENE_MANAGER);

	Helpers::DollarEmitter::Holder = std::make_shared<Scene::Node>();
	root->attach(Helpers::DollarEmitter::Holder);

	mTutorHolder = std::make_shared<TutorHolder>();
	root->attach(mTutorHolder);

	ENGINE->addSystem<TutorialSystem>(mTutorHolder);

	mGameplay = std::make_shared<Gameplay>();
}

void Application::frame()
{
	updateGameScale();
	mGameScene.frame();
	Cheats::ShowDevMenu();
}

void Application::makeLoadingScene()
{
	auto root = mLoadingScene.getRoot();

	auto bg = std::make_shared<Scene::Sprite>();
	bg->setTexture(TEXTURE("textures/loading.png"));
	bg->setAnchor(0.5f);
	bg->setPivot(0.5f);
	bg->setSampler(Renderer::Sampler::Linear);
	root->attach(bg);

	mProgressbar = std::make_shared<Helpers::StreetProgressbar>();
	mProgressbar->setAnchor({ 0.5f, 0.8f });
	mProgressbar->setPivot(0.5f);
	mProgressbar->setSize({ 828.0f, 24.0f });
	bg->attach(mProgressbar);
}

void Application::updateGameScale()
{
    {
        auto root = SCENE_MANAGER->getScreenHolder();
        auto scale = getScaleFactor(true); // TODO: must working with false
        root->setScale(scale);
        root->setStretch(1.0f / scale);
    }
    {
        auto root = SCENE_MANAGER->getWindowHolder();
        auto scale = getScaleFactor(false);
        root->setScale(scale);
        root->setStretch(1.0f / scale);
        
        Helpers::DollarEmitter::Holder->setScale(scale);
        Helpers::DollarEmitter::Holder->setStretch(1.0f / scale);

		mTutorHolder->setScale(scale);
		mTutorHolder->setStretch(1.0f / scale);
    }
}

void Application::updateLoadingScale()
{
	auto root = mLoadingScene.getRoot();
	auto scale = getScaleFactor(false);
	root->setScale(scale);
	root->setStretch(1.0f / scale);
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

void Application::event(const Profile::ProfileClearedEvent& e)
{
	if (!isInitialized())
		return;

	mGameplay->setEnabled(false);

	SCENE_MANAGER->popWindow(SCENE_MANAGER->getWindowsCount(), [this] {
		SCENE_MANAGER->switchScreen(nullptr, [this] {
			mGameplay = std::make_shared<Gameplay>();
			SCENE_MANAGER->switchScreen(mGameplay);
		});
	});
}
