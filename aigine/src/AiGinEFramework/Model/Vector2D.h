/* Game Engine Design */

#ifndef VECTOR2D_H
#define VECTOR2D_H
class Vector3D;

/**
 * Vector2D dient dem AiGinE Object bei der verarbeitung von 3d informationen(texturen)
 * @author Tobias Harpering
 * @author Frank Otto 
 */
class Vector2D
{
public:  

    /**
     * Konstruktor, initialisiert alle Koordinaten mit den
     * übergebenen Werten.
     * @param x Koordinate.
     * @param y Koordinate.
     * @param z Koordinate.
     */
    Vector2D(float x, float y  );

    /**
     * Konstruktor, initialisiert alle Koordinaten mit 0.0.
     */
	Vector2D();

    /**
     * Vektoraddition.
     * @param vector Summand
     * @return Summe beider Vektoren.
     */
	Vector2D operator+(Vector2D vector);

    /**
     * Vektorsubtraktion
     * @param vector Subtrahend.
     * @return Differenz beider Vektoren.
     */
    Vector2D operator-(Vector2D vector);

    /**
     * Multiplikation von Vektor und reeler Zahl.
     * @param num reele Zahl.
     * @return Produkt.
     */
	Vector2D operator*(float num);

    /**
     * Division von Vektor und reeler Zahl.
     * @param Divisor.
     * @return Quotient.
     */
    Vector2D operator/(float num);

    /**
       * y Koordinate des Vektors
       */
    float y;

    /**
     * z Koordinate des Vektors
     */
    float x;
};
#endif //VECTOR2D_H