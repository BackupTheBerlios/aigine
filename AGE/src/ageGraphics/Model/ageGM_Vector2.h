/* Game Engine Design */

#ifndef AGEGM_VECTOR2_H
#define AGEGM_VECTOR2_H
class ageGM_Vector3;

/**
 * Vector2D dient dem AiGinE Object bei der verarbeitung von 3d informationen(texturen)
 * @author Tobias Harpering
 * @author Frank Otto 
 */
class ageGM_Vector2
{
public:  

    /**
     * Konstruktor, initialisiert alle Koordinaten mit den
     * übergebenen Werten.
     * @param x Koordinate.
     * @param y Koordinate.
     * @param z Koordinate.
     */
    ageGM_Vector2(float x, float y  );

    /**
     * Konstruktor, initialisiert alle Koordinaten mit 0.0.
     */
	ageGM_Vector2();

    /**
     * Vektoraddition.
     * @param vector Summand
     * @return Summe beider Vektoren.
     */
	ageGM_Vector2 operator+(ageGM_Vector2 vector);

    /**
     * Vektorsubtraktion
     * @param vector Subtrahend.
     * @return Differenz beider Vektoren.
     */
    ageGM_Vector2 operator-(ageGM_Vector2 vector);

    /**
     * Multiplikation von Vektor und reeler Zahl.
     * @param num reele Zahl.
     * @return Produkt.
     */
	ageGM_Vector2 operator*(float num);

    /**
     * Division von Vektor und reeler Zahl.
     * @param Divisor.
     * @return Quotient.
     */
    ageGM_Vector2 operator/(float num);

    /**
       * y Koordinate des Vektors
       */
    float y;

    /**
     * z Koordinate des Vektors
     */
    float x;
};
#endif //AGEGM_VECTOR2_H