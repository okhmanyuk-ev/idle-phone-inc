#pragma once

#include <shared/all.h>
#include "profile.h"

#define TUTOR ENGINE->getSystem<PhoneInc::TutorialSystem>()

namespace PhoneInc
{
	class TutorialSystem
	{
	public:
		using CanStartCallback = std::function<bool()>;
		using Callback = std::function<void()>;

	public:
		virtual void play(const std::string& name, std::shared_ptr<Scene::Node> node, CanStartCallback canStartCallback = nullptr, 
			Callback beginCallback = nullptr, Callback endCallback = nullptr) = 0;
		virtual void complete() = 0;
		virtual bool isCompleted(const std::string& name) const = 0;
	};

	class TutorHolder : public Scene::Actionable<Scene::Node>, public TutorialSystem,
		public Common::EventSystem::Listenable<Profile::ProfileClearedEvent>
	{
	public:
		TutorHolder();

	public:
		void event(const Profile::ProfileClearedEvent& e) override;

	public:
		bool hitTest(const glm::vec2& value) const override;

	public:
		void play(const std::string& name, std::shared_ptr<Scene::Node> node, CanStartCallback canStartCallback = nullptr,
			Callback beginCallback = nullptr, Callback endCallback = nullptr) override;
		void complete() override;
		bool isCompleted(const std::string& name) const override;

	private:
		bool isPlaying() const;
		void removeOutdatedTutors();
		void showFinger(bool visible);
		void moveFinger();
		void chooseCurrentTutor();

	private:
		std::optional<std::string> mCurrentTutor = std::nullopt;

		struct Tutor
		{
			std::weak_ptr<Scene::Node> node;
			CanStartCallback canStartCallback;
			Callback beginCallback;
			Callback endCallback;
		};

		std::map<std::string, Tutor> mTutors;

		enum FingerState
		{
			Opening,
			Opened,
			Closing,
			Closed,
		};

		FingerState mFingerState = FingerState::Closed;

	private:
		std::shared_ptr<Scene::Sprite> mFinger;
	};
}