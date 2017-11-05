MatPlot
=======

a Visual C++ library that allows you to plot just as in Matlab.

Click [here](http://littleshi.cn/blog/MatPlot_Use/ "littleshi.com") for the Chinese page of this project. The library can be used in a Console program as well as any other Visual C++ program. To use MatPlot, you just need to include `MatPlot.h`, set the linker to input `MatPlot.lib` and copy `MatPlot.dll` to your project folder. For a detailed tutorial on how to do this in Visual Studio 2017, please read the wiki.

User's Guide
--------------

### 1. The First Example
![ex1](img/ex1_plot.png)

```
#include <Windows.h>
#include <cmath>
#include "MatPlot.h"

using namespace MatPlot;
int main()
{
	double x[5]{ 1,2,3,4,5 };
	double y[5]{ 1,3,2,4,3 };
	int N{ 5 };
	MatPlotInit();
	plot(x, y, N);
	Sleep(60000);
	MatPlotClose();
	return 0;
}
```
Note that every function in MatPlot has namespace `MatPlot`. Use `MatPlotInit()` and `MatPlotClose()` to initialize and close MatPlot. They can be used only once in a program, every MatPlot function must be invoked between `MatPlotInit()` and `MatPlotClose()`. Use `Sleep()` function so that the figure does not close immediately.

`plot()` function's prototype is<br>
`plot(double *px, double *py, int N, char color = 'a');`<br>
where `px` and `py` could be array names or pointers, `N` is the number of elements in `px` and `py`, `color` is a single character indicating the color. The default value `a` means automatic, which means the color will change every time `plot()` or `scatter()` is used. Like in Matlab, the possible `color` values can be 'r'(red), 'g'(green), 'b'(blue), 'y'(yellow), 'm'(magenta), 'c'(cyan), 'k'(black), 'w'(white). `plot()` always operate on the current figure, if there is none opened yet, figure1 will be created.

`scatter()` function is used to scatter '+' shaped data points on a figure, its prototype is the same as `figure()`, you can add the following line under `plot(...);` in the above example. <br>
`scatter(x, y, N);` <br>

![ex1](img/ex1_mod.png)

Note that the color has become orange now.

`figure()` function is used to create new figure dialog, the prototype is<br>
`figure(int FigNo = -1);`<br>
In the default case, `figure()` will number the dialogs starting from one. If there is a positive `FigNo` input, figure dialog with number (FigNo + 1) will be generated (since C++ is 0 based). `figure()` can also be used to specify the current figure for functions such as `plot()` and `scatter()` to work on.

`close()` function is used to close a figure, the prototype is<br>
`close(int FigNo = -1);`<br>
In the default case, it will close the current figure. When used with an argument, it will close the (FigNo + 1)th figure. In both cases, if the current figure is closed, the current figure will become the next opened figure with a larger number, if there is none, an opened figure with a smaller number is used. Closing a figure by clicking X button is the same as using a `close()` function.






```
#include <Windows.h>
#include <cmath>
#include "MatPlot.h"

using namespace MatPlot;
int main()
{
	const int N{ 100 };
	const double pi{ 3.1415926 };
	double x[N]{};
	double y[N]{};
	for (int i{}; i < N; i++)
	{
		x[i] = 2 * pi / (N - 1) * i;
		y[i] = sin(x[i]);
	}

	MatPlotInit();
	plot(x, y, N);
	xticks(0, 6.2, 32, 0, 5, 1);
	Sleep(100000);
	MatPlotClose();
	return 0;
}
```


The code above will plot __x__ against __t__ with default labels for both axes.
The function call `.exec()` must be the last command and object initialization `eggp::Eggplot objectName` must be the first. 
The orders of all the other setup and plot commands can be arbitrary.
See function `example1` in `src/main.cpp`.


### 2. Multiple curves with legends
![ex2](img/eggp-export-ex2.png)

Include the header file `eggplot.h` and assume __x1__, __x2__, and __t__ are `vector<double>` of the same length:

```
eggp::Eggplot curvePlot; 

curvePlot.xlabel("x");
curvePlot.ylabel("pdf_{{/Symbol m},{/Symbol s}} (x)");  // enhanced texts

curvePlot.plot({ t,x1, t,x2 });  // plot two curves by cascading data pairs

curvePlot.legend({"{/Symbol l}=1",
                  "{/Symbol l}=4"});
curvePlot.grid(true);            // turn on grids

curvePlot.exec();
```

Eggplot can plot more than one curve simultaneously on the same plot by cascading data pairs just as in MATLAB. 
Legends can be set up in a similar manner.

Texts can be formatted with subscripts (`"_"`), superscript (`"^"`), and breaklines (`"\\n"`).
Greek letters and other symbols are represented in different syntaxes from Latex. 
[See here](http://mathewpeet.org/lists/symbols/) for a list of available symbols.
See function `example2` in `src/main.cpp`.


### 3. Curve customization
![ex3](img/eggp-export-ex3.png)

The assumptions are the same as the previous example:

```
eggp::Eggplot curvePlot;

curvePlot.plot({ t,x1, t,x2 });

using namespace eggp;

// multiple property setup in one statement
curvePlot.linespec(1, {{MarkerSize, "0.5"}, {Marker, "*"}});

// single property setup in multiple statements
curvePlot.linespec(2, Color, "b");
curvePlot.linespec(2, LineWidth, 2);
curvePlot.linespec(2, Marker, "none");

curvePlot.exec();
```

Member function `.linespec()` customizes the line with the specified index. 
Two propertypes are implemented: the first one utilizes `std::initializer_list` and the 2nd one sets up a single property.

Customizable properties  include `LineStyle`, `LineWidth`, `Marker`, `MarkerSize`, and `Color`, each of which is defined within the namespace `eggp`.
A list of options can be referred in the API section.
See function `example3` in `src/main.cpp`.


### 4. File export

Eggplot can export plots to various image types, including `.png`, `.eps`, `.pdf`, `.html`, and `.svg`.

```
eggp::Eggplot curvePlot(SCREEN|PNG|EPS|PDF|HTML|SVG);

// set export file name, default: "eggp-export"
curvePlot.print("eggp-test");

curvePlot.plot({ t,x1, t,x2 });
curvePlot.exec();
```

The output files will then be `eggp-export.png`, `eggp-export.eps`, and so on. 
Note that both EPS and PDF modes also support LaTeX-formatted texts.
See function `example4` in `src/main.cpp`.


API
---

### class eggp::Eggplot

#### Constructor

+ **```Eggplot(unsigned mode=eggp::SCREEN)```** initializes object and sets up where to plot. 
The default is `eggp::SCREEN` to plot on screen. 
Other output modes include `eggp::PNG`, `eggp::EPS`, `eggp::PDF`, `eggp::HTML`, and `eggp::SVG` that plot in `.png`, `.eps`, `.pdf`, `.html`, and `.svg` files, respectively.

#### Member functions

#####_Text Related_

+ **```void xlabel(const std::string &label)```** sets up x axis label

+ **```void ylabel(const std::string &label)```** sets up y axis label

+ **```void title(const std::string &label)```** sets up figure title

+ **```void legend(std::initializer_list<std::string> legendVec)```** sets up figure legeneds. If the length of `legendVec` is shorter than the number of curves, default legends will be used for those whose legends are not specified.

#####_Line Property Related_

+ **```void linespec(unsigned lineIndex, LineSpecInput lineSpec)```** customizes curves with the index `lineIndex`, starting from 1. `LineSpecInput` is a `std::map` that takes different `LineProperty` (`eggp::LineStyle`, `eggp::LineWidth`, `eggp::Marker`, `eggp::MarkerSize`, `eggp::Color`) as the key and a string as the value. In practice, written the curve setups in an initializer list is useful as in Example 3. Color can be specified in five ways: color name, color name shortcut, hex code, decimal code, and rgb values between 0 and 1. For example, for red, "r", "red", "#ff0000", "(255,0,0)", and "[1.0, 0, 0]" are equivalent.


+ **```void linespec(unsigned lineIndex, LineProperty property, std::string value)```** customizes a single curve property. All options are the same as the previous one.


+ **```void linespec(unsigned lineIndex, LineProperty property, double value)```** customizes a single curve property with a `double` value. Same as the previous one except for the `double` data type. Only valid for `eggp::LineWidth` and `eggp::MarkerSize`.


+ **```void grid(bool flag)```** turns on or off grids of the plot

#####_Output Related_

+ **```void plot(std::initializer_list<DataVector> il)```** saves data in file `eggp.dat`. The argument must be paired (even-numbered vectors in `il`) such that each pair (the (2N-1)-th and (2N)-th vectors , N=1,2,...) has the same length. This command does not plot but only store data in hard drives. The actual plots and exports happen at function `.exec()`.

+ **```void print(const std::string &filenameExport)```** sets up export file name, or the default file name `eggp-export` will be used, otherwise. Again, this command does not really print to files but only set up the file name. The actual print and export processes happen at function `.exec()`.
 
+ **```void exec(bool run_gnuplot=true)```** executes everything. All previous functions only set up and store necessary information for plotting and export to a file. This function instead generates an actual input file `eggp.gp` for _gnuplot_ and makes a system call `gnuplot eggp.gp` in a terminal if `run_gnuplot` is true. This function must be the last command before generating plots to make settings effective.


Future features
---------------

+ xtic and ytic setup
+ axis range customization
+ Other types of plots are considered also in the future.





