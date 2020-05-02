#include "cheats.h"
#include "balance.h"

using namespace PhoneInc;

void Cheats::ShowDevMenu(std::shared_ptr<Shared::SceneManager> scene_manager)
{
#if !defined(BUILD_DEVELOPER)
	return;
#endif	

	auto categorySpacing = [] {
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
	};

	ImGui::Begin("dev", nullptr, ImGui::User::ImGuiWindowFlags_ControlPanel);

	auto pos = ImGui::User::TopLeftCorner();
	pos.y += 54.0f;

	ImGui::SetWindowPos(pos);

	static bool Enabled = false;

	ImGui::Checkbox("DEV", &Enabled);

	if (Enabled)
	{
		categorySpacing();

		auto ensureCashNonZero = [] {
			if (PROFILE->getCash() <= 0.0)
			{
				PROFILE->setCash(1.0);
			}
		};

		if (ImGui::Button("ADD CASH x10"))
		{
			ensureCashNonZero();
			PROFILE->setCash(PROFILE->getCash() * 10.0);
		}

		if (ImGui::Button("ADD CASH x100"))
		{
			ensureCashNonZero();
			PROFILE->setCash(PROFILE->getCash() * 100.0);
		}

		if (ImGui::Button("ADD CASH x1000"))
		{
			ensureCashNonZero();
			PROFILE->setCash(PROFILE->getCash() * 1000.0);
		}

		if (ImGui::Button("REMOVE ALL CASH"))
		{
			PROFILE->setCash(0.0);
		}
		
		categorySpacing();

		if (ImGui::Button("UNLOCK ALL ROOMS"))
		{
			for (int i = 0; i < Balance::MaxRooms; i++)
			{
				if (!PROFILE->isRoomLocked(i))
					continue;

				PROFILE->unlockRoom(i);
			}
		}

		if (ImGui::Button("UPGRADE ALL ROOMS"))
		{
			for (int i = 0; i < Balance::MaxRooms; i++)
			{
				if (PROFILE->isRoomLocked(i))
					continue;

				auto room = PROFILE->getRooms().at(i);
				room.product = Balance::MaxProductLevel;
				room.manager = Balance::MaxManagerLevel;
				room.worker1 = Balance::MaxWorkerLevel;
				room.worker2 = Balance::MaxWorkerLevel;
				room.worker3 = Balance::MaxWorkerLevel;
				PROFILE->setRoom(i, room);
			}
		}

		if (ImGui::Button("DOWNGRADE ALL ROOMS"))
		{
			for (int i = 0; i < Balance::MaxRooms; i++)
			{
				if (PROFILE->isRoomLocked(i))
					continue;

				auto room = PROFILE->getRooms().at(i);
				room.product = 0;
				room.manager = 0;
				room.worker1 = 0;
				room.worker2 = 0;
				room.worker3 = 0;
				PROFILE->setRoom(i, room);
			}
		}

		if (ImGui::Button("RANDOMIZE ALL ROOMS"))
		{
			for (int i = 0; i < Balance::MaxRooms; i++)
			{
				if (PROFILE->isRoomLocked(i))
					continue;

				auto room = PROFILE->getRooms().at(i);
				room.product = glm::linearRand(1, Balance::MaxProductLevel);
				room.manager = glm::linearRand(1, Balance::MaxManagerLevel);
				room.worker1 = glm::linearRand(1, Balance::MaxWorkerLevel);
				room.worker2 = glm::linearRand(1, Balance::MaxWorkerLevel);
				room.worker3 = glm::linearRand(1, Balance::MaxWorkerLevel);
				PROFILE->setRoom(i, room);
			}
		}

		categorySpacing();

		if (ImGui::Button("CLEAR PROFILE"))
		{
			PROFILE->clear();
			PROFILE->saveAsync();
		}
	}
	ImGui::End();
}