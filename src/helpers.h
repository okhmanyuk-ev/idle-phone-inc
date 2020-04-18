#pragma once

#include <shared/all.h>

namespace PhoneInc::Helpers
{
	using Callback = std::function<void()>;

	inline static const float Scale = 3.0f;
	inline static const float InvScale = 1.0f / 3.0f;

	class Label : public Scene::Label
	{
	public:
		Label();
	};

	class LabelSolid : public Label
	{
	public:
		LabelSolid();
	};

	class Button : public Scene::Clickable<Scene::Sprite>, public std::enable_shared_from_this<Button>
	{
	public:
		Button();

	private:
		void recursiveColorSet(const glm::vec4& value, std::shared_ptr<Scene::Node> node = nullptr);

	public:
		auto isActive() const { return mActive; }
		void setActive(bool value);

		auto getActiveTexture() const { return mActiveTexture; }
		void setActiveTexture(std::shared_ptr<Renderer::Texture> value);

		auto getInactiveTexture() const { return mInactiveTexture; }
		void setInactiveTexture(std::shared_ptr<Renderer::Texture> value);

	private:
		void ensureTexture();

	private:
		bool mActive = true;
		std::shared_ptr<Renderer::Texture> mActiveTexture = nullptr;
		std::shared_ptr<Renderer::Texture> mInactiveTexture = nullptr;
	};

	class StandardButton : public Button
	{
	public:
		StandardButton();

	public:
		auto getLabel() const { return mLabel; }

	private:
		std::shared_ptr<Label> mLabel;
	};

	class StandardLongButton : public Button
	{
	public:
		StandardLongButton();

	public:
		auto getLabel() const { return mLabel; }

	private:
		std::shared_ptr<Label> mLabel;
	};
}