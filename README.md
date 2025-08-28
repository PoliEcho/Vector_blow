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
> [SDL3](https://github.com/libsdl-org/SDL/releases/tag/release-3.2.4)  
> [SDL3_image](https://github.com/libsdl-org/SDL_image/releases/tag/release-3.2.4)  
<details>
  <summary>How to install SDL3</summary>
    <h3>Arch Linux</h3>
<pre><code class="language-bash"># use your favorite aur helper
yay -S sdl3 sdl3_image
</code></pre>

<h3>Gentoo Linux</h3>
<pre><code class="language-bash">emerge --ask media-libs/libsdl3 media-libs/sdl3-image</code></pre>

<h3>Ubuntu Linux >=25.04 Plucky or Debian Linux >=13 Trixie</h3>
<pre><code class="language-bash">apt install libsdl3-dev libsdl3-image-dev</code></pre>

<h3>Fedora Linux</h3>
<pre><code class="language-bash">dnf install SDL3 SDL3_image</code></pre>

<h3>SDL3 not in package manager Linux</h3>
<pre><code class="language-bash">wget https://github.com/libsdl-org/SDL/releases/download/release-3.2.20/SDL3-3.2.20.zip
wget https://github.com/libsdl-org/SDL_image/releases/download/release-3.2.4/SDL3_image-3.2.4.zip
unzip SDL3-3.2.20.zip
cd SDL3-3.2.20
cmake -S . -B build
cmake --build build
sudo cmake --install build --prefix /usr
unzip SDL3_image-3.2.4.zip
cd SDL3_image-3.2.4
cmake -S . -B build
cmake --build build
sudo cmake --install build --prefix /usr
</code></pre>
</details>  

#### to compile
##### this should work on any Unix-like system as well as WSL  
run:
```bash
make -j
```
#### to run  
run:
```bash
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
