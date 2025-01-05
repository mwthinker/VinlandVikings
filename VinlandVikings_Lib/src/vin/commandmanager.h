#ifndef VINLANDVIKINGS_VIN_COMMANDMANAGER_H
#define VINLANDVIKINGS_VIN_COMMANDMANAGER_H

#include <functional>
#include <vector>
#include <concepts>

namespace vin {

	template <typename Originator, typename Snapshot> // 2
	concept CommandOriginator = requires (Originator& o, const Originator & o2, const Snapshot& s) {
		{ o2.getSnapshot() } ->std::convertible_to<Snapshot>;
		{ o.setSnapshot(s) };
	} && std::is_copy_assignable_v<Snapshot>;
	
	template <typename Originator, typename Snapshot>
	class CommandManager {
	public:
		using Command = std::function<bool()>;

		explicit CommandManager(Originator& originator)
			: originator_{originator} {
		}

		bool canRedo() const {
			return !future_.empty();
		}

		void redo() {
			if (!future_.empty()) {
				history_.push_back(originator_.getSnapshot());
				originator_.setSnapshot(future_.back());
				future_.pop_back();
			}
		}

		void undo() {
			if (!history_.empty()) {
				future_.push_back(originator_.getSnapshot());
				originator_.setSnapshot(history_.back());
				history_.pop_back();
			}
		}

		void pushCommand(const Command& command) {
			auto copy = originator_.getSnapshot();
			if (command()) {
				future_.clear();
				history_.push_back(copy);
			}
		}

		void clear() {
			future_.clear();
			history_.clear();
		}

	private:
		Originator& originator_;
		
		std::vector<Snapshot> future_;
		std::vector<Snapshot> history_;
	};

}

#endif
