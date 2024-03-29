#ifndef __ENEMYWOLF_H__
#define __ENEMYWOLF_H__

#include "Entity.h"
#include "Point.h"
#include "DynArray.h"
#include "Animation.h"
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
		void Teleport(int x, int y);

		//create getters and setters
		int GetPositionX() const;
		int GetPositionY() const;
		bool GetAlive() const;
		int GetId() const;

		void SetPositionX(int x);
		void SetPositionY(int y);
		void SetAlive(bool alive);
		void SetId(int id);

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

	int id;

	bool alive = true;

	SDL_Texture* tileTex = nullptr;

	//Audio fx
	int hitFxId;
	//Declaro animaciones
	Animation wolfIdle;
	Animation wolfWalk;
	Animation wolfDeath;

	Animation* currentAnim;

private:
	// All spawned enemies in the scene
	// TODO : POSIBILIDAD SI DE CARA A FUTURO NECESITAMOS CONTROLAR LOS ENEMIGOS DEL JUEGO (CUANDO HAYA MUCHOS)
	//EnemyWolf* enemies[MAX_ENEMIES] = { nullptr };
};

#endif // __ENEMYWOLF_H__