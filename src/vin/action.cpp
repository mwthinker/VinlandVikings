#include "action.h"

namespace vin {

	Action::Action(SDL_Keycode key, const std::string& menuName, const Callback& callback) :
		callback_{callback}, key_{key}, menuName_{menuName} {
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
		return callbacks_.insert({action.getKey(), action})->second;
	}

	void ActionManager::update(const SDL_Keycode& key) {
		if (callbacks_.bucket_count() == 0) {
			return;
		}
		auto index = callbacks_.bucket(key);
		for (auto it = callbacks_.begin(index); it != callbacks_.end(index); ++it) {
			auto& callback = it->second;
			callback();
		}
	}

	void ActionManager::clear() {
		callbacks_.clear();
	}

} // Namespace vin.
