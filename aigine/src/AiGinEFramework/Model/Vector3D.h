/* Game Engine Design 
 * Vector3D.h  
 */

#ifndef VECTOR3D_H
#define VECTOR3D_H

/**
 * Bechreibung eines Vektors und statische Implemtierung
 * der Vektormathematik.
 * @author Danny Gräf
 * @since 2004-01-08
 */
class Vector3D {
public:

  /**
   * Konstruktor, initialisiert alle Koordinaten mit 0.0.
   */
	Vector3D();

  /**
   * Konstruktor, initialisiert alle Koordinaten mit den
   * übergebenen Werten.
   * @param x Koordinate.
   * @param y Koordinate.
   * @param z Koordinate.
   */
    Vector3D(float x, float y, float z);

  /**
   * Vektoraddition.
   * @param vector Summand
   * @return Summe beider Vektoren.
   */
	Vector3D operator+(Vector3D vector);

  /**
   * Vektorsubtraktion
   * @param vector Subtrahend.
   * @return Differenz beider Vektoren.
   */
  Vector3D operator-(Vector3D vector);

  /**
   * Multiplikation von Vektor und reeler Zahl.
   * @param num reele Zahl.
   * @return Produkt.
   */
	Vector3D operator*(float num);

  /**
   * Division von Vektor und reeler Zahl.
   * @param Divisor.
   * @return Quotient.
   */
  Vector3D operator/(float num);

    /**
     * Gibt das Kreuzprodukt (den senkrecht stehenden Vektor) 
     * der beiden Vektoren zurück.
     * @param vector1 erster Vektor
     * @param vector2 zweiter Vektor
     * @return Kreuzprodukt
     */
  static Vector3D cross(Vector3D * vector1, Vector3D * vector2);

  /**
   * Gibt den normalisierten Vektor zurück.
   * @param vector der normalisiert werden soll.
   * @return Normalisierter Vektor.
   */  
  static Vector3D getNormalVector(Vector3D vector);

  /**
   * Gibt die Länge eines Vektors zurück.
   * @param vector dessen Länge berechnet werden soll.
   * @return Länge des Vektors.
   */  
  static float magnitude(Vector3D vector);
  
  /**
   * Gibt die Beschreibung und die Vektorkoordinaten 
   * in der Konsole aus.
   * @param description Beschreibung des Vektors.
   */
	void print(char* description);

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
#endif //VECTOR3D_H