#include "World.h"

#include <plog/Log.h>
#include <godot_cpp/variant/vector2i.hpp>

#include <cassert>

using namespace godot;

namespace
{
double rng()
{
    return static_cast<double>(rand()) / RAND_MAX;
}

std::string toString(const Vector2 &v)
{
    return "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ")";
}

}

namespace treesim
{

std::string Block::toString() const
{
    return "dirty: " + std::to_string(dirty) + ", com: " + ::toString(com)
            + ", v: " + ::toString(v) + ", wet: " + std::to_string(wet);
}

World::World()
{
    blocks.resize(HEIGHT * WIDTH);

    // Ceiling.
    for (int x = 0; x < WIDTH; x++)
    {
        b(x, HEIGHT - 1).dirty = 1;
    }
    //
    //    // Floor.
    for (int x = 3; x < WIDTH - 3; x++)
    {
        b(x, 0).dirty = 1;
    }

    // Right wall
    for (int y = 0; y < HEIGHT - 0; y++)
    {
        b(WIDTH - 1, y).dirty = 1;
        b(WIDTH - 2, y).dirty = 1;
    }

    // Left wall
    for (int y = 0; y < HEIGHT - 0; y++)
    {
        b(0, y).dirty = 1;
        b(1, y).dirty = 1;
    }

    //    b(WIDTH - 4, 9).dirty = 1;
    //    b(WIDTH - 4, 9).v.x = -1;
    //    b(WIDTH - 4, 9).v.y = 5;

    //    b(4, 7).dirty = 1;
    //    b(4, 7).v.x = 2;
    //    b(4, 7).v.y = 2;

    // Horizontal line 5 high.
    //    for (int x = 4; x < WIDTH - 4; x++)
    //    {
    //        b(x, 5).dirty = 1;
    //        b(x, 5).v.y = x / 2.0;
    //    }

    // Vertical line in middle.
    for (int y = 4; y < HEIGHT - 4; y++)
    {
        b(WIDTH / 2, y).dirty = 1;
        b(WIDTH / 2, y).v.x = 0; //(rng() - 0.5) * 2;
        b(WIDTH / 2, y).v.y = 0; //(rng() - 0.5) * 2;
    }

    b(WIDTH / 2, HEIGHT - 1).dirty = 1;
}

Block& World::b(int x, int y)
{
    return blocks.at(x + y * WIDTH);
}

void World::physics(const double delta, const double g)
{
    struct DirtMove
    {
        double amount = 0;
        Vector2 remainder;
        Vector2 v;
    };

    struct SumCell
    {
        std::vector<DirtMove> moves;
    };

    // Initialize the sum area.
    std::vector<SumCell> sumArea;
    sumArea.resize(blocks.size());

    auto s = [&](int x, int y) -> SumCell&
    {
        return sumArea.at(y * WIDTH + x);
    };

    // Apply gravity.
    for (int y = 1; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            if (b(x, y).dirty == 0)
            {
                b(x, y) = {};
                continue;
            }

            // If the block below is not fully dirty, apply gravity normally.
            if (b(x, y - 1).dirty < 1 || b(x, y).dirty < 0.9)
            {
                b(x, y).v.y -= (g * delta);
            } else
            {
                double leftDirt = 2;
                double rightDirt = 2;
                if (x + 1 < WIDTH)
                {
                    rightDirt = b(x + 1, y).dirty;
                }
                if (x - 1 >= 0)
                {
                    leftDirt = b(x - 1, y).dirty;
                }

                // If this cell isn't more than 2x as dirty as it's emptiest neighbor,
                // don't bother altering the velocity.
                if (b(x, y).dirty < 2 * std::min(rightDirt, leftDirt))
                {
                    b(x, y).v.y -= (g * delta);
                    continue;
                }

                double sign = 0;
                if (leftDirt > rightDirt)
                {
                    sign = 1;
                } else if (leftDirt < rightDirt)
                {
                    sign = -1;
                }

                const double alpha = 0.5;
                if (sign == 0)
                {
                    b(x, y).v.x += (g * delta * SIGN(rng() - 0.5) * alpha);
                    b(x, y).v.y -= (g * delta * (1 - alpha));
                } else
                {
                    b(x, y).v.x += (g * delta * sign * alpha);
                    b(x, y).v.y -= (g * delta * (1 - alpha));
                }
            }
        }
    }

    // Air resistance.
    for (int y = 1; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            // The dirtier it is, the less resistance.
            double amount = std::pow(b(x, y).dirty, 0.02);

            b(x, y).v *= amount;
        }
    }

    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            const Block &block = b(x, y);
            assert(block.dirty >= 0);
            if (block.dirty == 0)
            {
                continue;
            }
            if (block.v.length() > 5)
            {
                LOGD << "going fast now";
            }

            LOGD << "block[ " << x << ", " << y << "]: " + block.toString();

            // Compute target square.
            const Vector2 target(x + block.com.x + block.v.x * delta,
                    y + block.com.y + block.v.y * delta);

            {
                // Is target square in bounds and is it full of dirt?  Then reflect.
                const bool isSameSquare = static_cast<int>(target.x) == x
                        && static_cast<int>(target.y) == y;
                if (!isSameSquare && target.x < WIDTH && target.x >= 0
                        && target.y < HEIGHT && target.y >= 0)
                {
                    if (b(target.x, target.y).dirty >= 1)
                    {
                        const bool isOnSameX = static_cast<int>(target.x) == x;
                        const bool isOnSameY = static_cast<int>(target.y) == y;

                        // TODO: Reflect more accurately here.
                        if (isOnSameX)
                        {
                            b(x, y).v.x *= 0.9;
                            b(x, y).v.y *= 0;

                        } else if (isOnSameY)
                        {
                            b(x, y).v.x *= 0;
                            b(x, y).v.y *= 0.9;
                        } else
                        {
                            b(x, y).v *= 0.1;
                        }
                    }
                }
            }

            if (target.x >= WIDTH || target.x < 0)
            {
                b(x, y).v.x = -block.v.x;
            }

            if (target.y < 0)
            {
                b(x, y).v.y = -(y + block.com.y) / delta * 0.5;
            } else if (target.y >= HEIGHT)
            {
                b(x, y).v.y = -block.v.y * 0.5;
            }

            Vector2 p = b(x, y).v * delta + block.com;
            p.x += x;
            p.y += y;

            Vector2i t(static_cast<int>(p.x), static_cast<int>(p.y));
            if (t.x < 0 || t.x > WIDTH - 1 || t.y < 0 || t.y > HEIGHT - 1)
            {
                LOGD << "oob, t(x,y): (" + toString(t);
                continue;
            }
            LOGD << "t: " << toString(t) << ", p: " << toString(p);

            // Determine amount to move.
            double amount = block.dirty * delta;
            // Limit to amount available in source/space free in destination.
            amount = std::min(amount, block.dirty);
            //            amount = std::min(amount, 1.0 - b(t.x, t.y).dirty);
            assert(amount > 0);

            // Queue up move.
            //            b(x, y).dirty -= amount;
            s(x, y).moves.push_back(DirtMove
                { .amount = -amount, .remainder = block.com, .v = b(x, y).v });

            s(t.x, t.y).moves.push_back(
                    DirtMove
                        { .amount = amount, .remainder = p - p.floor(), .v = b(
                                x, y).v });
        }
    }

    // Transfer changes from sum area back to the normal blocks.
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            if (s(x, y).moves.empty())
            {
                continue;
            }

            // Sum up dirt moves to this block.
            DirtMove sum;
            for (const DirtMove &move : s(x, y).moves)
            {
                sum.amount += move.amount;
            }
            const double nextMass = b(x, y).dirty + sum.amount;

            // Compute center of mass.
            {
                // All moves.
                Vector2 top;
                for (const DirtMove &move : s(x, y).moves)
                {
                    top += (move.amount * move.remainder);
                }
                // Plus the previous com.
                top += (b(x, y).dirty * b(x, y).com);

                if (nextMass != 0)
                {
                    assert(nextMass > 0);
                    b(x, y).com = top / nextMass;
                }
            }

            // Center of velocity.
            {
                // All moves.
                Vector2 top;
                for (const DirtMove &move : s(x, y).moves)
                {
                    top += (move.amount * move.v);
                }
                // Plus the previous cov.
                top += (b(x, y).v * b(x, y).dirty);

                if (nextMass != 0)
                {
                    assert(nextMass > 0);
                    b(x, y).v = top / nextMass;
                }
            }

            // Update mass.
            b(x, y).dirty = nextMass;

            LOGD << "[y,x] (" << x << ", " << y
                    << "), com: " + toString(b(x, y).com);
        }
    }

    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            if (s(x, y).moves.empty())
            {
                continue;
            }
        }
    }
}

}
