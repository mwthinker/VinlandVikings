#include "action.h"

#include <cassert>

using namespace std::string_literals;

namespace vin {

	namespace {

		constexpr int DEFAULT_POS = 19;

		std::string addKey(const std::string& str, SDL_Keycode code, int pos = DEFAULT_POS) {
			assert(pos > 0);
			std::string newValue(pos + 2, ' ');
			newValue.replace(0, str.size(), str);
			newValue.replace(pos, 1, SDL_GetKeyName(code));
			return newValue;
		}

	}

	Action::Action(SDL_Keycode key, const std::string& menuName, const Callback& callback) :
		callback_{callback}, key_{key}, menuName_{addKey(menuName, key)} {
	}

	void Action::update(const SDL_Keycode& key) {
		if (key_ == key && callback_) {
			callback_();
		}
	}

	const char* Action::getMenuName() const {
		return menuName_.c_str();
	}

	void Action::operator()() {
		if (callback_) {
			callback_();
		}
	}

	SDL_Keycode Action::getKey() const {
		return key_;
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
