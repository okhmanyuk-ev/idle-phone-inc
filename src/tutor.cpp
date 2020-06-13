#include "tutor.h"

using namespace PhoneInc;

TutorHolder::TutorHolder()
{
	setStretch(1.0f);
	setTouchable(true);
	setTouchMask(0xFFFFFFFF);

	mFinger = std::make_shared<Scene::Sprite>();
	mFinger->setTexture(TEXTURE("textures/tutor_finger.png"));
	mFinger->setOrigin({ 63.0f, 18.0f });
	mFinger->setEnabled(false);
	attach(mFinger);

	runAction(Shared::ActionHelpers::ExecuteInfinite([this] {
		mCurrentTutor = std::nullopt;

		removeOutdatedTutors();

		for (const auto& [name, tutor] : mTutors)
		{
			if (tutor.canStartCallback && !tutor.canStartCallback())
				continue;

			mCurrentTutor = name;
			break;
		}

		mFinger->setEnabled(isPlaying());
		
		if (!isPlaying())
			return;

		if (!mFinger->isTransformReady())
			return;

		auto tutor = mTutors.at(mCurrentTutor.value());
		auto node = tutor.node.lock();

		if (!node->isTransformReady())
			return;

		mFinger->setPosition(unproject(node->project(node->getSize() / 2.0f)));
	}));
}

void TutorHolder::event(const Profile::ProfileClearedEvent& e)
{
	mTutors.clear();
	mCurrentTutor = std::nullopt;
}

bool TutorHolder::hitTest(const glm::vec2& value) const
{
	if (!isPlaying())
		return false;

	auto tutor = mTutors.at(mCurrentTutor.value());
	auto node = tutor.node.lock();

	return !node->hitTest(node->unproject(project(value)));
}

void TutorHolder::play(const std::string& name, std::shared_ptr<Scene::Node> node, CanStartCallback canStartCallback)
{
	if (PROFILE->isTutorCompleted(name))
		return;

	assert(mTutors.count(name) == 0);

	Tutor tutor;
	tutor.node = node;
	tutor.canStartCallback = canStartCallback;
	mTutors.insert({ name, tutor });

	LOG("tutor \"" + name + "\" added");
}

void TutorHolder::complete()
{
	if (!isPlaying())
		return;

	auto name = mCurrentTutor.value();

	mTutors.erase(name);

	PROFILE->setTutorCompleted(name);
	mCurrentTutor = std::nullopt;

	LOG("tutor \"" + name + "\" completed");
}

bool TutorHolder::isCompleted(const std::string& name) const
{
	return PROFILE->isTutorCompleted(name);
}

bool TutorHolder::isPlaying() const
{
	return mCurrentTutor.has_value();
}

void TutorHolder::removeOutdatedTutors()
{
	for (const auto& [name, tutor] : mTutors)
	{
		if (!tutor.node.expired())
			continue;

		mTutors.erase(name);
		removeOutdatedTutors();
		break;
	}
}