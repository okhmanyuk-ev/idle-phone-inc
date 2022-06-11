#pragma once

#include "window.h"

namespace PhoneInc
{
	class BuildingWindow : public StandardWindow,
		public Common::Event::Listenable<Profile::CashChangedEvent>,
		public Common::Event::Listenable<Profile::WarehouseLevelChangedEvent>
	{
	public:
		BuildingWindow();

	public:
		void refresh();

	public:
		static bool CanUpgrade();

	private:
		void onEvent(const Profile::CashChangedEvent& e) override;
		void onEvent(const Profile::WarehouseLevelChangedEvent& e) override;

	private:
		struct MainPanel
		{
			std::shared_ptr<Scene::Sprite> building_icon;
			std::shared_ptr<Helpers::LabelSolid> building_name;
			std::shared_ptr<Helpers::LabelSolid> level;
			std::shared_ptr<Helpers::StreetProgressbar> progressbar;
		};

		struct ParameterPanel
		{
			std::shared_ptr<Scene::Sprite> icon;
			std::shared_ptr<Helpers::LabelSolid> title;
			std::shared_ptr<Helpers::LabelSolid> effect;
		};

	private:
		std::shared_ptr<Scene::Node> createMainPanel(MainPanel& panel);
		std::shared_ptr<Scene::Node> createParameterPanel(ParameterPanel& panel);
		
	protected:
		struct Parameter
		{
			Graphics::TexCell icon_texture;
			utf8_string title_text;
			utf8_string effect_text;
		};

	protected:
		Parameter getFirstParameter() const;
		Parameter getSecondParameter() const;
		Parameter getThirdParameter() const;

	private:
		MainPanel mMainPanel;
		ParameterPanel mParameterPanel1;
		ParameterPanel mParameterPanel2;
		ParameterPanel mParameterPanel3;
		std::shared_ptr<Helpers::StandardLongButton> mUpgradeButton;
		std::shared_ptr<Helpers::DollarEmitter> mDollarEmitter;
	};
}
