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

#include <assert.h>

#include "JoystickController.hpp"

namespace joystickcontroller
{

inline JoystickController::JoystickController()
	: m_areJoysticksConnected(8u)
	, m_numberOfConnectedJoysticks{ 0u }
{
	sf::Joystick::update();
	for (std::size_t j{ 0u }; j < m_areJoysticksConnected.size(); ++j)
	{
		if (m_areJoysticksConnected[j] = sf::Joystick::isConnected(static_cast<unsigned int>(j)))
		{
			++m_numberOfConnectedJoysticks;
			Calibration calibration;
			calibration.joystickIndex = j;
			m_calibrations.push_back(calibration);
		}
	}
}

inline std::size_t JoystickController::getConnectedCount() const
{
	return m_numberOfConnectedJoysticks;
}

inline std::vector<std::size_t> JoystickController::getConnectedIndices() const
{
	std::vector<std::size_t> indices;
	for (std::size_t j{ 0u }; j < m_areJoysticksConnected.size(); ++j)
	{
		if (m_areJoysticksConnected[j])
			indices.push_back(j);
	}
	return indices;
}

inline std::size_t JoystickController::getButtonCount(const std::size_t joystickIndex) const
{
	return sf::Joystick::getButtonCount(static_cast<unsigned int>(joystickIndex));
}

inline bool JoystickController::getHasAxis(const std::size_t joystickIndex, Axis axis) const
{
	return sf::Joystick::hasAxis(static_cast<unsigned int>(joystickIndex), priv_getSfmlJoystickAxis(axis));
}

inline std::size_t JoystickController::getAxisCount(const std::size_t joystickIndex) const
{
	std::size_t numberOfAxes{ 0u };
	if (getHasAxis(joystickIndex, Axis::X))
		++numberOfAxes;
	if (getHasAxis(joystickIndex, Axis::Y))
		++numberOfAxes;
	if (getHasAxis(joystickIndex, Axis::U))
		++numberOfAxes;
	if (getHasAxis(joystickIndex, Axis::V))
		++numberOfAxes;
	if (getHasAxis(joystickIndex, Axis::Z))
		++numberOfAxes;
	if (getHasAxis(joystickIndex, Axis::R))
		++numberOfAxes;
	if (getHasAxis(joystickIndex, Axis::PovX))
		++numberOfAxes;
	if (getHasAxis(joystickIndex, Axis::PovY))
		++numberOfAxes;
	return numberOfAxes;
}

inline std::vector<Axis> JoystickController::getAxes(const std::size_t joystickIndex) const
{
	std::vector<Axis> axes;
	if (getHasAxis(joystickIndex, Axis::X))
		axes.push_back(Axis::X);
	if (getHasAxis(joystickIndex, Axis::Y))
		axes.push_back(Axis::Y);
	if (getHasAxis(joystickIndex, Axis::U))
		axes.push_back(Axis::U);
	if (getHasAxis(joystickIndex, Axis::V))
		axes.push_back(Axis::V);
	if (getHasAxis(joystickIndex, Axis::Z))
		axes.push_back(Axis::Z);
	if (getHasAxis(joystickIndex, Axis::R))
		axes.push_back(Axis::R);
	if (getHasAxis(joystickIndex, Axis::PovX))
		axes.push_back(Axis::PovX);
	if (getHasAxis(joystickIndex, Axis::PovY))
		axes.push_back(Axis::PovY);
	return axes;
}

inline bool JoystickController::isButtonPressed(const std::size_t joystickIndex, const std::size_t buttonIndex) const
{
	return sf::Joystick::isButtonPressed(static_cast<unsigned int>(joystickIndex), static_cast<unsigned int>(buttonIndex));
}

inline float JoystickController::getAxisPositionRaw(const std::size_t joystickIndex, const Axis axis) const
{
	return sf::Joystick::getAxisPosition(static_cast<unsigned int>(joystickIndex), priv_getSfmlJoystickAxis(axis));
}

inline sf::Vector2f JoystickController::getAxis2dPositionRaw(const std::size_t joystickIndex, const AxisPair axisPair, const bool invertX, const bool invertY) const
{
	switch (axisPair)
	{
	case AxisPair::XY:
		return { getAxis2dPositionCustomAxisPairRaw(joystickIndex, Axis::X, Axis::Y, invertX, invertY) };
	case AxisPair::UV:
		return { getAxis2dPositionCustomAxisPairRaw(joystickIndex, Axis::U, Axis::V, invertX, invertY) };
	case AxisPair::PovXY:
		return { getAxis2dPositionCustomAxisPairRaw(joystickIndex, Axis::PovX, Axis::PovY, invertX, invertY) };
	default:
		return { 0.f, 0.f };
	}
}

inline sf::Vector2f JoystickController::getAxis2dPositionCustomAxisPairRaw(const std::size_t joystickIndex, const Axis axisX, const Axis axisY, const bool invertX, const bool invertY) const
{
	sf::Vector2f position{ 0.f, 0.f };
	position = { getAxisPositionRaw(joystickIndex, axisX), getAxisPositionRaw(joystickIndex, axisY) };
	if (invertX)
		position.x = -position.x;
	if (invertY)
		position.y = -position.y;
	return position;
}





inline float JoystickController::getAxisPosition(const std::size_t joystickIndex, const Axis axis) const
{
	// get calibrated position i.e. value is modified dependant on calibration settings

	float n;

	const Calibration& c{ m_calibrations[priv_getCalibrationIndex(joystickIndex)] };

	float origValue{ sf::Joystick::getAxisPosition(static_cast<unsigned int>(joystickIndex), priv_getSfmlJoystickAxis(axis)) };

	const AxisCalibration* a{ nullptr };

	switch (axis)
	{
	case Axis::X:
		a = &c.x;
		break;
	case Axis::Y:
		a = &c.y;
		break;
	case Axis::U:
		a = &c.u;
		break;
	case Axis::V:
		a = &c.v;
		break;
	case Axis::Z:
		a = &c.z;
		break;
	case Axis::R:
		a = &c.r;
		break;
	case Axis::PovX:
		a = &c.povX;
		break;
	case Axis::PovY:
		a = &c.povY;
		break;
	default:
		a = &c.x;
	}

	n = origValue - a->zero;
	float maxPositive{ a->maxPositive - a->zero };
	float maxNegative{ a->maxNegative + a->zero };
	float minPositive{ a->minPositive - a->zero };
	float minNegative{ a->minNegative + a->zero };

	if (n > 0.f)
	{
		if (n < minPositive)
			n = 0.f;
		else
		{
			n -= minPositive;
			n *= 100.f / (maxPositive - minPositive);
		}
	}
	else if (n < 0.f)
	{
		if (n > minNegative)
			n = 0.f;
		else
		{
			n -= minNegative;
			n *= -100.f / (maxNegative - minNegative);
		}
	}
	if (n > 100.f)
		n = 100.f;
	else if (n < -100.f)
		n = -100.f;
	return n;
}

inline sf::Vector2f JoystickController::getAxis2dPosition(const std::size_t joystickIndex, const AxisPair axisPair, const bool invertX, const bool invertY) const
{
	switch (axisPair)
	{
	case AxisPair::XY:
		return { getAxis2dPositionCustomAxisPair(joystickIndex, Axis::X, Axis::Y, invertX, invertY) };
	case AxisPair::UV:
		return { getAxis2dPositionCustomAxisPair(joystickIndex, Axis::U, Axis::V, invertX, invertY) };
	case AxisPair::PovXY:
		return { getAxis2dPositionCustomAxisPair(joystickIndex, Axis::PovX, Axis::PovY, invertX, invertY) };
	default:
		return { 0.f, 0.f };
	}
}

inline sf::Vector2f JoystickController::getAxis2dPositionCustomAxisPair(const std::size_t joystickIndex, const Axis axisX, const Axis axisY, const bool invertX, const bool invertY) const
{
	sf::Vector2f position{ 0.f, 0.f };
	position = { getAxisPosition(joystickIndex, axisX), getAxisPosition(joystickIndex, axisY) };
	if (invertX)
		position.x = -position.x;
	if (invertY)
		position.y = -position.y;
	return position;
}







inline float JoystickController::getAxis2dDirection(const std::size_t joystickIndex, const AxisPair axisPair, const bool invertX, const bool invertY) const
{
	switch (axisPair)
	{
	case AxisPair::XY:
		return { getAxis2dDirectionCustomAxisPair(joystickIndex, Axis::X, Axis::Y, invertX, invertY) };
	case AxisPair::UV:
		return { getAxis2dDirectionCustomAxisPair(joystickIndex, Axis::U, Axis::V, invertX, invertY) };
	case AxisPair::PovXY:
		return { getAxis2dDirectionCustomAxisPair(joystickIndex, Axis::PovX, Axis::PovY, invertX, invertY) };
	default:
		return { 0.f };
	}
}

inline float JoystickController::getAxis2dDirectionCustomAxisPair(const std::size_t joystickIndex, const Axis axisX, const Axis axisY, const bool invertX, const bool invertY) const
{
	sf::Vector2f position{ getAxisPosition(joystickIndex, axisX), -getAxisPosition(joystickIndex, axisY) };
	if (invertX)
		position.x = -position.x;
	if (invertY)
		position.y = -position.y;
	return std::atan2(position.x, position.y) - halfPiFloat;
}

inline float JoystickController::getAxis2dStrength(const std::size_t joystickIndex, const AxisPair axisPair) const
{
	switch (axisPair)
	{
	case AxisPair::XY:
		return { getAxis2dStrengthCustomAxisPair(joystickIndex, Axis::X, Axis::Y) };
	case AxisPair::UV:
		return { getAxis2dStrengthCustomAxisPair(joystickIndex, Axis::U, Axis::V) };
	case AxisPair::PovXY:
		return { getAxis2dStrengthCustomAxisPair(joystickIndex, Axis::PovX, Axis::PovY) };
	default:
		return { 0.f };
	}
}

inline float JoystickController::getAxis2dStrengthCustomAxisPair(const std::size_t joystickIndex, const Axis axisX, const Axis axisY) const
{
	sf::Vector2f position{ getAxisPosition(joystickIndex, axisX), getAxisPosition(joystickIndex, axisY) };
	float strength{ std::sqrt(position.x * position.x + position.y * position.y) * 0.01f };
	if (strength < 0.f)
		strength = 0.f;
	if (strength > 1.f)
		strength = 1.f;
	return strength;
}

inline std::size_t JoystickController::getAxis2dDirectionIndex(const std::size_t joystickIndex, const AxisPair axisPair, const std::size_t numberOfSegments, const bool invertX, const bool invertY) const
{
	switch (axisPair)
	{
	case AxisPair::XY:
		return { getAxis2dDirectionIndexCustomAxisPair(joystickIndex, Axis::X, Axis::Y, numberOfSegments, invertX, invertY) };
	case AxisPair::UV:
		return { getAxis2dDirectionIndexCustomAxisPair(joystickIndex, Axis::U, Axis::V, numberOfSegments,invertX, invertY) };
	case AxisPair::PovXY:
		return { getAxis2dDirectionIndexCustomAxisPair(joystickIndex, Axis::PovX, Axis::PovY, numberOfSegments,invertX, invertY) };
	default:
		return { 0u };
	}
}

inline std::size_t JoystickController::getAxis2dDirectionIndexCustomAxisPair(const std::size_t joystickIndex, const Axis axisX, const Axis axisY, const std::size_t numberOfSegments, const bool invertX, const bool invertY) const
{
	assert(numberOfSegments > 1u);

	float angle{ getAxis2dDirectionCustomAxisPair(joystickIndex, axisX, axisY, invertX, invertY) + halfPiFloat }; // add quarter turn so that first index is straight up
	while (angle > doublePiFloat)
		angle -= doublePiFloat;
	while (angle < 0.f)
		angle += doublePiFloat;

	constexpr float recipricalOfFullCircle{ 1.f / doublePiFloat };
	angle *= numberOfSegments * recipricalOfFullCircle;
	std::size_t directionIndex{ static_cast<std::size_t>(std::round(angle) + 0.5f) };
	if (directionIndex >= numberOfSegments)
		directionIndex -= numberOfSegments;
	return directionIndex;
}

inline std::size_t JoystickController::getAxis2dStrengthIndex(const std::size_t joystickIndex, const AxisPair axisPair, const std::size_t numberOfSteps) const
{
	switch (axisPair)
	{
	case AxisPair::XY:
		return { getAxis2dStrengthIndexCustomAxisPair(joystickIndex, Axis::X, Axis::Y, numberOfSteps) };
	case AxisPair::UV:
		return { getAxis2dStrengthIndexCustomAxisPair(joystickIndex, Axis::U, Axis::V, numberOfSteps) };
	case AxisPair::PovXY:
		return { getAxis2dStrengthIndexCustomAxisPair(joystickIndex, Axis::PovX, Axis::PovY, numberOfSteps) };
	default:
		return { 0u };
	}
}

inline std::size_t JoystickController::getAxis2dStrengthIndexCustomAxisPair(const std::size_t joystickIndex, const Axis axisX, const Axis axisY, const std::size_t numberOfSteps) const
{
	assert(numberOfSteps > 1u);

	return static_cast<std::size_t>(std::round(getAxis2dStrengthCustomAxisPair(joystickIndex, axisX, axisY) * (numberOfSteps - 1u)) + 0.5f);
}

inline int JoystickController::getAxis2dAsPovHat(const std::size_t joystickIndex, const AxisPair axisPair, const bool invertX, const bool invertY) const
{
	switch (axisPair)
	{
	case AxisPair::XY:
		return { getAxis2dAsPovHatCustomAxisPair(joystickIndex, Axis::X, Axis::Y, invertX, invertY) };
	case AxisPair::UV:
		return { getAxis2dAsPovHatCustomAxisPair(joystickIndex, Axis::U, Axis::V, invertX, invertY) };
	case AxisPair::PovXY:
		return { getAxis2dAsPovHatCustomAxisPair(joystickIndex, Axis::PovX, Axis::PovY, invertX, invertY) };
	default:
		return { 0u };
	}
}

inline int JoystickController::getAxis2dAsPovHatCustomAxisPair(const std::size_t joystickIndex, const Axis axisX, const Axis axisY, const bool invertX, const bool invertY) const
{
	if (getAxis2dStrengthIndexCustomAxisPair(joystickIndex, axisX, axisY, 2u) == 0u)
		return -1;

	return static_cast<int>(getAxis2dDirectionIndexCustomAxisPair(joystickIndex, axisX, axisY, 8u, invertX, invertY));
}







// CALIBRATION


inline void JoystickController::setCalibrationAxisZero(const std::size_t joystickIndex, const Axis axis, const float zeroValue)
{
	priv_setCalibrationValue(joystickIndex, axis, CalibrationField::Zero, zeroValue);
}

inline void JoystickController::setCalibrationAxisMinPositive(const std::size_t joystickIndex, const Axis axis, const float minPositive)
{
	priv_setCalibrationValue(joystickIndex, axis, CalibrationField::MinPositive, minPositive);
}

inline void JoystickController::setCalibrationAxisMaxPositive(const std::size_t joystickIndex, const Axis axis, const float maxPositive)
{
	priv_setCalibrationValue(joystickIndex, axis, CalibrationField::MaxPositive, maxPositive);
}

inline void JoystickController::setCalibrationAxisMinNegative(const std::size_t joystickIndex, const Axis axis, const float minNegative)
{
	priv_setCalibrationValue(joystickIndex, axis, CalibrationField::MinNegative, minNegative);
}

inline void JoystickController::setCalibrationAxisMaxNegative(const std::size_t joystickIndex, const Axis axis, const float maxNegative)
{
	priv_setCalibrationValue(joystickIndex, axis, CalibrationField::MaxNegative, maxNegative);
}


inline float JoystickController::getCalibrationAxisZero(std::size_t joystickIndex, Axis axis) const
{
	return priv_getCalibrationValue(joystickIndex, axis, CalibrationField::Zero);
}

inline float JoystickController::getCalibrationAxisMinPositive(std::size_t joystickIndex, Axis axis) const
{
	return priv_getCalibrationValue(joystickIndex, axis, CalibrationField::MinPositive);
}

inline float JoystickController::getCalibrationAxisMaxPositive(std::size_t joystickIndex, Axis axis) const
{
	return priv_getCalibrationValue(joystickIndex, axis, CalibrationField::MaxPositive);
}

inline float JoystickController::getCalibrationAxisMinNegative(std::size_t joystickIndex, Axis axis) const
{
	return priv_getCalibrationValue(joystickIndex, axis, CalibrationField::MinNegative);
}

inline float JoystickController::getCalibrationAxisMaxNegative(std::size_t joystickIndex, Axis axis) const
{
	return priv_getCalibrationValue(joystickIndex, axis, CalibrationField::MaxNegative);
}


















// PRIVATE

inline std::size_t JoystickController::priv_getCalibrationIndex(const std::size_t joystickIndex) const
{
	return static_cast<std::size_t>(std::distance(m_calibrations.begin(), std::find_if(m_calibrations.begin(), m_calibrations.end(), [joystickIndex](const Calibration& c) { return c.joystickIndex == joystickIndex; })));
}

inline void JoystickController::priv_setCalibrationValue(const std::size_t joystickIndex, const Axis axis, const CalibrationField calibrationField, const float newValue)
{
	Calibration& c{ m_calibrations[priv_getCalibrationIndex(joystickIndex)] };
	switch (axis)
	{
	case Axis::X:
		priv_setCalibrationFieldValue(c.x, calibrationField, newValue);
		break;
	case Axis::Y:
		priv_setCalibrationFieldValue(c.y, calibrationField, newValue);
		break;
	case Axis::U:
		priv_setCalibrationFieldValue(c.u, calibrationField, newValue);
		break;
	case Axis::V:
		priv_setCalibrationFieldValue(c.v, calibrationField, newValue);
		break;
	case Axis::Z:
		priv_setCalibrationFieldValue(c.z, calibrationField, newValue);
		break;
	case Axis::R:
		priv_setCalibrationFieldValue(c.r, calibrationField, newValue);
		break;
	case Axis::PovX:
		priv_setCalibrationFieldValue(c.povX, calibrationField, newValue);
		break;
	case Axis::PovY:
		priv_setCalibrationFieldValue(c.povY, calibrationField, newValue);
		break;
	}
}

inline void JoystickController::priv_setCalibrationFieldValue(AxisCalibration& axisCalibration, const CalibrationField calibrationField, const float newValue)
{
	switch (calibrationField)
	{
	case CalibrationField::Zero:
		axisCalibration.zero = newValue;
		break;
	case CalibrationField::MinPositive:
		axisCalibration.minPositive = newValue;
		break;
	case CalibrationField::MaxPositive:
		axisCalibration.maxPositive = newValue;
		break;
	case CalibrationField::MinNegative:
		axisCalibration.minNegative = newValue;
		break;
	case CalibrationField::MaxNegative:
		axisCalibration.maxNegative = newValue;
		break;
	}
}

inline float JoystickController::priv_getCalibrationValue(const std::size_t joystickIndex, const Axis axis, const CalibrationField calibrationField) const
{
	const Calibration& c{ m_calibrations[priv_getCalibrationIndex(joystickIndex)] };
	switch (axis)
	{
	case Axis::X:
		return priv_getCalibrationFieldValue(c.x, calibrationField);
	case Axis::Y:
		return priv_getCalibrationFieldValue(c.y, calibrationField);
	case Axis::U:
		return priv_getCalibrationFieldValue(c.u, calibrationField);
	case Axis::V:
		return priv_getCalibrationFieldValue(c.v, calibrationField);
	case Axis::Z:
		return priv_getCalibrationFieldValue(c.z, calibrationField);
	case Axis::R:
		return priv_getCalibrationFieldValue(c.r, calibrationField);
	case Axis::PovX:
		return priv_getCalibrationFieldValue(c.povX, calibrationField);
	case Axis::PovY:
		return priv_getCalibrationFieldValue(c.povY, calibrationField);
	default:
		return 0.f;
	}
}

inline float JoystickController::priv_getCalibrationFieldValue(const AxisCalibration& axisCalibration, const CalibrationField calibrationField) const
{
	switch (calibrationField)
	{
	case CalibrationField::Zero:
		return axisCalibration.zero;
	case CalibrationField::MinPositive:
		return axisCalibration.minPositive;
	case CalibrationField::MaxPositive:
		return axisCalibration.maxPositive;
	case CalibrationField::MinNegative:
		return axisCalibration.minNegative;
	case CalibrationField::MaxNegative:
		return axisCalibration.maxNegative;
	default:
		return 0.f;
	}
}

inline sf::Joystick::Axis JoystickController::priv_getSfmlJoystickAxis(const Axis axis) const
{
	switch (axis)
	{
	case Axis::X:
		return sf::Joystick::Axis::X;
	case Axis::Y:
		return sf::Joystick::Axis::Y;
	case Axis::U:
		return sf::Joystick::Axis::U;
	case Axis::V:
		return sf::Joystick::Axis::V;
	case Axis::Z:
		return sf::Joystick::Axis::Z;
	case Axis::PovX:
		return sf::Joystick::Axis::PovX;
	case Axis::PovY:
		return sf::Joystick::Axis::PovY;
	case Axis::R:
	default:
		return sf::Joystick::Axis::R;
	}
}

} // namespace joystickcontroller
