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
}

void MicrotasksHolder::event(const Microtasks::TaskCompletedEvent& e)
{
	hide([this] {
		runAction(Shared::ActionHelpers::Delayed(0.25f, Shared::ActionHelpers::Execute([this] {
			refresh();
			show(); 
		})));
	});
}

void MicrotasksHolder::start()
{
	refresh();
	show();
}

void MicrotasksHolder::show()
{
	runAction(Shared::ActionHelpers::ChangeVerticalPivot(mHolder, 0.0f, 0.25f, Common::Easing::CubicOut));
}

void MicrotasksHolder::hide(std::function<void()> finishCallback)
{
	runAction(Shared::ActionHelpers::MakeSequence(
		Shared::ActionHelpers::ChangeVerticalPivot(mHolder, 1.0f, 0.25f, Common::Easing::CubicOut),
		Shared::ActionHelpers::Execute(finishCallback)
	));
}

void MicrotasksHolder::refresh()
{
	if (!MICROTASKS->hasUnfinishedTasks())
	{
		mHolder->setEnabled(false);
		return;
	}
	mHolder->setEnabled(true);

	auto task = MICROTASKS->getCurrentTask();

	mLabel->setText(LOCALIZE_FMT(task.locale, task.target));
	mIcon->setTexture(TEXTURE("textures/microtasks/" + Microtasks::TaskTypeToString(task.type) + ".png"));
	mIcon->applyTextureSize();
}