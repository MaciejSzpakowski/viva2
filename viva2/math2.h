#pragma once
#include <cmath>
#include <xmmintrin.h>
#include <smmintrin.h>

#define NOTHING

namespace viva
{
    namespace math
    {
        struct alignas(16) vector
        {
            union
            {
                __m128 data;
                struct { float x, y, z, w; };
            };

            vector(float x, float y, float z, float w);
            vector(float x, float y, float z);
            vector(float x, float y);
            vector(__m128 data);
            vector();

            void xy(float x, float y);
            void xyz(float x, float y, float z);
            void add_xy(float x, float y);
            void add_xyz(float x, float y, float z);
            void mul_xy(float x, float z);
            void mul_xyz(float x, float y, float z);

            vector operator + (const vector& a);
            vector operator - (const vector& a);
            vector operator - ();
            vector operator * (float a);
            vector& operator += (const vector& a);
            vector& operator -= (const vector& a);
        };

        struct alignas(16) matrix
        {
            union
            {
                struct { __m128 r1, r2, r3, r4; };
                struct { vector v1, v2, v3, v4; };
                float f[4][4];
            };

            static matrix identity();
            static matrix translate(float x, float y);
            static matrix translate(float x, float y, float z);
            static matrix translate(const vector& v);
            static matrix translate_negy(float x, float y);
            static matrix translate_negy(float x, float y, float z);
            static matrix translate_negy(const vector& v);
            static matrix scale(float x, float y);
            static matrix scale(float x, float y, float z);
            static matrix scale(const vector& v);
            static matrix rotate(float angle);

            matrix();
            matrix(__m128 a, __m128 b, __m128 c, __m128 d);
            matrix(const vector& a, const vector& b, const vector& c, const vector& d);
            matrix transpose();
            matrix operator * (const matrix& a);
            vector operator * (const vector& a);
        };

        NOTHING vector::vector(float x, float y, float z, float w)
            :x(x),y(y),z(z),w(w)
        {
        }

        NOTHING vector::vector(float x, float y, float z)
            :x(x),y(y),z(z),w(1.0f)
        {
        }

        NOTHING vector::vector(float x, float y)
            :x(x),y(y),z(0.0f),w(1.0f)
        {
        }

        NOTHING vector::vector(__m128 data)
        {
            this->data = data;
        }

        NOTHING vector::vector()
        {
        }

        NOTHING void vector::xy(float x, float y)
        {
            this->x = x;
            this->y = y;
        }

        NOTHING void vector::xyz(float x, float y, float z)
        {
            this->x = x;
            this->y = y;
            this->z = z;
        }

        NOTHING void vector::add_xy(float x, float y)
        {
            this->x += x;
            this->y += y;
        }

        NOTHING void vector::add_xyz(float x, float y, float z)
        {
            this->data = _mm_add_ps(this->data, _mm_set_ps(1.0f, z, y, x));
        }

        NOTHING void vector::mul_xy(float x, float y)
        {
            this->x *= x;
            this->y *= y;
        }

        NOTHING void vector::mul_xyz(float x, float y, float z)
        {
            this->data = _mm_mul_ps(this->data, _mm_set_ps(1.0f, z, y, x));
        }

        NOTHING vector vector::operator + (const vector& a)
        {
            return vector(_mm_add_ps(this->data, a.data));
        }

        NOTHING vector vector::operator - (const vector& a)
        {
            return vector(_mm_sub_ps(this->data, a.data));
        }

        NOTHING vector vector::operator - ()
        {
            return vector(-this->x, -this->y, -this->z, -this->w);
        }
        
        NOTHING vector vector::operator * (float a)
        {
            return vector(this->x*a, this->y*a, this->z*a, this->w*a);
        }

        NOTHING vector operator * (float a, const vector& v)
        {
            return vector(v.x*a, v.y*a, v.z*a, v.w*a);
        }
        
        NOTHING vector& vector::operator += (const vector& a)
        {
            this->data = _mm_add_ps(this->data, a.data);
            return *this;
        }

        NOTHING vector& vector::operator -= (const vector& a)
        {
            this->data = _mm_sub_ps(this->data, a.data);
            return *this;
        }

        NOTHING matrix::matrix() {}

        NOTHING matrix::matrix(__m128 a, __m128 b, __m128 c, __m128 d)
            :r1(a), r2(b), r3(c), r4(d)
        {
        }

        NOTHING matrix::matrix(const vector& a, const vector& b, const vector& c, const vector& d)
            : r1(a.data), r2(b.data), r3(c.data), r4(d.data)
        {
        }

        NOTHING matrix matrix::identity()
        {
            return matrix(
                _mm_set_ps(0.f, 0.f, 0.f, 1.f),
                _mm_set_ps(0.f, 0.f, 1.f, 0.f),
                _mm_set_ps(0.f, 1.f, 0.f, 0.f),
                _mm_set_ps(1.f, 0.f, 0.f, 0.f));
        }

        NOTHING matrix matrix::translate(float x, float y)
        {
            return matrix::translate(x, y, 0.0f);
        }

        NOTHING matrix matrix::translate(float x, float y, float z)
        {
            return matrix(
                _mm_set_ps(0.f, 0.f, 0.f, 1.f),
                _mm_set_ps(0.f, 0.f, 1.f, 0.f),
                _mm_set_ps(0.f, 1.f, 0.f, 0.f),
                _mm_set_ps(1.f, z, y, x));
        }

        NOTHING matrix matrix::translate(const vector& v)
        {
            return matrix(
                _mm_set_ps(0.f, 0.f, 0.f, 1.f),
                _mm_set_ps(0.f, 0.f, 1.f, 0.f),
                _mm_set_ps(0.f, 1.f, 0.f, 0.f),
                v.data);
        }

        NOTHING matrix matrix::translate_negy(float x, float y)
        {
            return matrix::translate(x, -y, 0.0f);
        }

        NOTHING matrix matrix::translate_negy(float x, float y, float z)
        {
            return matrix::translate(x, -y, z);
        }

        NOTHING matrix matrix::translate_negy(const vector& v)
        {

            return matrix::translate(v.x, -v.y, v.z);
        }

        NOTHING matrix matrix::scale(float x, float y)
        {
            return matrix::scale(x, y, 1.0f);
        }

        NOTHING matrix matrix::scale(float x, float y, float z)
        {
            return matrix(
                _mm_set_ps(0.f, 0.f, 0.f, x),
                _mm_set_ps(0.f, 0.f, y, 0.f),
                _mm_set_ps(0.f, z, 0.f, 0.f),
                _mm_set_ps(1.f, 0.f, 0.f, 0.f));
        }

        NOTHING matrix matrix::scale(const vector& v)
        {            
            return matrix(
                _mm_set_ps(0.f, 0.f, 0.f, v.x),
                _mm_set_ps(0.f, 0.f, v.y, 0.f),
                _mm_set_ps(0.f, v.z, 0.f, 0.f),
                _mm_set_ps(1.f, 0.f, 0.f, 0.f));
        }

        NOTHING matrix matrix::rotate(float angle)
        {
            float sine = sinf(angle);
            float cosine = cosf(angle);
            
            return matrix(
                _mm_set_ps(0.f, 0.f, -sine, cosine),
                _mm_set_ps(0.f, 0.f, cosine, sine),
                _mm_set_ps(0.f, 1.f, 0.f, 0.f),
                _mm_set_ps(1.f, 0.f, 0.f, 0.f));
        }

        NOTHING matrix matrix::transpose()
        {            
            matrix result;
            result.f[0][0] = this->f[0][0];
            result.f[0][1] = this->f[1][0];
            result.f[0][2] = this->f[2][0];
            result.f[0][3] = this->f[3][0];
            result.f[1][0] = this->f[0][1];
            result.f[1][1] = this->f[1][1];
            result.f[1][2] = this->f[2][1];
            result.f[1][3] = this->f[3][1];
            result.f[2][0] = this->f[0][2];
            result.f[2][1] = this->f[1][2];
            result.f[2][2] = this->f[2][2];
            result.f[2][3] = this->f[3][2];
            result.f[3][0] = this->f[0][3];
            result.f[3][1] = this->f[1][3];
            result.f[3][2] = this->f[2][3];
            result.f[3][3] = this->f[3][3];
            return result;
        }
        
        NOTHING matrix matrix::operator * (const matrix& a)
        {
            
        }

        NOTHING vector matrix::operator * (const vector& a)
        {
            __m128 r1 = _mm_mul_ps(this->r1, a.data);
            __m128 r2 = _mm_mul_ps(this->r2, a.data);
            __m128 r3 = _mm_mul_ps(this->r3, a.data);
            __m128 r4 = _mm_mul_ps(this->r4, a.data);

            __m128 s1 = _mm_hadd_ps(r1, r2);
            __m128 s2 = _mm_hadd_ps(r3, r4);
            __m128 s3 = _mm_hadd_ps(s1, s2);
            return vector(s3);
        }
    }
}