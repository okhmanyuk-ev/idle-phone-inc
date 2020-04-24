#pragma once

#include "window.h"

namespace PhoneInc
{
	class RoomWindow : public Window,
		public Common::EventSystem::Listenable<Profile::RoomChangedEvent>
	{
	public:
		RoomWindow(int index);

	public:
		void refresh();

	public:
		void event(const Profile::RoomChangedEvent& e) override;

	private:

		/*	struct MajorPanel
		{
			std::shared_ptr<Scene::Sprite> icon;
			std::shared_ptr<Helpers::StandardButton> button;
		};

		struct MinorPanel
		{
			std::shared_ptr<Scene::Sprite> icon;
			std::shared_ptr<Helpers::LabelSolid> title;
			std::shared_ptr<Helpers::LabelSolid> description;
			std::shared_ptr<Helpers::StandardButton> button;
		};*/

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


	class RoomWindow::ProductPanel : public Scene::Sprite
	{
	public:
		ProductPanel();

	public:
		void refresh();
	};

	class RoomWindow::SmallPanel : public Scene::Sprite
	{
	public:
		SmallPanel();

	public:
		void refresh();

	protected:
		virtual std::shared_ptr<Renderer::Texture> getIconTexture() const = 0;

	private:
		std::shared_ptr<Scene::Sprite> mIcon;
	};

	class RoomWindow::ManagerPanel : public SmallPanel
	{
	public:
		std::shared_ptr<Renderer::Texture> getIconTexture() const override;
	};

	class RoomWindow::WorkerPanel : public SmallPanel
	{
	public:
		std::shared_ptr<Renderer::Texture> getIconTexture() const override;
	};
}
