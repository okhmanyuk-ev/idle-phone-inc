#pragma once

#include "window.h"

namespace PhoneInc
{
	class BuildingUpgradeWindow : public Window
	{
	public:
		BuildingUpgradeWindow();

	public:
		auto getTitleLabel() { return mTitleLabel; }

	private:
		std::shared_ptr<Helpers::LabelSolid> mTitleLabel;
	};
}
