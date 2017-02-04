//
// Created by igrek on 04/02/17.
//

#include "Event.h"
//#include <boost/type_index.hpp>
#include <typeinfo>


string Event::className() {
    string s = typeid(*this).name();
//    string s = boost::typeindex::type_id_with_cvr<decltype(*this)>().pretty_name();
    return s;
}