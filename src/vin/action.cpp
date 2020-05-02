#include "action.h"
#include "logger.h"

#include <cassert>

using namespace std::string_literals;

namespace vin {

	namespace {

		std::string addKey(const std::string& str, SDL_Keycode code) {
			return fmt::format("{:<15} {:>3} ", str, SDL_GetKeyName(code));
		}

		std::string addKey(const std::string& str, SDL_Keycode code1, SDL_Keycode code2) {
			return fmt::format("{:<15} {:>3}+{}", str, SDL_GetKeyName(code1), SDL_GetKeyName(code2));
		}

		std::string shortcut(SDL_Keycode code) {
			return fmt::format("{}", SDL_GetKeyName(code));
		}

		std::string shortcut(SDL_Keycode code1, SDL_Keycode code2) {
			return fmt::format("{}+{}", SDL_GetKeyName(code1), SDL_GetKeyName(code2));
		}

	}

	Action::Action(const std::string& menuName, const Callback& callback)
		: callback_{callback}
		, menuName_{menuName} {
	}

	Action::Action(SDL_Keycode key, const std::string& menuName, const Callback& callback)
		: callback_{callback}
		, key_{key}
		, menuName_{menuName}
		, shortcut_{shortcut(key)}
		, checkboxMenuName_{addKey(menuName, key)} {
	}

	Action::Action(SDL_Keycode key1, SDL_Keycode key2, const std::string& menuName, const Callback& callback)
		: callback_{callback}
		, key_{key1}
		, key2_{key2}
		, menuName_{menuName}
		, shortcut_{shortcut(key1, key2)}
		, checkboxMenuName_{addKey(menuName, key1, key2)} {
	}

	void Action::update(const SDL_Keycode& key) {
		if (key_ == key && callback_) {
			auto state = SDL_GetKeyboardState(nullptr);
			if (key2_ == 0 || state[SDL_GetScancodeFromKey(key2_)] == 1) {
				spdlog::info("[Action] Call callback based on key {}", menuName_);
				callback_();
			}
		}
	}

	const char* Action::getMenuName() const {
		return menuName_.c_str();
	}

	const char* Action::getShortcut() const {
		return shortcut_.c_str();
	}

	const char* Action::getCheckboxMenuName() const {
		return checkboxMenuName_.c_str();
	}

	void Action::operator()() {
		if (callback_) {
			callback_();
		}
	}

	SDL_Keycode Action::getKey() const {
		return key_;
	}

	std::pair<SDL_Keycode, SDL_Keycode> Action::getKeyPair() const {
		return {key_, key2_};
	}

	const Action& ActionManager::add(const Action& action) {
		return actions_.insert({action.getKey(), action})->second;
	}

	void ActionManager::update(const SDL_Keycode& key) {
		if (actions_.bucket_count() == 0) {
			return;
		}
		auto index = actions_.bucket(key);
		for (auto it = actions_.begin(index); it != actions_.end(index); ++it) {
			auto& action = it->second;
			action.update(key);
		}
	}

	void ActionManager::clear() {
		actions_.clear();
	}

} // Namespace vin.
