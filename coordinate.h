#ifndef COORDINATE_H
#define COORDINATE_H

struct Coordinate {

    enum Type { Vertical, Horizontal };

    Coordinate() : x(0), y(0), id(0), type(Vertical) {}
    Coordinate(int _x, int _y, int _id, Coordinate::Type _type = Vertical) : x(_x), y(_y), id(_id), type(_type) {}

    friend bool operator == (Coordinate lop, Coordinate rop) {

        return lop.x == rop.x && lop.y == rop.y;
    }

    friend bool operator != (Coordinate lop, Coordinate rop) {

        return lop.x != rop.x || lop.y != rop.y;
    }

    int x, y, id;
    Type type;
};

#endif // COORDINATE_H
