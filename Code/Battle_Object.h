#ifndef Battle_Object_H
#define Battle_Object_H

#include <SFML\Graphics.hpp>
#include "Battle_Methods.h"
#include "Attack.h"
#include "Battle_Animation.h"
#include "System.h"
#include "Graphic.h"

class Battle_Object
{
	public:
		virtual ~Battle_Object() { };

		/**
		*	BATTLE FUNCTIONS
		*/
		void setVelocity(float x, float y, bool isPlayer); // The x and y is the position the object is moving towards
		void setSpritePosition(float x, float y);
		void takeDamage(int damage, short hitStun);
		bool inActiveFrame();
		bool attackInProgress();
		bool inCrushState();
		bool attackIsLauncher();
		bool attackIsMeteorSmash();
		bool pastActiveFrame();
		bool launchSprite(float y, float elapsedTime);
		bool launchSpriteDown(float y, float elapsedTime);
		bool fall(float y, float elapsedTime);
		int getDamage(bool enemyInCrushState);
		short getHitStun(bool enemyInCrushState);
		sf::Vector2i getDimensions();
		sf::Vector2f getPosition();
		sf::Sprite getBattleSprite() { return battleSprite; };

		// Overridable battle functions
		virtual void drawSprite(bool animate) { };
		virtual void initialize() { };
		virtual short performBattleAction(sf::Event lastKeyPressed, short numAttacksPerformed) { return 0;  }; // Player version of battle action
		virtual short performBattleAction(short numAttacksPerformed) { return 0; };

		// Overridable movement functions
		virtual bool moveToPosition(float x, float y, float elapsedTime, bool movingToAttack); // Return true if the object has reached its destination , false otherwise
		virtual bool jumping(float y, float elapsedTime);

		// DEBUG
		virtual void toggleCrushState() { };

	protected:
		// CONSTANT VARIABLES
		const short LAUNCH_VELOCITY = -4000, SMASH_VELOCITY = 4000, FALL_VELOCITY = 2000;
		const float JUMP_VELOCITY = 1200.0f;
		
		// VARIABLES
		float jumpVelocity = 0, gravity = 0, movementVelocity = 0;
		sf::Vector2f battleVelocity = sf::Vector2f(0, 0);
		sf::Sprite battleSprite;
		sf::Clock battleAniClock;
		short battleHeight, battleWidth;
		bool isAttacking = false;
		bool isTakingDamage = false;
		bool isCrushable = false;
		bool isAirBound = false;
		bool decreaseVelocity = false;
		Battle_Animation moving, standing, attackStance, arielStance, returning, takingDamage, constantTakingDamage;
		
		// This structure atempts to facilaitate the process of animating sprite. 
		struct Animator
		{
		public:
			void animate(sf::Sprite* sprite, sf::Clock* clock, bool& isAttacking)
			{
				bool wasAttacking = isAttacking;

				if (isAttacking && currentAttack != NULL)
					isAttacking = currentAttack->animateAttack(sprite, clock, attackChanged);
				else if (currentBattleAnimation != NULL)
					currentBattleAnimation->animate(sprite, clock, animationChanged);
			
				if (animationChanged && !wasAttacking)
					animationChanged = false;
				if (attackChanged)
					attackChanged = false;
			}

			void changeAttackAnimation(Attack* attack)
			{
				attackChanged = true;
				currentAttack = attack;
			}

			void changeBattleAnimation(Battle_Animation* battleAnimation)
			{
				animationChanged = true;
				currentBattleAnimation = battleAnimation;
			}

			Attack* getCurrentAttack()
			{
				return currentAttack;
			}

			Battle_Animation* getCurrentAnimation()
			{
				return currentBattleAnimation;
			}

		private:
			Attack* currentAttack = NULL;
			Battle_Animation* currentBattleAnimation = NULL;
			bool animationChanged = false, attackChanged = false;
		} animator;
};

#endif