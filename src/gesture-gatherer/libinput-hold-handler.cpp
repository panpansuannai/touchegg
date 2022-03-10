/**
 * Copyright 2011 - 2022 José Expósito <jose.exposito89@gmail.com>
 *
 * This file is part of Touchégg.
 *
 * Touchégg is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License  as  published by  the  Free Software
 * Foundation,  either version 3 of the License,  or (at your option)  any later
 * version.
 *
 * Touchégg is distributed in the hope that it will be useful,  but  WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.  See the  GNU General Public License  for more details.
 *
 * You should have received a copy of the  GNU General Public License along with
 * Touchégg. If not, see <http://www.gnu.org/licenses/>.
 */
#include "gesture-gatherer/libinput-hold-handler.h"

#include <libinput.h>
#include <ctime>
#include <iostream>

#include "gesture/gesture.h"

void LininputHoldHandler::handleHoldBegin(struct libinput_event * event) {
  this->beginTime = LininputHoldHandler::getTimestamp();
}

void LininputHoldHandler::handleHoldEnd(struct libinput_event *event, libinput_event_type nextEventType) {
  if (nextEventType == LIBINPUT_EVENT_GESTURE_SWIPE_BEGIN
      || nextEventType == LIBINPUT_EVENT_GESTURE_PINCH_BEGIN
      || nextEventType == LIBINPUT_EVENT_POINTER_SCROLL_FINGER
      /* holding too short time, do not trigger */
      || LininputHoldHandler::getTimestamp() - this->beginTime < 300) {

    /* TODO: handle hold cancel */
    std::cout << "Hold gesture cancel" << std::endl;
  } else {
    /* Debug */
    std::cout << "Hold gesture trigger" << std::endl;

    int fingers = libinput_event_gesture_get_finger_count(libinput_event_get_gesture_event(event));
  auto beginGesture = std::make_unique<Gesture>(
      GestureType::HOLD, GestureDirection::UNKNOWN, 0.0,
      fingers, DeviceType::TOUCHPAD, LininputHandler::calculateElapsedTime(this->beginTime));
  this->gestureController->onGestureBegin(std::move(beginGesture));
    auto gesture = std::make_unique<Gesture>(
        GestureType::HOLD, GestureDirection::UNKNOWN, 100.0,
        fingers, DeviceType::TOUCHPAD, LininputHandler::calculateElapsedTime(this->beginTime));
    this->gestureController->onGestureEnd(std::move(gesture));
  }
}
