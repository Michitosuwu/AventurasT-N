#ifndef __ENEMYWOLF_H__
#define __ENEMYWOLF_H__

#include "Entity.h"
#include "Point.h"
#include "Physics.h"
#include "DynArray.h"
#include "SDL/include/SDL.h"

struct SDL_Texture; 

class EnemyWolf : public Entity
{
	public:

		EnemyWolf();

		virtual ~EnemyWolf();

		bool Awake();

		bool Start();

		bool Update(float dt);

		bool CleanUp();

		//// Define OnCollision function for the player. 
		void OnCollision(PhysBody * physA, PhysBody * physB);

		//// Add an enemy into the queue to be spawned later 
		//// TODO : POSIBILIDAD SI DE CARA A FUTURO NECESITAMOS CONTROLAR LOS ENEMIGOS DEL JUEGO (CUANDO HAYA MUCHOS)
		////bool AddEnemy(Enemy_Type type, int x, int y);

		////funcion de teleport para el debug
		bool TeleportTo();

		////setear posicion
		//void SetPosition(int x, int y);

		//// Return position X
		//int GetPositionX();

		//// Return position Y
		//int GetPositionY();

public:

	//Declare enemy parameters
	SDL_Texture* texture = NULL;

	// Add physics to the enemy - declare a Physics body
	float speed = 3.0f;
	PhysBody* pbody;
	
	pugi::xml_node config;
	uint texW, texH;

	iPoint origin, destiny;

	DynArray<iPoint> lastPath;

	bool alive = true;

private:
	// All spawned enemies in the scene
	// TODO : POSIBILIDAD SI DE CARA A FUTURO NECESITAMOS CONTROLAR LOS ENEMIGOS DEL JUEGO (CUANDO HAYA MUCHOS)
	//EnemyWolf* enemies[MAX_ENEMIES] = { nullptr };
};

#endif // __ENEMYWOLF_H__