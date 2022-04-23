#ifndef PLT_MATHS
#define PLT_MATHS

namespace pmath
{
    class Vector
    {
    public:
        Vector(int, int);
        Vector(Vector &&) = default;
        Vector(const Vector &) = default;
        // Operators
        // assignment
        Vector &operator=(Vector &&) = default;
        Vector &operator=(const Vector &) = default;
        // comparison
        bool operator==(Vector v) {return v.x == x && v.y == y;}
        bool operator<=(Vector v) {return (*this).norm() <= v.norm();}
        bool operator<(Vector v) {return (*this).norm() < v.norm();}
        bool operator>=(Vector v) {return (*this).norm() >= v.norm();}
        bool operator>(Vector v) {return (*this).norm() > v.norm();}
        
        // mathematical operations
        Vector operator+(Vector vector) {return Vector(x + vector.x, y + vector.y);}
        Vector operator-(Vector vector) {return Vector(x - vector.x, y - vector.y);}
        Vector &operator+=(Vector vector)
        {
            x += vector.x;
            y += vector.y;
            return *this;
        }
        Vector &operator-=(Vector vector)
        {
            x -= vector.x;
            y -= vector.y;

            return *this;
        }
        Vector &operator*=(int scalar)
        {
            x *= scalar;
            y *= scalar;
            return *this;
        }
        Vector &operator/=(int scalar)
        {
            x /= scalar;
            y /= scalar;
            return *this;
        }

        // overloaded mathematical operators
        // these aren't actually class members, they just fit in nicely here 
        friend int operator*(const Vector vec1, const Vector vec2)
        {
            return vec1.x * vec2.x + vec1.y * vec2.y;
        }

        friend Vector operator*(int scalar, Vector vector)
        {
            return Vector(vector.x * scalar, vector.y * scalar);
        }

        friend Vector operator*(Vector vector, int scalar)
        {
            return Vector(vector.x * scalar, vector.y * scalar);
        }

         friend Vector operator/(int scalar, Vector vector)
        {
            return Vector(vector.x / scalar, vector.y / scalar);
        }

        friend Vector operator/(Vector vector, int scalar)
        {
            return Vector(vector.x / scalar, vector.y / scalar);
        }



        ~Vector();

        double norm();
        Vector orth();

    private:
        int x, y;
    };

    Vector::Vector(int x_init, int y_init)
    {
        x = x_init;
        y = y_init;
    }

    Vector::~Vector(){}

} // namespace plt (plotter_maths)

#endif // !PLT_MATHS
