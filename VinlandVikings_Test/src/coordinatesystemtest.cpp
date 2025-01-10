#include "print.h"

#include <vin/coordinatesystem.h>
#include <glm/gtx/intersect.hpp>

#include <gtest/gtest.h>

namespace vin {

	namespace {

		constexpr HexDimension Dimension;

		void assertEq(const Mat4& a, const Mat4& b) {
			for (int i = 0; i < 4; ++i) {
				for (int j = 0; j < 4; ++j) {
					EXPECT_NEAR(a[i][j], b[i][j], 0.00001f);
				}
			}
		}

		void assertEq(const Vec4& a, const Vec4& b) {
			for (int i = 0; i < 4; ++i) {
				EXPECT_NEAR(a[i], b[i], 0.00001f);
			}
		}

	}

	class CoordinateSystemTest : public ::testing::Test {
	protected:
		CoordinateSystemTest() {}

		~CoordinateSystemTest() override {}

		void SetUp() override {}

		void TearDown() override {}
	};

	TEST_F(CoordinateSystemTest, getMatrix_from_to_itself) {
		// Given
		CoordinateSystem coordinateSystem{Dimension};

		// When
		auto I_1 = coordinateSystem.getMatrix(CoordinateSystem::Space::World, CoordinateSystem::Space::World);
		auto I_2 = coordinateSystem.getMatrix(CoordinateSystem::Space::Screen, CoordinateSystem::Space::Screen);
		auto I_3 = coordinateSystem.getMatrix(CoordinateSystem::Space::Clip, CoordinateSystem::Space::Clip);
		auto I_4 = coordinateSystem.getMatrix(CoordinateSystem::Space::Camera, CoordinateSystem::Space::Camera);

		// Then
		auto I = glm::mat4{1};

		assertEq(I_1, I);
		assertEq(I_2, I);
		assertEq(I_3, I);
		assertEq(I_4, I);
	}

	TEST_F(CoordinateSystemTest, getMatrix_inverse_cameraToWorld) {
		// Given
		CoordinateSystem coordinateSystem{Dimension};

		// When
		auto worldToCamera = coordinateSystem.getMatrix(CoordinateSystem::Space::World, CoordinateSystem::Space::Camera);
		auto cameraToWorld = coordinateSystem.getMatrix(CoordinateSystem::Space::Camera, CoordinateSystem::Space::World);

		// Then
		assertEq(worldToCamera, glm::inverse(cameraToWorld));
	}

	TEST_F(CoordinateSystemTest, getMatrix_inverse_cameraToClip) {
		// Given
		CoordinateSystem coordinateSystem{Dimension};
		
		// When
		auto cameraToClip = coordinateSystem.getMatrix(CoordinateSystem::Space::Camera, CoordinateSystem::Space::Clip);
		auto clipToCamera = coordinateSystem.getMatrix(CoordinateSystem::Space::Clip, CoordinateSystem::Space::Camera);

		// Then
		assertEq(cameraToClip, glm::inverse(clipToCamera));
	}

	TEST_F(CoordinateSystemTest, getMatrix_inverse_cameraToScreen) {
		// Given
		CoordinateSystem coordinateSystem{Dimension};
		
		// When
		auto cameraToScreen = coordinateSystem.getMatrix(CoordinateSystem::Space::Camera, CoordinateSystem::Space::Screen);
		auto screenToCamera = coordinateSystem.getMatrix(CoordinateSystem::Space::Screen, CoordinateSystem::Space::Camera);
		
		// Then
		assertEq(cameraToScreen, glm::inverse(screenToCamera));
	}

	TEST_F(CoordinateSystemTest, getMatrix_inverse_worldToClip) {
		// Given
		CoordinateSystem coordinateSystem{Dimension};

		// When
		auto worldToClip = coordinateSystem.getMatrix(CoordinateSystem::Space::World, CoordinateSystem::Space::Clip);
		auto clipToWorld = coordinateSystem.getMatrix(CoordinateSystem::Space::Clip, CoordinateSystem::Space::World);

		// Then
		assertEq(worldToClip, glm::inverse(clipToWorld));
	}

	TEST_F(CoordinateSystemTest, getMatrix_inverse_worldToScreen) {
		// Given
		CoordinateSystem coordinateSystem{Dimension};

		// When
		auto worldToScreen = coordinateSystem.getMatrix(CoordinateSystem::Space::World, CoordinateSystem::Space::Screen);
		auto screenToWorld = coordinateSystem.getMatrix(CoordinateSystem::Space::Screen, CoordinateSystem::Space::World);

		// Then
		assertEq(worldToScreen, glm::inverse(screenToWorld));
	}

	TEST_F(CoordinateSystemTest, getMatrix_inverse_screenToClip) {
		// Given
		CoordinateSystem coordinateSystem{Dimension};
		
		// When
		auto screenToClip = coordinateSystem.getMatrix(CoordinateSystem::Space::Screen, CoordinateSystem::Space::Clip);
		auto clipToScreen = coordinateSystem.getMatrix(CoordinateSystem::Space::Clip, CoordinateSystem::Space::Screen);
		
		// Then
		assertEq(screenToClip, glm::inverse(clipToScreen));
	}

	TEST_F(CoordinateSystemTest, worldToHex) {
		// Given
		CoordinateSystem coordinateSystem{Dimension};
		coordinateSystem.setCameraToClip(glm::ortho(0.f, 1.f, 0.f, 1.f, -1.f, 1.f));
		coordinateSystem.setCameraToClip(glm::mat4{8});

		// When
		hex::Hexi hex = coordinateSystem.worldToHex({0.f, 9.5});

		// Then
		ASSERT_EQ(hex::Hexi(0, 5), hex);
	}

	TEST_F(CoordinateSystemTest, screenToWorld_from_origo) {
		// Given
		CoordinateSystem coordinateSystem{Dimension};
		coordinateSystem.setViewport(100, 100, {0, 0, 100, 100});
		coordinateSystem.setCameraToClip(glm::mat4{1});

		// When
		Vec4 pos = coordinateSystem.getMatrix(CoordinateSystem::Space::Screen, CoordinateSystem::Space::World) * glm::vec4{0.f, 0.f, 0.f, 1.f};

		// Then
		assertEq(Vec4(-1.f, 1.f, 0.f, 1.f), pos);
	}

	TEST_F(CoordinateSystemTest, screenToWorld_to_origo) {
		// Given
		CoordinateSystem coordinateSystem{Dimension};
		coordinateSystem.setViewport(100, 100, {0, 0, 100, 100});
		coordinateSystem.setCameraToClip(glm::mat4{1});

		// When
		Vec4 pos = coordinateSystem.getMatrix(CoordinateSystem::Space::Screen, CoordinateSystem::Space::World) * glm::vec4{50.f, 50.f, 0.f, 1.f};

		// Then
		assertEq(Vec4(0.f, 0.f, 0.f, 1.f), pos);
	}

	TEST_F(CoordinateSystemTest, screenToWorld_toOrigo_thenTilt) {
		// Given
		CoordinateSystem coordinateSystem{Dimension};
		coordinateSystem.setViewport(100, 100, {0, 0, 100, 100});
		coordinateSystem.setCameraToClip(glm::mat4{1});
		coordinateSystem.tilt(Pi / 4.f);

		// When
		Vec4 pos = coordinateSystem.getMatrix(CoordinateSystem::Space::Screen, CoordinateSystem::Space::World) * glm::vec4{50.f, 50.f, 0.f, 1.f};

		// Then
		assertEq(Vec4(0.f, 0.f, 0.f, 1.f), pos);
	}

}
