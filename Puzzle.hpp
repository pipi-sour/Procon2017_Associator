#pragma once

#include <iostream>
#include <cmath>
#include <tuple>
#include <vector>
#include <cassert>

#define PI 3.14159265358979323846

class Puzzle
{
public:
    //頂点データ
    struct Vertex
    {
        std::pair<int, int> point;  //座標 firstがx, secondがy
        double angle;               //角度
        double side;                //辺の長さ
    };

    Puzzle();
    ~Puzzle();

    void setDegree(const int);
    int getDegree() const;

    void setPoint(const int, const std::pair<int, int>&);
    std::pair<int, int> getPoint(const int) const; 
    int getX(const int) const;
    int getY(const int) const;
    
    void putAngle();
    void putAngle_toPlayer();
    double getAngle(const int) const;

    void putSide();
    double getSide(const int) const;

    void putArea();
    void setArea(const double);
    double getArea() const;

    void show() const;

    template <typename T>
    static constexpr T calcVector(T x1, T x2)
    {
        return x2 - x1;
    }

    template <typename T>
    static constexpr double calcSide(T x1, T y1, T x2, T y2)
    {
        return sqrt(calcVector(x1, x2) * calcVector(x1, x2) + calcVector(y1, y2) * calcVector(y1, y2));
    }

	template <typename T>
	static constexpr double calcInner(T x1, T y1, T x2, T y2, T x3, T y3)
	{
		return acos((calcVector(x1, x2) * calcVector(x1, x3) + calcVector(y1, y2) * calcVector(y1, y3)) / (calcSide(x1, y1, x2, y2) * calcSide(x1, y1, x3, y3))) * 180.0 / PI;
	}

    template <typename T>
    static constexpr double calcAngle(T x1, T y1, T x2, T y2, T x3, T y3)
    {
        return (calcVector(x1, x2) * calcVector(y1, y3) - calcVector(y1, y2) * calcVector(x1, x3) < 0) ? 360.0 - calcInner(x1, y1, x2, y2, x3, y3) : calcInner(x1, y1, x2, y2, x3, y3);
    }

    static constexpr bool judgeLine(int x1, int y1, int x2, int y2, int x3, int y3)
    {
        if(x2 > x3)
        {
            std::swap(x2, x3);
            std::swap(y2, y3);
        }

        return x2 <= x1 && x1 <= x3 && ((y2 <= y3 && y2 <= y1 && y1 <= y3) || (y2 > y3 && y3 <= y1 && y1 <= y2)) && (y1 - y2) * (x3 - x2) == (y3 - y2) * (x1 - x2);
    }
protected:
    int degree;     //頂点数
    double area;    //面積
    std::vector<Vertex> vertex;
};
