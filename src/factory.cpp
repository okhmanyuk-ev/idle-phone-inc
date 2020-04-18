#include "factory.h"
#include "helpers.h"
#include "room.h"

using namespace PhoneInc;

Factory::Factory()
{
	glm::vec2 cell_size = { 895.0f, 376.0f };

	std::vector<std::shared_ptr<Node>> rooms;

	for (int i = 0; i < 30; i++)
	{
		auto room = std::make_shared<LockedRoom>(i + 1);
		room->setUnlockCallback([room, i] {
			auto parent = room->getParent();
			parent->detach(room);
			auto room = std::make_shared<Room>(i + 1);
			parent->attach(room);
		});
		rooms.push_back(room);
	}

	auto grid = Shared::SceneHelpers::MakeVerticalGrid(cell_size, rooms);
	grid->setAnchor({ 1.0f, 0.0f });
	grid->setPivot({ 1.0f, 0.0f });
	attach(grid);

	setHeight(grid->getHeight());

	auto conveyor_hat = std::make_shared<Scene::Sprite>();
	conveyor_hat->setTexture(TEXTURE("textures/factory/conveyor_hat.png"));
	attach(conveyor_hat);
}