#include "application.h"

using namespace PhoneInc;

Application::Application() : Shared::Application(PROJECT_NAME, { Flag::Audio, Flag::Scene })
{
	PLATFORM->setTitle(PRODUCT_NAME);
#if defined(PLATFORM_MAC)
	PLATFORM->resize(720, 1280);
#else
	PLATFORM->resize(360, 640);
#endif
#if defined(PLATFORM_WINDOWS)
	PLATFORM->rescale(1.5f);
#endif

	// limit maximum time delta to avoid animation breaks
	FRAME->setTimeDeltaLimit(sky::FromSeconds(1.0f / 30.0f));

	getScene()->getTimestepFixer().setEnabled(false);

	//GRAPHICS->setSdfSmoothFactor(Helpers::Scale);

	STATS->setAlignment(Shared::StatsSystem::Align::BottomRight);

	sky::Locator<Microtasks>::Init(std::make_shared<Microtasks>());
	sky::Locator<Profile>::Init(std::make_shared<Profile>());

	PROFILE->load();
	PROFILE->setNightBackground(!PROFILE->isNightBackground());

	MICROTASKS->checkForCompletion();

	PLATFORM->initializeBilling({
		//{ "rubies.001", [this] {
		//	addRubies(500);
		//} }
	});

	Shared::SceneHelpers::XmlCreateFuncs["MyButton"] = [](const auto& root) {
		auto node = std::make_shared<Helpers::Button>();
		Shared::SceneHelpers::ParseSpriteFromXml(*node, root);
		return node;
	};
	Shared::SceneHelpers::XmlCreateFuncs["MyLabel"] = [](const auto& root) {
		auto node = std::make_shared<Helpers::Label>();
		Shared::SceneHelpers::ParseLabelFromXml(*node, root);
		return node;
	};
	Shared::SceneHelpers::XmlCreateFuncs["MyLabelSolid"] = [](const auto& root) {
		auto node = std::make_shared<Helpers::LabelSolid>();
		Shared::SceneHelpers::ParseLabelFromXml(*node, root);
		return node;
	};

	initializeScene();
}

Application::~Application()
{
	PROFILE->save();
	sky::Locator<TutorialSystem>::Reset();
}

void Application::onFrame()
{
	Cheats::ShowDevMenu();
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
	getScene()->setScreenAdaption(glm::vec2{ 1080.0f, 1920.0f });
	Scene::Sampler::DefaultSampler = skygfx::Sampler::Linear;

	auto loading = std::make_shared<LoadingScreen>();
	loading->setTasks({
		[] { sky::PrecacheFont("fonts/rubik/Rubik-Medium.ttf", "default"); },
		[] { sky::PrecacheFont("fonts/rubik/Rubik-Bold.ttf", "default_bold"); },
		[] { sky::GetService<sky::Cache>()->makeAtlases(); },
		[] {
			// prepare

			sky::GetFont("default")->setCustomVerticalOffset(-4.0f);
			sky::GetFont("default_bold")->setCustomVerticalOffset(-4.0f);

			Scene::Sprite::DefaultTexture = sky::GetTexture("textures/pink.png");
			Scene::Label::DefaultFont = sky::GetFont("default");

			// particles holder

			auto particles_holder = std::make_shared<Scene::Node>();
			SCENE_MANAGER->attach(particles_holder);
			Helpers::DollarEmitter::Holder = particles_holder;

			// turor holder

			auto tutor_holder = std::make_shared<TutorHolder>();
			SCENE_MANAGER->attach(tutor_holder);

			sky::Locator<TutorialSystem>::Init(tutor_holder);

			// gameplay

			SCENE_MANAGER->switchScreen(std::make_shared<GameplayScreen>());
		}
	});
	SCENE_MANAGER->switchScreen(loading, [loading] {
		loading->runTasks();
	});
}
