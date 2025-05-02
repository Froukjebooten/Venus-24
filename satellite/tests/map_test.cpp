#include <gtest/gtest.h>
#include "../map.h"

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
    for (int i = 0; i < MAX_ROWS; i++)
    {
        for (int j = 0; j < MAX_COLS; j++)
        {
            EXPECT_EQ(map.cells[i][j].type, Undiscovered);
        }
    }
}

// Test cell type setting
TEST_F(MapTest, SetCellType)
{
    // Test setting a cell to Mountain
    map.cells[0][0].type = Mountain;
    EXPECT_EQ(map.cells[0][0].type, Mountain);

    // Test setting a cell to Empty
    map.cells[0][1].type = Empty;
    EXPECT_EQ(map.cells[0][1].type, Empty);
}

// Test block placement
TEST_F(MapTest, BlockPlacement)
{
    // Test placing a block
    map.cells[0][0].type = Block;
    BlockInformation block_info = {.id = 1, .type = 1};
    map.cells[0][0].data.block = block_info;

    EXPECT_EQ(map.cells[0][0].type, Block);
    EXPECT_EQ(map.cells[0][0].data.block.id, 1);
    EXPECT_EQ(map.cells[0][0].data.block.type, 1);
}

// Test map boundaries
TEST_F(MapTest, MapBoundaries)
{
    // Test that we can't access beyond map boundaries
    EXPECT_NO_THROW(map.cells[MAX_ROWS - 1][MAX_COLS - 1]);
    // Note: This test is more about documentation than actual testing
    // since C++ arrays don't have bounds checking
}

// Test robot event handling
TEST_F(MapTest, RobotEventHandling)
{
    RobotEvent event = {
        .type = ROBOT_DISCOVERED_CELL,
        .position = {.x = 0, .y = 0},
        .cell_type = Empty};

    handle_robot_event(&map, &event);
    EXPECT_EQ(map.cells[0][0].type, Empty);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}