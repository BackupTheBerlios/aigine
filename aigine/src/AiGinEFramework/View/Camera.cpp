/* Game Engine Design 
 * Camera.cpp  
 */

#include <math.h>

#include "Camera.h"
#include "../Model/Vector3D.h"


//____________________________________________________________________________
Camera::Camera()
{
    this->upVector = new Vector3D(0.0, 1.0, 0.0);
}

//____________________________________________________________________________
Camera::Camera(Vector3D * position, Vector3D * lookAtPosition,
   Vector3D * upVector)
   {
       this->position = position;
       this->lookAtPosition = lookAtPosition;
       this->upVector = upVector;
}

//____________________________________________________________________________
Vector3D * Camera::getPosition()
{
    return this->position;
}

//____________________________________________________________________________
Vector3D * Camera::getLookAtPosition()
{
    return this->lookAtPosition;
}

//____________________________________________________________________________
void Camera::setLookAtPosition(Vector3D * lookAtPosition)
{
    this->lookAtPosition = lookAtPosition;
}

//____________________________________________________________________________
Vector3D * Camera::getUpVector() { return upVector; }

//____________________________________________________________________________
void Camera::setRotation(int mouseX, int mouseY, int * winSize)
{
    // Mittelpunkt des Fensters mit binärem Shift ermitteln
    int middleX = winSize[0] >> 1; // x / 2 = middle x
    int middleY = winSize[1] >> 1;
    // Winkel für oben/unten Achse  0.5 = 180 Grad Drehung
    float angleY = 0.0f;
    // Winkel für rechts/links Achse
    float angleZ = 0.0f;
    static float currentRotX = 0.0f;

    // wenn die Maus im Mittelpunkt ist, hat sich nichts getan => ENDE
    if ((mouseX == middleX) && (mouseY == middleY)) return;

    // setze den Mauszeiger auf den Mittelpunkt des Fensters
    glutWarpPointer(middleX, middleY);

    // Differenz zwischen Mittelpunkt und Mausposition ermitteln und
    // auf eine Wert zwischen 0 und 1 bringen.
    // TODO: Teiler für Steuerung der Mausempfindlichkeit nutzen
    angleY = (float)((middleX - mouseX)) / 1000.0f;
    angleZ = (float)((middleY - mouseY)) / 1000.0f;

    // Zwischenspeichern der z Rotation
    // Here we keep track of the current rotation (for up and down) so that
    // we can restrict the camera from doing a full 360 loop.
    currentRotX -= angleZ;

    // If the current rotation (in radians) is greater than 1.0, we want to cap it.
    if (currentRotX > 1.0f) currentRotX = 1.0f;
    // Check if the rotation is below -1.0, if so we want to make sure it doesn't continue
    else if (currentRotX < -1.0f) currentRotX = -1.0f;
    // Otherwise, we can rotate the view around our position
    else
    {
        // Um den Vektor für die Drehung um die z - Achse zu ermitteln
        // wird das Kreuzprodukt des Kamerarichtungsvektor = Blickpunkt - Position
        // und des Normalenvektors der Kamera gebildet.
        Vector3D temp = * lookAtPosition - * position;
        Vector3D zAxis = Vector3D::cross(& temp, upVector);

        zAxis = Vector3D::getNormalVector(zAxis);

        // Rotation um die beiden Achsen
        rotateView(angleZ, zAxis.x, zAxis.y, zAxis.z);
        rotateView(angleY, 0, 1, 0); // yAxis

        // Ausgabe der aktuellen Positionen
        position->print("\nPosition : ");
        lookAtPosition->print("lookAtPosition : ");
    }
}

//____________________________________________________________________________
void Camera::rotateView(float angle, float x, float y, float z)
{
    Vector3D vNewView = Vector3D(0.0, 0.0, 0.0);
    // Get the view vector (The direction we are facing)	
    Vector3D vView = * lookAtPosition - * position;
    // Calculate the sine and cosine of the angle once
    float cosTheta = (float)cos(angle);
    float sinTheta = (float)sin(angle);

    // Find the new x position for the new rotated point
    vNewView.x = (cosTheta + (1 - cosTheta) * x * x) * vView.x;
    vNewView.x += ((1 - cosTheta) * x * y - z * sinTheta) * vView.y;
    vNewView.x += ((1 - cosTheta) * x * z + y * sinTheta) * vView.z;

    // Find the new y position for the new rotated point
    vNewView.y = ((1 - cosTheta) * x * y + z * sinTheta) * vView.x;
    vNewView.y += (cosTheta + (1 - cosTheta) * y * y) * vView.y;
    vNewView.y += ((1 - cosTheta) * y * z - x * sinTheta) * vView.z;

    // Find the new z position for the new rotated point
    vNewView.z = ((1 - cosTheta) * x * z - y * sinTheta) * vView.x;
    vNewView.z += ((1 - cosTheta) * y * z + x * sinTheta) * vView.y;
    vNewView.z += (cosTheta + (1 - cosTheta) * z * z) * vView.z;

    // Now we just add the newly rotated vector to our position to set
    // our new rotated view of our camera.
    * lookAtPosition = * position + vNewView;
}


//____________________________________________________________________________
void Camera::moveCamera(float speed)
{
    // Get the current view vector (the direction we are looking)
    Vector3D vector = * lookAtPosition - * position;

    // I snuck this change in here!  We now normalize our view vector when
    // moving throughout the world.  This is a MUST that needs to be done.
    // That way you don't move faster than you strafe, since the strafe vector
    // is normalized too.
    vector = Vector3D::getNormalVector(vector);

    // setze Kameraposition 
    position->x = position->x + vector.x * speed;
    position->z = position->z + vector.z * speed;
    // setze Blickpunkt
    lookAtPosition->x = lookAtPosition->x + vector.x * speed; 
    lookAtPosition->z = lookAtPosition->z + vector.z * speed; 
}

//____________________________________________________________________________
void Camera::strafeCamera(float speed)
{
    Vector3D temp = * lookAtPosition - * position;
    // Initialize a variable for the cross product result
    Vector3D strafe = Vector3D::cross(& temp, upVector);

    // Normalize the strafe vector
    strafe = Vector3D::getNormalVector(strafe);

    // Strafing is quite simple if you understand what the cross product is.
    // If you have 2 vectors (say the up vVector and the view vVector) you can
    // use the cross product formula to get a vVector that is 90 degrees from
    // the 2 vectors.
    // For a better explanation on how this works, check out the OpenGL
    // "Normals" tutorial at our site.
    // In our new Update() function, we set the strafing vector (m_vStrafe).  Due
    // to the fact that we need this vector for many things including the strafing
    // movement and camera rotation (up and down), we just calculate it once.
    //
    // Like our MoveCamera() function, we add the strafing vector to our current position
    // and view.  It's as simple as that.  It has already been calculated in Update().

    // Add the strafe vector to our position
    position->x = position->x + strafe.x * speed;
    position->z = position->z + strafe.z * speed;

    // Add the strafe vector to our view
    lookAtPosition->x = lookAtPosition->x + strafe.x * speed;
    lookAtPosition->z = lookAtPosition->z + strafe.z * speed;
}



//____________________________________________________________________________
void Camera::moveRight(float speed)
{
    strafeCamera(speed);
}

//____________________________________________________________________________
void Camera::moveLeft(float speed)
{
    strafeCamera(-speed);
}

//____________________________________________________________________________
void Camera::moveUp(float speed) 
{ 
    position->y = position->y + speed;
}

//____________________________________________________________________________
void Camera::moveDown(float speed)
{
    position->y = position->y - speed;
}

//____________________________________________________________________________
void Camera::moveForward(float speed)
{
    moveCamera(speed);

}

//____________________________________________________________________________
void Camera::moveBack(float speed)
{
    moveCamera(-speed);
}

/**
 * @author
 * @return
 * @param
 */
void Camera::attachViewport(Viewport * param) { }

/**
 * @author
 * @return
 * @param
 */
void Camera::set() { }

/**
 * Destruktor
 * @author
 * @return
 * @param
 */
Camera::~Camera() { }
