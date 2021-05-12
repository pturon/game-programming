# Woche 2

## Zusammenfassung

Nachdem letzte Woche das Grundgerüst aufgebaut wurde, wurden diese Woche die ersten wichtigsten Gameplayaspekte eingebaut. Zuerst wurde das Springen eingebaut. Dabei wird ein nicht physikalischer Sprung umgesetzt, da dieser sich etwas langsam anfühlt. Deswegen fällt er nach dem Sprung schneller, als er aufsteigt. Zudem wurde um die Animationen vernünftig zu wechseln eine Zustandsklasse für die Spielfigur erstellt, die entscheidet, welche Animation abgespielt wird. Außerdem kann der Spieler nun in alle vier Richtungen angreifen. Komischerweise wird die Animation zum angreifen zur Zeit während des Springens noch zu lang abgespielt. Das wird in den nächsten Wochen noch gefixed. 

## Springen 

Die Spielfigur springt normalerweise in einer Parabel, aber wie bereits in der Zusammenfassung erklärt wird die Parabel etwas verändert. Für eine Parabel muss jeden Frame die Beschleunigung nach Oben um einen konstanten Wert abnehmen. Sobald die Figur am Fallen ist, wirkt eine größere Kraft auf die Beschleunigung. Für mehr Kontrolle über den Sprung wirkt sich die Dauer des Drucks auf die Leertaste auf die Höhe des Sprungs. Bei einem kurzen Druck auf die Leertaste wird die Mindestsprunghöhe gesprungen. Bei längerem Drücken wird der Sprung beendet, wenn die maximale Sprunghöhe erreicht ist oder die Leertaste losgelassen wird. 

```c++
void TransformComponent::update() {
	if (velocity.y > 0) {
		velocity.y += fallMultiplier;
		if (state) {
			if (state->isAttacking()) {
				state->lastState = falling; 
			}
			else {
				state->setState(falling);
			}			
		}
	}
	else if (velocity.y < 0 && !spaceDown) {
		velocity.y += lowJumpMultiplier;
	}
	if (state) {
		if (state->currentState == jumping || state->currentState==falling || (state->isAttacking() && (state->lastState == falling || state->lastState == jumping))) {
			float t = 0.16f;
			position.x += velocity.x * speed;
			position.y += t * velocity.y;
			velocity.x += t * (gravity.x * speed);
			velocity.y += t * gravity.y;
		}
		else {
			position.x += velocity.x * speed;
			position.y += velocity.y * speed;
			if (gravityAffected) {
				position.x += gravity.x;
				position.y += gravity.y;
			}
		}
	}	
}
```

```c++
void TransformComponent::jump() {
	if (!state->isAttacking()) {
		if (state->currentState != jumping && state->currentState != falling) {
			state->setState(jumping);
			velocity.y = static_cast<float>(-jumpHeight);
		}
	}	
}
```

Bei einer Kollision mit der Decke wird der Sprung abgebrochen und das Fallen wird eingeleitet. 

```c++
void TransformComponent::startFall() {
	if (state->isAttacking()) {
		state->lastState = falling; 
	}
	else {
		state->setState(falling);
	}	
	velocity.y = 0;
}
```

Bei einer Kollision mit dem Boden wird der Sprung beendet. 

```c++
void TransformComponent::stopJump() {	
	if (state->currentState == jumping || state->currentState == falling) {
		if (velocity.x == 0) {
			if (state->isAttacking()) {
				state->lastState = idle; 
			}
			else {
				state->setState(idle);
			}			
		}
		else {
			if (state->isAttacking()) {
				state->lastState = walking;
			}
			else {
				state->setState(walking);
			}			
		}
		velocity.y = 0;
	}	
}
```



## StateComponent

Das StateComponent speichert den aktuellen und den letzten Zustand des Entitys. Dieser kann geändert werden und es kann zum letzten Zustand zurück gewechselt werden. Dies ist beim Angreifen sehr wichtig. Beim ändern des Zustandes wird direkt die Animation des SpriteComponents geändert.

```c++
class StateComponent : public Component {
private:
public:
	State currentState = idle;
	State lastState = idle;
	StateComponent() = default; 
	void setState(State s);
	void switchToLastState() {
		State s = currentState;
		currentState = lastState;
		lastState = s; 
	}
	bool isAttacking() {
		return currentState == attackingBottom || currentState == attackingSide || currentState == attackingTop;
	}
};
```

```c++
void StateComponent::setState(State s) {
	lastState = currentState;
	currentState = s;
	parent->getComponent<SpriteComponent>().switchAnimation(s);
}
```



### Änderungen durch StateComponent im SpriteComponent

Die Map der Animationen hat ab jetzt einen Zustand als Key. Für jeden Zustand gibt es also eine Animation. Außerdem kann man per Methode einfach neue Animationen hinzufügen. 

```c++
std::map<const State, Animation> animations;
```

```c++
void SpriteComponent::switchAnimation(const State s) {
	frames = animations[s].frames; 
	animationIndex = animations[s].index;
	speed = animations[s].speed;
}

void SpriteComponent::addAnimation(State s, int index, int length, int frameDuration) {
	Animation a =  Animation(index, length, frameDuration);
	animations.emplace(s, a);
}
```



### State Enum 

```c++
enum State {
	idle, 
	walking, 
	jumping, 
	falling, 
	fallingAfterJump,
	attackingSide,
	attackingBottom, 
	attackingTop,
};
```



## Angreifen 

Der Spieler kann nun in die Richtung angreifen, in die er zur Zeit schaut. Dafür habe ich ein AttackComponent erstellt. Dies platziert einen Collider an der Stelle, an der die Spielfigur angreift und wechselt auf den richtigen Zustand. 

```c++
void AttackComponent::update() {
	Direction curDirection = transform->direction;
	switch (curDirection) {
	case up:
		attackCollider.x = transform->position.x - Game::camera.x;
		attackCollider.y = transform->position.y - ATTACK_RANGE - Game::camera.y;
		attackCollider.w = transform->width;
		attackCollider.h = ATTACK_RANGE;
		break;
	case right:
		attackCollider.x = transform->position.x + transform->width - Game::camera.x;
		attackCollider.y = transform->position.y - Game::camera.y;
		attackCollider.w = ATTACK_RANGE;
		attackCollider.h = transform->height;
		break;
	case down:
		attackCollider.x = transform->position.x - Game::camera.x;
		attackCollider.y = transform->position.y + transform->height - Game::camera.y;
		attackCollider.w = transform->width;
		attackCollider.h = ATTACK_RANGE;
		break;
	case left:
		attackCollider.x = transform->position.x - ATTACK_RANGE - Game::camera.x;
		attackCollider.y = transform->position.y - Game::camera.y;
		attackCollider.w = ATTACK_RANGE;
		attackCollider.h = transform->height;
		break;
	default:
		break;
	}
	if (SDL_GetTicks() - attackStart >= attackDuration) {
		if (state->isAttacking()) {
			state->switchToLastState();
		}		
	}
}
```

```c++
void AttackComponent::attack() {
	if (!state->isAttacking()) {
		attackStart = SDL_GetTicks();
		Direction curDirection = transform->direction;		
		switch (curDirection) {
		case up:
			state->setState(attackingTop);
			break;
		case left:
			state->setState(attackingSide);
			break;
		case right:
			state->setState(attackingSide);
			break;
		case down:
			state->setState(attackingBottom);			
			break;

		}
	}
}
```

