#include <gtest/gtest.h>
extern "C"
{
#include "map.h"
}

// return the string representation of the cell type value if it is a valid cell type
const char *cell_type_string(CellType type)
{
    switch (type)
    {
    case Undiscovered:
        return "Undiscovered";
    case Block:
        return "Block";
    case Empty:
        return "Empty";
    default:
        return "Unknown";
    }
}

// ExpectCellType macro
#define ExpectCellType(map, col, row, expected_type)   \
    EXPECT_EQ(map.cells[row][col].type, expected_type) \
        << "Cell at position (" << col << "," << row << ") should be " << cell_type_string(expected_type) << " was " << cell_type_string(map.cells[row][col].type);

class MapTest : public ::testing::Test
{
protected:
    Map map;
    void SetUp() override
    {
        init_map(&map);
    }
};

// Test map initialization
TEST_F(MapTest, Initialization)
{
    // Check if all cells are initialized to Undiscovered
    for (size_t c = 0; c < MAX_COLS; c++)
    {
        for (size_t r = 0; r < MAX_ROWS; r++)
        {
            EXPECT_EQ(map.cells[c][r].type, Undiscovered)
                << "Cell at position (" << c << "," << r << ") should be Undiscovered";
        }
    }
    /* Useful test cases:
     * - Verify map dimensions are correct
     * - Check all cells are initialized to empty state
     * - Verify boundary cells are properly marked
     * - Test map reset functionality
     */
}

// Test cell state management
TEST_F(MapTest, CellStateManagement)
{
    // Test setting a cell to Block state
    size_t test_col = 5;
    size_t test_row = 7;

    // Create a robot event for discovering a block
    RobotEvent event;
    event.type = DiscoveredBlock;
    event.column = test_col;
    event.row = test_row;
    event.data.block.size = Small;
    event.data.block.color = Green;

    // Handle the event
    handle_robot_event(&map, &event);

    // Verify the cell state was updated
    ExpectCellType(map, test_col, test_row, Block);

    // Verify other cells remain unchanged
    for (size_t c = 0; c < MAX_COLS; c++)
    {
        for (size_t r = 0; r < MAX_ROWS; r++)
        {
            if (c == test_col && r == test_row)
            {
                ExpectCellType(map, c, r, Block);
            }
            else
            {
                ExpectCellType(map, c, r, Undiscovered);
            }
        }
    }

    // Test setting a cell to Empty state via DiscoveredLine
    size_t line_col = 5;
    size_t line_row = 10;

    event.type = DiscoveredLine;
    event.column = line_col;
    event.row = line_row;
    event.data.line.initial_column = line_col;
    event.data.line.initial_row = line_row;

    // Handle the line discovery event
    handle_robot_event(&map, &event);

    // Verify the line cell was set to Empty
    ExpectCellType(map, line_col, line_row, Empty);

    // Verify the block cell remains unchanged
    ExpectCellType(map, test_col, test_row, Block);
}

// Test robot position tracking
TEST_F(MapTest, RobotPositionTracking)
{
    EXPECT_TRUE(true); // Placeholder
    /* Useful test cases:
     * - Test robot position updates
     * - Verify position history tracking
     * - Test position validation (within bounds)
     * - Check collision detection with obstacles
     */
}

// Test map exploration
TEST_F(MapTest, MapExploration)
{
    EXPECT_TRUE(true); // Placeholder
    /* Useful test cases:
     * - Test discovery of new cells
     * - Verify exploration progress tracking
     * - Test exploration completion detection
     * - Check unexplored area identification
     */
}

// Test obstacle handling
TEST_F(MapTest, ObstacleHandling)
{
    EXPECT_TRUE(true); // Placeholder
    /* Useful test cases:
     * - Test obstacle placement
     * - Verify obstacle detection
     * - Test path finding around obstacles
     * - Check obstacle avoidance behavior
     */
}

// Test map serialization
TEST_F(MapTest, MapSerialization)
{
    EXPECT_TRUE(true); // Placeholder
    /* Useful test cases:
     * - Test map state saving
     * - Verify map state loading
     * - Test partial map updates
     * - Check data integrity after serialization
     */
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}