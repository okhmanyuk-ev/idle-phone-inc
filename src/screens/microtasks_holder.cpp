#include "microtasks_holder.h"

using namespace PhoneInc;

MicrotasksHolder::MicrotasksHolder()
{
	setStretch(1.0f);

	mHolder = std::make_shared<Scene::Node>();
	mHolder->setHorizontalStretch(1.0f);
	mHolder->setHeight(144.0f);
	mHolder->setVerticalPivot(1.0f);
	attach(mHolder);

	auto bg = std::make_shared<Scene::Sprite>();
	bg->setTexture(sky::GetTexture("textures/microtasks/bg.png"));
	bg->setAnchor(0.5f);
	bg->setPivot(0.5f);
	mHolder->attach(bg);

	mIcon = std::make_shared<Scene::Sprite>();
	mIcon->setAnchor({ 0.0f, 0.5f });
	mIcon->setPivot(0.5f);
	bg->attach(mIcon);

	mLabel = std::make_shared<Helpers::Label>();
	mLabel->setFontSize(32.0f);
	mLabel->setMultiline(true);
	mLabel->setAnchor({ 0.0f, 0.5f });
	mLabel->setPivot({ 0.0f, 0.5f });
	mLabel->setWidth(302.0f);
	mLabel->setX(72.0f);
	bg->attach(mLabel);

	mRewardButton = std::make_shared<Helpers::StandardLongButton>();
	mRewardButton->setEnabled(false);
	mRewardButton->setAnchor({ 0.5f, 1.0f });
	mRewardButton->setPivot({ 0.5f, 0.0f });
	mRewardButton->setY(32.0f);
	mRewardButton->getLabel()->setText(sky::Localize("MICROTASK_REWARD_BUTTON"));
	mRewardButton->setActiveCallback([this] {
		runAction(sky::Actions::Delayed(0.25f, [this] {
			MICROTASKS->complete();
		}));
		mRewardButton->setEnabled(false);
	});
	bg->attach(mRewardButton);
}

void MicrotasksHolder::onEvent(const Microtasks::TaskReadyEvent& e)
{
	refreshWithAnim();
}

void MicrotasksHolder::onEvent(const Microtasks::TaskCompletedEvent& e)
{
	refreshWithAnim();
}

void MicrotasksHolder::start()
{
	refresh();
	show();
}

void MicrotasksHolder::show()
{
	runAction(sky::Actions::Sequence(
		sky::Actions::ChangeVerticalPivot(mHolder, 0.0f, 0.25f, Easing::CubicOut),
		[this] {
			mRewardButton->setEnabled(MICROTASKS->isReady());
		},
		sky::Actions::Wait(0.5f),
		[this] {
			MICROTASKS->checkForCompletion();
		}
	));
}

void MicrotasksHolder::hide(std::function<void()> finishCallback)
{
	runAction(sky::Actions::Sequence(
		[this] {
			mRewardButton->setEnabled(false);
		},
		sky::Actions::ChangeVerticalPivot(mHolder, 1.0f, 0.25f, Easing::CubicOut),
		finishCallback
	));
}

void MicrotasksHolder::refresh()
{
	if (!MICROTASKS->hasUnfinishedTasks())
	{
		mHolder->setVisible(false);
		return;
	}
	mHolder->setVisible(true);

	auto task = MICROTASKS->getCurrentTask();
	mLabel->setText(sky::format(sky::Localize(task.locale), task.target));

	if (MICROTASKS->isReady())
		mIcon->setTexture(sky::GetTexture("textures/microtasks/done.png"));
	else
		mIcon->setTexture(sky::GetTexture("textures/microtasks/" + Microtasks::TaskTypeToString(task.type) + ".png"));

	mIcon->applyTextureSize();
}

void MicrotasksHolder::refreshWithAnim()
{
	hide([this] {
		runAction(sky::Actions::Delayed(0.25f, [this] {
			refresh();
			show();
		}));
	});
}