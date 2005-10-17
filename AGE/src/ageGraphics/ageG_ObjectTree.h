/* Game Engine Design */



#ifndef AGEG_OBJECTTREE_H

#define AGEG_OBJECTTREE_H


class ageG_Object;



class ageG_ObjectTree
{

private:



    /**
     * @clientCardinality 1
     * @supplierCardinality 1..*
     */

    ageG_Object * lnkObject;

};

#endif //AGEG_OBJECTTREE_H