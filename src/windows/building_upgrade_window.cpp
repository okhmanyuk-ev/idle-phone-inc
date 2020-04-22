#include "building_upgrade_window.h"

using namespace PhoneInc;

BuildingWindow::BuildingWindow()
{
	auto bg = std::make_shared<Scene::Sprite>();
	bg->setTexture(TEXTURE("textures/windows/building_upgrade_window/bg.png"));
	bg->setAnchor(0.5f);
	bg->setPivot(0.5f);
	bg->setTouchable(true);
	getContent()->attach(bg);

	auto frame = std::make_shared<Scene::Sprite>();
	frame->setTexture(TEXTURE("textures/windows/building_upgrade_window/frame.png"));
	frame->setAnchor(0.5f);
	frame->setPivot(0.5f);
	getContent()->attach(frame);

	auto header_bg = std::make_shared<Scene::Sprite>();
	header_bg->setTexture(TEXTURE("textures/windows/building_upgrade_window/header_bg.png"));
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
	white_bg->setTexture(TEXTURE("textures/windows/building_upgrade_window/white_bg.png"));
	white_bg->setAnchor(0.5f);
	white_bg->setPivot(0.5f);
	white_bg->setPosition({ 0.0f, 4.0f });
	bg->attach(white_bg);

	auto main_panel = createMainPanel(mMainPanel);
	main_panel->setAnchor({ 0.5f, 0.0f });
	main_panel->setPivot({ 0.5f, 0.0f });
	main_panel->setPosition({ 0.0f, 42.0f });
	white_bg->attach(main_panel);

	auto parameter_panel_1 = createParameterPanel(mParameterPanel1);
	parameter_panel_1->setAnchor({ 0.5f, 0.0f });
	parameter_panel_1->setPivot({ 0.5f, 0.0f });
	parameter_panel_1->setPosition({ 0.0f, 492.0f });
	white_bg->attach(parameter_panel_1);

	auto parameter_panel_2 = createParameterPanel(mParameterPanel2);
	parameter_panel_2->setAnchor({ 0.5f, 0.0f });
	parameter_panel_2->setPivot({ 0.5f, 0.0f });
	parameter_panel_2->setPosition({ 0.0f, 662.0f  });
	white_bg->attach(parameter_panel_2);

	mUpgradeButton = std::make_shared<Helpers::StandardLongButton>();
	mUpgradeButton->setAnchor({ 0.5f, 0.0f });
	mUpgradeButton->setPivot({ 0.5f, 0.0f });
	mUpgradeButton->setPosition({ 0.0f, 838.0f });
	white_bg->attach(mUpgradeButton);
}

void BuildingWindow::refresh()
{
	mTitleLabel->setText(getTitle());
	mUpgradeButton->getLabel()->setText("$ " + Helpers::NumberToString(getUpgradePrice()));

	mMainPanel.level->setText(LOCALIZE_FMT("BUILDING_WINDOW_LEVEL", getLevel()));
	mMainPanel.building_name->setText(getBuildingName());
	mMainPanel.building_icon->setTexture(getBuildingTexture());

	auto first = getFirstParameter();
	mParameterPanel1.icon->setTexture(first.icon_texture);
	mParameterPanel1.title->setText(first.title_text);
	mParameterPanel1.effect->setText(first.effect_text);

	auto second = getSecondParameter();
	mParameterPanel2.icon->setTexture(second.icon_texture);
	mParameterPanel2.title->setText(second.title_text);
	mParameterPanel2.effect->setText(second.effect_text);
}

std::shared_ptr<Scene::Node> BuildingWindow::createMainPanel(MainPanel& panel)
{
	auto bg = std::make_shared<Scene::Sprite>();
	bg->setTexture(TEXTURE("textures/windows/building_upgrade_window/main_panel.png"));
	
	auto building_icon = std::make_shared<Scene::Sprite>();
	building_icon->setAnchor({ 0.0f, 0.5f });
	building_icon->setPivot(0.5f);
	building_icon->setPosition({ 164.0f, 0.0f });
	bg->attach(building_icon);

	auto building_name = std::make_shared<Helpers::LabelSolid>();
	building_name->setPivot({ 0.0f, 0.5f });
	building_name->setPosition({ 340.0f, 114.0f });
	building_name->setColor(Graphics::Color::ToNormalized(12, 22, 44));
	building_name->setFontSize(14.0f);
	bg->attach(building_name);

	auto level = std::make_shared<Helpers::LabelSolid>();
	level->setPivot({ 0.0f, 0.5f });
	level->setPosition({ 340.0f, 184.0f });
	level->setColor(Graphics::Color::ToNormalized(23, 0, 164));
	level->setFontSize(19.0f);
	bg->attach(level);

	panel.building_icon = building_icon;
	panel.building_name = building_name;
	panel.level = level;

	return bg;
}

std::shared_ptr<Scene::Node> BuildingWindow::createParameterPanel(ParameterPanel& panel)
{
	auto bg = std::make_shared<Scene::Sprite>();
	bg->setTexture(TEXTURE("textures/windows/building_upgrade_window/additional_panel.png"));

	auto icon = std::make_shared<Scene::Sprite>();
	icon->setAnchor({ 0.0f, 0.5f });
	icon->setPivot({ 0.0f, 0.5f });
	icon->setPosition({ 32.0f, 0.0f });
	bg->attach(icon);

	auto title = std::make_shared<Helpers::LabelSolid>();
	title->setAnchor({ 0.0f, 0.5f });
	title->setPivot({ 0.0f, 0.5f });
	title->setPosition({ 176.0f, 0.0f });
	title->setColor(Graphics::Color::Black);
	title->setFontSize(12.0f);
	bg->attach(title);

	auto effect = std::make_shared<Helpers::LabelSolid>();
	effect->setAnchor({ 1.0f, 0.5f });
	effect->setPivot({ 1.0f, 0.5f });
	effect->setPosition({ -40.0f, 0.0f });
	effect->setColor(Graphics::Color::ToNormalized(6, 103, 0));
	effect->setFontSize(16.0f);
	bg->attach(effect);

	panel.icon = icon;
	panel.title = title;
	panel.effect = effect;

	return bg;
}
