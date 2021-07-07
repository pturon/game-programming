# Spielkonzept

Geplant ist ein 2D Jump-and-Run nach dem Vorbild des Spiels **[Hollow Knight](https://store.steampowered.com/app/367520/Hollow_Knight/)**. Bei dem Spiel handelt es sich um ein sog. [Metroidvania](https://de.wikipedia.org/wiki/Metroidvania). In diesem Projekt wird der Fokus auf die Verwirklichung der im Spiel vorhandenen Gameplaymechaniken wie Steuerung, Angriffe, Fähigkeiten und Gegner. 

## Aufbau

Die Spielwelt besteht aus vielen einzelnen aber zusammenhängenden Räumen. Jeder Raum besteht aus 3 Ebenen: Vordergrund, Mittelgrund und Hintergrund. Das Spiel findet auf dem Mittelgrund statt. Für das Level-Design wird ein Tilesystem verwendet. Dafür wird das Level in ein Raster aufgeteilt und jede Zelle des Rasters bekommt aus einer Auswahl von Bildern ein Bild zugewiesen. 

In den Räumen können Gegner und Items auftauchen. Außerdem gibt es Räume in denen Bossgegner sind. Diese Räume können nur verlassen werden, wenn der Boss besiegt wurde. 

## Gameplay

### Grundlagen

#### Bewegung:

Die Spielfigur kann sich nach Links und Rechts bewegen und nach Oben Springen. Die Figur springt so lange nach Oben bis die maximale Sprunghöhe erreicht ist oder die Sprungtaste losgelassen wurde. 

#### Angreifen:

Die Spielfigur greift immer in die aktuelle Blickrichtung an. Wenn im Fallen ein Entity unter der Spielfigur getroffen wird von einem Angriff, springt die Spielfigur wieder leicht nach Oben. Alle Bewegungsfähigkeiten (siehe Unten), wie Dashs usw., können danach wieder verwendet werden. 

#### Erweiterungen

Durch Erreichen bestimmter Punkte im Spiel schaltet die Spielfigur weitere Bewegungsfähigkeiten frei. Diese können pro Sprung je einmal eingesetzt werden. 
Mögliche Bewegungsfähigkeiten:

- Dash - Schnell nach Links und Rechts springen 
- Doppelsprung - Während des Sprunges ein weiteres mal Springen 
- Walljump - Bei der Kollision mit einer Wand im Sprung kann man von dieser wieder Abspringen. 

#### Fähigkeiten

Fähigkeiten werden durch Gegenstände aufgesammelt. Durch das Ausrüsten dieser Gegenstände, werden der Spielfigur die Fähigkeiten der Gegenstände verliehen. Es kann aber immer nur eine bestimmte Anzahl an Gegenständen ausgerüstet werden. Jede Kombination aus Gegenständen ist möglich und kann jederzeit ausgewechselt werden in einem Menü.

### Ziel des Spiels

Ziel des Spiels ist das Erkunden der Spielwelt und das Erreichen neuer Bereiche durch das Erlangen von  Bewegungsfähigkeiten. Die Fähigkeiten sollen dem Spieler ein individuelles Spielgefühl geben und das Besiegen der Gegner erleichtern oder erschweren. 

