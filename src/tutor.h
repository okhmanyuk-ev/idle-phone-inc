#pragma once

#include <shared/all.h>

#define TUTOR ENGINE->getSystem<PhoneInc::ITutor>()

namespace PhoneInc
{
	class ITutor
	{
	public:
		using CanStartCallback = std::function<bool()>;

	public:
		virtual void play(const std::string& name, std::shared_ptr<Scene::Node> node, CanStartCallback canStartCallback = nullptr) = 0;
		virtual void complete() = 0;
		virtual bool isCompleted(const std::string& name) const = 0;
	};

	class TutorHolder : public Scene::Actionable<Scene::Node>, public ITutor
	{
	public:
		TutorHolder();

	public:
		bool hitTest(const glm::vec2& value) const override;

	public:
		void play(const std::string& name, std::shared_ptr<Scene::Node> node, CanStartCallback canStartCallback) override;
		void complete() override;
		bool isCompleted(const std::string& name) const override;

	private:
		bool isPlaying() const;
		void removeOutdatedTutors();
		
	private:
		std::optional<std::string> mCurrentTutor = std::nullopt;

		struct Tutor
		{
			std::weak_ptr<Scene::Node> node;
			CanStartCallback canStartCallback;
		};

		std::map<std::string, Tutor> mTutors;

	private:
		std::shared_ptr<Scene::Sprite> mFinger;
	};
}