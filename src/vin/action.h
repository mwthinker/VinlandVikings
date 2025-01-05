#ifndef VINLANDVIKINGS_VIN_ACTION_H
#define VINLANDVIKINGS_VIN_ACTION_H

#include <sdl/initsdl.h>

#include <functional>
#include <string>

namespace vin {
	
	class Action {
	public:
		using Callback = std::function<void()>;

		Action() = default;

		Action(const std::string& menuName, const Callback& callback);
		Action(SDL_Keycode key, const std::string& menuName, const Callback& callback);
		Action(SDL_Keycode key1, SDL_Keycode key2, const std::string& menuName, const Callback& callback);

		void update(const SDL_Keycode& key);

		const char* getMenuName() const;
		const char* getShortcut() const;
		const char* getCheckboxMenuName() const;

		void operator()();

		SDL_Keycode getKey() const;

		std::pair<SDL_Keycode, SDL_Keycode> getKeyPair() const;

		bool isActive() const {
			return active_;
		}

		void setActive(bool active) {
			active_ = active;
		}

	private:
		bool active_ = true;
		Callback callback_;
		SDL_Keycode key_{};
		SDL_Keycode key2_{};
		std::string menuName_;
		std::string checkboxMenuName_;
		std::string shortcut_;
	};

	class ActionManager {
	public:
		const Action& add(const Action& action);

		void update(const SDL_Keycode& key);

		void clear();

	private:
		std::unordered_multimap<SDL_Keycode, Action> actions_;
	};

}

#endif
