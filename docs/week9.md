# Woche 9

## Zusammenfassung 

Diese Woche ging es in den Endspurt und das erste richtige Spielerlebnis konnte endgültig umgesetzt werden. Zuerst wurde Feedback eingebaut, wenn ein Gegner getroffen wurde und ein weiterer Gegner wurde hinzugefügt. 

## Trefferfeedback

Bei einem Treffer sollen die Gegner zurückgeschlagen werden und um Schaden zu symbolisieren leuchten diese einmal rot auf dabei. 

Für den Rückschlag wurde eine weitere Funktion zum TransformComponent hinzugefügt. 

```c++
void TransformComponent::startRecoil(Vector2D rV) {
	recoil = true; 
	recoilDuration = 10; 
	recoilV = rV; 
}
```

Für die Berechnung der Richtung des Rückschlag ist das Behavior verantwortlich und berechnet abhängig von der Angriffsrichtung des Spielers die Richtung. 

```c++
void GoombaBehaviour::onHit(Direction d) {
	Vector2D recoilV = { 0,0 };
	switch (d) {
	case up:
		recoilV.y = -1;
		break;
	case right:
		recoilV.x = 1;
		break;
	case down:
		recoilV.y = 1;
		break;
	case left:
		recoilV.x = -1;
		break;
	default:
		break;
	}
	parent->getComponent<TransformComponent>().startRecoil(recoilV);
}
```

In der update()-Methode des TransformComponents wird nun der Rückschlag auf die aktuelle Position angewendet. 

```c++
if (recoil) {
		if (recoilDuration > 0) {
			position.x += recoilV.x * recoilSpeed;
			position.y += recoilV.y * recoilSpeed;
			recoilDuration--;
		}
		else {
			recoil = false; 
		}		
	}
```

Für die rote Färbung während des Rückstoßes wird im SpriteComponent ein Filter auf die Textur des Spritesheets angewandt und wieder entfernt, sobald der Rückstoß vorbei ist. 

```c++
if (parent->getComponent<TransformComponent>().recoil == true) {
		SDL_SetTextureColorMod(texture, 255, 0, 0);
	}
	else {
		SDL_SetTextureColorMod(texture, 255, 255, 255);
	}
```

## Weiterer Gegner 

Die Ratte ist kein anspruchsvoller Gegner. Deshalb ist der nächste Gegner eine größere Herausforderung für den Spieler. Dieser verfolgt die Spielfigur, sobald diese in Sichtweite ist. Wenn er nah genug am Spieler ist, wird er angreifen. Ist der Spieler außerhalb des Sichtfeldes oder nicht mehr zu sehen für den Gegner patrouilliert er wie die Ratte von links nach rechts. 

### Verfolgung

Für die Abfrage der Distanz zwischen Spieler und Gegner wird ein Vektor berechnet, welcher vom Mittelpunkt des Gegners zum Mittelpunkt des Spielers geht. Dieser Vektor stellt auch gleichzeitig die Sichtlinie zwischen den beiden dar. Kollidiert dieser Vektor mit einem Collider der Karte, kann der Gegner den Spieler nicht sehen, da etwas im Weg ist. Ist der Weg frei, wird die Distanz berechnet. Ist der Spieler also in Sichtweite und nichts blockiert die Sicht, bewegt sich der Gegner auf den Spieler zu und wechselt somit in den "detecting" Zustand. Im Detection Zustand verfolgt der Gegner den Spieler und greift den Spieler an, wenn dieser in Reichweite ist. 

```c++
if (parent->getComponent<TransformComponent>().recoil == false) {
			Vector2D pos = { (parent->getComponent<TransformComponent>().position.x + parent->getComponent<TransformComponent>().width) / 2,  (parent->getComponent<TransformComponent>().position.y + parent->getComponent<TransformComponent>().height) / 2 };
			Vector2D playerPos = { (Game::player.getComponent<TransformComponent>().position.x + Game::player.getComponent<TransformComponent>().width) / 2,  (Game::player.getComponent<TransformComponent>().position.y + Game::player.getComponent<TransformComponent>().height) / 2 };;
			Vector2D dist = { playerPos.x - pos.x, playerPos.y - pos.y };
			int d = sqrt(dist.x * dist.x + dist.y * dist.y);
			if (!Game::rayHitsCollider(pos, dist)) {
				if (d <= attackRange) {
					parent->getComponent<TransformComponent>().velocity = { 0,0 };
					tickStart = SDL_GetTicks();;
					parent->getComponent<StateComponent>().setState(charge);			
				}
				else if (d <= detectionRange) {
					if (dist.x > 0) {
						parent->getComponent<TransformComponent>().velocity = { 1,0 };
						parent->getComponent<SpriteComponent>().flipAnimation(false);
						parent->getComponent<StateComponent>().currentState == detecting;
						parent->getComponent<TransformComponent>().direction = right;
					}
					else if (dist.x < 0) {
						parent->getComponent<TransformComponent>().velocity = { -1,0 };
						parent->getComponent<SpriteComponent>().flipAnimation(true);
						parent->getComponent<StateComponent>().currentState == detecting;
						parent->getComponent<TransformComponent>().direction = left;
					}
				}
				else {
					parent->getComponent<StateComponent>().currentState == walking;
				}
			}
			else {
				parent->getComponent<StateComponent>().currentState == walking;
				if (parent->getComponent<TransformComponent>().velocity.x == 0) {
					if (parent->getComponent<SpriteComponent>().spriteFlip) {
						parent->getComponent<TransformComponent>().velocity = { -1,0 };
						parent->getComponent<TransformComponent>().direction = left;
					}
					else {
						parent->getComponent<TransformComponent>().velocity = { 1,0 };
						parent->getComponent<TransformComponent>().direction = right;
					}
				}
			}
		}
```

### Sichtberechnung 

```c++
bool Game::rayHitsCollider(Vector2D origin, Vector2D ray) {
	bool hit = false;
	Vector2D cp, cn;
	float t;
	for (auto& c : colliders) {
		SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
		if (Collision::RayRect(origin, ray, cCol, cp, cn, t)) {
			if (t <= 1) {
				hit = true;
			}		
		}
	}
	return hit; 
}
```

### Angreifen 

Der Gegner greift nicht direkt an, wenn der Spieler in Reichweite ist. Der Angriff des Gegners erfolgt in drei Phasen. Zuerst holt dieser aus und verharrt am Ende kurz. Danach greift er erst an. Nach dem Angriff verharrt der Gegner bevor er sich weiter bewegt oder erneut angreift. 

```c++
tickCounter = SDL_GetTicks();
	if (parent->getComponent<StateComponent>().currentState == charge) {		
		if (tickCounter - tickStart >= charge) {
			tickStart = SDL_GetTicks();
			parent->getComponent<StateComponent>().setState(attackingSide);		
			parent->getComponent<AttackComponent>().attack();
		}
	}
	else if (parent->getComponent<StateComponent>().currentState == attackCooldown) {	
		if (tickCounter - tickStart >= cooldown) {
			parent->getComponent<StateComponent>().setState(walking);			
		}
	}
	else if (parent->getComponent<StateComponent>().currentState == attackingSide) {		
		if (tickCounter - tickStart >= attackDuration) {
			tickStart = SDL_GetTicks();
			parent->getComponent<StateComponent>().setState(attackCooldown);			
		}
	}
```

### Trefferabfrage 

Dieser Gegner verteilt keinen Schaden durch bloßes Berühren. Die Abfrage eines Treffers ist damit ähnlich wie die des Spielers. 

```c++
if (e->hasComponent<AttackComponent>()) {
						if (e->getComponent<StateComponent>().isAttacking()) {
							SDL_Rect eAttack = { e->getComponent<AttackComponent>().attackCollider.x + camera.x, e->getComponent<AttackComponent>().attackCollider.y + camera.y, e->getComponent<AttackComponent>().attackCollider.w, e->getComponent<AttackComponent>().attackCollider.h };
							if (Collision::RectRect(player.getComponent<ColliderComponent>().collider, eAttack)) {								
								if (player.getComponent<StatsComponent>().iFrames == 0) {
									player.getComponent<StatsComponent>().curHealth -= e->getComponent<StatsComponent>().attackDamage;
									player.getComponent<StatsComponent>().iFrames = player.getComponent<StatsComponent>().maxIFrames;
								}
							}
						}
					}
```

### Tod eines Gegners

Damit die Gegner nicht einfach verpuffen, wenn die Lebensanzahl auf 0 fällt, wird vorher noch eine Animation abgespielt. Nach dem Ende der Animation wird das Objekt aus dem Spiel entfernt. 

```c++
else if (parent->getComponent<StateComponent>().currentState == dying) {	
		if (tickCounter - tickStart >= dyingDuration) {
			tickStart = SDL_GetTicks();
			parent->getComponent<StateComponent>().setState(dead);
		}
	}
	else if (parent->getComponent<StateComponent>().currentState == dead) {	
		if (tickCounter - tickStart >= despawnTime) {
			parent->destroy();
		}
	}
```

```c++
void SkeletonBehaviour::die() {
	if (parent->getComponent<StateComponent>().currentState != dying && parent->getComponent<StateComponent>().currentState != dead) {
		parent->getComponent<StateComponent>().setState(dying);
		tickCounter = SDL_GetTicks();
		parent->getComponent<TransformComponent>().velocity.x = 0;
	}
}
```

## Collideranpassung

Vorher hatte der Collider eines Objektes die Größe der Bilddatei. Deshalb wurde der Spieler in manchen Situationen getroffen, obwohl die angezeigte Spielfigur gar nicht getroffen wurde. Daher habe ich ein Offset in X- und Y-Richtung eingebaut. Dadurch kann der Collider kleiner sein als die Bilddatei und nur den Bereich der Spielfigur enthalten. 

```c++
void ColliderComponent::init(){
	if (!parent->hasComponent<TransformComponent>()) {
		parent->addComponent<TransformComponent>();
	}
	transform = &parent->getComponent<TransformComponent>();

	texture = TextureManager::loadTexture("assets/c.png");

	if (tag != "terrain") {
		collider.x = static_cast<int>(transform->position.x + xOffset);
		collider.y = static_cast<int>(transform->position.y + yOffset);
		collider.w = (transform->width - 2 * xOffset) * transform->scale;
		collider.h = (transform->height - 2 * yOffset) * transform->scale;
	}	

	srcRect.x = srcRect.y = 0; 
	srcRect.w = TILE_WIDTH;
	srcRect.h = TILE_HEIGHT;
	destRect = { collider.x, collider.y, collider.w, collider.h };
}

void ColliderComponent::update() {
	if (tag != "terrain") {		
	
		collider.x = static_cast<int>(transform->position.x + xOffset);
		collider.y = static_cast<int>(transform->position.y + yOffset);
		collider.w = (transform->width - 2 * xOffset) * transform->scale;
		collider.h = (transform->height - 2 * yOffset) * transform->scale;

	}
	destRect.x = collider.x - Game::camera.x;
	destRect.y = collider.y - Game::camera.y;	
}
```

## Pogo

Eine Fähigkeit, welche in das Spiel eingebaut werden sollte, ist das Pogon. Wird ein Gegner unter der Spielfigur durch einen Angriff getroffen hüpft diese wieder nach Oben durch den Schlag und kann alle Bewegungsfähigkeiten erneut einsetzen. 

```c++
if (player.getComponent<TransformComponent>().direction == down) {
									player.getComponent<TransformComponent>().pogo();
								}
```

```c++
void TransformComponent::pogo() {
	state->setState(jumping);
	velocity.y = static_cast<float>(-jumpHeight);
	canDash = true; 
	canDoubleJump = true; 
}
```

## Neuer Sprite 

Die alte Spielfigur passte überhaupt nicht mehr in den Stil des Spiels. Deshalb habe ich ihn etwas angepasst und angefangen diesen neu zu zeichnen. Noch bin ich nicht fertig, aber in der kommenden Woche wird das, wie ein Sprite für den neuen Gegner abgeschlossen. 

