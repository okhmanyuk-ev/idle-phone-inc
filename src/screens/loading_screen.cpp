#include "loading_screen.h"

using namespace PhoneInc;

LoadingScreen::LoadingScreen()
{
	auto bg = std::make_shared<Scene::Sprite>();
	bg->setTexture(TEXTURE("textures/loading.png"));
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
	auto seq = Actions::Collection::MakeSequence();

	const float Duration = 0.5f / (float)mTasks.size();

	for (size_t i = 0; i < mTasks.size(); i++)
	{
		seq->add(Actions::Collection::Insert([this, i, Duration] {
			auto start = mProgressbar->getProgress();
			auto dest = float(i + 1) / (float)mTasks.size();

			return Actions::Collection::Interpolate(start, dest, Duration, Easing::CubicInOut, [this](float value) {
				mProgressbar->setProgress(value);
			});
		}));
		seq->add(Actions::Collection::WaitGlobalFrame());
		seq->add(Actions::Collection::Execute(mTasks.at(i)));
		seq->add(Actions::Collection::WaitGlobalFrame());
	}

	runAction(std::move(seq));
}