#include "warehouse_window.h"

using namespace PhoneInc;

WarehouseWindow::WarehouseWindow()
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

	auto title = std::make_shared<Helpers::LabelSolid>();
	title->setAnchor(0.5f);
	title->setPivot(0.5f);
	title->setText("title");
	title->setFontSize(15.0f);
	header_bg->attach(title);

	auto close = std::make_shared<Helpers::CloseWindowButton>();
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

	auto choose_x_bg = std::make_shared<Scene::Sprite>();
	choose_x_bg->setTexture(TEXTURE("textures/windows/common/choose_x_bg.png"));
	choose_x_bg->setAnchor({ 0.5f, 1.0f });
	choose_x_bg->setPivot({ 0.5f, 1.0f });
	choose_x_bg->setPosition({ 0.0f, -26.0f });
	bg->attach(choose_x_bg);

	auto choose_x = std::make_shared<Scene::Sprite>();
	choose_x->setTexture(TEXTURE("textures/windows/common/choose_x.png"));
	choose_x->setAnchor(0.5f);
	choose_x->setPivot(0.5f);
	choose_x_bg->attach(choose_x);
}