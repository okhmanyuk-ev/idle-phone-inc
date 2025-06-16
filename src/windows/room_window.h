#pragma once

#include "window.h"

namespace PhoneInc
{
	class RoomWindow : public StandardWindow,
		public sky::Listenable<Profile::RoomChangedEvent>,
		public sky::Listenable<Profile::CashChangedEvent>
	{
	public:
		RoomWindow(int index);

	public:
		void onCloseBegin() override;

	public:
		void refresh();

	public:
		static bool CanUpgradeSomething(int room_index);

	public:
		void onEvent(const Profile::RoomChangedEvent& e) override;
		void onEvent(const Profile::CashChangedEvent& e) override;

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
		virtual int getMaxLevel() const = 0;
		virtual int getLevelsPerStage() const = 0;
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
		bool isOpened() const;
		bool isLastLevel() const;
		float getProgress() const;

	public:
		virtual std::wstring getOpenButtonText() const = 0;
		virtual std::wstring getUpgradeButtonText() const = 0;

	protected:
		std::wstring getLevelText() const;

	public:
		int getRoomIndex() const { return mRoomIndex; }
		const auto& getRoom() const { return PROFILE->getRooms().at(getRoomIndex()); }

		auto getButton() const { return mButton; }

	private:
		int mRoomIndex = 0;
		std::shared_ptr<Helpers::StandardButton> mButton;
		std::shared_ptr<Helpers::DollarEmitter> mDollarEmitter;
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
		int getMaxLevel() const override;
		int getLevelsPerStage() const override;
		bool isOpenAvailable() const override;
		double getUpgradeCost() const override;
		void increaseLevel() override;

	public:
		std::wstring getOpenButtonText() const override;
		std::wstring getUpgradeButtonText() const override;

	private:
		std::shared_ptr<Scene::Adaptive<Scene::Sprite>> mIcon;
		std::shared_ptr<Helpers::LabelSolid> mTitle;
		std::shared_ptr<Helpers::LabelSolid> mNameLabel;
		std::shared_ptr<Helpers::LabelSolid> mDescriptionLabel;
		std::shared_ptr<Helpers::LabelSolid> mLevelLabel;
		std::shared_ptr<Helpers::StreetProgressbar> mProgressbar;
		std::shared_ptr<Helpers::LabelSolid> mEffectLabelKey;
		std::shared_ptr<Helpers::LabelSolid> mEffectLabelValue;
	};

	class RoomWindow::SmallPanel : public Panel
	{
	public:
		SmallPanel(int roomIndex);

	public:
		void refresh() override;

	public:
		std::wstring getOpenButtonText() const override;
		std::wstring getUpgradeButtonText() const override;

	public:
		virtual Graphics::TexturePart getIconTexture() const = 0;
		virtual std::wstring getTitleText() const = 0;
		virtual std::wstring getDescriptionText() const = 0;
		virtual std::wstring getEffectText() const = 0;

	private:
		std::shared_ptr<Scene::Sprite> mIcon;
		std::shared_ptr<Helpers::LabelSolid> mTitle;
		std::shared_ptr<Helpers::LabelSolid> mLandingDescriptionLabel;
		std::shared_ptr<Helpers::LabelSolid> mLevelLabel;
		std::shared_ptr<Helpers::StreetProgressbar> mProgressbar;
		std::shared_ptr<Helpers::LabelSolid> mEffectLabelKey;
		std::shared_ptr<Helpers::LabelSolid> mEffectLabelValue;
	};

	class RoomWindow::ManagerPanel : public SmallPanel
	{
	public:
		ManagerPanel(int roomIndex);

	public:
		int getLevel() const override;
		int getMaxLevel() const override;
		int getLevelsPerStage() const override;
		bool isOpenAvailable() const override;
		double getUpgradeCost() const override;
		void increaseLevel() override;

	public:
		Graphics::TexturePart getIconTexture() const override;
		std::wstring getTitleText() const override;
		std::wstring getDescriptionText() const override;
		std::wstring getEffectText() const override;
	};

	class RoomWindow::WorkerPanel : public SmallPanel
	{
	public:
		WorkerPanel(int roomIndex, int number);

	public:
		int getLevel() const override;
		int getMaxLevel() const override;
		int getLevelsPerStage() const override;
		bool isOpenAvailable() const override;
		double getUpgradeCost() const override;
		void increaseLevel() override;

	public:
		Graphics::TexturePart getIconTexture() const override;
		std::wstring getTitleText() const override;
		std::wstring getDescriptionText() const override;
		std::wstring getEffectText() const override;

	private:
		int mNumber = 0;
	};
}
