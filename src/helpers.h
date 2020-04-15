#pragma once

#include <shared/all.h>

namespace PhoneInc::Helpers
{
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

	class Button : public Scene::Sprite
	{
	public:
		Button();

	public:
		auto getLabel() const { return mLabel; }

		auto isActive() const { return mActive; }
		void setActive(bool value);

	private:
		std::shared_ptr<Label> mLabel;
		bool mActive = true;
	};
}