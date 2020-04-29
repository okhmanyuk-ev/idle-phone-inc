#pragma once

#include "window.h"

namespace PhoneInc
{
	class BuildingWindow : public Window,
		public Common::EventSystem::Listenable<Profile::CashChangedEvent>
	{
	public:
		BuildingWindow();

	public:
		void refresh();

	private:
		void event(const Profile::CashChangedEvent& e) override;

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
			std::shared_ptr<Renderer::Texture> icon_texture;
			utf8_string title_text;
			utf8_string effect_text;
		};

	protected:
		virtual utf8_string getTitle() const = 0;
		virtual utf8_string getBuildingName() const = 0;
		virtual int getLevel() const = 0;
		virtual int getMaxLevel() const = 0;
		virtual int getLevelsPerStage() const = 0;
		virtual std::shared_ptr<Renderer::Texture> getBuildingTexture() const = 0;
		virtual double getUpgradePrice() const = 0;
		virtual Parameter getFirstParameter() const = 0;
		virtual Parameter getSecondParameter() const = 0;
		virtual void upgrade() = 0;

	private:
		MainPanel mMainPanel;
		ParameterPanel mParameterPanel1;
		ParameterPanel mParameterPanel2;
		std::shared_ptr<Helpers::LabelSolid> mTitleLabel;
		std::shared_ptr<Helpers::StandardLongButton> mUpgradeButton;
	};
}
