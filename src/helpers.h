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

	class Button : public Shared::SceneHelpers::BouncingButtonBehavior<Shared::SceneHelpers::SpriteButton>
	{
	private:
		const float MaxAutoclickTime = 0.5f;

	public:
		Button();

	protected:
		void onClick() override;
		void onChooseBegin() override;

	private:
		void internalClick();
		
	public:
		auto isAutoclick() const { return mAutoclick; }
		void setAutoclick(bool value) { mAutoclick = value; }

	private:
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

	class CloseButton : public Button
	{
	public:
		CloseButton();
	};

	class Progressbar : public Scene::SlicedSprite
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

	class DollarEmitter : public Scene::Emitter
	{
	public:
		static inline std::shared_ptr<Scene::Node> Holder = nullptr;

	public:
		DollarEmitter();

	public:
		void emitPack();
	};
}
