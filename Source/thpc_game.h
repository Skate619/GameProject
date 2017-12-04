#ifndef THPC_GAME_H
#define THPC_GAME_H

#include "thpc_defines.h"
#include "thpc_debug.h"
#include "thpc_maths.h"
#include "thpc_time.h"
#include "thpc_input.h"
#include "thpc_net.h"
#include "thpc_player.h"
#include "thpc_platform.h"
#include "thpc_menu.h"

namespace thpc
{
	class Game
	{
	public:
		Game(Platform *platform);
		void init();
		void update(float elapse);
		void update(Time elapse);
		void draw();
		int end();

		Time time;
		InputManager inputManager;

	private:
		// Reference to platform code
		// Used for all platform-related actions (drawing, etc.)
		Platform *platform;
		// Game objects
		Player player; // Player
		floatXY cursor;
	};
}


#endif
