3dschrome:


das ding liest und zeigt eine 3ds obj mit sammt all seinen texturen an und 
hat noch ein kleines feature 

und zwar einen getrixten chrome shader (welcher sehr abhängig von der textur ist mit der dieser gestarten wird)

's' 	on/off
'w'	wired
Pfeiltasten 	navigieren vor zurück und slide rechts und links
es gibt noch mehr...

es liegen in dem verzeichnis noch einige beispiele von dem Author mit verschiedenen Texturen und chrome shading
(Zeile 55-65 in der testloader einfach das jeweilige objekt reinnehmen und die 
dateien:	zusaetzlicheTexturen.rar 	zusaetzlicheObjekte.rar	entpacken...)

Abhängigkeiten:
----------------------------------------------------
gl\glut.h
gl\glaux.h

tgaLoader.h
tgaLoader.cpp

3dsObject.h
3dsObject.cpp

3dsLoader.h
3dsLoader.cpp

3dsTypes.h
----------------------------------------------------
Algemein zum Loader:

libary:

	#include "3dsObject.h"
----------------------------------------------------
struct Name:

	Object3DS my3DSObject;

----------------------------------------------------
laden:

my3DSObject.loadObject("relativerPfad.3DS");

----------------------------------------------------
rendern:	//er übernimmt den in 3dstudio gesetzten PivotPoint als Bezugspunkt
		//Die koordinaten sollten auf generic gesetzt sein (default wert bei Max)


----------------------------------------------------







