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

	mBackground = std::make_shared<Scene::Sprite>();
	mBackground->setTexture(TEXTURE("textures/microtasks/bg.png"));
	mBackground->setAnchor(0.5f);
	mBackground->setPivot(0.5f);
	mHolder->attach(mBackground);

	mIcon = std::make_shared<Scene::Sprite>();
	mIcon->setAnchor({ 0.0f, 0.5f });
	mIcon->setPivot(0.5f);
	mBackground->attach(mIcon);

	mLabel = std::make_shared<Helpers::Label>();
	mLabel->setFontSize(32.0f);
	mLabel->setMultiline(true);
	mLabel->setAnchor({ 0.0f, 0.5f });
	mLabel->setPivot({ 0.0f, 0.5f });
	mLabel->setWidth(302.0f);
	mLabel->setX(72.0f);
	mBackground->attach(mLabel);

	mRewardButton = std::make_shared<Helpers::StandardLongButton>();
	mRewardButton->setEnabled(false);
	mRewardButton->setAnchor({ 0.5f, 1.0f });
	mRewardButton->setPivot({ 0.5f, 0.0f });
	mRewardButton->setY(32.0f);
	mRewardButton->getLabel()->setText(LOCALIZE("MICROTASK_REWARD_BUTTON"));
	mRewardButton->setActiveCallback([this] {
		runAction(Actions::Collection::Delayed(0.25f, Actions::Collection::Execute([this] {
			MICROTASKS->complete();
		})));
		mRewardButton->setEnabled(false);
	});
	mBackground->attach(mRewardButton);
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
	runAction(Actions::Collection::MakeSequence(
		Actions::Collection::ChangeVerticalPivot(mHolder, 0.0f, 0.25f, Easing::CubicOut),
		Actions::Collection::Execute([this] {
			mRewardButton->setEnabled(MICROTASKS->isReady());
		}),
		Actions::Collection::Wait(0.5f),
			Actions::Collection::Execute([this] {
			MICROTASKS->checkForCompletion();
		})
	));
}

void MicrotasksHolder::hide(std::function<void()> finishCallback)
{
	runAction(Actions::Collection::MakeSequence(
		Actions::Collection::Execute([this] {
			mRewardButton->setEnabled(false);
		}),
		Actions::Collection::ChangeVerticalPivot(mHolder, 1.0f, 0.25f, Easing::CubicOut),
			Actions::Collection::Execute(finishCallback)
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

	mLabel->setText(LOCALIZE_FMT(task.locale, task.target));
	
	if (MICROTASKS->isReady())
		mIcon->setTexture(TEXTURE("textures/microtasks/done.png"));
	else
		mIcon->setTexture(TEXTURE("textures/microtasks/" + Microtasks::TaskTypeToString(task.type) + ".png"));

	mIcon->applyTextureSize();
}

void MicrotasksHolder::refreshWithAnim()
{
	hide([this] {
		runAction(Actions::Collection::Delayed(0.25f, Actions::Collection::Execute([this] {
			refresh();
			show();
		})));
	});
}