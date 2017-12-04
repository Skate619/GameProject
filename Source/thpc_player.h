#ifndef THPC_PLAYER_H
#define THPC_PLAYER_H

#include "thpc_defines.h"
#include "thpc_maths.h"


namespace thpc
{
	class Player
	{
	public:
		Player();
		Player(Vec2 pos);
	protected:

	private:
		Vec2 pos;
	};
}

#endif