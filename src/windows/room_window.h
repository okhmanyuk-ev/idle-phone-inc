#pragma once

#include "window.h"

namespace PhoneInc
{
	class RoomWindow : public Window,
		public Common::EventSystem::Listenable<Profile::RoomChangedEvent>,
		public Common::EventSystem::Listenable<Profile::CashChangedEvent>
	{
	public:
		RoomWindow(int index);

	public:
		void refresh();

	public:
		void event(const Profile::RoomChangedEvent& e) override;
		void event(const Profile::CashChangedEvent& e) override;

	private:

		class Panel;
		class ProductPanel;
		class SmallPanel;
		class ManagerPanel;
		class WorkerPanel;

	private:
		std::shared_ptr<ProductPanel> mProductPanel;
		std::shared_ptr<ManagerPanel> mManagerPanel;
		std::shared_ptr<WorkerPanel> mWorkerPanel1;
		std::shared_ptr<WorkerPanel> mWorkerPanel2;
		std::shared_ptr<WorkerPanel> mWorkerPanel3;

	private:
		int mIndex = 0;
	};

	class Upgradable
	{
	public:
		virtual int getLevel() const = 0;
		virtual bool isOpened() const = 0;
		virtual bool isLastLevel() const = 0;
		virtual bool isOpenAvailable() const = 0;
		virtual double getUpgradeCost() const = 0;
		virtual void increaseLevel() = 0;
	};

	class RoomWindow::Panel : public Scene::Sprite, public Upgradable
	{
	public:
		Panel(int roomIndex);

	public:
		virtual void refresh();

	public:
		virtual utf8_string getOpenButtonText() const = 0;
		virtual utf8_string getUpgradeButtonText() const = 0;

	protected:
		utf8_string getLevelText() const;

	public:
		int getRoomIndex() const { return mRoomIndex; }
		const auto& getRoom() const { return PROFILE->getRooms().at(getRoomIndex()); }

	private:
		int mRoomIndex = 0;
		std::shared_ptr<Helpers::StandardButton> mButton;
		std::shared_ptr<Helpers::Label> mButtonAdditionalLabel;
	};

	class RoomWindow::ProductPanel : public Panel
	{
	public:
		ProductPanel(int roomIndex);

	public:
		void refresh() override;

	public:
		int getLevel() const override;
		bool isOpened() const override;
		bool isLastLevel() const override;
		bool isOpenAvailable() const override;
		double getUpgradeCost() const override;
		void increaseLevel() override;

	public:
		utf8_string getOpenButtonText() const override;
		utf8_string getUpgradeButtonText() const override;

	private:
		std::shared_ptr<Helpers::Adaptive<Scene::Sprite>> mIcon;
		std::shared_ptr<Helpers::LabelSolid> mTitle;
		std::shared_ptr<Helpers::LabelSolid> mLevelLabel;
		std::shared_ptr<Helpers::LabelSolid> mOpenDescriptionLabel;
	};

	class RoomWindow::SmallPanel : public Panel
	{
	public:
		SmallPanel(int roomIndex);

	public:
		void refresh() override;

	public:
		utf8_string getOpenButtonText() const override;
		utf8_string getUpgradeButtonText() const override;

	public:
		virtual std::shared_ptr<Renderer::Texture> getIconTexture() const = 0;
		virtual utf8_string getTitleText() const = 0;
		virtual utf8_string getHireDescriptionText() const = 0;

	private:
		std::shared_ptr<Scene::Sprite> mIcon;
		std::shared_ptr<Helpers::LabelSolid> mTitle;
		std::shared_ptr<Helpers::LabelSolid> mLevelLabel;
		std::shared_ptr<Helpers::LabelSolid> mHireDescriptionLabel;
	};

	class RoomWindow::ManagerPanel : public SmallPanel
	{
	public:
		ManagerPanel(int roomIndex);

	public:
		int getLevel() const override;
		bool isOpenAvailable() const override;
		bool isOpened() const override;
		bool isLastLevel() const override;
		double getUpgradeCost() const override;
		void increaseLevel() override;

	public:
		std::shared_ptr<Renderer::Texture> getIconTexture() const override;
		utf8_string getTitleText() const override;
		utf8_string getHireDescriptionText() const override;
	};

	class RoomWindow::WorkerPanel : public SmallPanel
	{
	public:
		WorkerPanel(int roomIndex, int number);

	public:
		int getLevel() const override;
		bool isOpenAvailable() const override;
		bool isOpened() const override;
		bool isLastLevel() const override;
		double getUpgradeCost() const override;
		void increaseLevel() override;

	public:
		std::shared_ptr<Renderer::Texture> getIconTexture() const override;
		utf8_string getTitleText() const override;
		utf8_string getHireDescriptionText() const override;

	private:
		int mNumber = 0;
	};
}
