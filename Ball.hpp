#pragma once
#include "Color.h"
#include "Painter.h"
#include "Point.h"
#include "Velocity.h"

class Ball {
public:
    Ball(const Point& center, const Velocity& velocity, double radius,
         const Color& color, bool collidable);
    void setVelocity(const Velocity& velocity);
    Velocity getVelocity() const;
    void draw(Painter& painter) const;
    void setCenter(const Point& center);
    Point getCenter() const;
    double getRadius() const;
    double getMass() const;
    bool isCollidable() const;

private:
    Point center;
    Velocity velocity;
    double radius;
    Color color;
    bool collidable;
};
