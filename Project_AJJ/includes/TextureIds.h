#pragma once

/*TextureID
* Enum class for the different sprites in the texture atlas
* This makes it more clear which sprites are used
*/

enum class TEXTURE_ID
{
	NONE,
	IDLE,
	WALK,
	RUN,
	JUMP,
	FALL,
	ATTACK,
	HOVER,
	PRESSED
};