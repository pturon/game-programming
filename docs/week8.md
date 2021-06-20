# Woche 8

## Zusammenfassung

Diese Woche habe ich einen simplen Gegner eingebaut. Dabei gab es einige Probleme und deshalb sind die Ergebnisse etwas dürftig ausgefallen und einen Fehler behoben.

## WallCling 

Zuvor war es möglich, dass die Spielfigur immer noch an einer Wand herunter gleitet, obwohl keine Wand mehr da ist. Diesen Fehler habe ich diese Woche zuerst behoben.  

Dafür wird jedes mal zuerst die Position der Wand, an der sich festgehalten wird, abgefragt. Abhängig von dieser wird die Position des überprüften Colliders abgefragt. Falls dieser auf der Seite der Wand ist, wird eine Variable auf falsch gesetzt. Ist diese am Ende der Überprüfung wahr, lässt die Spielfigur los.

```c++
if (player.getComponent<StateComponent>().currentState == wallCling) {
bool stopWallCling = true;
for (auto& c : colliders) {
	SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
	if (player.getComponent<TransformComponent>().clingedWallPos == right) {
		if (cCol.x == pCol.x + pCol.w && cCol.y + cCol.h >= pCol.y && cCol.y < pCol.y + pCol.h) {
			stopWallCling = false;
		}
	}
	else {
		if (cCol.x + cCol.w == pCol.x && cCol.y + cCol.h >= pCol.y && cCol.y < pCol.y + pCol.h) {
			stopWallCling = false;
			}
	}
}

	if (stopWallCling) {
		player.getComponent<TransformComponent>().stopWallCling();
	}
}
```

## Gegner

Der erste Gegner hat ein ganz einfaches Verhalten. Er läuft einfach von Rechts nach Links und bei einer Kollision mit einer Wand auf der X-Achse wird die Richtung gewechselt. Wird der Gegner berührt, bekommt der Spieler Schaden. Ist der Gegner im Bereich des Schwertes während des Angriffes, bekommt der Gegner Schaden. 

### BehaviorComponent

Um das Verhalten eines Gegners zu verwalten, speichert das BehaviorComponent ein Objekt einer Unterklasse der Behaviour Klasse und gibt die Ereignisse an dieses weiter. 

```c++
class BehaviourComponent : public Component{
public: 
	std::vector<std::unique_ptr<Behaviour>> behaviours;

	BehaviourComponent() = default;
	~BehaviourComponent();

	void init() override;
	void update() override;
	void onCollision(Vector2D cn);
	void onHit(); 

	template <typename T, typename... TArgs> T& setBehaviour(TArgs&&... mArgs) {
		T* c(new T(std::forward<TArgs>(mArgs)...));
		c->parent = this->parent; 
		std::unique_ptr<Behaviour> uPtr{ c };
		behaviours.emplace_back(std::move(uPtr));
		c->init();
		return *c; 
	}
};
```

Beim Speichern des Behaviour Objektes gab es einige Komplikationen, da ich kein Objekt der Unterklasse einfach so speichern konnte. Deshalb habe ich die Methode addComponent() aus dem Entity so umgeschrieben, dass sie ein neues Objekt einer Unterklasse von Behaviour speichert. 

### Behaviour

Die Klasse Behaviour ist eine abstrakte Klasse, die das Verhalten bei einer Kollision und bei einem Treffer durch den Spieler realisiert. Das Genaue Verhalten wird in den Unterklassen beschrieben. 

```c++
class Behaviour {
public:
	Entity* parent;
	virtual void init() {};
	virtual void update() {};
	virtual void onCollision(Vector2D cn) {}
	virtual void onHit() {};
};
```

### GoombaBehaviour

Das Verhalten des ersten Gegners wird in der GoombaBehaviour Klasse definiert. Ein Goomba ist der erste Pilzgegner in Super Mario und hat genau das gleiche Verhalten. Zuerst läuft der Gegner nach Rechts bis eine Wand im Weg ist. Danach wird die Richtung gewechselt. 

```c++
class GoombaBehaviour : public Behaviour{
public:
	bool directionSwitched = false; 
	void init() override;
	void update() override;
	void onCollision(Vector2D cn) override;
	void onHit() override;
};
```

```
void GoombaBehaviour::init() {
	parent->getComponent<TransformComponent>().velocity = { 1,0 };
}

void GoombaBehaviour::update() {	
	directionSwitched = false; 
}

void GoombaBehaviour::onCollision(Vector2D cn) {
	if (cn.x != 0 && cn.y == 0) {
		parent->getComponent<TransformComponent>().velocity = cn;
	}
}

void GoombaBehaviour::onHit() {

}
```

Beim Erstellen wird die Bewegungsrichtung nach Rechts gesetzt. Bei einer Kollision wird die Normale der geschnittenen Seite der Wand abgefragt. Falls diese auf der X-Achse ist, wird die Bewegungsrichtung mit der Normalen gleichgesetzt. 

### Kollisionsabfrage

Die Kollisionsabfrage für die Gegner ist nicht anders als die Abfrage für den Spieler. Hier wird nur die Liste aller Gegner durchgegangen und die Kollision wird für jeden einzeln abgefragt und korrigiert. 

```c++
for (auto& e : enemies) {
				SDL_Rect eBefore = { e->getComponent<TransformComponent>().lastPos.x, e->getComponent<TransformComponent>().lastPos.y, e->getComponent<TransformComponent>().width, e->getComponent<TransformComponent>().height };
				Vector2D eVelocity = { e->getComponent<TransformComponent>().position.x - e->getComponent<TransformComponent>().lastPos.x, e->getComponent<TransformComponent>().position.y - e->getComponent<TransformComponent>().lastPos.y };

				std::vector<std::pair<Entity*, float>> eZ;

				for (auto& c : colliders) {
					SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
					if (Collision::DynamicRectRect(eBefore, eVelocity, cCol, cp, cn, ct)) {
						eZ.push_back({ c,ct });
					}
				}

				std::sort(eZ.begin(), eZ.end(), [](const std::pair<Entity*, float>& a, const std::pair<Entity*, float>& b) {
					return a.second < b.second;
					});
				for (auto c : eZ) {
					SDL_Rect cCol = c.first->getComponent<ColliderComponent>().collider;
					if (Collision::DynamicRectRect(eBefore, eVelocity, cCol, cp, cn, ct)) {
						eVelocity.x += cn.x * std::abs(eVelocity.x) * (1 - ct);
						eVelocity.y += cn.y * std::abs(eVelocity.y) * (1 - ct);
						e->getComponent<BehaviourComponent>().onCollision(cn);

					}
				}
				e->getComponent<TransformComponent>().position.x = eBefore.x + eVelocity.x;
				e->getComponent<TransformComponent>().position.y = eBefore.y + eVelocity.y;
			}
```



### Trefferüberprüfung

Bei der Trefferüberprüfung werden alle Gegner überprüft. Zuerst wird geschaut, ob der Spieler zu diesem Zeitpunkt angreift. In diesem Fall wird eine Überschneidung des AttackColliders mit dem Collider des Gegners. Bei einem Treffer wird das Leben des Gegners um den Schaden des Spielers reduziert. 

Bei einer Überschneidung des Colliders des Spielers mit dem des Gegners. Genau wie beim Angreifen des Spielers wird das Leben des Spielers um den Schaden des Gegners reduziert. 

```c++
for (auto& e : enemies) {
				if (player.getComponent<AttackComponent>().attacking) {
					if (Collision::RectRect(player.getComponent<AttackComponent>().attackCollider, e->getComponent<ColliderComponent>().collider)) {
						if (e->getComponent<StatsComponent>().iFrames == 0) {
							e->getComponent<StatsComponent>().curHealth -= player.getComponent<StatsComponent>().attackDamage;
							e->getComponent<StatsComponent>().iFrames = e->getComponent<StatsComponent>().maxIFrames;
						}
					}
				}
				if (Collision::RectRect(player.getComponent<ColliderComponent>().collider, e->getComponent<ColliderComponent>().collider)) {
					if (player.getComponent<StatsComponent>().iFrames == 0) {
						player.getComponent<StatsComponent>().curHealth -= e->getComponent<StatsComponent>().attackDamage;
						player.getComponent<StatsComponent>().iFrames = player.getComponent<StatsComponent>().maxIFrames;
					}
				}
			}
```



### iFrames

iFrames sind ein Indikator dafür, dass ein Entity zu diesem Zeitpunkt keinen Schaden nehmen kann. Bei einem Treffer wird das Entity unverwundbar. Bei der Trefferüberprüfung wird der Schaden nur dann verarbeitet, wenn die Anzahl der iFrames 0 ist. Diese werden im StatComponent gespeichert. Dort wird die Anzahl der iFrames jeden Frame um 1 reduziert bis diese auf 0 sind. 

```c++
void StatsComponent::update() {
	iFrames--;
	if (iFrames < 0) {
		iFrames = 0; 
	}
	if (curHealth == 0) {
		//parent->destroy();
	}
}
```

## Zusatz 

Zurzeit gibt es keinen Indikator für einen Treffer. Der Gegner ist also quasi uninteressiert von dem Schaden. Der Spieler wird auch nicht gelöscht bei einer Herzen Anzahl von 0, da das Programm noch abstürzt. Diese Dinge werden in der nächsten Woche umgesetzt. 