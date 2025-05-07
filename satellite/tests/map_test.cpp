#include <gtest/gtest.h>
#include <sys/param.h>

extern "C" {
#include "map.h"
}

// return the string representation of the cell type value if it is a valid cell
// type
const char *cell_type_string(CellType type) {
  switch (type) {
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
#define ExpectCellType(map, row, col, expected_type)                           \
  EXPECT_EQ(map.cells[row][col].type, expected_type)                           \
      << "Cell at position (" << col << "," << row << ") should be "           \
      << cell_type_string(expected_type) << " was "                            \
      << cell_type_string(map.cells[row][col].type);

class MapTest : public ::testing::Test {
protected:
  Map map;
  void SetUp() override { init_map(&map); }
};

// Test map initialization
TEST_F(MapTest, Initialization) {
  // Check if all cells are initialized to Undiscovered
  for (size_t c = 0; c < MAX_COLS; c++) {
    for (size_t r = 0; r < MAX_ROWS; r++) {
      ExpectCellType(map, r, c, Undiscovered);
    }
  }
  /* Useful test cases:
   * - Verify map dimensions are correct
   * - Check all cells are initialized to empty state
   * - Verify boundary cells are properly marked
   * - Test map reset functionality
   */
}

TEST_F(MapTest, DiscoveredBlock) {
  // Test setting a cell to Block state
  size_t test_col = 5;
  size_t test_row = 7;

  // Create a robot event for discovering a block
  RobotEvent discovered_block_event = {
      .type = DiscoveredBlock,
      .row = test_row,
      .column = test_col,
      .data = {.block = {.size = Small, .color = Green}}};

  // Handle the event
  handle_robot_event(&map, &discovered_block_event);

  // Verify the cell state was updated
  ExpectCellType(map, test_row, test_col, Block);

  // Verify other cells remain unchanged
  for (size_t c = 0; c < MAX_COLS; c++) {
    for (size_t r = 0; r < MAX_ROWS; r++) {
      if (c == test_col && r == test_row) {
        ExpectCellType(map, r, c, Block);
      } else {
        ExpectCellType(map, r, c, Undiscovered);
      }
    }
  }
}

TEST_F(MapTest, DiscoveredLineVertical) {

  // draw a vertical line
  size_t end_col = 5;
  size_t end_row = 10;
  size_t start_col = 5;
  size_t start_row = 15;

  RobotEvent discovered_line_event = {.type = DiscoveredLine,
                                      .row = end_row,
                                      .column = end_col,
                                      .data = {.line = {
                                                   .initial_row = start_row,
                                                   .initial_column = start_col,
                                               }}};

  // Handle the line discovery event
  handle_robot_event(&map, &discovered_line_event);

  for (size_t r = start_row; r <= end_row; r++) {
    ExpectCellType(map, r, start_col, Empty);
  }
}

TEST_F(MapTest, DiscoveredLineHorizontal) {

  // draw a horizontal line
  size_t end_col = 10;
  size_t end_row = 5;
  size_t start_col = 15;
  size_t start_row = 5;

  RobotEvent discovered_line_event = {.type = DiscoveredLine,
                                      .row = end_row,
                                      .column = end_col,
                                      .data = {.line = {
                                                   .initial_row = start_row,
                                                   .initial_column = start_col,
                                               }}};

  // Handle the line discovery event
  handle_robot_event(&map, &discovered_line_event);

  for (size_t c = start_col; c <= end_col; c++) {
    ExpectCellType(map, start_row, c, Empty);
  }
}

TEST_F(MapTest, DiscoveredLineDescendingCoordinates) {
    // draw a vertical line
    size_t col = 5;
    size_t start_row = 15;
    size_t end_row = 10;
    // (5, 15) -> (5, 10)
    RobotEvent discovered_line_event = {.type = DiscoveredLine,
                                      .row = end_row,
                                      .column = col,
                                      .data = {.line = {
                                                   .initial_row = start_row,
                                                   .initial_column = col,
                                               }}};

  // Handle the line discovery event
  handle_robot_event(&map, &discovered_line_event);

  for (size_t r = MIN(start_row, end_row); r <= MAX(start_row, end_row); r++) {
    ExpectCellType(map, r, col, Empty);
  }
}

// Test robot position tracking
TEST_F(MapTest, RobotPositionTracking) {
  EXPECT_TRUE(true); // Placeholder
                     /* Useful test cases:
                      * - Test robot position updates
                      * - Verify position history tracking
                      * - Test position validation (within bounds)
                      * - Check collision detection with obstacles
                      */
}

// Test map exploration
TEST_F(MapTest, MapExploration) {
  EXPECT_TRUE(true); // Placeholder
                     /* Useful test cases:
                      * - Test discovery of new cells
                      * - Verify exploration progress tracking
                      * - Test exploration completion detection
                      * - Check unexplored area identification
                      */
}

// Test obstacle handling
TEST_F(MapTest, ObstacleHandling) {
  EXPECT_TRUE(true); // Placeholder
                     /* Useful test cases:
                      * - Test obstacle placement
                      * - Verify obstacle detection
                      * - Test path finding around obstacles
                      * - Check obstacle avoidance behavior
                      */
}

// Test map serialization
TEST_F(MapTest, MapSerialization) {
  EXPECT_TRUE(true); // Placeholder
                     /* Useful test cases:
                      * - Test map state saving
                      * - Verify map state loading
                      * - Test partial map updates
                      * - Check data integrity after serialization
                      */
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}