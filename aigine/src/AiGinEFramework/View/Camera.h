/* Game Engine Design */

//####################################################################
//                       C A M E R A . H                        	   #
//                                                                   #
//___________________________________________________________________#
//                                                                   #
// @author Danny Graef, Tobias Harpering                             #
// @description: Camara control implementation                       # 
// @date                                                             #  
//####################################################################

#ifndef CAMERA_H

#define CAMERA_H

#include <vector>
//#include "../Input/Mouse.h"
#include "../Model/Point3D.h"
#include "../Model/Vector3D.h"


using namespace std;

class Viewport;



/** 
* Kamerasteuerung
* 
* @author Danny Graef, Tobias Harpering       
* 
*/

class Camera
{


public:
	
	/**      
  * Gibt die Position der actuellen Kammera zurueck  
  *        
  * @author Danny Graef, Tobias Harpering
  * @return Vector 3D
  * @param -
  */ 	
	Point3D* getPosition();

	/**      
	* Gibt den Vector der actuellen Blickrichtung zurueck  
	*        
	* @author Danny Graef, Tobias Harpering
	* @return Richtungsvektor
	* @param -
	*/       
	Point3D * getLookAtPosition();
    
	/**      
	* Setzt die Blickrichtung der Kammera        
	*        
	* @author Danny Graef, Tobias Harpering
	* @return -
	* @param Uebernimmt einen 3d Vektor
	*/       
	void setLookAtPosition(Point3D * lookAtPosition);
    
	/**      
	* Gibt den Normalvektor der Kammera zurueck       
	*        
	* @author Danny Graef, Tobias Harpering
	* @return 3D (Up)Vector
	* @param -
	*/       
	Point3D * getRotation();
	
	/*
	 * Die Funktion berechnet den neuen Rotationsvektor der Kamera
   * einen Vektor und
	 * verwendet ihn in normalisierter Form als Kameravektor.
   * @author Danny Graef, Tobias Harpering
   * @return -
   * @param Vektor 
	*/
	void setRotation(Point3D * rotation, int mouseX, int mouseY, int* CurrentWinSize);

	void RotateView(float angle, float x, float y, float z);

    /**      
    *        
	*        
	* @author
	* @return
	* @param 
	*/       
    Camera();
    
    /**      
    *        
    *        
    * @author
    * @return
    * @param 
    */       
    Camera(Point3D* position, Point3D* lookAtPosiont, Point3D* normalVector);

    /**      
    *        
    *        
    * @author
    * @return
    * @param 
    */           
    void attachViewport(Viewport * param);
	
    /**      
    *        
    *        
    * @author
    * @return
    * @param 
    */       
    virtual void set();

//////////////////////////M O U S E   M O V E M E N T///////////////////////////
    /**      
    *        
    *        
    * @author
    * @return
    * @param 
    */       
    void turnDown();

    /**      
    *        
    *        
    * @author
    * @return
    * @param 
    */       
    void turnRight();

      /**      
	*        
	*        
	* @author
	* @return
	* @param 
	*/
	void turnUp();

	/**      
	*        
	*        
	* @author
	* @return
	* @param speet die Geschwindigkeit in float mit der die Kammera sich
  * bewegen soll
	*/
	void turnLeft();

/////////////////////////K E Y B O A R D   M O V E M E N T//////////////////////
  
  /**      
    *        
    *        
    * @author
    * @return
    * @param speet die Geschwindigkeit in float mit der die Kammera sich
  * bewegen soll
    */       
    void moveDown(float speet);

    /**      
    *        
    *        
    * @author
    * @return
    * @param speet die Geschwindigkeit in float mit der die Kammera sich
  * bewegen soll 
    */           
	void moveForward(float speet);


  	/**
   	* bewegt die Kamera um die angegebene
   	* Anzahl nach hinten.
   	* Viewpoint und LookAt werden entweder
   	* auf der derzeitigen Ebene "direction = 0"
   	* oder in Richtung der Kamera "direction = 1"
   	* bewegt
   	*/
	void moveBack(float speet);

  
  
  	/**      
	*        
	*        
	* @author
	* @return
	* @param speet die Geschwindigkeit in float mit der die Kammera sich
  * bewegen soll
	*/

  void moveUp(float speet);

  	/**      
	*        
	*        
	* @author
	* @return
	* @param speet die Geschwindigkeit in float mit der die Kammera sich
  * bewegen soll
	*/
  void moveLeft(float speet);


  	/**      
	*        
	*        
	* @author
	* @return
	* @param speet die Geschwindigkeit in float mit der die Kammera sich
  * bewegen soll 
	*/
  void moveRight(float speet);

  
  
  
  /*
  * Destructor
  */
  ~Camera();

  /////////////////////////////////P R I V A T E///////////////////////////////

private:
  
	Point3D * rotation; // 
	Point3D * lookAtPosition; 
    Point3D * position;

    /**
     * @supplierCardinality 1
     * @clientCardinality 1
     */
    Viewport * lnkViewport;
  
  /**      
	*        
	*        
	* @author
	* @return
	* @param 
	*/

	Point3D cross(Point3D * vVector1, Point3D * vVector2);
	
  	/**      
	*        
	*        
	* @author
	* @return
	* @param 
	*/
  Point3D getNormalVector(Point3D vVector);

  /**      
	*        
	*        
	* @author
	* @return
	* @param 
	*/
  float magnitude(Point3D vNormal);

  /**      
	* Bewegt die Kammera auf der einer Vorgegebenen hoehe in der 3D Welt       
	*        
	* @author 
	* @return -
	* @param speet die Geschwindigkeit in float mit der die Kammera sich
  * bewegen soll
	*/
  void moveCamera(float speed);
  
  /**      
	* Bewegt die Kammera seitwerts (sonst das selbe wie 'moveCamera')       
	*        
	* @author
	* @return
	* @param speet die Geschwindigkeit in float mit der die Kammera sich
  * bewegen soll
	*/
  void strafeCamera(float speed);
};

#endif //CAMERA_H