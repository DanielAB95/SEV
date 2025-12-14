#include "Space.h"
#include <algorithm>


Space::Space(float gravity) {
	this->gravity = gravity;
}

void Space::addDynamicActor(Actor* actor) {
	dynamicActors.push_back(actor);
}

void Space::addStaticActor(Actor* actor) {
	staticActors.push_back(actor);
}

void Space::removeDynamicActor(Actor* actor) {
	dynamicActors.remove(actor);
}

void Space::removeStaticActor(Actor* actor) {
	staticActors.remove(actor);
}

bool Space::hasCollision(Actor* actor) {
	// Comprobar colisiones con actores estáticos
	for (auto const& staticActor : staticActors) {
		if (staticActor != nullptr && actor->isOverlap(staticActor)) {
			return true;
		}
	}
	return false;
}

bool Space::checkCollisionDirection(Actor* actor, bool& collidesHorizontal, bool& collidesVertical) {
	// Detectar si hay colisión y en qué dirección
	collidesHorizontal = false;
	collidesVertical = false;
	bool hasAnyCollision = false;
	
	for (auto const& staticActor : staticActors) {
		if (staticActor != nullptr && actor->isOverlap(staticActor)) {
			hasAnyCollision = true;
			
			// Calcular centros
			float actorCenterX = actor->x;
			float actorCenterY = actor->y;
			float staticCenterX = staticActor->x;
			float staticCenterY = staticActor->y;
			
			// Calcular distancias en cada eje
			float deltaX = abs(actorCenterX - staticCenterX);
			float deltaY = abs(actorCenterY - staticCenterY);
			
			// Determinar qué tipo de colisión es más probable basándose en las distancias
			// Si deltaX es mayor, probablemente colisiona horizontalmente
			// Si deltaY es mayor, probablemente colisiona verticalmente
			float minOverlapX = (actor->width + staticActor->width) / 2.0f;
			float minOverlapY = (actor->height + staticActor->height) / 2.0f;
			
			float overlapX = minOverlapX - deltaX;
			float overlapY = minOverlapY - deltaY;
			
			// La colisión es en la dirección con menor penetración
			if (overlapX < overlapY) {
				collidesHorizontal = true; // Colisión lateral (izq/der)
			} else {
				collidesVertical = true; // Colisión vertical (arriba/abajo)
			}
		}
	}
	
	return hasAnyCollision;
}

void Space::update() {
	for (auto const& actor : dynamicActors) {
		// Solo aplicar gravedad si es diferente de 0
		if (gravity != 0) {
			actor->vy = actor->vy + gravity;
			// máxima velocidad de caída por gravedad
			if (actor->vy > 20) {
				actor->vy = 20;
			}
		}

		// MoverDerecha / izquierda
		updateMoveRight(actor);
		updateMoveLeft(actor);
        updateMoveTop(actor);
		updateMoveDown(actor);
	}
}


void Space::updateMoveRight(Actor* dynamicAct) {
    if (dynamicAct->vx > 0) {
        float originalVx = dynamicAct->vx; // Guardar velocidad original
        float possibleMovement = dynamicAct->vx;
        // El mejor "idealmente" vx partimos de ese

        for (auto const& staticAct : staticActors) {
            float rightDynamic = dynamicAct->x + dynamicAct->width / 2;
            float topDynamic = dynamicAct->y - dynamicAct->height / 2;
            float downDynamic = dynamicAct->y + dynamicAct->height / 2;

            float leftStatic = staticAct->x - staticAct->width / 2;
            float topStatic = staticAct->y - staticAct->height / 2;
            float downStatic = staticAct->y + staticAct->height / 2;

            // Alerta!, Elemento estático en la trayectoria.
            if ((rightDynamic + dynamicAct->vx) >= leftStatic
                && rightDynamic <= leftStatic
                && topDynamic < downStatic
                && downDynamic > topStatic) {

                // Comprobamos si la distancia al estático es menor
                // que nuestro movimientoPosible actual
                if (possibleMovement >= leftStatic - rightDynamic) {
                    // La distancia es MENOR que nuestro movimiento posible
                    // Tenemos que actualizar el movimiento posible a uno menor
                    possibleMovement = leftStatic - rightDynamic;
                }

            }

        }
        // Ya se han comprobado todos los estáticos
        dynamicAct->x = dynamicAct->x + possibleMovement;
        // ARREGLO: No sobrescribir vx para mantener el comportamiento de IA
        // Solo establecer vx a 0 si hay colisión completa
        if (possibleMovement <= 0) {
            dynamicAct->vx = 0;
        } else {
            dynamicAct->vx = originalVx; // Mantener velocidad original
        }
    }
}




void Space::updateMoveLeft(Actor* dynamicAct) {
    if (dynamicAct->vx < 0) {
        float originalVx = dynamicAct->vx; // Guardar velocidad original
        float possibleMovement = dynamicAct->vx;
        // El mejor "idealmente" vx partimos de ese

        for (auto const& staticAct : staticActors) {
            float leftDynamic = dynamicAct->x - dynamicAct->width / 2;
            float topDynamic = dynamicAct->y - dynamicAct->height / 2;
            float downDynamic = dynamicAct->y + dynamicAct->height / 2;

            float rightStatic = staticAct->x + staticAct->width / 2;
            float topStatic = staticAct->y - staticAct->height / 2;
            float downStatic = staticAct->y + staticAct->height / 2;

            // Alerta!, Elemento estático en la trayectoria.
            if ((leftDynamic + dynamicAct->vx) <= rightStatic
                && leftDynamic >= rightStatic
                && topDynamic < downStatic
                && downDynamic > topStatic) {

                // Comprobamos si la distancia al estático es menor
                // que nuestro movimientoPosible actual
                if (possibleMovement <= rightStatic - leftDynamic) {
                    // La distancia es MENOR que nuestro movimiento posible
                    // Tenemos que actualizar el movimiento posible a uno menor
                    possibleMovement = rightStatic - leftDynamic;
                }

            }

        }
        // Ya se han comprobado todos los estáticos
        dynamicAct->x = dynamicAct->x + possibleMovement;
        // ARREGLO: No sobrescribir vx para mantener el comportamiento de IA
        // Solo establecer vx a 0 si hay colisión completa
        if (possibleMovement >= 0) {
            dynamicAct->vx = 0;
        } else {
            dynamicAct->vx = originalVx; // Mantener velocidad original
        }
    }
}

void Space::updateMoveTop(Actor* dynamicAct) {
    if (dynamicAct->vy < 0) {
        float originalVy = dynamicAct->vy; // Guardar velocidad original
        float possibleMovement = dynamicAct->vy;
        // El mejor "idealmente" vy partimos de ese

        for (auto const& staticAct : staticActors) {
            float topDynamic = dynamicAct->y - dynamicAct->height / 2;
            float downDynamic = dynamicAct->y + dynamicAct->height / 2;
            float rightDynamic = dynamicAct->x + dynamicAct->width / 2;
            float leftDynamic = dynamicAct->x - dynamicAct->width / 2;

            float topStatic = staticAct->y - staticAct->height / 2;
            float downStatic = staticAct->y + staticAct->height / 2;
            float rightStatic = staticAct->x + staticAct->width / 2;
            float leftStatic = staticAct->x - staticAct->width / 2;

            // Alerta!, Elemento estático en la trayectoria.
            if ((topDynamic + dynamicAct->vy) <= downStatic
                && downDynamic > topStatic
                && leftDynamic < rightStatic
                && rightDynamic > leftStatic) {

                // Comprobamos si la distancia al estático es menor
                // que nuestro movimientoPosible actual
                if (possibleMovement <= downStatic - topDynamic) {
                    // La distancia es MENOR que nuestro movimiento posible
                    // Tenemos que actualizar el movimiento posible a uno menor
                    possibleMovement = downStatic - topDynamic;
                }
            }
        }
        // Ya se han comprobado todos los estáticos
        dynamicAct->y = dynamicAct->y + possibleMovement;
        // ARREGLO: No sobrescribir vy para mantener el comportamiento de IA
        // Solo establecer vy a 0 si hay colisión completa
        if (possibleMovement >= 0) {
            dynamicAct->vy = 0;
        } else {
            dynamicAct->vy = originalVy; // Mantener velocidad original
        }
    }
}




void Space::updateMoveDown(Actor* dynamicAct) {
    if (dynamicAct->vy > 0) {
        float originalVy = dynamicAct->vy; // Guardar velocidad original
        float possibleMovement = dynamicAct->vy;
        // El mejor "idealmente" vy partimos de ese

        for (auto const& staticAct : staticActors) {
            float topDynamic = dynamicAct->y - dynamicAct->height / 2;
            float downDynamic = dynamicAct->y + dynamicAct->height / 2;
            float rightDynamic = dynamicAct->x + dynamicAct->width / 2;
            float leftDynamic = dynamicAct->x - dynamicAct->width / 2;

            float topStatic = staticAct->y - staticAct->height / 2;
            float downStatic = staticAct->y + staticAct->height / 2;
            float rightStatic = staticAct->x + staticAct->width / 2;
            float leftStatic = staticAct->x - staticAct->width / 2;

            // Alerta!, Elemento estático en la trayectoria.
            if ((downDynamic + dynamicAct->vy) >= topStatic
                && topDynamic < downStatic
                && leftDynamic < rightStatic
                && rightDynamic > leftStatic) {

                // Comprobamos si la distancia al estático es menor
                // que nuestro movimientoPosible actual
                if (possibleMovement >= topStatic - downDynamic) {
                    // La distancia es MENOR que nuestro movimiento posible
                    // Tenemos que actualizar el movimiento posible a uno menor
                    possibleMovement = topStatic - downDynamic;
                }
            }
        }
        // Ya se han comprobado todos los estáticos
        dynamicAct->y = dynamicAct->y + possibleMovement;
        // ARREGLO: No sobrescribir vy para mantener el comportamiento de IA
        // Solo establecer vy a 0 si hay colisión completa
        if (possibleMovement <= 0) {
            dynamicAct->vy = 0;
        } else {
            dynamicAct->vy = originalVy; // Mantener velocidad original
        }
    }
}


