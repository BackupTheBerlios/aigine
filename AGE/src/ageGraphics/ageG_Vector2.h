/* Game Engine Design */

#ifndef AGEG_VECTOR2_H
#define AGEG_VECTOR2_H
#include "ageG_Vector2.h"
//class ageGM_Vector3;

/**
 * Vector2D dient dem AiGinE Object bei der verarbeitung von 3d informationen(texturen)
 * @author Tobias Harpering
 * @author Frank Otto 
 */
class ageG_Vector2
{
public:  

    /**
     * Konstruktor, initialisiert alle Koordinaten mit den
     * übergebenen Werten.
     * @param x Koordinate.
     * @param y Koordinate.
     * @param z Koordinate.
     */
    ageG_Vector2(float x, float y  );

    /**
     * Konstruktor, initialisiert alle Koordinaten mit 0.0.
     */
	ageG_Vector2();

    /**
     * Vektoraddition.
     * @param vector Summand
     * @return Summe beider Vektoren.
     */
	ageG_Vector2 operator+(ageG_Vector2 vector);

    /**
     * Vektorsubtraktion
     * @param vector Subtrahend.
     * @return Differenz beider Vektoren.
     */
    ageG_Vector2 operator-(ageG_Vector2 vector);

    /**
     * Multiplikation von Vektor und reeler Zahl.
     * @param num reele Zahl.
     * @return Produkt.
     */
	ageG_Vector2 operator*(float num);

    /**
     * Division von Vektor und reeler Zahl.
     * @param Divisor.
     * @return Quotient.
     */
    ageG_Vector2 operator/(float num);

    /**
       * y Koordinate des Vektors
       */
    float y;

    /**
     * z Koordinate des Vektors
     */
    float x;
};
#endif //AGEG_VECTOR2_H