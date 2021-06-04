# Epicycles
You can learn more about it [here](https://efyang.dev/media/documents/efyang_KAM_Paper_Approximating_Images_with_Epicycles.pdf) page.

## Materials that helped
* [3blue1brown](https://www.youtube.com/watch?v=spUNpyF58BY) - in FT concept
* [TheCodingTrain](https://www.youtube.com/watch?v=7_vKzcgpfvU&t=1298s) - in Coding
* [Mathologer](https://www.youtube.com/watch?v=qS4H6PEcCCA) - in Epicycles
* [Mathematica](https://mathematica.stackexchange.com/questions/171755/how-can-i-draw-a-homer-with-epicycloids) - in Overall

## Snapshot
![alt tag](https://github.com/AchyutBurlakoti/epicycles/blob/master/epicycles.png)

## Things to be done
* ~Optimization to render more without lagging.~
* Add function to extract data from image directly to eliminate hard coding of image data.

## Building Process
# Cmake
* Go inside main dir (.../epicycles)
* Delete build folder
* Open terminal
```
mkdir build
cd build
cmake -G "Visual Studio 16 2019" ..
```
* Go inside build folder
* Run vs soln (epicycles.sln)
* Build the project in vs (Build ALL_BUILD)
* Run ..epicycles/build/DEBUG/epicycles.exe to see output


# Makefile
* open cmd inside epicycles dir
* run make
