/* Game Engine Design */



#ifndef OBJECTLIST_H

#define OBJECTLIST_H


class ageGC_Object;



class ageGM_ObjectTree
{

private:



    /**
     * @clientCardinality 1
     * @supplierCardinality 1..*
     */

    ageGC_Object * lnkObject;

};

#endif //OBJECTLIST_H