#include "building_upgrade_window.h"

using namespace PhoneInc;

BuildingUpgradeWindow::BuildingUpgradeWindow()
{
	auto bg = std::make_shared<Scene::Sprite>();
	bg->setTexture(TEXTURE("textures/windows/warehouse_window/bg.png"));
	bg->setAnchor(0.5f);
	bg->setPivot(0.5f);
	bg->setTouchable(true);
	getContent()->attach(bg);

	auto frame = std::make_shared<Scene::Sprite>();
	frame->setTexture(TEXTURE("textures/windows/warehouse_window/frame.png"));
	frame->setAnchor(0.5f);
	frame->setPivot(0.5f);
	getContent()->attach(frame);

	auto header_bg = std::make_shared<Scene::Sprite>();
	header_bg->setTexture(TEXTURE("textures/windows/warehouse_window/header_bg.png"));
	header_bg->setAnchor({ 0.5f, 0.0f });
	header_bg->setPivot({ 0.5f, 0.0f });
	bg->attach(header_bg);

	mTitleLabel = std::make_shared<Helpers::LabelSolid>();
	mTitleLabel->setAnchor(0.5f);
	mTitleLabel->setPivot(0.5f);
	mTitleLabel->setFontSize(15.0f);
	header_bg->attach(mTitleLabel);

	auto close = std::make_shared<Helpers::CloseButtonWidget>();
	close->setPivot(0.5f);
	close->setAnchor({ 1.0f, 0.0f });
	close->setPosition({ -72.0f, 62.0f });
	header_bg->attach(close);

	auto white_bg = std::make_shared<Scene::Sprite>();
	white_bg->setTexture(TEXTURE("textures/windows/warehouse_window/white_bg.png"));
	white_bg->setAnchor(0.5f);
	white_bg->setPivot(0.5f);
	white_bg->setPosition({ 0.0f, 4.0f });
	bg->attach(white_bg);

	auto main_panel = std::make_shared<Scene::Sprite>();
	main_panel->setTexture(TEXTURE("textures/windows/warehouse_window/main_panel.png"));
	main_panel->setAnchor({ 0.5f, 0.0f });
	main_panel->setPivot({ 0.5f, 0.0f });
	main_panel->setPosition({ 0.0f, 42.0f });
	white_bg->attach(main_panel);

	auto additional_panel1 = std::make_shared<Scene::Sprite>();
	additional_panel1->setTexture(TEXTURE("textures/windows/warehouse_window/additional_panel.png"));
	additional_panel1->setAnchor({ 0.5f, 0.0f });
	additional_panel1->setPivot({ 0.5f, 0.0f });
	additional_panel1->setPosition({ 0.0f, 492.0f });
	white_bg->attach(additional_panel1);

	auto additional_panel2 = std::make_shared<Scene::Sprite>();
	additional_panel2->setTexture(TEXTURE("textures/windows/warehouse_window/additional_panel.png"));
	additional_panel2->setAnchor({ 0.5f, 0.0f });
	additional_panel2->setPivot({ 0.5f, 0.0f });
	additional_panel2->setPosition({ 0.0f, 666.0f });
	white_bg->attach(additional_panel2);

	auto button = std::make_shared<Helpers::StandardLongButton>();
	button->setAnchor({ 0.5f, 0.0f });
	button->setPivot({ 0.5f, 0.0f });
	button->setPosition({ 0.0f, 838.0f });
	button->getLabel()->setText("awdawd");
	white_bg->attach(button);

	auto choose_x = std::make_shared<Helpers::ChooseMultiplierWidget>();
	choose_x->setAnchor({ 0.5f, 1.0f });
	choose_x->setPivot({ 0.5f, 1.0f });
	choose_x->setPosition({ 0.0f, -26.0f });
	bg->attach(choose_x);
}