# Real-Time 2D Fluid Simulation
A real-time, interactive 2D fluid simulation implemented in **C++ with OpenGL**, based on the **"Stable Fluids"** method by [Jos Stam (1999)](https://www.dgp.toronto.edu/public_user/stam/reality/Research/pdf/ns.pdf).  
This project showcases how physical models can be used to simulate incompressible fluid dynamics on a grid, with visualization and user interaction.

## Preview

<p align="center">
  <img src="https://i.ibb.co/FqL8km7J/fluids-3-OFeg3n-KHK.gif" width="400px">
</p>



## Features

- Add density and momentum with the mouse  
- Moving the window applies global velocity to the fluid  
- Real-time numerical integration and visualization  
- Based on a stable Navier-Stokes solver


## Dependencies

- [OpenGL](https://www.opengl.org/)  
- [GLUT / freeglut](http://freeglut.sourceforge.net/)  
- Visual Studio + NuGet (auto-restores freeglut)



## How to Build

1. Clone the repository:

```bash
   git clone https://github.com/torresds/fluids.git
````

2. Open `fluids.sln` in **Visual Studio 2022 or later**

3. Build the solution (`Ctrl + Shift + B`)

4. Run the simulation (`F5`)

> Make sure NuGet restores the `freeglut` package. If not, run `Restore NuGet Packages` via Visual Studio.



## Controls

* **Left mouse click & drag**: inject density and velocity
* **Move the window**: shake effect that stirs the fluid

## Reference

> Stam, Jos. (1999).
> *Stable Fluids*. SIGGRAPH.
> \[[Read the paper](https://www.dgp.toronto.edu/public_user/stam/reality/Research/pdf/ns.pdf)]



