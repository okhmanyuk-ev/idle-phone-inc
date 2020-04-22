#pragma once

#include "building_window.h"

namespace PhoneInc
{
	class ShopWindow : public BuildingWindow
	{
	public:
		ShopWindow();

	protected:
		utf8_string getTitle() const override;
		utf8_string getBuildingName() const override;
		int getLevel() const override;
		std::shared_ptr<Renderer::Texture> getBuildingTexture() const override;
		double getUpgradePrice() const override;
		Parameter getFirstParameter() const override;
		Parameter getSecondParameter() const override;
	};
}
