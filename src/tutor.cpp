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
	mFinger->setScale(0.0f);
	attach(mFinger);

	runAction(Shared::ActionHelpers::ExecuteInfinite([this] {
		removeOutdatedTutors();
		chooseCurrentTutor();
		showFinger(isPlaying());
		moveFinger();
	}));

	// finger pulse

	runAction(Shared::ActionHelpers::RepeatInfinite([this]() -> Shared::ActionHelpers::Action {
		const float Delay = 1.25f;
		const float Duration = 0.125f;
		const glm::vec2 ModifiedValue = { 0.75f, 0.75f };
		const glm::vec2 NormalValue = { 1.0f, 1.0f };

		if (mFingerState != FingerState::Opened)
			return nullptr;

		return Shared::ActionHelpers::Limit([this] { return mFingerState == FingerState::Opened; },
			Shared::ActionHelpers::Delayed(Delay, Shared::ActionHelpers::MakeSequence(
				Shared::ActionHelpers::ChangeScale(mFinger, ModifiedValue, Duration, Common::Easing::CubicInOut),
				Shared::ActionHelpers::ChangeScale(mFinger, NormalValue, Duration, Common::Easing::CubicInOut),
				Shared::ActionHelpers::ChangeScale(mFinger, ModifiedValue, Duration, Common::Easing::CubicInOut),
				Shared::ActionHelpers::ChangeScale(mFinger, NormalValue, Duration, Common::Easing::CubicInOut)
			))
		);
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

void TutorHolder::play(const std::string& name, std::shared_ptr<Scene::Node> node, CanStartCallback canStartCallback,
	Callback beginCallback, Callback endCallback)
{
	if (PROFILE->isTutorCompleted(name))
		return;

	assert(mTutors.count(name) == 0);

	Tutor tutor;
	tutor.node = node;
	tutor.canStartCallback = canStartCallback;
	tutor.beginCallback = beginCallback;
	tutor.endCallback = endCallback;
	mTutors.insert({ name, tutor });

	LOG("tutor \"" + name + "\" added");
}

void TutorHolder::complete()
{
	if (!isPlaying())
		return;

	auto name = mCurrentTutor.value();

	auto tutor = mTutors.at(name);

	if (tutor.endCallback)
		tutor.endCallback();

	mTutors.erase(name);
	PROFILE->setTutorCompleted(name);
	mCurrentTutor = std::nullopt;

	showFinger(false);

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

		if (mCurrentTutor.has_value())
		{
			assert(name != mCurrentTutor.value());
		}
		mTutors.erase(name);
		removeOutdatedTutors();
		break;
	}
}

void TutorHolder::showFinger(bool visible)
{
	const float Duration = 0.25f;

	if (visible && mFingerState == FingerState::Closed)
	{
		mFingerState = FingerState::Opening;
		mFinger->setEnabled(true);

		runAction(Shared::ActionHelpers::MakeSequence(
			Shared::ActionHelpers::ChangeScale(mFinger, { 1.0f, 1.0f }, Duration, Common::Easing::BackOut),
			Shared::ActionHelpers::Execute([this] {
				mFingerState = FingerState::Opened;
			})
		));
	}
	else if (!visible && mFingerState == FingerState::Opened)
	{
		mFingerState = FingerState::Closing;

		runAction(Shared::ActionHelpers::MakeSequence(
			Shared::ActionHelpers::ChangeScale(mFinger, { 0.0f, 0.0f }, Duration, Common::Easing::BackIn),
			Shared::ActionHelpers::Execute([this] {
				mFingerState = FingerState::Closed;
				mFinger->setEnabled(false);
			})
		));
	}
}

void TutorHolder::moveFinger()
{
	if (!isPlaying())
		return;

	if (mFingerState != FingerState::Opening && mFingerState != FingerState::Opened)
		return;

	if (!mFinger->isTransformReady())
		return;

	auto tutor = mTutors.at(mCurrentTutor.value());
	auto node = tutor.node.lock();

	if (!node->isTransformReady())
		return;

	if (!node->hasScene())
		return;

	mFinger->setPosition(unproject(node->project(node->getSize() * glm::vec2(0.5f, 0.75f))));
}

void TutorHolder::chooseCurrentTutor()
{
	if (SCENE_MANAGER->isScreenBusy())
		return;

	if (mCurrentTutor.has_value())
		return;

	for (const auto& [name, tutor] : mTutors)
	{;
		if (tutor.canStartCallback && !tutor.canStartCallback())
			continue;

		mCurrentTutor = name;
		if (tutor.beginCallback)
		{
			tutor.beginCallback();
		}
		break;
	}
}