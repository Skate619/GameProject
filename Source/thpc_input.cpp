#include "thpc_input.h"
using namespace thpc;


InputRef::InputRef()
	: type(IPT_NULL),
	code(0),
	index(-1)
{}

InputRef::InputRef(InputType type, unsigned int code, int index)
	: type(type),
	code(code),
	index(index)
{}


ActionBinds::ActionBinds()
{
	this->clear();
}

bool ActionBinds::clear(int index)
{
	if (index == -1)
	{
		this->bindCount = 0;

		for (int b=0; b<MAX_BINDS_PER_ACTION; ++b)
		{
			this->refs[b] ={};
			this->refs[b].type = IPT_NULL;
			this->inverted[b] = false;
			this->toggle[b] = TOGTYPE_Normal;
		}

		return true;
	}
	else
	{
		if (bindCount > 0)
		{
			--this->bindCount;

			for (int b=index; b<this->bindCount; ++b)
			{
				this->refs[b].code = this->refs[b+1].code;
				this->refs[b].index = this->refs[b+1].index;
				this->refs[b].type = this->refs[b+1].type;
				this->inverted[b] = this->inverted[b+1];
				this->toggle[b] = this->toggle[b+1];
			}

			this->refs[this->bindCount] ={};
			this->refs[this->bindCount].type = IPT_NULL;
			this->inverted[this->bindCount] = false;
			this->toggle[this->bindCount] = TOGTYPE_Normal;
            
			return true;
		}
	}

	return false;
}

int ActionBinds::findBinding(InputRef inputRef)
{
	for (int b=0; b<this->bindCount; ++b)
	{
		if (this->refs[b].type == inputRef.type &&
			this->refs[b].code == inputRef.code &&
			this->refs[b].index == inputRef.index)
		{
			return b;
		}
	}
	return -1;
}

int ActionBinds::addBinding(InputRef inputRef, bool inverted, ToggleType toggle)
{
	int result = -1;
	if (this->bindCount < MAX_BINDS_PER_ACTION)
	{
		this->refs[this->bindCount].type = inputRef.type;
		this->refs[this->bindCount].code = inputRef.code;
		this->refs[this->bindCount].index = inputRef.index;
		this->inverted[this->bindCount] = inverted;
		this->toggle[this->bindCount] = toggle;

		result = this->bindCount;
		++this->bindCount;
	}
	return result;
}


ActionGroup::ActionGroup()
{
	this->resetValues();
	this->clearBinds();
}

void ActionGroup::resetValues(ActionType action)
{
	if (action == ACT_NULL)
		for (int a=0; a<ACT_COUNT; ++a)
		{
			this->actionState[a] ={};
		}
	else
		this->actionState[action] ={};
}

void ActionGroup::clearBinds(ActionType action, int bindIndex)
{
	if (action == ACT_NULL)
		for (int a=0; a<ACT_COUNT; ++a)
		{
			this->actionBinds[action].clear(bindIndex);
		}
	else
		this->actionBinds[action].clear(bindIndex);
}

float ActionGroup::getActionValue(ActionType action)
{
	return this->actionState[action].value;
}

float ActionGroup::getActionValue(ActionType action, float clampA, float clampB)
{
	return Clamp(this->actionState[action].value, clampA, clampB);
}

float ActionGroup::getActionDelta(ActionType action)
{
	return this->actionState[action].delta;
}

float ActionGroup::getActionDelta(ActionType action, float clampA, float clampB)
{
	return Clamp(this->actionState[action].delta, clampA, clampB);
}

InputState ActionGroup::getActionInputState(ActionType action)
{
	return this->actionState[action].state;
}

bool ActionGroup::getActionPressed(ActionType action, bool onlyDetectFirstPress)
{
	bool result = (this->actionState[action].state == IPS_Press);
	if (onlyDetectFirstPress)
		result = result && (this->actionState[action].heldBinds==1);
	return result;
}

bool ActionGroup::getActionReleased(ActionType action, bool onlyDetectLastRelease)
{
	bool result = (this->actionState[action].state == IPS_Release);
	if (onlyDetectLastRelease)
		result = result && (this->actionState[action].heldBinds==0);
	return result;
}

bool ActionGroup::getActionHeld(ActionType action)
{
	return (this->actionState[action].heldBinds != 0);
}

bool ActionGroup::getActionUnheld(ActionType action)
{
	return (this->actionState[action].heldBinds == 0);
}

int ActionGroup::getActionHeldBindsCount(ActionType action)
{
	return this->actionState[action].heldBinds;
}


InputManager::InputManager()
	: actionGroupCount(0)
{
	this->clearBindings();
	this->resetActions();
}

void InputManager::clearBindings(int index)
{
	if (index == -1)
		for (int g=0; g<MAX_LOCAL_PLAYERS; ++g)
		{
			this->actionGroups[g].clearBinds();
		}
	else
		this->actionGroups[index].clearBinds();
}

void InputManager::resetActions(int index)
{
	if (index == -1)
		for (int g=0; g<MAX_LOCAL_PLAYERS; ++g)
		{
			this->actionGroups[g].resetValues();
		}
	else
		this->actionGroups[index].resetValues();
}

_result_findBinding InputManager::findBinding(InputRef inputRef, int actionGroupToSearch, ActionType action)
{
	_result_findBinding result;
	result.error = BINDERR_NotFound;
	result.actionGroup = -1;
	result.actionType = ACT_NULL;
	result.bindIndex = -1;

	if (actionGroupToSearch != -1)
	{
		if (action != ACT_NULL)
		{
			int bIndex = this->actionGroups[actionGroupToSearch].actionBinds[action].findBinding(inputRef);
			if (bIndex != -1)
			{
				result.error = BINDERR_SameReference;
				result.actionGroup = actionGroupToSearch;
				result.actionType = action;
				result.bindIndex = bIndex;
				return result;
			}
		}
		else
		{
			for (int a=0; a<ACT_COUNT; ++a)
			{
				int bIndex = this->actionGroups[actionGroupToSearch].actionBinds[a].findBinding(inputRef);
				if (bIndex != -1)
				{
					result.error = BINDERR_OtherAction;
					result.actionGroup = actionGroupToSearch;
					result.actionType = (ActionType)a;
					result.bindIndex = bIndex;
					return result;
				}
			}
		}
	}
	else
	{
		for (int g=0; g<actionGroupCount; ++g)
		{
			for (int a=0; a<ACT_COUNT; ++a)
			{
				int bIndex = this->actionGroups[g].actionBinds[a].findBinding(inputRef);
				if (bIndex != -1)
				{
					result.error = BINDERR_OtherGroup;
					result.actionGroup = g;
					result.actionType = (ActionType)a;
					result.bindIndex = bIndex;
					return result;
				}
			}
		}
	}

	return result;
}

_result_findBinding InputManager::bind(unsigned int actionGroupIndex, ActionType action, InputRef inputRef, bool inverted, ToggleType toggle)
{
	_result_findBinding result = this->findBinding(inputRef,actionGroupIndex,action);

    if(result.error == BINDERR_NotFound)
    {
        this->actionGroups[actionGroupIndex].actionBinds[action].addBinding(inputRef,inverted,toggle);
        actionGroupCount = actionGroupIndex+1;
    }
    else if(result.error == BINDERR_SameReference)
    {
        if (this->actionGroups[actionGroupIndex].actionBinds[action].inverted[result.bindIndex] == inverted &&
			this->actionGroups[actionGroupIndex].actionBinds[action].toggle[result.bindIndex] == toggle)
		{
			result.error = BINDERR_ExactMatch;
		}
    }

	return result;
}

bool InputManager::unbind(unsigned int actionGroupIndex, ActionType action, InputRef inputRef)
{
	int indexToUnbind = this->actionGroups[actionGroupIndex].actionBinds[action].findBinding(inputRef);

	if (indexToUnbind == -1)
		return false;
	else
		return this->unbind(actionGroupIndex, action, indexToUnbind);
}

bool InputManager::unbind(unsigned int actionGroupIndex, ActionType action, int bindIndex)
{
	return this->actionGroups[actionGroupIndex].actionBinds[action].clear(bindIndex);
}

void InputManager::preEvents()
{
	for(int g=0; g<this->actionGroupCount; ++g)
	{
		for(int a=0; a<ACT_COUNT; ++a)
		{
			this->actionGroups[g].actionState[a].delta = 0.f;
			this->actionGroups[g].actionState[a].state = IPS_Static;
		}
	}
}

void InputManager::processEvent(InputRef inputRef, InputState state, float valueMult)
{
	// Search through every action group
	for (int g=0; g<this->actionGroupCount; ++g)
	{
		// Run through every action in the group
		for (int a=0; a<ACT_COUNT; ++a)
		{
			// If the group contains a binding for that action
			int bindIndex = this->actionGroups[g].actionBinds[a].findBinding(inputRef);
			if (bindIndex!=-1)
			{
				ActionState *aState = &(this->actionGroups[g].actionState[a]);
				ActionBinds *aBinds = &(this->actionGroups[g].actionBinds[a]);

				bool inverted = aBinds->inverted[bindIndex];
				ToggleType togType = aBinds->toggle[bindIndex];

				float valueDelta = inverted ? -valueMult : valueMult;

				if(inputRef.type == IPT_Keyboard ||
				   inputRef.type == IPT_MouseButton ||
				   inputRef.type == IPT_GamepadButton)
				{
					if(state == thpc::IPS_Press)
					{
						if(togType == TOGTYPE_Normal ||
						   togType == TOGTYPE_Press)
						{
							++aState->heldBinds;
							aState->value += valueDelta;
							aState->delta += valueDelta;
							aState->state = IPS_Press;
							if(togType == TOGTYPE_Press)
								aBinds->toggle[bindIndex] = TOGTYPE_Press_Held;
						}
						else if(togType == TOGTYPE_Press_Held)
						{
							--aState->heldBinds;
							aState->value -= valueDelta;
							aState->delta -= valueDelta;
							if(aState->state != IPS_Press)
								aState->state = IPS_Release;
							aBinds->toggle[bindIndex] = TOGTYPE_Press;
						}
					}
					else if(state == thpc::IPS_Release)
					{
						if(togType == TOGTYPE_Normal ||
						   togType == TOGTYPE_Release_Held)
						{
							--aState->heldBinds;
							aState->value -= valueDelta;
							aState->delta -= valueDelta;
							if(aState->state != IPS_Press)
								aState->state = IPS_Release;
						}
						else if(togType == TOGTYPE_Release)
						{
							++aState->heldBinds;
							aState->value += valueDelta;
							aState->delta += valueDelta;
							aState->state = IPS_Press;
						}
					}
				}
				else if(inputRef.type == IPT_MouseAxis ||
					    inputRef.type == IPT_GamepadAxis)
				{
					aState->value += valueDelta;
					aState->delta += valueDelta;
					aState->state = IPS_Press;
				}
			}
		}
	}
}

float thpc::InputManager::getActionValue(unsigned int actionGroupIndex, ActionType action)
{
	return this->actionGroups[actionGroupIndex].getActionValue(action);
}

float thpc::InputManager::getActionValue(unsigned int actionGroupIndex, ActionType action, float clampA, float clampB)
{
	return this->actionGroups[actionGroupIndex].getActionValue(action, clampA, clampB);
}

float thpc::InputManager::getActionDelta(unsigned int actionGroupIndex, ActionType action)
{
	return this->actionGroups[actionGroupIndex].getActionDelta(action);
}

float thpc::InputManager::getActionDelta(unsigned int actionGroupIndex, ActionType action, float clampA, float clampB)
{
	return this->actionGroups[actionGroupIndex].getActionDelta(action, clampA, clampB);
}

InputState thpc::InputManager::getActionInputState(unsigned int actionGroupIndex, ActionType action)
{
	return this->actionGroups[actionGroupIndex].getActionInputState(action);
}

bool thpc::InputManager::getActionPressed(unsigned int actionGroupIndex, ActionType action, bool onlyDetectFirstPress)
{
	return this->actionGroups[actionGroupIndex].getActionPressed(action);
}

bool thpc::InputManager::getActionReleased(unsigned int actionGroupIndex, ActionType action, bool onlyDetectLastRelease)
{
	return this->actionGroups[actionGroupIndex].getActionReleased(action);
}

bool thpc::InputManager::getActionHeld(unsigned int actionGroupIndex, ActionType action)
{
	return this->actionGroups[actionGroupIndex].getActionHeld(action);
}

bool thpc::InputManager::getActionUnheld(unsigned int actionGroupIndex, ActionType action)
{
	return this->actionGroups[actionGroupIndex].getActionUnheld(action);
}

int thpc::InputManager::getActionHeldBindsCount(unsigned int actionGroupIndex, ActionType action)
{
	return this->actionGroups[actionGroupIndex].getActionHeldBindsCount(action);
}
