/* Game Engine Design */



#ifndef OBJECTLIST_H

#define OBJECTLIST_H


class AiGinEObject;



class ObjectTree {

private:



    /**
     * @clientCardinality 1
     * @supplierCardinality 1..*
     */

    AiGinEObject * lnkObject;

};

#endif //OBJECTLIST_H