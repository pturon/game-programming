#pragma once

enum groupLabels {
	groupBackground,
	groupMap,
	groupPlayers,
	groupEnemies,
	groupColliders,
	groupForeground
};

enum Direction {
	up, 
	rightUp, 
	right, 
	rightDown, 
	down,
	leftDown, 
	left, 
	leftUp
};

enum State {
	idle, 
	walking, 
	jumping, 
	falling, 
	fallingAfterJump,
	attackingSide,
	attackingBottom, 
	attackingTop,
	dashing,
	wallCling
};