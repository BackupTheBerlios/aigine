// ageLight.h: Schnittstelle für die Klasse ageLight.
//
//////////////////////////////////////////////////////////////////////

#ifndef LIGHT_H

#define LIGHT_H	



#include "../Model/LookAtObjects.h"
#include "../View/GraphicPipeline.h"
#include "../Model/Translation3D.h"

class AiGinEObject;

class Light : public LookAtObjects {


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

public:

    //Konstrucktor
    Light();

    /**
     *Konstructor
     * @param Gewünschte Position, und die Blickrichtung in Vectoren
     */
    Light(Translation3D * position, Vector3D * lookAt);

    /**
    * Virtuelle Methode muss ueberschrieben werden. Hier wird dann das Licht
    * mit seinen Atributen ( one vorherige Eingabe mit Standartwerten) gesetzt
    */
    void set();

	int getPos_direct();

	void setPos_direct(int art);
	

	float * getDiffuse();

    void setDiffuse(float * value);

    float getCone();

    void setCone(float cone);

    float * getSpecular();

    void setSpecular(float * specular);

    float * getAmbient();

    void setAmbient(float * ambient);

    virtual ~Light();

    

    

};

#endif // AGELIGHT_H
