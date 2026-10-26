#include "Dust.h"

Dust::Dust(const Point& center, const Point& velocity, double lifetime)
    : center{center}, velocity{velocity}, restTime{lifetime} {}

void Dust::update(double time) {
    center = center + velocity * time;
    restTime -= time;
}

void Dust::draw(Painter& painter) const {
    painter.draw(center, 10., Color(0.6, 0.6, 0.6));
}

bool Dust::isAlive() const {
    return restTime > 0.;
}
