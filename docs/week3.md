# Woche 3

## Zusammenfassung 

Diese Woche werden alle Bewegungsaspekte für das Spiel fertig gestellt. Im Moment kann die Spielfigur alle Fähigkeiten von Anfang an. Später wird man die Fähigkeiten über bestimmte Events bekommen. 
Leider ist beim Testen aufgefallen, dass die Kollision von Rechts nach Links nicht funktioniert. 

## Move-Abilities

### Doppelsprung 

Die Umsetzung des Doppelsprunges ist ganz einfach. Die Variable **canDoubleJump** definiert, ob die Spielfigur während des Sprunges noch einmal Springen kann. Diese wird nach dem Aufprall auf dem Boden wieder auf true gesetzt. 

```c++
void TransformComponent::jump() {
	if (!state->isAttacking()) {
		if (state->currentState != jumping && state->currentState != falling) {
			stopWallCling();
			state->setState(jumping);
			velocity.y = static_cast<float>(-jumpHeight);
		}
		else if (canDoubleJump) {
			canDoubleJump = false; 
			state->setState(jumping);
			velocity.y = static_cast<float>(-jumpHeight);
		}
	}	
}
```



### Dashing

Genauso wie der Doppelsprung soll die Spielfigur nur einmal pro Sprung dashen können. Beim dashen springt die Figur schnell nach rechts oder links bei gleicher Höhe. Dieser wird abgebrochen sobald eine Wand berührt wird. Man kann auch nicht während des Dashs die Richtung wechseln. 

```c++
void TransformComponent::dash() {
	if (state->currentState != dashing && canDash) {
		canDash = false; 
		switch (direction){
		case left: 
			velocity.x = -1; 
			break; 
		case right:
			velocity.x = 1; 
			break; 
		default:
			break;
		}
		state->setState(dashing);
		dashStart = SDL_GetTicks();
	}	
}
```



### Wall-Jump

Der Wall-Jump wird ausgeführt, wenn die Spielfigur an der Wand hängt. Sobald man gegen eine Wand springt, fällt oder dashed hält sich die Spielfigur fest und rutscht die Wand langsam herunter. Während die Figur an der Wand ist kann sie immer springen. Sobald man sich von der Wand wegbewegt wird diese losgelassen. 

```c++
void TransformComponent::startWallCling(Direction d) {
	if (state->currentState != wallCling) {
		state->setState(wallCling);
		velocity.x = 0;
		velocity.y = 0;
		clingedWallPos = d;
	}	
}

void TransformComponent::stopWallCling() {
	if (state->currentState == wallCling) {
		state->setState(falling);
	}	
}
```

```c++
if (state->currentState == wallCling) {
			position.y += gravity.y / 3; 
		}
```

```c++
if (player.getComponent<StateComponent>().currentState != walking) {
						player.getComponent<TransformComponent>().startWallCling(right);
					}
					
if (player.getComponent<StateComponent>().currentState != walking) {
						player.getComponent<TransformComponent>().startWallCling(left);
					}
```



## Zusatz

Den Rest der Woche habe ich versucht die Kollision zu überarbeiten. Bisher kam ich aber zu keinem Erfolg. 