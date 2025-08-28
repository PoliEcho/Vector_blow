# Vector blow
[![Build on macOS](https://github.com/PoliEcho/Vector_blow/actions/workflows/build_macos.yml/badge.svg)](https://github.com/PoliEcho/Vector_blow/actions/workflows/build_macos.yml)  
simple side scrolling sci-fi arcade game inspired by Tempest, Defender(1981) and others

## How to play  
> press SPACE to start  
> than SPACE to shoot but caution you weapon may overheat  
> WASD or ARROW to move  
> you health is showed on top by red bar  
> on the end screen wait for few seconds and than press SPACE to play again or Q to quit (your highscore is saved to file automaticly)  


## how to get the game 
either go to release page of this repo [HERE](https://github.com/PoliEcho/Vector_blow/releases) and download the game or build it see instructions below  


### how to build  
#### you will need:
> gcc (g++) (C++23 compatable)
> pkg-config  
> make  
> CMake  

#### to compile
##### this should work on any Unix-like system as well as WSL  
run:
```bash
mkdir build
cd build
cmake ..
make -j
cd ..
```
#### to run  
run:
```bash
# has to be run from project root
./build/bin/Vector_blow
```
## Gameplay  

### shooting  
press SPACE to shoot
while you shoot overheat meter in bottom left starts to fill up
if it fills up you have to wait to reach half before you can shoot again
</br></br></br>

> [!NOTE]
> enemies and powerups are sorted in order of likeliness of apperence so for example you will see RANDOM ememy 3 times more likely than ACE, same with power ups

### enemies 
> [!NOTE]
> enemies give increasing number of points based on there size and type

<img src="assets/enemy_1.svg" alt="enemy sprite" width=200 /></br>
| type | behavior |
|------|----------|
| RANDOM | randomly moves and fires forward speed is 2x in comparison to other directions |
| FLYER | on average faster firerate, moves up and down on screen and shoot providing cover fire for other units |
| GUNNER | this enemy will lock on you ships signature and always stays drectly in front of the player and randomly fire |
| ACE | this enemy will always fire if it sees the player in front of it, also it will attempt to doge player's shots, if not evading player's projectiles, it will attempt to get into fire position |
| BOSS | spawns on every multiple of 5000 points e.g. 5000,10000... Likes to wait until all other ships in the area are destroyed then attacks once he reaches his fireing position, he start using his deflector, but there is hope enemy engineers cheaped out on power supply for the deflector so the pulse itegnity is compromised maybe you could use this to survive his attacks, boss health is allways increased e.g. 1000, 2000, 3000, .... |

### power ups  

> [!NOTE]
> powerups generaly last 15s

| visual | type | efect |
|--------|------|-------|
| ![2x](assets/powerups/2x.svg) | 2x | increase score multiplier by 2 |
| ![boom](assets/powerups/boom.svg) | boom | multiply projectile damage by 3 |
| ![3x](assets/powerups/3x.svg) | 3x | increase score multiplier by 3 |
| ![health](assets/powerups/health.svg) | health | restores half of health |
| ![beam](assets/powerups/beam.svg) | beam | devides fire cooldown by 5 and disables weapon overheat |
| ![5x](assets/powerups/5x.svg) | 5x | increase score multiplier by 5 |
