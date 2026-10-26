#pragma once
#include "Color.h"
#include "Painter.h"
#include "Point.h"

class Dust {
  public:
    Dust(const Point& center, const Point& velocity, double lifetime);
    void update(double time);
    void draw(Painter& painter) const;
    bool isAlive() const;

  private:
    Point center;
    Point velocity;
    double restTime;
};
