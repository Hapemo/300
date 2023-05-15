#pragma once

/*!*****************************************************************************
\brief ColliderType enum class that encapsulates the different types of colliders
*******************************************************************************/
enum class ColliderType : int {
	RECT = 1,				// Rect
	CIRCLE,					// Circle
	// CONVEX_POLYGON		// Convex polygon
	MAXTYPESOFCOLLIDERS		// Max size
};