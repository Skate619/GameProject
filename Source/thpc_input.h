#ifndef THPC_INPUT_H
#define THPC_INPUT_H

#include "thpc_defines.h"
#include "thpc_maths.h"
#include "thpc_debug.h"


namespace thpc
{
	// [ACT_###] The type of in-game action being affected
	typedef enum ActionType
	{
		ACT_NULL=-1,
		ACT_DebugKill,
		ACT_CursorAbsX,
		ACT_CursorAbsY,
		ACT_CursorRelX,
		ACT_CursorRelY,
		ACT_CursorModeToggle,
		ACT_UISelect,
		ACT_UIContext,
		ACT_Accelerate,
		ACT_COUNT
	} ActionType;

	// [IPT_###] Form of input being referred to
	//  IPT_Keyboard
	//  IPT_MouseButton
	//  IPT_GamepadButton
	//  IPT_MouseAxis
	//  IPT_GamepadAxis
	typedef enum InputType
	{
		IPT_NULL=-1,
		IPT_Keyboard,
		IPT_MouseButton,
		IPT_GamepadButton,
		IPT_MouseAxis,
		IPT_GamepadAxis,
		IPT_COUNT
	} InputType;

	// [IPC_Axis_###] Mouse axis codes for use with InputRef
	enum InputCode_Axis_Mouse
	{
		IPC_Axis_MouseX,
		IPC_Axis_MouseY,
		IPC_Axis_MouseWheelV,
		IPC_Axis_MouseWheelH
	};
	// [IPC_Axis_###] 360 gamepad axis codes for use with InputRef
	enum InputCode_Axis_360Pad
	{
		IPC_Axis_LStickX,
		IPC_Axis_LStickY,
		IPC_Axis_RStickX,
		IPC_Axis_RStickY,
		IPC_Axis_LTrigger,
		IPC_Axis_RTrigger
	};

	// [IPS_###] The current press/release state of a given input
	typedef enum InputState
	{
		IPS_Release=-1, // Input was released on last check
		IPS_Static=0, // Input state has not changed
		IPS_Press=1, // Input was pressed on last check
	} InputState;

	// [TOGTYPE_###] The toggle effect of a given input on an action
	typedef enum ToggleType
	{
		TOGTYPE_Release_Held=-2, // Input is a toggle-on-release that is waiting for de-toggle
		TOGTYPE_Release=-1, // Input is a toggle-on-release that is not currently active
		TOGTYPE_Normal=0, // Input is not a toggle, has a regular on-hold effect
		TOGTYPE_Press=1, // Input is a toggle-on-press that is not currently active
		TOGTYPE_Press_Held=2 // Input is a toggle-on-press that is waiting for de-toggle
	} ToggleType;

	// [BINDERR_###] The error type encountered when setting or checking for a binding
	typedef enum BindingError
	{
		BINDERR_NotFound, // No error was encountered, binding is safe
		BINDERR_ActionFull, // Action already has too many input references bound to it
		BINDERR_OtherGroup, // Given input reference is already bound to another action in a different action group
		BINDERR_OtherAction, // Given input reference is already bound to another action in the given action group
		BINDERR_SameReference, // Given input reference is already bound to this action
		BINDERR_ExactMatch // Given input reference is already bound to this action and has the same invert/toggle properties
	} BindingError;


	// Reference to an input
	//  type - Input Type;  see enum InputType [IPT_###] for definitions
	//  code - Key/Button/Axis code;  fairly obvious, but note that values may change between platforms for one key, etc.
	//  index - Input index;  use to determine source, e.g. which gamepad sent a message or owns this binding
	struct InputRef
	{
		InputRef();
		InputRef(InputType type, unsigned int code, int index=0);
		InputType type; // Input Type;  see enum InputType [IPT_###] for definitions
		unsigned int code; // Key/Button/Axis code;  fairly obvious, but note that values may change between platforms for one key, etc.
		int index; // Input index;  use to determine source, e.g. which gamepad sent a message or owns this binding
	};

	// Collection of bindings for a given action
	//  bindCount - Number of registered binds for this action
	//  refs - Input reference for a given binding
	//  inverted - Does a given binding affect the action value inverseley?
	//  toggle - Toggle state of a given binding;  see enum ToggleType [TOGTYPE_###] for definitions
	struct ActionBinds
	{
		ActionBinds();
		// Clears the binds from this group
		bool clear(int index=-1);
		// Checks for a bind in this group;  if found, returns the index
		int findBinding(InputRef inputRef);
		// Adds a bind to this group;  returns the index of the new bind, or if at cap, returns -1
		int addBinding(InputRef inputRef, bool inverted, ToggleType toggle);
		int bindCount; // Number of registered binds for this action
		InputRef refs[MAX_BINDS_PER_ACTION]; // Input reference for a given binding index
		bool inverted[MAX_BINDS_PER_ACTION]; // Does a given binding affect the action value inverseley?
		ToggleType toggle[MAX_BINDS_PER_ACTION]; // Toggle state of a given binding;  see enum ToggleType [TOGTYPE_###] for definitions
	};

	// Current state of a given action
	//  value - Current end value of the action state;  generally this will be between 0 and 1, or -1 and 1, depends on the action
	//  delta - Difference between current and previous values of the action state
	//  state - Denotes the last effect upon this action by a binding;  see enum InputState [IPS_###] for definitions
	//  heldBinds - Used to determine how many binds associated with the action are currently being held
	struct ActionState
	{
		float value; // Current end value of the action state;  generally this will be between 0 and 1, or -1 and 1, depends on the action
		float delta; // Difference between current and previous values of the action state
		InputState state; // Denotes the last effect upon this action by a binding;  see enum InputState [IPS_###] for definitions
		int heldBinds; // Used to determine how many binds associated with the action are currently being held
	};

	// A collection of action states, generally there'll be one of these per local player
	//  actionState - Current states of each action in this group;  should only need to be directly edited by an InputManager
	class ActionGroup
	{
	public:
		ActionGroup();
		// Resets all values for actions in this group;  using parameter ACT_NULL (or leaving blank) resets ALL actions
		void resetValues(ActionType action=ACT_NULL);
		// Clears all binds for actions in this group;  using parameter ACT_NULL (or leaving blank) clears ALL binds
		void clearBinds(ActionType action=ACT_NULL, int bindIndex=-1);
		// Return the current action value of a given action
		float getActionValue(ActionType action);
		// Return the current action value of a given action, clamped to given boundaries
		float getActionValue(ActionType action, float clampA, float clampB=0.f);
		// Return the current update tick's action delta (change in value) of a given action
		float getActionDelta(ActionType action);
		// Return the current update tick's action delta (change in value) of a given action, clamped to given boundaries
		float getActionDelta(ActionType action, float clampA, float clampB=0.f);
		// Return the current update tick's input state for the given action
		InputState getActionInputState(ActionType action);
		// Returns true if a binding associated with a given action was pressed/changed in the current update tick
		bool getActionPressed(ActionType action, bool onlyDetectFirstPress=true);
		// Returns true if a binding associated with a given action was released in the current update tick
		bool getActionReleased(ActionType action, bool onlyDetectLastRelease=true);
		// Returns true if any number of bindings associated with a given action are currently being held
		bool getActionHeld(ActionType action);
		// Returns true if no bindings associated with a given action are currently being held
		bool getActionUnheld(ActionType action);
		// Returns the current number of bindings associated with a given action that are being held down
		int getActionHeldBindsCount(ActionType action);
		// Current states of each action in this group;  should only need to be directly edited by an InputManager
		ActionState actionState[ACT_COUNT];
		// Bindings for each action in this group;  should only need to be directly edited by an InputManager
		ActionBinds actionBinds[ACT_COUNT];
	};


	// Only useful for returning the result of InputManager::findBinding()
	//  error - Error code encountered when searching for a binding;  see enum BindingError [BINDERR_###] for definitions
	//  actionGroup - Index of the action group in which a bind was found;  if not found, this will be -1
	//  actionType - Action type the binding was found associated with;  if not found, this will be ACT_NULL
	//  bindIndex - Index of the binding inside the group in which it was found;  if not found, this will be -1
	struct _result_findBinding
	{
		BindingError error; // Error code encountered when searching for a binding;  see enum BindingError [BINDERR_###] for definitions
		int actionGroup; // Index of the action group in which a bind was found;  if not found, this will be -1
		ActionType actionType; // Action type the binding was found associated with;  if not found, this will be ACT_NULL
		int bindIndex; // Index of the binding inside the group in which it was found;  if not found, this will be -1
	};

	// Manages the input within the game and translates it directly to action values for ease of use;
	// Also makes it much easier to implement rebinding of any controls for any input method
	class InputManager
	{
	public:
		InputManager();
		// Clears binding groups;  using parameter -1 (or leaving blank) clears ALL binding groups
		void clearBindings(int index=-1);
		// Resets action groups;  using parameter -1 (or leaving blank) resets ALL action groups
		void resetActions(int index=-1);
		// Checks to see if a binding has already been made
		_result_findBinding findBinding(InputRef inputRef, int actionGroupToSearch=-1, ActionType action=ACT_NULL);
		_result_findBinding bind(unsigned int actionGroupIndex, ActionType action, InputRef inputRef, bool inverted=false, ToggleType toggle=TOGTYPE_Normal);
		bool unbind(unsigned int actionGroupIndex, ActionType action, InputRef inputRef);
		bool unbind(unsigned int actionGroupIndex, ActionType action, int bindIndex);
		// Call this before you call any other event processing - it readies action data for receiving events
		void preEvents();
		// Call this from anywhere to process an event with the InputManager
		//  When deciding what to pass, bear in mind the following;
		//  -  Set "state" to IPS_Press/IPS_Release if the event is digital, such as a Keyboard key press
		//  -  Set "state" to IPS_Static if the event is analogue, such as with mouse movement, or joystick/gamepad axis movement
		//  -  Set "valueMult" according to the value generated in analogue events, or as a general effect multiplier for digital events
		//  It's unlikely that "valueMult" will need to be set at all for digital events, but it still functions the same
		void processEvent(InputRef inputRef, InputState state, float valueMult=1.f);
		// Return the current action value of a given action
		float getActionValue(unsigned int actionGroupIndex, ActionType action);
		// Return the current action value of a given action, clamped to given boundaries
		float getActionValue(unsigned int actionGroupIndex, ActionType action, float clampA, float clampB=0.f);
		// Return the current update tick's action delta (change in value) of a given action
		float getActionDelta(unsigned int actionGroupIndex, ActionType action);
		// Return the current update tick's action delta (change in value) of a given action, clamped to given boundaries
		float getActionDelta(unsigned int actionGroupIndex, ActionType action, float clampA, float clampB=0.f);
		// Return the current update tick's input state for the given action
		InputState getActionInputState(unsigned int actionGroupIndex, ActionType action);
		// Returns true if a binding associated with a given action was pressed/changed in the current update tick
		bool getActionPressed(unsigned int actionGroupIndex, ActionType action, bool onlyDetectFirstPress=true);
		// Returns true if a binding associated with a given action was released in the current update tick
		bool getActionReleased(unsigned int actionGroupIndex, ActionType action, bool onlyDetectLastRelease=true);
		// Returns true if any number of bindings associated with a given action are currently being held
		bool getActionHeld(unsigned int actionGroupIndex, ActionType action);
		// Returns true if no bindings associated with a given action are currently being held
		bool getActionUnheld(unsigned int actionGroupIndex, ActionType action);
		// Returns the current number of bindings associated with a given action that are being held down
		int getActionHeldBindsCount(unsigned int actionGroupIndex, ActionType action);
	private:
		int actionGroupCount;
		ActionGroup actionGroups[MAX_LOCAL_PLAYERS];

	};
}


#endif
