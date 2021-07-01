#pragma once

enum groupLabels {
	groupBackground,
	groupMidground,
	groupTransitions,
	groupPlayers,
	groupEnemies,
	groupColliders,
	groupForeground
};

enum  Direction {
	up, 
	rightUp, 
	right, 
	rightDown, 
	down,
	leftDown, 
	left, 
	leftUp,
	none
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
	wallCling,
	detecting,
	charge,
	attackCooldown,
	dying, 
	dead,
	rage,
	leaping, 
	slamBuildUp,
	slam, 
	leapingBludgeon,
	rageJump,
	cooldown
};