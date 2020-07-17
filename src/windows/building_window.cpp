#include "building_window.h"

using namespace PhoneInc;

BuildingWindow::BuildingWindow()
{
	getBackground()->setSize({ 962.0f, 1326.0f });
	getTitle()->setText(LOCALIZE("WAREHOUSE_WINDOW_TITLE"));

	auto white_bg = std::make_shared<Scene::Sprite>();
	white_bg->setTexture(TEXTURE("textures/windows/building_window/white_bg.png"));
	white_bg->setAnchor({ 0.5f, 0.0f });
	white_bg->setPivot({ 0.5f, 0.0f });
	white_bg->setPosition({ 0.0f, 140.0f });
	getBackground()->attach(white_bg);

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
	parameter_panel_2->setPosition({ 0.0f, 662.0f });
	white_bg->attach(parameter_panel_2);

	auto parameter_panel_3 = createParameterPanel(mParameterPanel3);
	parameter_panel_3->setAnchor({ 0.5f, 0.0f });
	parameter_panel_3->setPivot({ 0.5f, 0.0f });
	parameter_panel_3->setPosition({ 0.0f, 832.0f });
	white_bg->attach(parameter_panel_3);

	mUpgradeButton = std::make_shared<Helpers::StandardLongButton>();
	mUpgradeButton->setAnchor({ 0.5f, 0.0f });
	mUpgradeButton->setPivot({ 0.5f, 0.0f });
	mUpgradeButton->setPosition({ 0.0f, 992.0f });
	mUpgradeButton->setAutoclick(true);
	mUpgradeButton->setActiveCallback([this] {
		auto prev_stage = Balance::GetWarehouseStage();
		PROFILE->spendCash(Balance::GetWarehouseCost());
		PROFILE->setWarehouseLevel(PROFILE->getWarehouseLevel() + 1);
		mDollarEmitter->emitPack();
		if (Balance::GetWarehouseStage() != prev_stage)
			AUDIO->play(SOUND("sounds/success.wav"));
	});
	white_bg->attach(mUpgradeButton);

	mDollarEmitter = std::make_shared<Helpers::DollarEmitter>();
	mUpgradeButton->attach(mDollarEmitter);

	refresh();
}

void BuildingWindow::refresh()
{
	auto level = PROFILE->getWarehouseLevel();

	mUpgradeButton->setEnabled(level < Balance::MaxWarehouseLevel);
	if (mUpgradeButton->isEnabled())
	{
		mUpgradeButton->setActive(CanUpgrade());
		mUpgradeButton->getLabel()->setText("$ " + Helpers::NumberToString(Balance::GetWarehouseCost()));
	}

	mMainPanel.level->setText(LOCALIZE_FMT("BUILDING_WINDOW_LEVEL", level));
	mMainPanel.building_name->setText(LOCALIZE(fmt::format("WAREHOUSE_NAME_{}", Balance::GetWarehouseStage())));
	mMainPanel.building_icon->setTexture(TEXTURE(fmt::format("textures/warehouse/{}.png", Balance::GetWarehouseStage())));
	mMainPanel.building_icon->applyTextureSize();

	auto current = (((level - 1) % Balance::WarehouseLevelsPerStage) + 1);
	auto total = Balance::WarehouseLevelsPerStage;
	mMainPanel.progressbar->setProgress((float)current / (float)total);

	auto first = getFirstParameter();
	mParameterPanel1.icon->setTexture(first.icon_texture);
	mParameterPanel1.title->setText(first.title_text);
	mParameterPanel1.effect->setText(first.effect_text);

	auto second = getSecondParameter();
	mParameterPanel2.icon->setTexture(second.icon_texture);
	mParameterPanel2.title->setText(second.title_text);
	mParameterPanel2.effect->setText(second.effect_text);

	auto third = getThirdParameter();
	mParameterPanel3.icon->setTexture(third.icon_texture);
	mParameterPanel3.title->setText(third.title_text);
	mParameterPanel3.effect->setText(third.effect_text);
}

bool BuildingWindow::CanUpgrade()
{
	return PROFILE->isEnoughCash(Balance::GetWarehouseCost());
}

void BuildingWindow::event(const Profile::CashChangedEvent& e)
{
	refresh();
}

void BuildingWindow::event(const Profile::WarehouseLevelChangedEvent& e)
{
	refresh();
}

std::shared_ptr<Scene::Node> BuildingWindow::createMainPanel(MainPanel& panel)
{
	auto bg = std::make_shared<Scene::Sprite>();
	bg->setTexture(TEXTURE("textures/windows/building_window/main_panel.png"));
	
	auto building_icon = std::make_shared<Scene::Sprite>();
	building_icon->setAnchor({ 0.0f, 0.5f });
	building_icon->setPivot(0.5f);
	building_icon->setPosition({ 164.0f, 0.0f });
	bg->attach(building_icon);

	auto building_name = std::make_shared<Helpers::LabelSolid>();
	building_name->setPivot({ 0.0f, 0.5f });
	building_name->setPosition({ 340.0f, 114.0f });
	building_name->setColor(Graphics::Color::ToNormalized(12, 22, 44));
	building_name->setFontSize(42.0f);
	bg->attach(building_name);

	auto level = std::make_shared<Helpers::LabelSolid>();
	level->setPivot({ 0.0f, 0.5f });
	level->setPosition({ 340.0f, 184.0f });
	level->setColor(Graphics::Color::ToNormalized(23, 0, 164));
	level->setFontSize(57.0f);
	bg->attach(level);

	auto progressbar = std::make_shared<Helpers::StreetProgressbar>();
	progressbar->setPivot({ 0.0f, 0.5f });
	progressbar->setPosition({ 340.0f, 272.0f });
	progressbar->setSize({ 436.0f, 32.0f });
	bg->attach(progressbar);

	panel.building_icon = building_icon;
	panel.building_name = building_name;
	panel.level = level;
	panel.progressbar = progressbar;

	return bg;
}

std::shared_ptr<Scene::Node> BuildingWindow::createParameterPanel(ParameterPanel& panel)
{
	auto bg = std::make_shared<Scene::Sprite>();
	bg->setTexture(TEXTURE("textures/windows/building_window/additional_panel.png"));

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
	title->setFontSize(36.0f);
	bg->attach(title);

	auto effect = std::make_shared<Helpers::LabelSolid>();
	effect->setAnchor({ 1.0f, 0.5f });
	effect->setPivot({ 1.0f, 0.5f });
	effect->setPosition({ -40.0f, 0.0f });
	effect->setColor(Graphics::Color::ToNormalized(6, 103, 0));
	effect->setFontSize(48.0f);
	bg->attach(effect);

	panel.icon = icon;
	panel.title = title;
	panel.effect = effect;

	return bg;
}

BuildingWindow::Parameter BuildingWindow::getFirstParameter() const
{
	auto duration = Balance::GetWarehouseDuration();
	auto multiplier = Balance::WarehouseMaxDuration / duration;

	multiplier -= 1.0f;
	multiplier *= 100.0f;

	auto result = BuildingWindow::Parameter();
	result.title_text = LOCALIZE("WAREHOUSE_WINDOW_PARAM_NAME_1");
	result.effect_text = fmt::format("+{:.0f}%", multiplier); // https://fmt.dev/latest/syntax.html
	result.icon_texture = TEXTURE("textures/windows/building_window/icon1.png");
	return result;
}

BuildingWindow::Parameter BuildingWindow::getSecondParameter() const
{
	auto result = BuildingWindow::Parameter();
	result.title_text = LOCALIZE("WAREHOUSE_WINDOW_PARAM_NAME_2");
	result.effect_text = Helpers::NumberToString(Balance::GetWarehouseTruckCapacity());
	result.icon_texture = TEXTURE("textures/windows/building_window/icon2.png");
	return result;
}

BuildingWindow::Parameter BuildingWindow::getThirdParameter() const
{
	auto result = BuildingWindow::Parameter();
	result.title_text = LOCALIZE("WAREHOUSE_WINDOW_PARAM_NAME_3");
	result.effect_text = Helpers::NumberToString(Balance::GetWarehouseEarning());
	result.icon_texture = TEXTURE("textures/windows/building_window/icon3.png");
	return result;
}