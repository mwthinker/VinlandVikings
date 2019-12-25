#ifndef VINLANDVIKINGS_VIN_ACTION_H
#define VINLANDVIKINGS_VIN_ACTION_H

#include <sdl/initsdl.h>

#include <functional>

namespace vin {
	
	class Action {
	public:
		using Callback = std::function<void()>;

		Action() = default;

		Action(SDL_Keycode key, const std::string& menuName, const Callback& callback);

		void update(const SDL_Keycode& key);

		const char* getMenuName() const;

		void operator()();

		SDL_Keycode getKey() const;

	private:
		Callback callback_;
		SDL_Keycode key_{};
		std::string menuName_;
	};

	class ActionManager {
	public:
		const Action& add(const Action& action);

		void update(const SDL_Keycode& key);

		void clear();

	private:
		std::unordered_multimap<SDL_Keycode, Action> actions_;
	};

} // Namespace vin.

#endif // VINLANDVIKINGS_VIN_ACTION_H
