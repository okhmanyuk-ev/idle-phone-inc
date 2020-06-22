#include "loading_screen.h"

using namespace PhoneInc;

LoadingScreen::LoadingScreen()
{
	auto root = getContent();

	auto bg = std::make_shared<Scene::Sprite>();
	bg->setTexture(TEXTURE("textures/loading.png"));
	bg->setAnchor(0.5f);
	bg->setPivot(0.5f);
	root->attach(bg);

	mProgressbar = std::make_shared<Helpers::StreetProgressbar>();
	mProgressbar->setAnchor({ 0.5f, 0.8f });
	mProgressbar->setPivot(0.5f);
	mProgressbar->setSize({ 828.0f, 24.0f });
	mProgressbar->setProgress(0.0f);
	bg->attach(mProgressbar);
}

void LoadingScreen::runTasks()
{
	auto seq = Shared::ActionHelpers::MakeSequence();

	for (size_t i = 0; i < mTasks.size(); i++)
	{
		seq->add(Shared::ActionHelpers::Insert([this, i] {
			auto start = mProgressbar->getProgress();
			auto dest = float(i + 1) / (float)mTasks.size();

			return Shared::ActionHelpers::Interpolate(start, dest, 0.25f, Common::Easing::Linear, [this](float value) {
				mProgressbar->setProgress(value);
			});
		}));
		seq->add(Shared::ActionHelpers::Execute(mTasks.at(i)));
		seq->add(Shared::ActionHelpers::WaitOneFrame());
	}

	runAction(std::move(seq));
}