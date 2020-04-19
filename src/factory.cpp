#include "factory.h"
#include "helpers.h"
#include "room.h"

using namespace PhoneInc;

Factory::Factory()
{
	glm::vec2 cell_size = { 895.0f, 376.0f };

	for (int i = 0; i < 30; i++)
	{
		if (PROFILE->isRoomLocked(i))
			mRooms.push_back(std::make_shared<LockedRoom>(i));
		else
			mRooms.push_back(std::make_shared<Room>(i));
	}

	auto grid = Shared::SceneHelpers::MakeVerticalGrid(cell_size, mRooms);
	grid->setAnchor({ 1.0f, 0.0f });
	grid->setPivot({ 1.0f, 0.0f });
	attach(grid);

	setHeight(grid->getHeight());
	 
	auto conveyor_path = std::make_shared<Scene::Actionable<Scene::Sprite>>();
	conveyor_path->setTexture(TEXTURE("textures/factory/conveyor_segment.png"));
	conveyor_path->setHeight(getHeight() * 2.0f);
	conveyor_path->setTextureAddress(Renderer::TextureAddress::Wrap);
	conveyor_path->setTexRegion({ { 0.0f, 0.0f }, { 0.0f, conveyor_path->getHeight() } });
	conveyor_path->setX(6.0f);
	attach(conveyor_path);
	conveyor_path->runAction(Shared::ActionHelpers::ExecuteInfinite([conveyor_path] {
		auto y = conveyor_path->getY();
		y -= Clock::ToSeconds(FRAME->getTimeDelta()) * 100.0f;
		auto tex_h = (float)conveyor_path->getTexture()->getHeight();
		if (y <= -tex_h) 
		{
			y += tex_h;
		}
		conveyor_path->setY(y);
	}));

	auto conveyor_hat = std::make_shared<Scene::Sprite>();
	conveyor_hat->setTexture(TEXTURE("textures/factory/conveyor_hat.png"));
	attach(conveyor_hat);
}

void Factory::event(const Profile::RoomUnlockedEvent& e)
{
	auto locked_room = mRooms.at(e.index);
	auto parent = mRooms[e.index]->getParent();
	parent->detach(locked_room);
	parent->attach(std::make_shared<Room>(e.index));
}