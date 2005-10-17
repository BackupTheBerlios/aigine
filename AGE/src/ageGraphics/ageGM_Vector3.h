/* Game Engine Design 
 * Vector3D.h  
 */

#ifndef AGEGM_VECTOR3_H
#define AGEGM_VECTOR3_H
#include "ageGM_Vector3.h"

/**
 * Bechreibung eines Vektors und statische Implemtierung
 * der Vektormathematik.
 * @author Danny Gräf
 * @since 2004-01-08
 */
class ageGM_Vector3 {
public:

  /**
   * Konstruktor, initialisiert alle Koordinaten mit 0.0.
   */
	ageGM_Vector3();

  /**
   * Konstruktor, initialisiert alle Koordinaten mit den
   * übergebenen Werten.
   * @param x Koordinate.
   * @param y Koordinate.
   * @param z Koordinate.
   */
    ageGM_Vector3(float x, float y, float z);

  /**
   * Vektoraddition.
   * @param vector Summand
   * @return Summe beider Vektoren.
   */
	ageGM_Vector3 operator+(ageGM_Vector3 vector);

  /**
   * Vektorsubtraktion
   * @param vector Subtrahend.
   * @return Differenz beider Vektoren.
   */
  ageGM_Vector3 operator-(ageGM_Vector3 vector);

  /**
   * Multiplikation : SkalarProdukt; vector * vector = i;.
   * @param mit dem zu multiplizierenden Vector.
   * @return Scalar - Produkt.
   */
	float skalarProdukt (ageGM_Vector3 vector);

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
	ageGM_Vector3 operator*(float num);

  /**
   * Division von Vektor und reeler Zahl.
   * @param Divisor.
   * @return Quotient.
   */
  ageGM_Vector3 operator/(float num);

    /**
     * Gibt das Kreuzprodukt (den senkrecht stehenden Vektor) 
     * der beiden Vektoren zurück.
     * @param vector1 erster Vektor
     * @param vector2 zweiter Vektor
     * @return Kreuzprodukt
     */
  static ageGM_Vector3 cross(ageGM_Vector3 * vector1, ageGM_Vector3 * vector2);

  /**
   * Gibt den normalisierten Vektor zurück.
   * @param vector der normalisiert werden soll.
   * @return Normalisierter Vektor.
   */  
  static ageGM_Vector3 getNormalVector(ageGM_Vector3 vector);

  /**
   * Gibt die Länge eines Vektors zurück.
   * @param vector dessen Länge berechnet werden soll.
   * @return Länge des Vektors.
   */  
  static float magnitude(ageGM_Vector3 vector);  

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
#endif //AGEGM_VECTOR3_H

