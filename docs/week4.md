# Woche 4

## Zusammenfassung

Diese Woche wurde die Kollisionsabfrage endgültig fertiggestellt. Diese funktioniert nun fehlerfrei und alle anderen bestehenden Funktionen wurden dieser angepasst. 

## Kollisionsabfrage

Zuvor mussten die Abfragen für die Kollision neu geschrieben werden. Zuvor wurde abgefragt, ob sich zwei Rechtecke überschneiden. Damit war die Korrektur der Position aber schwer bis unmöglich. Dafür wurde eine Funktion geschrieben, die überprüft ob ein Vektor ein Rechteck schneidet. Wenn dies der Fall ist, wird der Kollisionspunkt und die Normale der geschnittenen Seite des Rechtecks ausgegeben.

```c++
bool Collision::RayRect(const Vector2D& origin, const Vector2D& dir, const SDL_Rect& target, Vector2D &contactPoint, Vector2D &contactNormal, float &t_hit_near) {	
	contactNormal = { 0,0 };
	contactPoint = { 0,0 };

	Vector2D invDir = { 1.0f / dir.x, 1.0f / dir.y };
	
	Vector2D t_near = { (target.x - origin.x ) * invDir.x, ((target.y - origin.y) * invDir.y) };
	Vector2D t_far = { (target.x + target.w - origin.x) * invDir.x, (target.y + target.h - origin.y) * invDir.y };

	if (std::isnan(t_far.y) || std::isnan(t_far.x)) return false;
	if (std::isnan(t_near.y) || std::isnan(t_near.x)) return false;
	
	if (t_near.x > t_far.x) std::swap(t_near.x, t_far.x);
	if (t_near.y > t_far.y) std::swap(t_near.y, t_far.y);

	if (t_near.x > t_far.y || t_near.y > t_far.x) return false; 

	t_hit_near = std::max(t_near.x, t_near.y);
	float t_hit_far = std::min(t_far.x, t_far.y);

	if (t_hit_far < 0) return false; 

	contactPoint = {(origin.x + dir.x * t_hit_near), (origin.y + dir.y * t_hit_near)};

	if (t_near.x > t_near.y) {
		if (invDir.x < 0) {
			contactNormal = { 1, 0 };
		}
		else {
			contactNormal = { -1, 0 };
		}
	}
	else if (t_near.x < t_near.y) {
		if (invDir.y < 0) {
			contactNormal = { 0,1 };
		}
		else {
			contactNormal = { 0,-1 };
		}
	}

	return true; 
}
```

Für die Korrektur wird geprüft, ob ein Vektor ein größeres Rechteck schneidet. Das Rechteck wird dabei um die Breite und Höhe der Spielfigur vergrößert. Der Mittelpunkt dabei bleibt gleich. Der Schnittpunkt der dabei heraus kommt, ist der Mittelpunkt der Spielfigur. Wenn die Spielfigur auf dieser Position ist liegt die Figur direkt an dem Rechteck an. 

```c++
bool Collision::DynamicRectRect(const SDL_Rect& in, const Vector2D inVelocity, const SDL_Rect& target, Vector2D& contactPoint, Vector2D& contactNormal, float& contactTime) {
	if (inVelocity.x == 0 && inVelocity.y == 0) {
		return false; 
	}
	SDL_Rect expandedTarget;
	expandedTarget.x = target.x - in.w / 2;
	expandedTarget.y = target.y - in.h / 2; 
	expandedTarget.w = target.w + in.w;
	expandedTarget.h = target.h + in.h;

	

	Vector2D centerPoint = { static_cast<float>(in.x + in.w / 2),  static_cast<float>(in.y + in.h / 2) };

	if (RayRect(centerPoint, inVelocity, expandedTarget, contactPoint, contactNormal, contactTime)) {
		if (contactTime <= 1.0f && contactTime >= 0.0f) {			
			return true;
		}
	}

	return false; 
}
```

Die Bewegung der Spielfigur wird bei einer Kollision nur in eine Richtung blockiert. Welche dies ist, wird durch die Normale der geschnittenen Seite entschieden. Doch bevor die Position korrigiert wird, wird eine Liste aller Collider erstellt, die mit dem Spieler kollidieren. Diese Liste wird nach der Entfernung sortiert. Danach wird die Liste abgearbeitet und die Position so korrigiert, dass keine Kollision mehr vorliegt. Das hat den Grund, dass die Korrektur manchmal zuerst auf der X-Achse durchgeführt, obwohl sie zuerst auf der Y-Achse durchgeführt werden soll und anders herum. 

```c++
std::vector<std::pair<Entity*, float>> z; 

		for (auto& c : colliders) {
			SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
			if (Collision::DynamicRectRect(playerColliderPosBefore, playerVelocity, cCol, cp, cn, ct)) {					
				z.push_back({ c,ct });
				c->getComponent<ColliderComponent>().collided = true;				
			}
			else {
				c->getComponent<ColliderComponent>().collided = false;
			}
		}

		std::sort(z.begin(), z.end(), [](const std::pair<Entity*, float>& a, const std::pair<Entity*, float>& b) {
			return a.second < b.second;
		});
		for (auto c : z) {
			SDL_Rect cCol = c.first->getComponent<ColliderComponent>().collider;
			if (Collision::DynamicRectRect(playerColliderPosBefore, playerVelocity, cCol, cp, cn, ct)) {				
				playerVelocity.x += cn.x * std::abs(playerVelocity.x) * (1 - ct);
				playerVelocity.y += cn.y * std::abs(playerVelocity.y) * (1 - ct);	
				if (cn.x == 0 && cn.y == -1) {
					player.getComponent<TransformComponent>().stopJump();
					if (player.getComponent<StateComponent>().currentState == wallCling) {
						player.getComponent<TransformComponent>().stopWallCling();
					}
				}
				else if (cn.x == 0 && cn.y == 1) {
					player.getComponent<TransformComponent>().startFall();
				}
				else if (cn.x == -1 && cn.y == 0) {
					if (player.getComponent<StateComponent>().currentState == dashing) {
						player.getComponent<TransformComponent>().stopDash();
					}
					if (player.getComponent<StateComponent>().currentState != walking) {
						player.getComponent<TransformComponent>().startWallCling(right);
					}
				}
				else if (cn.x == 1 && cn.y == 0) {
					if (player.getComponent<StateComponent>().currentState == dashing) {
						player.getComponent<TransformComponent>().stopDash();
					}
					if (player.getComponent<StateComponent>().currentState != walking) {
						player.getComponent<TransformComponent>().startWallCling(left);
					}
				}
			}
		}		

		player.getComponent<TransformComponent>().position.x = playerColliderPosBefore.x + playerVelocity.x;
		player.getComponent<TransformComponent>().position.y = playerColliderPosBefore.y + playerVelocity.y;		
```

