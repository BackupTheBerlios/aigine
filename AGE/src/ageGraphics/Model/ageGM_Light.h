// ageLight.h: Schnittstelle für die Klasse ageLight.
//
//////////////////////////////////////////////////////////////////////

#ifndef AGEGM_LIGHT_H

#define AGEGM_LIGHT_H	



#include "ageGM_LookAtObjects.h"
#include "ageGraphics/View/ageGV_GraphicPipeline.h"
#include "ageGM_Translation3D.h"
#include "ageGM_LookAtObjects.h"
class ageGM_Vector3;

class ageGC_Object;


/**
*	@description 
*	@author Tobias Harpering alias mr_nice@berlios.de
*	@date 21-01-04
*
*/

class ageGM_Light : public ageGM_LookAtObjects {


private: 


    //GraphicPipeline * lnkGraphicPipeline;

    /**
         *                                        Definition der Lichteigenschaften
         * appearance stellt einen Vector mit 4 floats bereit, der angibt, wie das Licht auf den einzelnen Oberflächen erscheinen
         * soll. Zur Auswahl stehen: - GL_SPECULAR - GL_SHININESS - GL_DIFFUSE - GL_AMBIENT
         * - GL_POSITION  - die position des actuellen lichtes Jeder dieser erscheinungsarten können in openGl gesetzt werden.
         * (die position ist natuerlich unerlässlich) der konkrete Gl aufruf für das licht (welches bei uns dann warscheinlich in
         * der GraphicPipline landen wird, lautet: - glLightfv(GL_LIGHT, GL_POSITION, position); - das jetzt nur als Bsp.
         * und dann natülich das ganze auch scharf schalten. - glEnable(GL_LIGHTING); - glEnable(GL_LIGHT);
         */    
	float * diffuse;
    float * specular;
    float * ambient;
   
	/**
    *   der Radius des Viewports wird dann warscheinlich im Viewport gespeichert
    *  oder von Viewprot abgeleitet*/
    float cone;

	/**Diese Variable ist der letzte ganzzahlige wert für die lichtposition. ergibt an ob
		das licht 'directional (0)' oder 'positional (1)' ist*/
	int pos_direct;

	/***/
	ageGV_GraphicPipeline * lnkPipeline;

public:

    //Konstrucktor
    ageGM_Light(ageGV_GraphicPipeline * lnkPipe);

    /**
     *Konstructor
     * @param Gewünschte Position, und die Blickrichtung in Vectoren
     */
    ageGM_Light(ageGV_GraphicPipeline * lnkPipe,ageGM_Translation3D * position, ageGM_Vector3 * lookAt);

    /**
    * Virtuelle Methode muss ueberschrieben werden. Hier wird dann das Licht
    * mit seinen Atributen ( one vorherige Eingabe mit Standartwerten) gesetzt
    */
    void set();
	
	/**Diese Variable ist der letzte ganzzahlige wert für die lichtposition. ergibt an ob
		das licht 'directional (0)' oder 'positional (1)' ist*/
	int getPos_direct();

	/**Diese Variable ist der letzte ganzzahlige wert für die lichtposition. ergibt an ob
		das licht 'directional (0)' oder 'positional (1)' ist*/
	void setPos_direct(int art);
	
	/**Licht-Art:die defuse beleuchtung auf ein object mit bestimmter (zu uebergebener) position*/
	float * getDiffuse();
	
	/**Licht-Art:die defuse beleuchtung auf ein object mit bestimmter (zu uebergebener) position*/
    void setDiffuse(float * value);
	
	/**
    *  der Radius des Viewports wird dann warscheinlich im Viewport gespeichert
    *  oder von Viewprot abgeleitet*/
    float getCone();
	/**
    *  der Radius des Viewports wird dann warscheinlich im Viewport gespeichert
    *  oder von Viewprot abgeleitet*/
    void setCone(float cone);
	
	/**Licht-Art:der hellste wiederschein auf einer glenzenden oberfläche*/
    float * getSpecular();
	
	/**
	* @description Licht-Art:der hellste wiederschein auf einer glenzenden oberfläche*/
    void setSpecular(float * specular);
	
	/**
	* @description Licht-Art: beleuchtet alle objekte aus keiner bestimmten richtung*/
    float * getAmbient();
	
	/**
	*@description Licht-Art: beleuchtet alle objekte aus keiner bestimmten richtung*/
    void setAmbient(float * ambient);

   

    /**
	*@description display ueberschrieben um dem standart zu entsprechen
	*
	* @param die GraphicPipeline um auf eine ihrer Funktionen zugreifen zu können. vieleicht ist es besser
	* datte wo anders zu machen
	*
	* @param index ist der index des lichtes. wichtig da nur 8 lichter in openGL zur Verfügung stehen!!
	*		0 - wird bereitz in der GraphicPipeline gesetzt!!!
	*
	* @param kind [noch nicht implementiert ] ist die art des lichtes also spot o.ä. 
	*/
	void display(int index,string kind);
	/**Standart Licht wird gesetzt*/
	void display();

    virtual ~ageGM_Light();

    

    

};

#endif // AGELIGHT_H
