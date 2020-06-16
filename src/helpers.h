#pragma once

#include <shared/all.h>
#include <profile.h>

namespace PhoneInc::Helpers
{
	// types

	using Callback = std::function<void()>;
	
	struct MoveGlobalScrollEvent { glm::vec2 pos; };
	struct BlockGlobalScrollEvent { bool blocked; };

	// constants

	inline static const float Scale = 3.0f;
	
	// static

	std::string NumberToString(double value);

	template <typename T> T GetRandomElement(const std::vector<T>& elements)
	{
        // TODO: int must be size_t, but size_t does not compiles in xcode
		return elements.at(glm::linearRand<int>(0, elements.size() - 1));
    }

	// classes

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

	class LabelSolidBold : public LabelSolid
	{
	public:
		LabelSolidBold();
	};

	class Button : public Scene::Clickable<Scene::Sprite>, public std::enable_shared_from_this<Button>
	{
	private:
		const float MaxAutoclickTime = 0.5f;

	protected:
		void update() override;

	protected:
		void onClick() override;
		void onChooseBegin() override;
		void onChooseEnd() override;

	private:
		void internalClick();
		void recursiveColorSet(const glm::vec4& value, std::shared_ptr<Scene::Node> node = nullptr);

	public:
		auto isActive() const { return mActive; }
		void setActive(bool value);

		auto getActiveTexture() const { return mActiveTexture; }
		void setActiveTexture(std::shared_ptr<Renderer::Texture> value);

		auto getInactiveTexture() const { return mInactiveTexture; }
		void setInactiveTexture(std::shared_ptr<Renderer::Texture> value);

		auto getActiveCallback() const { return mActiveCallback; }
		void setActiveCallback(Callback value) { mActiveCallback = value; }
	
		auto getInactiveCallback() const { return mInactiveCallback; }
		void setInactiveCallback(Callback value) { mInactiveCallback = value; }

		auto isAutoclick() const { return mAutoclick; }
		void setAutoclick(bool value) { mAutoclick = value; }

	private:
		void ensureTexture();

	private:
		bool mActive = true;
		std::shared_ptr<Renderer::Texture> mActiveTexture = nullptr;
		std::shared_ptr<Renderer::Texture> mInactiveTexture = nullptr;
		Callback mActiveCallback = nullptr;
		Callback mInactiveCallback = nullptr;
		bool mAutoclick = false;
		float mNextAutoclick = 0.0f;
		int mAutoclickCount = 0;
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

	class CloseButtonWidget : public Button
	{
	public:
		CloseButtonWidget();
	};

	class Progressbar : public Scene::Actionable<Scene::SlicedSprite>
	{
	public:
		Progressbar();

	public:
		float getProgress() const { return mClip->getHorizontalStretch(); }
		void setProgress(float value) { mClip->setHorizontalStretch(value); }

	protected:
		std::shared_ptr<Scene::Node> mHolder;
		std::shared_ptr<Scene::SlicedSprite> mContent;

	private:
		std::shared_ptr<Scene::Clippable<Scene::Node>> mClip;
	};

	class StreetProgressbar : public Progressbar
	{
	public:
		StreetProgressbar();
	};

	class DollarEmitter : public Shared::SceneHelpers::SpriteEmitter
	{
	public:
		static inline std::shared_ptr<Scene::Node> Holder = nullptr;

	public:
		DollarEmitter();

	public:
		void emitPack();
	};
}
