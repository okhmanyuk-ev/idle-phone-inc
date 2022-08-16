#include "application.h"

using namespace PhoneInc;

Application::Application() : Shared::Application(PROJECT_NAME, { Flag::Audio, Flag::Scene })
{
	PLATFORM->setTitle(PRODUCT_NAME);
	PLATFORM->resize(360, 640);
#if !defined(PLATFORM_MOBILE)
	PLATFORM->rescale(1.5f);
#endif
	RENDERER->setVsync(true);

	// limit maximum time delta to avoid animation breaks
	FRAME->setTimeDeltaLimit(Clock::FromSeconds(1.0f / 30.0f));

	getScene()->getTimestepFixer().setEnabled(false);

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
}

Application::~Application()
{
	PROFILE->save();
	ENGINE->removeSystem<TutorialSystem>();
}

void Application::onFrame()
{
	adaptToScreen(SCENE_MANAGER, { 1080.0f, 1920.0f });
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
	Scene::Sampler::DefaultSampler = skygfx::Sampler::Linear;

	auto loading = std::make_shared<LoadingScreen>();
	loading->setTasks({
		[] { PRECACHE_FONT_ALIAS("fonts/rubik/Rubik-Medium.ttf", "default"); },
		[] { PRECACHE_FONT_ALIAS("fonts/rubik/Rubik-Bold.ttf", "default_bold"); },
		[] { CACHE->makeAtlases(); },
		[] {
			// prepare
			
			FONT("default")->setCustomVerticalOffset(-4.0f);
			FONT("default_bold")->setCustomVerticalOffset(-4.0f);

			Scene::Sprite::DefaultTexture = TEXTURE("textures/pink.png");
			Scene::Label::DefaultFont = FONT("default");

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
