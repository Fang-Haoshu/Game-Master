#include<Windows.h>
#include <cstring>
#include "Leap.h"

using namespace Leap;

class SampleListener : public Listener {
  public:
	  virtual void onInit(const Controller&);
	  virtual void onConnect(const Controller&);
	  virtual void onDisconnect(const Controller&);
	  virtual void onExit(const Controller&);
	  virtual void onFrame(const Controller&);
	  virtual void onFocusGained(const Controller&);
	  virtual void onFocusLost(const Controller&);
	  virtual void onDeviceChange(const Controller&);
	  virtual void onServiceConnect(const Controller&);
	  virtual void onServiceDisconnect(const Controller&);

  private:
};

const std::string fingerNames[] = {"Thumb", "Index", "Middle", "Ring", "Pinky"};
const std::string boneNames[] = {"Metacarpal", "Proximal", "Middle", "Distal"};
const std::string stateNames[] = {"STATE_INVALID", "STATE_START", "STATE_UPDATE", "STATE_END"};

void SampleListener::onInit(const Controller& controller) {

  std::cout << "Initialized" << std::endl;
}

void SampleListener::onConnect(const Controller& controller) {
  std::cout << "Connected" << std::endl;
  controller.enableGesture(Gesture::TYPE_CIRCLE);
  controller.enableGesture(Gesture::TYPE_KEY_TAP);
  controller.enableGesture(Gesture::TYPE_SCREEN_TAP);
  controller.enableGesture(Gesture::TYPE_SWIPE);
}

void SampleListener::onDisconnect(const Controller& controller) {
  // Note: not dispatched when running in a debugger.
  std::cout << "Disconnected" << std::endl;
}

void SampleListener::onExit(const Controller& controller) {
  std::cout << "Exited" << std::endl;
}

void SampleListener::onFrame(const Controller& controller) {
  // Get the most recent frame and report some basic information
  const Frame frame = controller.frame();

  HandList hands = frame.hands();
  for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {
    // Get the first hand
    const Hand hand = *hl;
    std::string handType = hand.isLeft() ? "Left hand" : "Right hand";

	std::cout << ", palm position: " << hand.palmPosition() << std::endl;
	::SetCursorPos((hand.palmPosition()[0]) * 4 + 620, 600-hand.palmPosition()[1]*1.2);
	if (hand.palmPosition()[2]<-50)
		mouse_event(MOUSEEVENTF_LEFTDOWN , 0, 0, 0, 0);
	else if (hand.palmPosition()[2]>50)
		mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
	else
	{
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
	}
  }
  if (!frame.hands().isEmpty() ) {
    std::cout << std::endl;
  }

}

void SampleListener::onFocusGained(const Controller& controller) {
  std::cout << "Focus Gained" << std::endl;
}

void SampleListener::onFocusLost(const Controller& controller) {
  std::cout << "Focus Lost" << std::endl;
}

void SampleListener::onDeviceChange(const Controller& controller) {
  std::cout << "Device Changed" << std::endl;
  const DeviceList devices = controller.devices();

  for (int i = 0; i < devices.count(); ++i) {
    std::cout << "id: " << devices[i].toString() << std::endl;
    std::cout << "  isStreaming: " << (devices[i].isStreaming() ? "true" : "false") << std::endl;
  }
}

void SampleListener::onServiceConnect(const Controller& controller) {
  std::cout << "Service Connected" << std::endl;
}

void SampleListener::onServiceDisconnect(const Controller& controller) {
  std::cout << "Service Disconnected" << std::endl;
}

int main(int argc, char** argv) {
  // Create a sample listener and controller
  SampleListener listener;
  Controller controller;

	controller.config().setInt32("background_app_mode", 2);
	 controller.config().save();


  // Have the sample listener receive events from the controller
  controller.addListener(listener);
  controller.setPolicy(Leap::Controller::POLICY_BACKGROUND_FRAMES);

  controller.config().setInt32("background_app_mode", 2);
  controller.config().save();

  // Keep this process running until Enter is pressed
  std::cout << "Press Enter to quit..." << std::endl;
  std::cin.get();

  // Remove the sample listener when done
  controller.removeListener(listener);

  return 0;
}
