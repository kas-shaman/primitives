
#include <cassert>
#include <limits>

#include "math.h"
#include "math_tests.h"

#define REQUIRE(x) assert(x)

namespace math {
    namespace {
        math::scalar eps = math::scalar(0.000001);

        bool equal(math::scalar a, math::scalar b) {
            return std::abs(a - b) <= eps;
        }
        bool equal(const math::vector2f &a, const math::vector2f &b) {
            return
                std::abs(a.x - b.x) <= eps &&
                std::abs(a.y - b.y) <= eps;
        }
        bool equal(const math::vector3f &a, const math::vector3f &b) {
            return
                std::abs(a.x - b.x) <= eps &&
                std::abs(a.y - b.y) <= eps &&
                std::abs(a.z - b.z) <= eps;
        }
        bool equal(const math::vector4f &a, const math::vector4f &b) {
            return
                std::abs(a.x - b.x) <= eps &&
                std::abs(a.y - b.y) <= eps &&
                std::abs(a.z - b.z) <= eps &&
                std::abs(a.w - b.w) <= eps;
        }
        bool equal(const math::quaternion &a, const math::quaternion &b) {
            return
                std::abs(a.x - b.x) <= eps &&
                std::abs(a.y - b.y) <= eps &&
                std::abs(a.z - b.z) <= eps &&
                std::abs(a.w - b.w) <= eps;
        }
        bool equal(const math::transform2f &a, const math::transform2f &b) {
            return
                std::abs(a._11 - b._11) <= eps &&
                std::abs(a._12 - b._12) <= eps &&
                std::abs(a._13 - b._13) <= eps &&
                std::abs(a._21 - b._21) <= eps &&
                std::abs(a._22 - b._22) <= eps &&
                std::abs(a._23 - b._23) <= eps &&
                std::abs(a._31 - b._31) <= eps &&
                std::abs(a._32 - b._32) <= eps &&
                std::abs(a._33 - b._33) <= eps;
        }

        void vector2Construction() {
            math::vector2f a {1, 2};
            math::vector2f b {3};
            math::vector2f c = a.yx;
            
            REQUIRE(equal(a.x, 1));
            REQUIRE(equal(a.y, 2));
            REQUIRE(equal(b.x, 3));
            REQUIRE(equal(b.y, 3));
            REQUIRE(equal(c.x, a.y));
            REQUIRE(equal(c.y, a.x));
            REQUIRE(equal(a[0], 1));
            REQUIRE(equal(a[1], 2));
        }

        void vector2Arithmetic() {
            math::vector2f a {3, 4};
            math::vector2f b {1, 2};
            math::vector2f c1;
            math::vector2f c2;
            math::vector2f c3;
            math::vector2f c4;
            math::scalar s = 2;

            c1 = a + b;
            c2 = a - b;
            c3 = a * b;
            c4 = a / b;
            REQUIRE(equal(c1, {4, 6}));
            REQUIRE(equal(c2, {2, 2}));
            REQUIRE(equal(c3, {3, 8}));
            REQUIRE(equal(c4, {3, 2}));

            c1 = a + s;
            c2 = a - s;
            c3 = a * s;
            c4 = a / s;
            REQUIRE(equal(c1, {5, 6}));
            REQUIRE(equal(c2, {1, 2}));
            REQUIRE(equal(c3, {6, 8}));
            REQUIRE(equal(c4, {math::scalar(1.5), 2}));

            c1 = s + a;
            c2 = s - a;
            c3 = s * a;
            c4 = s / a;
            REQUIRE(equal(c1, {5, 6}));
            REQUIRE(equal(c2, {-1, -2}));
            REQUIRE(equal(c3, {6, 8}));
            REQUIRE(equal(c4, {math::scalar(0.666666666), math::scalar(0.5)}));
        }

        void vector2Operating() {
            math::vector2f a {-1, 2};
            math::vector2f b {3, -4};
            math::vector2f r {1, 0};
            math::vector2f c = a;

            REQUIRE(equal(a.min(), -1));
            REQUIRE(equal(a.max(), 2));
            REQUIRE(equal(a.sign(), {-1, 1}));
            REQUIRE(equal(a.nearestAxis(), {0, 1}));
            REQUIRE(equal(a.farthestAxis(), {-1, 0}));
            REQUIRE(equal(a.orthogonalLeft(), {2, 1}));
            REQUIRE(equal(a.orthogonalRight(), {-2, -1}));
            REQUIRE(a.cross(a.orthogonalLeft()) < math::scalar(0.0));
            REQUIRE(a.cross(a.orthogonalRight()) > math::scalar(0.0));
            REQUIRE(equal(a.clamped(-math::scalar(1.5), math::scalar(1.5)), {-math::scalar(1.0), math::scalar(1.5)}));
            REQUIRE(equal(a.lengthSq(), a.dot(a)));
            REQUIRE(equal(a.length(), std::sqrt(a.dot(a))));
            REQUIRE(equal(a.normalized() * c.length(), a));
            REQUIRE(equal(a.distanceTo(b), (b - a).length()));
            REQUIRE(equal(a.distanceSqTo(b), (b - a).dot(b - a)));
            REQUIRE(equal(a.lerpTo(b, math::scalar(0.5)), {1, -1}));
            REQUIRE(equal(a.lerpTo(b, math::scalar(0.333333333)), a * math::scalar(0.666666666) + b * math::scalar(0.333333333)));
            REQUIRE(r.cross(r.rotated(math::PI_6)) > math::scalar(0.0)); // clockwise rotation
            REQUIRE(equal(r.rotated(math::PI_6).angleTo(r), math::PI_6));
        }

        void vector3Construction() {
            math::vector3f a {1, 2, 3};
            math::vector3f b {4};
            math::vector3f c {a.yz, 4};
            math::vector3f d {4, a.zy};

            REQUIRE(equal(a.x, 1));
            REQUIRE(equal(a.y, 2));
            REQUIRE(equal(a.z, 3));
            REQUIRE(equal(b.x, 4));
            REQUIRE(equal(b.y, 4));
            REQUIRE(equal(b.z, 4));
            REQUIRE(equal(c, {2, 3, 4}));
            REQUIRE(equal(d, {4, 3, 2}));
            REQUIRE(equal(d[0], 4));
            REQUIRE(equal(d[1], 3));
            REQUIRE(equal(d[2], 2));
        }

        void vector3Arithmetic() {
            math::vector3f a {3, 4, 5};
            math::vector3f b {1, 2, 3};
            math::vector3f c1;
            math::vector3f c2;
            math::vector3f c3;
            math::vector3f c4;
            math::scalar s = 2;

            c1 = a + b;
            c2 = a - b;
            c3 = a * b;
            c4 = a / b;
            REQUIRE(equal(c1, {4, 6, 8}));
            REQUIRE(equal(c2, {2, 2, 2}));
            REQUIRE(equal(c3, {3, 8, 15}));
            REQUIRE(equal(c4, {3, 2, math::scalar(1.6666666)}));

            c1 = a + s;
            c2 = a - s;
            c3 = a * s;
            c4 = a / s;
            REQUIRE(equal(c1, {5, 6, 7}));
            REQUIRE(equal(c2, {1, 2, 3}));
            REQUIRE(equal(c3, {6, 8, 10}));
            REQUIRE(equal(c4, {math::scalar(1.5), 2, math::scalar(2.5)}));

            c1 = s + a;
            c2 = s - a;
            c3 = s * a;
            c4 = s / a;
            REQUIRE(equal(c1, {5, 6, 7}));
            REQUIRE(equal(c2, {-1, -2, -3}));
            REQUIRE(equal(c3, {6, 8, 10}));
            REQUIRE(equal(c4, {math::scalar(0.666666666), math::scalar(0.5), math::scalar(0.4)}));
        }

        void vector3Operating() {
            math::vector3f a {-1, 2, math::scalar(0.5)};
            math::vector3f b {3, -4, 1};
            math::vector3f c = a;

            REQUIRE(equal(a.min(), -1));
            REQUIRE(equal(a.max(), 2));
            REQUIRE(equal(a.sign(), {-1, 1, 1}));
            REQUIRE(equal(b.nearestAxis(), {0, -1, 0}));
            REQUIRE(equal(a.farthestAxis(), {0, 0, 1}));
            REQUIRE(equal(a.clamped(-math::scalar(1.5), math::scalar(1.5)), {-math::scalar(1.0), math::scalar(1.5), math::scalar(0.5)}));
            REQUIRE(equal(a.lengthSq(), a.dot(a)));
            REQUIRE(equal(a.length(), std::sqrt(a.dot(a))));
            REQUIRE(equal(a.normalized() * c.length(), a));
            REQUIRE(equal(a.distanceTo(b), (b - a).length()));
            REQUIRE(equal(a.distanceSqTo(b), (b - a).dot(b - a)));
            REQUIRE(equal(a.lerpTo(b, math::scalar(0.5)), {1, -1, math::scalar(0.75)}));
            REQUIRE(equal(a.lerpTo(b, math::scalar(0.333333333)), a * math::scalar(0.666666666) + b * math::scalar(0.333333333)));
            REQUIRE(equal(std::cos(a.normalized().angleTo(b.normalized())), a.normalized().dot(b.normalized())));
            REQUIRE(equal(a.cross({0, 1, 0}).angleTo({0, 1, 0}), math::PI_2));
        }

        void vector4Construction() {
            math::vector4f a {1, 2, 3, 4};
            math::vector4f b {5};
            math::vector4f c {a.yz, 5, 6};
            math::vector4f d {5, a.yz, 6};
            math::vector4f e {5, 6, a.yz};
            math::vector4f f {a.yzx, 7};
            math::vector4f g {7, a.yzx};

            REQUIRE(equal(a.x, 1));
            REQUIRE(equal(a.y, 2));
            REQUIRE(equal(a.z, 3));
            REQUIRE(equal(a.w, 4));
            REQUIRE(equal(b.x, 5));
            REQUIRE(equal(b.y, 5));
            REQUIRE(equal(b.z, 5));
            REQUIRE(equal(b.w, 5));
            REQUIRE(equal(c, {2, 3, 5, 6}));
            REQUIRE(equal(d, {5, 2, 3, 6}));
            REQUIRE(equal(e, {5, 6, 2, 3}));
            REQUIRE(equal(f, {2, 3, 1, 7}));
            REQUIRE(equal(g, {7, 2, 3, 1}));
            REQUIRE(equal(f[0], 2));
            REQUIRE(equal(f[1], 3));
            REQUIRE(equal(f[2], 1));
            REQUIRE(equal(f[3], 7));
        }

        void vector4Arithmetic() {
            math::vector4f a {3, 4, 5, 6};
            math::vector4f b {1, 2, 3, 4};
            math::vector4f c1;
            math::vector4f c2;
            math::vector4f c3;
            math::vector4f c4;
            math::scalar s = 2;

            c1 = a + b;
            c2 = a - b;
            c3 = a * b;
            c4 = a / b;
            REQUIRE(equal(c1, {4, 6, 8, 10}));
            REQUIRE(equal(c2, {2, 2, 2, 2}));
            REQUIRE(equal(c3, {3, 8, 15, 24}));
            REQUIRE(equal(c4, {3, 2, math::scalar(1.6666666), math::scalar(1.5)}));

            c1 = a + s;
            c2 = a - s;
            c3 = a * s;
            c4 = a / s;
            REQUIRE(equal(c1, {5, 6, 7, 8}));
            REQUIRE(equal(c2, {1, 2, 3, 4}));
            REQUIRE(equal(c3, {6, 8, 10, 12}));
            REQUIRE(equal(c4, {math::scalar(1.5), 2, math::scalar(2.5), 3}));

            c1 = s + a;
            c2 = s - a;
            c3 = s * a;
            c4 = s / a;
            REQUIRE(equal(c1, {5, 6, 7, 8}));
            REQUIRE(equal(c2, {-1, -2, -3, -4}));
            REQUIRE(equal(c3, {6, 8, 10, 12}));
            REQUIRE(equal(c4, {math::scalar(0.666666666), math::scalar(0.5), math::scalar(0.4), math::scalar(0.333333333)}));
        }

        void vector4Swizzling() {
            math::vector4f a {3, 4, 5, 6};
            math::vector4f b {1, 2, 3, 4};
            math::vector4f r {0};
            
            r.xy = a.wy;
            r.wz = b.xw;
            REQUIRE(equal(r, {6, 4, 4, 1}));
            r.xyz = a.wyx;
            REQUIRE(equal(r, {6, 4, 3, 1}));
            r.wzy = b.zzx;
            REQUIRE(equal(r, {6, 1, 3, 3}));
        }

        void quaternionOperating() {
            math::vector3f v {1, 7, 3};
            math::quaternion q1 = math::quaternion::identity();
            math::quaternion q2 ({1, 0, 0}, math::PI_2);
            math::quaternion q3 ({0, 1, 0}, math::PI_2);
            math::quaternion q4 ({0, 0, 1}, math::PI_2);
            math::quaternion q5 = q1.slerpTo(q3, math::scalar(0.25));

            REQUIRE(equal(q5 * q5.inverted(), q1));
            REQUIRE(equal(q5 * q5 * q5 * q5, q3));
            REQUIRE(equal(v.transformed(q1), v));
            REQUIRE(equal(v.transformed(q2), {1, 3, -7})); // clockwise rotation
            REQUIRE(equal(v.transformed(q3), {-3, 7, 1}));
            REQUIRE(equal(v.transformed(q4), {7, -1, 3}));
        }

        void transform2Construction() {
            math::transform2f t1 {math::PI_6};
            math::transform2f t2 {{5, 7}};
            math::transform2f t3 {{3, 4}, -math::PI_6};
            
            REQUIRE(equal(t1.rows[0].xy.length(), 1));
            REQUIRE(equal(t2.rows[0].xy.length(), 1));
            REQUIRE(equal(t2.translation(), {5, 7}));
            REQUIRE(equal(t3.translation(), {3, 4}));
        }

        void transform2Operating() {
            math::transform2f t1 {math::PI_6};
            math::transform2f t2 {{3, 4}, -math::PI_6};
            math::transform2f t3 {-math::PI_6};
            math::transform2f t4 {{5, 7}};
            math::transform2f t5 {{-5, -7}};
            math::transform2f t6 {math::PI_2};
            math::transform2f r = t1 * t2;

            REQUIRE(equal(r.rows[0], {1, 0, 0}));
            REQUIRE(equal(r.rows[1], {0, 1, 0}));
            REQUIRE(equal(r.translation(), {3, 4}));
            REQUIRE(equal(r._33, 1));

            REQUIRE(equal(math::vector2f{t2._31, t2._32}, {3, 4}));
            REQUIRE(equal(t2.translation(), {3, 4}));
            REQUIRE(equal(t1, t3.transposed()));
            REQUIRE(equal(t1 * t1.transposed(), math::transform2f::identity()));
            REQUIRE(equal(t1.inverted(), t3));
            REQUIRE(equal(t4.inverted(), t5));
            REQUIRE(equal(t2.inverted() * t2, math::transform2f::identity()));
            REQUIRE(equal(t2 * t2.inverted(), math::transform2f::identity()));
            REQUIRE(equal(t4.rotated(math::PI_2).translation(), {-7, 5}));
            REQUIRE(equal(math::transform2f::identity().translated({-7, 5}).translation(), {-7, 5}));
            REQUIRE(equal((t4 * t6).translation(), {-7, 5}));
            REQUIRE(equal((t6 * t4).translation(), {5, 7}));
            REQUIRE(equal(math::transform2f::identity().translated({1, 2}).scaled({3, 4}).translation(), {3, 8}));

            math::vector2f v1 {1, 7};

            REQUIRE(equal(v1.transformed(t4), {1, 7}));
            REQUIRE(equal(v1.transformed(t4, true), {6, 14}));
            REQUIRE(equal(v1.transformed(t1).angleTo(v1), math::PI_6));
            REQUIRE(equal(v1.transformed(t6), {-7, 1}));
        }

        void transform3Construction() {
            math::quaternion  q ({0, 1, 0}, math::PI_2);
            math::transform3f t1 ({1, 2, 3});
            math::transform3f t2 ({1, 2, 3}, q );
            math::transform3f t3 ({0, 1, 0}, math::PI_2 );
            
            REQUIRE(equal(t1.rows[0].xyz.length(), 1));
            REQUIRE(equal(t2.rows[0].xyz.length(), 1));
            REQUIRE(equal(t2.translation(), {1, 2, 3}));
            REQUIRE(equal(t3.translation(), {0, 0, 0}));
        }

        void transform3Operating() {
            math::quaternion  q1 {{0, 1, 0}, math::PI_6};
            math::quaternion  q2 {{1, 0, 0}, math::PI_2};
            math::transform3f t1 {q1};
            math::transform3f t2 {{3, 4, 5}, -q1};
            math::transform3f t3 {-q1};
            math::transform3f t4 {{5, 7, 9}};
            math::transform3f t5 {{-5, -7, -9}};
            math::transform3f t6 {q2};
            math::transform3f r = t1 * t2;

            REQUIRE(equal(r.rows[0], {1, 0, 0, 0}));
            REQUIRE(equal(r.rows[1], {0, 1, 0, 0}));
            REQUIRE(equal(r.rows[2], {0, 0, 1, 0}));
            REQUIRE(equal(r.translation(), {3, 4, 5}));
            
            REQUIRE(equal(math::vector3f{t2._41, t2._42, t2._43}, {3, 4, 5}));
            REQUIRE(equal(t2.translation(), {3, 4, 5}));
            REQUIRE(equal(t1, t3.transposed()));
            REQUIRE(equal(t1 * t1.transposed(), math::transform3f::identity()));
            REQUIRE(equal(t1.inverted(), t3));
            REQUIRE(equal(t4.inverted(), t5));
            REQUIRE(equal(t2.inverted() * t2, math::transform3f::identity()));
            REQUIRE(equal(t2 * t2.inverted(), math::transform3f::identity()));
            
            REQUIRE(equal(t4.rotated(q2).translation(), {5, 9, -7}));
            REQUIRE(equal(math::transform3f::identity().translated({-7, 5, 1}).translation(), {-7, 5, 1}));
            REQUIRE(equal((t4 * t6).translation(), {5, 9, -7}));
            REQUIRE(equal((t6 * t4).translation(), {5, 7, 9}));
            REQUIRE(equal(math::transform3f::identity().translated({1, 2, 3}).scaled({3, 4, 5}).translation(), {3, 8, 15}));

            math::vector3f v1 {1, 7, 3};

            REQUIRE(equal(v1.transformed(t4), {1, 7, 3}));
            REQUIRE(equal(v1.transformed(t4, true), {6, 14, 12}));
            
            REQUIRE(equal(v1.transformed(t1).xz.angleTo(v1.xz), math::PI_6));
            REQUIRE(equal(v1.transformed(t6), {1, 3, -7}));
        }
    }

    void runTests() {
        vector2Construction();
        vector2Arithmetic();
        vector2Operating();
        vector3Construction();
        vector3Arithmetic();
        vector3Operating();
        vector4Construction();
        vector4Arithmetic();
        vector4Swizzling();
        quaternionOperating();
        transform2Construction();
        transform2Operating();
        transform3Construction();
        transform3Operating();
    }
}

