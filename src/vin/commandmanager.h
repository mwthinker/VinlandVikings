#ifndef VINLANDVIKINGS_VIN_COMMANDMANAGER_H
#define VINLANDVIKINGS_VIN_COMMANDMANAGER_H

#include <functional>
#include <vector>

namespace vin {
	
	template <typename Originator, typename Snapshot>
	class CommandManager {
	public:
		using Command = std::function<bool()>;

		explicit CommandManager(Originator& originator)
			: originator_{originator} {
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
				future_.emplace_back(originator_.getSnapshot());
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
