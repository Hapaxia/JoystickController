//////////////////////////////////////////////////////////////////////////////
//
// Joystick Controller (https://github.com/Hapaxia/JoystickController
//
// Copyright(c) 2023-2026 M.J.Silk
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions :
//
// 1. The origin of this software must not be misrepresented; you must not
// claim that you wrote the original software.If you use this software
// in a product, an acknowledgment in the product documentation would be
// appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not be
// misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
// M.J.Silk
// MJSilk2@gmail.com
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>
#include <SFML/Window/Joystick.hpp>
#include <SFML/System/Vector2.hpp>

namespace joystickcontroller
{

enum class AxisPair
{
	XY,
	UV,
	PovXY,
};

enum class Axis
{
	X,
	Y,
	U,
	V,
	Z,
	R,
	PovX,
	PovY,
};

class JoystickController
{
public:
	JoystickController();

	std::size_t getConnectedCount() const;
	std::vector<std::size_t> getConnectedIndices() const;

	std::size_t getButtonCount(std::size_t joystickIndex) const;
	bool getHasAxis(std::size_t joystickIndex, Axis axis) const;
	std::size_t getAxisCount(std::size_t joystickIndex) const;
	std::vector<Axis> getAxes(std::size_t joystickIndex) const;

	bool isButtonPressed(std::size_t joystickIndex, std::size_t buttonIndex) const;
	float getAxisPositionRaw(std::size_t joystickIndex, Axis axis) const;
	sf::Vector2f getAxis2dPositionRaw(std::size_t joystickIndex, AxisPair axisPair, bool invertX = false, bool invertY = false) const;
	sf::Vector2f getAxis2dPositionCustomAxisPairRaw(std::size_t joystickIndex, Axis axisX, Axis axisY, bool invertX = false, bool invertY = false) const;

	float getAxisPosition(std::size_t joystickIndex, Axis axis) const;
	sf::Vector2f getAxis2dPosition(std::size_t joystickIndex, AxisPair axisPair, bool invertX = false, bool invertY = false) const;
	sf::Vector2f getAxis2dPositionCustomAxisPair(std::size_t joystickIndex, Axis axisX, Axis axisY, bool invertX = false, bool invertY = false) const;

	float getAxis2dDirection(std::size_t joystickIndex, AxisPair axisPair, bool invertX = false, bool invertY = false) const;
	float getAxis2dDirectionCustomAxisPair(std::size_t joystickIndex, Axis axisX, Axis axisY, bool invertX = false, bool invertY = false) const;
	float getAxis2dStrength(std::size_t joystickIndex, AxisPair axisPair) const;
	float getAxis2dStrengthCustomAxisPair(std::size_t joystickIndex, Axis axisX, Axis axisY) const;

	std::size_t getAxis2dDirectionIndex(std::size_t joystickIndex, AxisPair axisPair, std::size_t numberOfSegments, bool invertX = false, bool invertY = false) const;
	std::size_t getAxis2dDirectionIndexCustomAxisPair(std::size_t joystickIndex, Axis axisX, Axis axisY, std::size_t numberOfSegments, bool invertX = false, bool invertY = false) const;
	std::size_t getAxis2dStrengthIndex(std::size_t joystickIndex, AxisPair axisPair, std::size_t numberOfSteps) const;
	std::size_t getAxis2dStrengthIndexCustomAxisPair(std::size_t joystickIndex, Axis axisX, Axis axisY, std::size_t numberOfSteps) const;

	int getAxis2dAsPovHat(std::size_t joystickIndex, AxisPair axisPair, bool invertX = false, bool invertY = false) const;
	int getAxis2dAsPovHatCustomAxisPair(std::size_t joystickIndex, Axis axisX, Axis axisY, bool invertX = false, bool invertY = false) const;





	void setCalibrationAxisZero(std::size_t joystickIndex, Axis axis, float zeroValue);
	void setCalibrationAxisMinPositive(std::size_t joystickIndex, Axis axis, float minPositive);
	void setCalibrationAxisMaxPositive(std::size_t joystickIndex, Axis axis, float maxPositive);
	void setCalibrationAxisMinNegative(std::size_t joystickIndex, Axis axis, float minNegative);
	void setCalibrationAxisMaxNegative(std::size_t joystickIndex, Axis axis, float maxNegative);

	float getCalibrationAxisZero(std::size_t joystickIndex, Axis axis) const;
	float getCalibrationAxisMinPositive(std::size_t joystickIndex, Axis axis) const;
	float getCalibrationAxisMaxPositive(std::size_t joystickIndex, Axis axis) const;
	float getCalibrationAxisMinNegative(std::size_t joystickIndex, Axis axis) const;
	float getCalibrationAxisMaxNegative(std::size_t joystickIndex, Axis axis) const;



private:
	static constexpr float piFloat{ 3.141592653f };
	static constexpr float halfPiFloat{ piFloat / 2.f };
	static constexpr float doublePiFloat{ piFloat * 2.f };

	std::vector<bool> m_areJoysticksConnected;
	std::size_t m_numberOfConnectedJoysticks;

	enum class CalibrationField
	{
		Zero,
		MinPositive,
		MaxPositive,
		MinNegative,
		MaxNegative,
	};

	struct AxisCalibration
	{
		float zero{ 0.f };
		float minPositive{ 0.f };
		float maxPositive{ 100.f };
		float minNegative{ 0.f };
		float maxNegative{ -100.f };
	};

	struct Calibration
	{
		std::size_t joystickIndex{ 0u };
		AxisCalibration x{};
		AxisCalibration y{};
		AxisCalibration u{};
		AxisCalibration v{};
		AxisCalibration z{};
		AxisCalibration r{};
		AxisCalibration povX{};
		AxisCalibration povY{};
	};

	std::vector<Calibration> m_calibrations;



	std::size_t priv_getCalibrationIndex(const std::size_t joystickIndex) const;
	void priv_setCalibrationValue(const std::size_t joystickIndex, const Axis axis, const CalibrationField calibrationField, const float newValue);
	void priv_setCalibrationFieldValue(AxisCalibration& axisCalibration, const CalibrationField calibrationField, const float newValue);
	float priv_getCalibrationValue(const std::size_t joystickIndex, const Axis axis, const CalibrationField calibrationField) const;
	float priv_getCalibrationFieldValue(const AxisCalibration& axisCalibration, const CalibrationField calibrationField) const;
	sf::Joystick::Axis priv_getSfmlJoystickAxis(const Axis axis) const;
};

} // namespace

#include "JoystickController.inl"

#ifndef JOYSTICKCONTROLLER_NO_NAMESPACE_SHORTCUT
namespace jc = joystickcontroller;
#endif // JOYSTICKCONTROLLER_NO_NAMESPACE_SHORTCUT
