#include "tutor.h"

using namespace PhoneInc;

TutorHolder::TutorHolder()
{
	setStretch(1.0f);
	setTouchable(true);

	mFinger = std::make_shared<Scene::Sprite>();
	mFinger->setTexture(sky::GetTexture("textures/tutor_finger.png"));
	mFinger->setOrigin({ 63.0f, 18.0f });
	mFinger->setEnabled(false);
	mFinger->setScale(0.0f);
	attach(mFinger);

	runAction(sky::Actions::ExecuteInfinite([this] {
		removeOutdatedTutors();
		chooseCurrentTutor();
		showFinger(isPlaying());
		moveFinger();
	}));

	// finger pulse

	runAction(sky::Actions::RepeatInfinite([this]() -> std::optional<sky::Action> {
		const float Delay = 1.25f;
		const float Duration = 0.125f;
		const glm::vec2 ModifiedValue = { 0.75f, 0.75f };
		const glm::vec2 NormalValue = { 1.0f, 1.0f };

		if (mFingerState != FingerState::Opened)
			return std::nullopt;

		return sky::Actions::Breakable([this] { return mFingerState == FingerState::Opened; },
			sky::Actions::Delayed(Delay, sky::Actions::Sequence(
				sky::Actions::ChangeScale(mFinger, ModifiedValue, Duration, Easing::CubicInOut),
				sky::Actions::ChangeScale(mFinger, NormalValue, Duration, Easing::CubicInOut),
				sky::Actions::ChangeScale(mFinger, ModifiedValue, Duration, Easing::CubicInOut),
				sky::Actions::ChangeScale(mFinger, NormalValue, Duration, Easing::CubicInOut)
			))
		);
	}));
}

void TutorHolder::onEvent(const Profile::ProfileClearedEvent& e)
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

	sky::Log("tutor \"" + name + "\" added");
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

	sky::Log("tutor \"" + name + "\" completed");
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

		runAction(sky::Actions::Sequence(
			sky::Actions::ChangeScale(mFinger, { 1.0f, 1.0f }, Duration, Easing::BackOut),
			sky::Actions::Execute([this] {
				mFingerState = FingerState::Opened;
			})
		));
	}
	else if (!visible && mFingerState == FingerState::Opened)
	{
		mFingerState = FingerState::Closing;

		runAction(sky::Actions::Sequence(
			sky::Actions::ChangeScale(mFinger, { 0.0f, 0.0f }, Duration, Easing::BackIn),
			sky::Actions::Execute([this] {
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