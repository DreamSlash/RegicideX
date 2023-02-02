# RegicideX

Hack'n Slash game developed with Unreal 4 as a final master project for Máster en Videojuegos: Diseño, Creación y Programación, offered by Barcelona School of Management.

The project has been developed by 5 programmers and 3 artists, each one participating in game design decisions.

<h4>Programmers:</h4> Àlvar Alonso, Pau Florentí, Sergio Martínez, Pablo Acedo, Ricard Hidalgo

<h4>Artists:</h4> Albert del Arco, Gerard Díaz, Ignacio Navas

## The Game

Is a 3D Hack'n Slash inspired by games like Nier Automata, Devil May Cry, Bayonetta and Hades. It takes place in a dark fantasy world plagued with rotten and evil angels (visually inspired by biblically accurate angels) and possesed people who have invaded the castle of our protagonist. Our protagonist fights in a series of consecutives arenas, each one with multiple waves featuring different enemies, until he reaches the throne and overcomes the invasion. Failure means starting again from the beginning.

The game is fast paced and movility is key, giving importance to dashes, dodges, jumps (and double jumps) and well timing attacks from the attack toolset the player has available. The player also has access to limited powers useful for combat. Each attack or action is suitable for a particular situation or enemy.

The final wave of each arena presents a new angel as a final boss to put the player's skills to test and overcome the arena, being able to advance to the next one.

<p float="left">
  <img src="https://github.com/AlvarAlonso/RegicideX/blob/main/Captures/dodge_laser.gif" alt="dodge_laser" width="49%"/>
  <img src="https://github.com/AlvarAlonso/RegicideX/blob/main/Captures/air_combo.gif" alt="air_combo" width="49%"/>
</p>

## Technical Notes

This project uses Unreal Engine 4. 

### Character Controller

#### Camera Controller

We feature a dynamic camera controller that changes the distance from player dynamically depending on the amount of enemies in the arena or if the player has enemies in the back.

#### Combo System

A combo system is implemented making heavy use of animation montages and notify event states. This combo system reads and saves input while in a combo animation to lead to the next animation at the exact frame is needed. This way, the combat feels responsive and natural, and the player doesn't have a tiny window to input for the next attack in a combo chain.
It's very expansive. There is little to no effort in adding new combos and animations or changing the settings of the currently created ones, like the input needed to follow the chain, the next animation in the chain, the input reading frames, etc...

### Combat Framework

#### GAS
The game has multiple enemies plus the player. To make it easier to implement all the interactions, attacks, skills, etc... we use extensibly the GAS framework that comes with Unreal as a plugin. Using GAS and the animation system, we built a framework for the combat of our game, which is expandable. It would be easy to add new mechanics and enemies into the combat framework, because we can reuse a lot of classes and functions. Our philosohpy was to build a framework that would require little code to add new attacks or actions as long as we got the art, specially the animations. Right now, is very easy to import a new animation, plug it into the combat system linked with a Gameplay Ability, parametrize it and use existing animation events and our payload system to make the Ability behave the way we want.

#### Hitbox System
GAS and animations are fundamental to the combat framework, but the holy trinity is completed with the hitbox system. The hitbox system is the interface between the ability and the game, and is responsible for sending events that will be forwaded to the Gameplay Ability itself which will be the one applying its effects. 
It also manages what happen with projectiles when they hit something.

### AI

#### Slot System

The AI features a slot system to make the combat more organized and less chaotic. Without it, all the enemies would run into the player and make their attacks, giving the player too much pressure and zero windows to take the iniciative.
With our slot system, enemies "take turns" so no more that a specified number of enemies moves to the player vicinity and no more than a specified number are able to attack at the same time. Game is fast, so the subset of enemies that go to the vicinity of the player is changing dynamically. Non activated enemies (vicinity enemies that can't attack right now or farther enemies) can orbit or move around the player, taunt him or switch position with another enemy.
Ranged and melee enemies are managed separately because they are different use cases. Ranged have their own slots that manage how many ranged enemies can be shooting to the player at the same time.


#### Behaviour Trees

We feature behaviour trees with pretty complex behaviour. Enemies whose behaviour changes dynamically (for example depending on the amount of health left), with long attack sequences dependant on context conditions and complex idle behaviours (not just standing by but orbiting, playing some extra animations...). They are also easy to expand.
Taking a look to our behaviour trees is a good way to learn or get ideas to make an AI seems more organic and natural, and less like a state machine or a dice roll where depending on the result the AI does X or Y.

### Data Oriented

We parametrized everything we could so the game is easier to fine tune and modify. 

All player/enemy stats ara parametrized using tables modifiable outside Unreal. 

We have a payload system where Gameplay Effects take data assets as a parameter to control better what we want each effect to do. For example, the launch effect receives a data structure that controls the launch force in all axis, if this launch is absolute or the force comes from an origin point, if it causes knockup or knockback, if it is additive, etc...

AI's slot system is heavily parametrized to make every arena or wave potentially different (enemies acting more or less agressive for example). This parameters could even be change dynamically at runtime.

Camera is also heavily parametrized.

## Gallery
![alt text](https://github.com/AlvarAlonso/RegicideX/blob/main/Captures/letsdance.gif "Let's Dance")
