#include "factory.h"
#include "helpers.h"
#include "room.h"

using namespace PhoneInc;

Factory::Factory()
{
	setHeight(300.0f);

	glm::vec2 cell_size = { 895.0f, 376.0f };

	auto grid = Shared::SceneHelpers::MakeVerticalGrid(cell_size, {
		std::make_shared<Room>(1),
		std::make_shared<Room>(2),
		std::make_shared<Room>(3)
	});

	grid->setAnchor({ 1.0f, 0.0f });
	grid->setPivot({ 1.0f, 0.0f });
	attach(grid);

	auto conveyor_hat = std::make_shared<Scene::Sprite>();
	conveyor_hat->setTexture(TEXTURE("textures/factory/conveyor_hat.png"));
	attach(conveyor_hat);
}