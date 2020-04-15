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
		ImGui::SetWindowPos(ImGui::User::BottomLeftCorner());

		static bool Enabled = false;

		if (Enabled)
		{
			if (ImGui::Button("CLEAR PROFILE"))
			{
				PROFILE->clear();
			}
		}

		ImGui::Checkbox("DEV", &Enabled);
		ImGui::End();
	}
}