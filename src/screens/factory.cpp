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

	auto column = std::make_shared<Scene::Column>();
	for (auto node : mRooms)
	{
		auto cell = std::make_shared<Scene::Node>();
		cell->setSize(cell_size);
		column->attach(cell);
		cell->attach(node);
	}
	column->setSize({ cell_size.x, cell_size.y * mRooms.size() });
	column->setAnchor({ 1.0f, 0.0f });
	column->setPivot({ 1.0f, 0.0f });
	attach(column);

	setHeight(column->getHeight());

	auto conveyor_path = std::make_shared<Scene::Sprite>();
	conveyor_path->setTexture(sky::GetTexture("textures/factory/conveyor/segment.png"));
	conveyor_path->setStretch({ 0.0f, 2.0f });
	conveyor_path->setTextureAddress(skygfx::TextureAddress::Wrap);
	conveyor_path->setX(6.0f);
	attach(conveyor_path);
	runAction(Actions::Collection::ExecuteInfinite([this, conveyor_path](auto dt) {
		conveyor_path->setTexRegion({ { 0.0f, 0.0f }, { 0.0f, conveyor_path->getAbsoluteHeight() } });
		auto y = conveyor_path->getY();

		auto speed = ConveyorSpeed;

		if (mBoxHolder->getNodes().empty())
			speed /= 5.0f;

		y -= sky::ToSeconds(dt) * 100.0f * speed;
		auto tex_h = (float)conveyor_path->getTexture()->getHeight();
		while (y <= -tex_h)
		{
			y += tex_h;
		}
		conveyor_path->setY(y);
	}));

	mBoxHolder = std::make_shared<Scene::Node>();
	attach(mBoxHolder);

	auto conveyor_hat = std::make_shared<Scene::Sprite>();
	conveyor_hat->setTexture(sky::GetTexture("textures/factory/conveyor/hat.png"));
	conveyor_hat->setY(-2.0f);
	attach(conveyor_hat);
}

void Factory::onEvent(const Profile::RoomUnlockedEvent& e)
{
	auto locked_room = mRooms.at(e.index);
	auto parent = mRooms[e.index]->getParent();
	parent->detach(locked_room);
	parent->attach(std::make_shared<Room>(e.index));
}

void Factory::onEvent(const ProductSpawnEvent& e)
{
	auto room = mRooms.at(e.room_index);
	auto height = room->getHeight();
	auto multiplier = (float)e.room_index;

	auto y = (height * multiplier) + (height * 0.75f);

	const float MaxDistance = 64.0f;

	for (auto node : mBoxHolder->getNodes())
	{
		if (glm::distance(node->getY(), y) > MaxDistance)
			continue;

		auto box = std::static_pointer_cast<Box>(node);
		box->setCount(box->getCount() + e.produce_count);
		if (box->isSpawnAnimationCompleted())
		{
			const float FullDuration = 0.25f;
			const float Duration = FullDuration / 2.0f;

			box->runAction(
				Actions::Collection::MakeSequence(
					Actions::Collection::MakeParallel(
						Actions::Collection::ChangeHorizontalScale(box, 0.75f, Duration, Easing::BackIn),
						Actions::Collection::ChangeVerticalScale(box, 1.25f, Duration, Easing::BackIn)
					),
					Actions::Collection::MakeParallel(
						Actions::Collection::ChangeHorizontalScale(box, 1.0f, Duration, Easing::BackIn),
						Actions::Collection::ChangeVerticalScale(box, 1.0f, Duration, Easing::BackIn)
					)
				)
			);
		}
		return;
	}

	auto box = std::make_shared<Box>();
	box->setTexture(sky::GetTexture("textures/factory/conveyor/box.png"));
	box->setPivot({ 0.5f, 1.0f });
	box->setX(96.0f);
	box->setY(y);
	box->setScale(0.0f);
	box->setCount(e.produce_count);
	box->runAction(Actions::Collection::MakeSequence(
		Actions::Collection::ChangeScale(box, { 1.0f, 1.0f }, 0.25f, Easing::BackOut),
		Actions::Collection::Execute([box] {
			box->setSpawnAnimationCompleted(true);
		}),
		Actions::Collection::ExecuteInfinite([box](auto dt) {
			auto y = box->getY();
			y -= sky::ToSeconds(dt) * 100.0f * ConveyorSpeed;
			box->setY(y);
			if (y <= 0)
			{
				PROFILE->setWarehouseStorage(PROFILE->getWarehouseStorage() + box->getCount());
				box->runAction(Actions::Collection::Kill(box));
			}
		})
	));
	mBoxHolder->attach(box);

	// sort boxes by y

	auto nodes = mBoxHolder->getNodes();
	nodes.sort([](auto left, auto right) {
		return left->getY() < right->getY();
	});
	mBoxHolder->clear();
	for (auto node : nodes)
	{
		mBoxHolder->attach(node);
	}
}