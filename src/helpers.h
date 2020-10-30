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
		return elements.at(glm::linearRand<int>(0, static_cast<int>(elements.size()) - 1));
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
		
	public:
		auto isActive() const { return mActive; }
		void setActive(bool value);

		auto getActiveTexture() const { return mActiveTexture; }
		void setActiveTexture(Graphics::TexCell value);

		auto getInactiveTexture() const { return mInactiveTexture; }
		void setInactiveTexture(Graphics::TexCell value);

		auto getActiveCallback() const { return mActiveCallback; }
		void setActiveCallback(Callback value) { mActiveCallback = value; }
	
		auto getInactiveCallback() const { return mInactiveCallback; }
		void setInactiveCallback(Callback value) { mInactiveCallback = value; }

		auto isAutoclick() const { return mAutoclick; }
		void setAutoclick(bool value) { mAutoclick = value; }

		void setActiveSound(std::shared_ptr<Audio::Sound> value) { mActiveSound = value; }
		void setInactiveSound(std::shared_ptr<Audio::Sound> value) { mInactiveSound = value; }

	private:
		void ensureTexture();

	private:
		bool mActive = true;
		Graphics::TexCell mActiveTexture;
		Graphics::TexCell mInactiveTexture;
		std::shared_ptr<Audio::Sound> mActiveSound = SOUND("sounds/click.wav");
		std::shared_ptr<Audio::Sound> mInactiveSound = SOUND("sounds/click.wav");
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

		auto getContent() const { return mContent; }

	protected:
		std::shared_ptr<Scene::Node> mHolder;
		std::shared_ptr<Scene::SlicedSprite> mContent;

	private:
		std::shared_ptr<Scene::ClippableScissor<Scene::Node>> mClip;
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
