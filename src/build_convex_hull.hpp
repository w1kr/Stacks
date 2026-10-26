#pragma once

#include <vector>


struct point
{
    /* data */
    double x, y;
    point operator+(const point &p);
    point operator-(const point &p);
    bool operator<(const point &p);
    // rotate from *this to vector p counterclockwise
};

point point::operator+(const point& p)
{
    
}

point point::operator-(const point &p)
{

}

bool point::operator<(const point &p)
{

}


std::vector<std::size_t> build_convex_hull(const std::vector<point> &p)
{

}
