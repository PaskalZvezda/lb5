#include "World.h"
#include "Painter.h"
#include <algorithm>
#include <fstream>

// Длительность одного тика симуляции.
// Подробнее см. update()
// Изменять не следует
static constexpr double timePerTick = 0.001;

/**
 * Конструирует объект мира для симуляции
 * @param worldFilePath путь к файлу модели мира
 */
World::World(const std::string& worldFilePath) {

    std::ifstream stream(worldFilePath);
    stream >> topLeft >> bottomRight;
    physics.setWorldBox(topLeft, bottomRight);

    Point center;
    Point velocity;
    Color color;
    double radius;
    bool isCollidable;

    while (stream >> center >> velocity >> color >> radius >> std::boolalpha >>
           isCollidable) {
        balls.emplace_back(center, Velocity(velocity), radius, color,
                           isCollidable);
    }
}

/// @brief Отображает состояние мира
void World::show(Painter& painter) const {
    // Рисуем белый прямоугольник, отображающий границу
    // мира
    painter.draw(topLeft, bottomRight, Color(1, 1, 1));

    // Вызываем отрисовку каждого шара
    for (const Ball& ball : balls) {
        ball.draw(painter);
    }

    for (const Dust& particle : dust) {
        particle.draw(painter);
    }
}

/// @brief Обновляет состояние мира
void World::update(double time) {
    /**
     * В реальном мире время течет непрерывно. Однако
     * компьютеры дискретны по своей природе. Поэтому
     * симуляцию взаимодействия шаров выполняем дискретными
     * "тиками". Т.е. если с момента прошлой симуляции
     * прошло time секунд, time / timePerTick раз обновляем
     * состояние мира. Каждое такое обновление - тик -
     * в physics.update() перемещаем шары и обрабатываем
     * коллизии - ситуации, когда в результате перемещения
     * один шар пересекается с другим или с границей мира.
     * В общем случае время не делится нацело на
     * длительность тика, сохраняем остаток в restTime
     * и обрабатываем на следующей итерации.
     */

    // учитываем остаток времени, который мы не "доработали" при прошлом update
    time += restTime;
    const auto ticks = static_cast<size_t>(std::floor(time / timePerTick));
    restTime = time - double(ticks) * timePerTick;

    std::vector<Point> collisions;
    physics.update(balls, ticks, collisions);

    for (const Point& point : collisions) {
        addDust(point);
    }

    const double elapsed = double(ticks) * timePerTick;
    for (Dust& particle : dust) {
        particle.update(elapsed);
    }
    dust.erase(std::remove_if(dust.begin(), dust.end(),
                              [](const Dust& particle) {
                                  return !particle.isAlive();
                              }),
               dust.end());
}

void World::addDust(const Point& center) {
    const int count = 8;
    const double speed = 400.;
    const double lifetime = 0.5;
    for (int i = 0; i < count; ++i) {
        const double angle = 2. * M_PI * i / count;
        dust.emplace_back(center, Velocity(speed, angle).vector(), lifetime);
    }
}