#include "Puzzle.hpp"

Puzzle::Puzzle()
{

}

Puzzle::~Puzzle()
{
    
}

void Puzzle::setDegree(const int degree)
{
    this->degree = degree;
    vertex.resize(degree);
}

int Puzzle::getDegree() const
{
    return degree;
}

void Puzzle::setPoint(const int i, const std::pair<int, int>& point)
{
    assert(i < degree);
    vertex[i].point = point;
}

std::pair<int, int> Puzzle::getPoint(const int i) const
{
    assert(i < degree);
    return vertex[i].point;
}

int Puzzle::getX(const int i) const
{
    assert(i < degree);
    return vertex[i].point.first;
}

int Puzzle::getY(const int i) const
{
    assert(i < degree);
    return vertex[i].point.second;
}

void Puzzle::putAngle()
{
    for(int i = 0, j = 1, k = degree - 1; i < degree; ++i, j = (j + 1) % degree, k = (k + 1) % degree)
    {
        vertex[i].angle = Puzzle::calcAngle(getX(i), getY(i), getX(j), getY(j), getX(k), getY(k));
    }
}

void Puzzle::putAngle_toPlayer()
{
    for(int i = 0, j = 1, k = degree - 1; i < degree; ++i, j = (j + 1) % degree, k = (k + 1) % degree)
    {
        vertex[i].angle = Puzzle::calcAngle(getX(i), getY(i), getX(k), getY(k), getX(j), getY(j));

        if(Puzzle::judgeLine(getX(i), getY(i), getX(k), getY(k), getX(j), getY(j)))
        {
            vertex.erase(vertex.begin() + i);
            degree--;

            i = -1;
            j = 0;
            k = degree - 2;
        }
    }
}

double Puzzle::getAngle(const int i) const
{
    assert(i < degree);
    return vertex[i].angle;
}

void Puzzle::putSide()
{
    for(int i = 0, j = 1; i < degree; ++i, j = (j + 1) % degree)
    {
        vertex[i].side = Puzzle::calcSide(getX(i), getY(i), getX(j), getY(j));
    }
}

double Puzzle::getSide(const int i) const
{
    assert(i < degree);
    return vertex[i].side;
}

void Puzzle::putArea()
{
    area = 0;

    for(int i = 0, j = 1, k = degree - 1; i < degree; ++i, j = (j + 1) % degree, k = (k + 1) % degree)
    {
        area += getY(i) * (getX(k) - getX(j));
    }

    area = abs(area) * 0.5;
}

void Puzzle::setArea(const double area)
{
    this->area = area;
}

double Puzzle::getArea() const
{
    return area;
}

void Puzzle::show() const
{
    std::cout << "Degree : "  << degree << std::endl;
    std::cout << "Area : " << area << std::endl;
    std::cout << "Point : " << std::endl;

    for(int i = 0; i < degree; ++i)
    {
        std::cout << vertex[i].point.first << " " << vertex[i].point.second << std::endl;
    }

    std::cout << "Angle" << std::endl;
    for(int i = 0; i < degree; ++i)
    {
        std::cout << vertex[i].angle << std::endl;
    }

    std::cout << "Side" << std::endl;
    for(int i = 0; i < degree; ++i)
    {
        std::cout << vertex[i].side << std::endl;
    }
}