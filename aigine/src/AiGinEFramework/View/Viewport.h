/* Game Engine Design */



#ifndef VIEWPORT_H

#define VIEWPORT_H

/**
 * Einstellungen der Perspektive, Fenstergröße, Linsenkrümmung etc. werden vom Viewport übernommen. 
 */
class Viewport
{

public:



    virtual void set();



    void setSize(float width, float height);

};

#endif //VIEWPORT_H