#include "cheats.h"

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

	auto pos = ImGui::User::TopLeftCorner();
	pos.y += 54.0f;

	ImGui::Begin("dev", nullptr, ImGui::User::ImGuiWindowFlags_ControlPanel);
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

		if (ImGui::Button("CLEAR PROFILE"))
		{
			PROFILE->clear();
		}
	}
	ImGui::End();
}