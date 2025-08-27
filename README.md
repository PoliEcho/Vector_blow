# Vector blow
simple side scrolling sci-fi arcade game inspired by Tempest, Defender(1981) and others

## How to play  
> press SPACE to start  
> than SPACE to shoot
> on the end screen wait for few seconds and than press SPACE to play again or Q to quit (your highscore is saved to file automaticly)  


## how to get the game 
either go to release page of this repo TODO PUT LINK HERE and download the game or build it see instructions below  
you will still need to install folowing dependencies:  
> SDL3  
> SDL3_image  



### how to build  
#### you will need:
> gcc (g++) (C++23 compatable)  
> make  
> SDL3  
> SDL3_image  

#### to compile
run:
```bash
make -j
```

#### to cross compile
#### you will need:
> x86_64-w64-mingw32  
> other dependencies mxe will tell you to install  
```bash
git submodule --init
cd mxe
make MXE_TARGETS='x86_64-w64-mingw32.static' sdl3 sdl3_image
```