#ifndef VINLANDVIKINGS_VIN_VINLANDWINDOW_H
#define VINLANDVIKINGS_VIN_VINLANDWINDOW_H

#include "hexagonbatch.h"
#include "canvas.h"
#include "hexdata.h"

#include <sdl/imguiwindow.h>
#include <sdl/sprite.h>
#include <sdl/textureatlas.h>

#include <vector>
#include <map>

namespace vin {
	
	class HexSidesKey {
	public:
		HexSidesKey() = default;

		HexSidesKey(const HexSidesKey&) = default;

		HexSidesKey& operator=(const HexSidesKey&) = default;

		// Assumes one side is not bigger than one byte.
		HexSidesKey(const HexSides& sides)
			: key_(((int_least64_t)sides[0] << 8)
			+ ((int_least64_t)sides[1] << 8 * 2)
			+ ((int_least64_t)sides[2] << 8 * 3)
			+ ((int_least64_t)sides[3] << 8 * 4)
			+ ((int_least64_t)sides[4] << 8 * 5)
			+ ((int_least64_t)sides[5] << 8 * 6)) {
		}

		HexSidesKey(const HexImage& hexImage) : HexSidesKey(hexImage.getHexSides()) {
		}

		bool operator==(const HexSidesKey& key) const {
			return key_ == key.key_;
		}

		bool operator<(const HexSidesKey& key) const {
			return key_ < key.key_;
		}
		
	private:
		int_least64_t key_;
	};

	class HexImageTypeVector {
	public:
		HexImageTypeVector() : index_(0) {
		}

		int index_;
		std::vector<HexImage> hexImages_;
	};

	class VinlandWindow : public sdl::ImGuiWindow {
	public:
		VinlandWindow();

		~VinlandWindow();

		void beginMain();

		void endMain();

        enum class Page { START };
	private:
		void initPreLoop() override;

		void initOpenGl() override;

		void update(double deltaTime) override;

        void updateImGui(double deltaTime) override;

		void eventUpdate(const SDL_Event& windowEvent) override;

		void drawHexTypesButtons();

		Canvas canvas_;
		Page page_;
		HexagonBatch hexagonBatch_;
		sdl::Texture whiteSquare_;
		float zoom_;
		float x_, y_;
		std::vector<HexImage> hexImages_;
		std::map<HexSidesKey, HexImageTypeVector> hexTypes_;
	};

} // Namespace vin.

#endif // VINLANDVIKINGS_VIN_VINLANDWINDOW_H
