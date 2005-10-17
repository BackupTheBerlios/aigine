/* Game Engine Design 
 * Vector3D.h  
 */

#ifndef AGEG_VECTOR3_H
#define AGEG_VECTOR3_H
#include "ageG_Vector3.h"

/**
 * Bechreibung eines Vektors und statische Implemtierung
 * der Vektormathematik.
 * @author Danny Gräf
 * @since 2004-01-08
 */
class ageG_Vector3 {
public:

  /**
   * Konstruktor, initialisiert alle Koordinaten mit 0.0.
   */
	ageG_Vector3();

  /**
   * Konstruktor, initialisiert alle Koordinaten mit den
   * übergebenen Werten.
   * @param x Koordinate.
   * @param y Koordinate.
   * @param z Koordinate.
   */
    ageG_Vector3(float x, float y, float z);

  /**
   * Vektoraddition.
   * @param vector Summand
   * @return Summe beider Vektoren.
   */
	ageG_Vector3 operator+(ageG_Vector3 vector);

  /**
   * Vektorsubtraktion
   * @param vector Subtrahend.
   * @return Differenz beider Vektoren.
   */
  ageG_Vector3 operator-(ageG_Vector3 vector);

  /**
   * Multiplikation : SkalarProdukt; vector * vector = i;.
   * @param mit dem zu multiplizierenden Vector.
   * @return Scalar - Produkt.
   */
	float skalarProdukt (ageG_Vector3 vector);

    /**
     * Gibt die Beschreibung und die Vektorkoordinaten 
     * in der Konsole aus.
     * @param description Beschreibung des Vektors.
     */
	void print(char* description);

  /**
   * Multiplikation von Vektor und reeler Zahl.
   * @param num reele Zahl.
   * @return Produkt.
   */
	ageG_Vector3 operator*(float num);

  /**
   * Division von Vektor und reeler Zahl.
   * @param Divisor.
   * @return Quotient.
   */
  ageG_Vector3 operator/(float num);

    /**
     * Gibt das Kreuzprodukt (den senkrecht stehenden Vektor) 
     * der beiden Vektoren zurück.
     * @param vector1 erster Vektor
     * @param vector2 zweiter Vektor
     * @return Kreuzprodukt
     */
  static ageG_Vector3 cross(ageG_Vector3 * vector1, ageG_Vector3 * vector2);

  /**
   * Gibt den normalisierten Vektor zurück.
   * @param vector der normalisiert werden soll.
   * @return Normalisierter Vektor.
   */  
  static ageG_Vector3 getNormalVector(ageG_Vector3 vector);

  /**
   * Gibt die Länge eines Vektors zurück.
   * @param vector dessen Länge berechnet werden soll.
   * @return Länge des Vektors.
   */  
  static float magnitude(ageG_Vector3 vector);  

  /**
   * x Koordinate des Vektors
   */
    float z;

  /**
   * y Koordinate des Vektors
   */
    float y;

  /**
   * z Koordinate des Vektors
   */
    float x;


private:


  /**
   * Gibt die Vektorkoordinaten in der Konsole aus.
   */
  void print();
};
#endif //AGEG_VECTOR3_H

