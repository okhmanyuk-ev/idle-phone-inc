#include "application.h"

void sky_main()
{
	sky::Locator<sky::Application>::Init(std::make_shared<sky::Application>(PROJECT_NAME,
		std::set{ sky::Application::Flag::Audio, sky::Application::Flag::Scene }));
	{
		PhoneInc::Application game;
		sky::Locator<sky::Application>::GetService()->run();
	}
	sky::Locator<sky::Application>::Reset();
}