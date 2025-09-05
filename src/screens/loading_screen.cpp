#include "loading_screen.h"

using namespace PhoneInc;

LoadingScreen::LoadingScreen()
{
	auto bg = std::make_shared<Scene::Sprite>();
	bg->setTexture(sky::GetTexture("textures/loading.png"));
	bg->setAnchor(0.5f);
	bg->setPivot(0.5f);
	getContent()->attach(bg);

	mProgressbar = std::make_shared<Helpers::StreetProgressbar>();
	mProgressbar->setAnchor({ 0.5f, 0.8f });
	mProgressbar->setPivot(0.5f);
	mProgressbar->setSize({ 828.0f, 24.0f });
	mProgressbar->setProgress(0.0f);
	bg->attach(mProgressbar);
}

void LoadingScreen::runTasks()
{
	std::list<sky::Action> seq;

	const float Duration = 0.5f / (float)mTasks.size();

	for (size_t i = 0; i < mTasks.size(); i++)
	{
		seq.push_back(sky::Actions::Insert([this, i, Duration] {
			auto start = mProgressbar->getProgress();
			auto dest = float(i + 1) / (float)mTasks.size();

			return sky::Actions::Interpolate(start, dest, Duration, Easing::CubicInOut, [this](float value) {
				mProgressbar->setProgress(value);
			});
		}));
		seq.push_back(sky::Actions::WaitGlobalFrame());
		seq.push_back(sky::Actions::Execute(mTasks.at(i)));
		seq.push_back(sky::Actions::WaitGlobalFrame());
	}

	runAction(sky::Actions::Sequence(std::move(seq)));
}