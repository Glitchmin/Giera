#include "TestFramework.h"
#include "Coordinates.h"
#include "Cuboid.h"
#include "LineSegment.h"
#include "Position.h"
#include <cmath>

TEST_CASE("Geometry", "coordinates operators")
{
    Coordinates c1(1, 2), c2(3, 5);
    Coordinates c3 = c1 + c2;
    CHECK_EQ(c3.getX(), 4u);
    CHECK_EQ(c3.getY(), 7u);
    Coordinates c4 = c3 - c1;
    CHECK_EQ(c4.getX(), 3u);
    CHECK_EQ(c4.getY(), 5u);
    c4 += Coordinates(1, 1);
    CHECK_EQ(c4.getX(), 4u);
    CHECK_EQ(c4.getY(), 6u);
}

TEST_CASE("Geometry", "coordinates from position truncate")
{
    Coordinates c(Position(3.9, 7.2, 1.0));
    CHECK_EQ(c.getX(), 3u);
    CHECK_EQ(c.getY(), 7u);
    CHECK(c.isInsideMap(4, 8));
    CHECK(!c.isInsideMap(3, 8));
}

TEST_CASE("Geometry", "position arithmetic")
{
    Position a(1, 2, 3), b(4, 6, 3);
    Position d = b - a;
    CHECK_APPROX(d.getX(), 3);
    CHECK_APPROX(d.getY(), 4);
    CHECK_APPROX(d.getZ(), 0);
    CHECK_APPROX(d.getNorm(), 5);
    CHECK_APPROX(d.getNormSq(), 25);
    Position scaled = d * 2;
    CHECK_APPROX(scaled.getY(), 8);
    CHECK_APPROX(a.grounded().getZ(), 0);
    CHECK_APPROX(a.grounded().getX(), 1);
    a += Position(1, 1, 1);
    CHECK(a == Position(2, 3, 4));
}

TEST_CASE("Geometry", "line segment length")
{
    LineSegment ls(Position(0, 0, 0), Position(3, 4, 0));
    CHECK_APPROX(ls.getLength(), 5);
}

TEST_CASE("Geometry", "cuboid straight hit")
{
    Cuboid box(Position(4, -1, 0), Position(5, 1, 2));
    LineSegment ls(Position(0, 0, 1), Position(10, 0, 1));
    auto hit = box.getLineSegmentIntersect(ls);
    REQUIRE(hit.has_value());
    CHECK_APPROX(hit->getX(), 4);
    CHECK_APPROX(hit->getY(), 0);
    CHECK_APPROX(hit->getZ(), 1);
}

TEST_CASE("Geometry", "cuboid segment ending before box misses")
{
    Cuboid box(Position(4, -1, 0), Position(5, 1, 2));
    LineSegment ls(Position(0, 0, 1), Position(3, 0, 1));
    CHECK(!box.getLineSegmentIntersect(ls).has_value());
}

TEST_CASE("Geometry", "cuboid diagonal near miss is not a hit (regression)")
{
    // x-range overlaps at t in [0.4,0.5], y-range at t in [0.6,0.7]: never inside the box.
    // The original implementation reported a hit at (6,6).
    Cuboid box(Position(4, 6, 0), Position(5, 7, 2));
    LineSegment ls(Position(0, 0, 1), Position(10, 10, 1));
    CHECK(!box.getLineSegmentIntersect(ls).has_value());
}

TEST_CASE("Geometry", "cuboid diagonal hit")
{
    Cuboid box(Position(4, 4, 0), Position(6, 6, 2));
    LineSegment ls(Position(0, 0, 1), Position(10, 10, 1));
    auto hit = box.getLineSegmentIntersect(ls);
    REQUIRE(hit.has_value());
    CHECK_APPROX(hit->getX(), 4);
    CHECK_APPROX(hit->getY(), 4);
}

TEST_CASE("Geometry", "cuboid parallel segment on face has no NaN (regression)")
{
    // Segment parallel to the y and z slabs, starting exactly on the box's lower y face.
    Cuboid box(Position(4, 0, 0), Position(5, 1, 2));
    LineSegment ls(Position(0, 0, 0), Position(10, 0, 0));
    auto hit = box.getLineSegmentIntersect(ls);
    REQUIRE(hit.has_value());
    CHECK(std::isfinite(hit->getX()));
    CHECK_APPROX(hit->getX(), 4);
}

TEST_CASE("Geometry", "cuboid segment starting inside hits at start")
{
    Cuboid box(Position(0, 0, 0), Position(2, 2, 2));
    LineSegment ls(Position(1, 1, 1), Position(5, 1, 1));
    auto hit = box.getLineSegmentIntersect(ls);
    REQUIRE(hit.has_value());
    CHECK_APPROX(hit->getX(), 1);
}

TEST_CASE("Geometry", "cuboid passing above misses")
{
    Cuboid box(Position(4, -1, 0), Position(5, 1, 2));
    LineSegment ls(Position(0, 0, 3), Position(10, 0, 3));
    CHECK(!box.getLineSegmentIntersect(ls).has_value());
}

TEST_CASE("Geometry", "cuboid center and bounding box")
{
    Cuboid box(Position(0, 0, 0), Position(2, 4, 6));
    CHECK(box.getCenter() == Position(1, 2, 3));
    auto bb = box.getBoundingBox();
    CHECK(bb.first == Position(0, 0, 0));
    CHECK(bb.second == Position(2, 4, 6));
    auto moved = box.clone(Position(1, 1, 1));
    CHECK(moved->getCenter() == Position(2, 3, 4));
}

TEST_CASE("Geometry", "cuboid corner touches (ported IntersectTest)")
{
    // From RectangleTest in the old UnitTests/BasicTests.cpp.
    Cuboid box(Position(0.0, 0.0, 0.0), Position(3.0, 4.0, 12.0));

    LineSegment through(Position(-1.0, -1.0, -1.0), Position(1.0, 1.0, 1.0));
    auto hit = box.getLineSegmentIntersect(through);
    REQUIRE(hit.has_value());
    CHECK(hit.value() == Position(0.0, 0.0, 0.0));

    LineSegment before(Position(-2.0, -2.0, -2.0), Position(-1.0, -1.0, -1.0));
    CHECK(!box.getLineSegmentIntersect(before).has_value());

    LineSegment endsOnCorner(Position(-1.0, -1.0, -1.0), Position(0.0, 0.0, 0.0));
    auto cornerHit = box.getLineSegmentIntersect(endsOnCorner);
    REQUIRE(cornerHit.has_value());
    CHECK(cornerHit.value() == Position(0.0, 0.0, 0.0));
}
