#include "window.h"
#include "helpers.h"

using namespace PhoneInc;

Window::Window()
{
}

StandardWindow::StandardWindow()
{
	mBackground = std::make_shared<Scene::SlicedSprite>();
	mBackground->setTexture(sky::GetTexture("textures/windows/background_slice.png"));
	mBackground->setCenterRegion({ { 14.0f, 131.0f }, { 30.0f, 17.0f } });
	mBackground->setAnchor(0.5f);
	mBackground->setPivot(0.5f);
	mBackground->setTouchable(true);
	getContent()->attach(mBackground);

	mTitle = std::make_shared<Helpers::LabelSolid>();
	mTitle->setAnchor({ 0.5f, 0.0f });
	mTitle->setPivot(0.5f);
	mTitle->setFontSize(45.0f);
	mTitle->setPosition({ 0.0f, 64.0f });
	mTitle->setText(L"title");
	getBackground()->attach(mTitle);

	mCloseButton = std::make_shared<Helpers::CloseButton>();
	mCloseButton->setPivot(0.5f);
	mCloseButton->setAnchor({ 1.0f, 0.0f });
	mCloseButton->setPosition({ -72.0f, 72.0f });
	getBackground()->attach(mCloseButton);
}