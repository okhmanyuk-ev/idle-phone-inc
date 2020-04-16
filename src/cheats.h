#pragma once

#include <shared/all.h>
#include "defines.h"
#include "profile.h"

namespace PhoneInc
{
	inline void ShowCheatsMenu(std::shared_ptr<Shared::SceneManager> scene_manager)
	{
#if !defined(BUILD_DEVELOPER)
		return;
#endif	

		ImGui::Begin("dev", nullptr, ImGui::User::ImGuiWindowFlags_ControlPanel);
		
		auto pos = ImGui::User::TopLeftCorner();
		pos.y += 54.0f;

		ImGui::SetWindowPos(pos);

		static bool Enabled = false;

		ImGui::Checkbox("DEV", &Enabled);

		if (Enabled)
		{
			if (ImGui::Button("CLEAR PROFILE"))
			{
				PROFILE->clear();
			}
		}
		ImGui::End();
	}
}