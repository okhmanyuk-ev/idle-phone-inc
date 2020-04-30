#include "factory.h"
#include "helpers.h"
#include "room.h"
#include "balance.h"

using namespace PhoneInc;

Factory::Factory()
{
	glm::vec2 cell_size = { 895.0f, 376.0f };

	for (int i = 0; i < Balance::MaxRooms; i++)
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
	 
	auto conveyor_path = std::make_shared<Scene::Sprite>();
	conveyor_path->setTexture(TEXTURE("textures/factory/conveyor_segment.png"));
	conveyor_path->setStretch({ -1.0f, 2.0f });
	conveyor_path->setTextureAddress(Renderer::TextureAddress::Wrap);
	conveyor_path->setX(6.0f);
	attach(conveyor_path);
	runAction(Shared::ActionHelpers::ExecuteInfinite([conveyor_path] {
		conveyor_path->setTexRegion({ { 0.0f, 0.0f }, { 0.0f, conveyor_path->getHeight() } });
		auto y = conveyor_path->getY();
		y -= Clock::ToSeconds(FRAME->getTimeDelta()) * 100.0f * ConveyorSpeed;
		auto tex_h = (float)conveyor_path->getTexture()->getHeight();
		while (y <= -tex_h) 
		{
			y += tex_h;
		}
		conveyor_path->setY(y);
	}));

	mBoxHolder = std::make_shared<Scene::Sprite>();
	attach(mBoxHolder);

	auto conveyor_hat = std::make_shared<Scene::Sprite>();
	conveyor_hat->setTexture(TEXTURE("textures/factory/conveyor_hat.png"));
	conveyor_hat->setY(-1.0f);
	attach(conveyor_hat);
}

void Factory::event(const Profile::RoomUnlockedEvent& e)
{
	auto locked_room = mRooms.at(e.index);
	auto parent = mRooms[e.index]->getParent();
	parent->detach(locked_room);
	parent->attach(std::make_shared<Room>(e.index));
}

void Factory::event(const ProductSpawnEvent& e)
{
	auto room = mRooms.at(e.room_index);
	auto height = room->getHeight();
	auto multiplier = (float)e.room_index;

	auto box = std::make_shared<Scene::Actionable<Scene::Cullable<Scene::Sprite>>>();
	box->setTexture(TEXTURE("textures/factory/box.png"));
	box->setPivot({ 0.5f, 1.0f });
	box->setX(96.0f);
	box->setY((height * multiplier) + (height * 0.75f));
	box->setScale(0.0f);
	box->runAction(Shared::ActionHelpers::MakeSequence(
		Shared::ActionHelpers::ChangeScale(box, { 1.0f, 1.0f }, 0.25f, Common::Easing::BackOut),
		Shared::ActionHelpers::ExecuteInfinite([box] {
			auto y = box->getY();
			y -= Clock::ToSeconds(FRAME->getTimeDelta()) * 100.0f * ConveyorSpeed;
			box->setY(y);
			if (y <= 0)
			{
				PROFILE->increaseWarehouseStorage();
				box->runAction(Shared::ActionHelpers::Kill(box));
			}
		})
	));
	mBoxHolder->attach(box);
}