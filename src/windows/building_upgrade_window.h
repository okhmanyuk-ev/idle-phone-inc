#pragma once

#include "window.h"
#include "profile.h"

namespace PhoneInc
{
	class BuildingUpgradeWindow : public Window
	{
	public:
		BuildingUpgradeWindow();

	public:
		void refresh();

	private:
		std::shared_ptr<Scene::Node> createMainPanel();

	protected:
		virtual utf8_string getTitle() const = 0;
		virtual int getLevel() const = 0;

	private:
		std::shared_ptr<Helpers::LabelSolid> mTitleLabel;
		std::shared_ptr<Helpers::LabelSolid> mLevelLabel;
	};
}
