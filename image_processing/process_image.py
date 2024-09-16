
import sys
from enum import Enum


class MoveDir(Enum):
    NORTH = 0,
    SOUTH = 1,
    EAST = 2,
    WEST = 3,

blob_contour_priority_dir_check = {
    MoveDir.NORTH: (
        (MoveDir.EAST,  (-1,  0)),
        (MoveDir.NORTH, (-1, -1)),
        (MoveDir.WEST,  None   ),
    ),
    MoveDir.SOUTH: (
        (MoveDir.WEST,  (0,  -1)),
        (MoveDir.SOUTH, (0,   0)),
        (MoveDir.EAST,  None   ),
    ),
    MoveDir.EAST: (
        (MoveDir.SOUTH, (0,  0)),
        (MoveDir.EAST,  (-1, 0)),
        (MoveDir.NORTH, None   ),
    ),
    MoveDir.WEST: (
        (MoveDir.NORTH, (-1, -1)),
        (MoveDir.WEST,  ( 0, -1)),
        (MoveDir.SOUTH, None   ),
    ),
}

def move_direction(current_pos, dir):
    if dir == MoveDir.NORTH:
        return (current_pos[0] - 1, current_pos[1])
    if dir == MoveDir.SOUTH:
        return (current_pos[0] + 1, current_pos[1])
    if dir == MoveDir.EAST:
        return (current_pos[0], current_pos[1] + 1)
    if dir == MoveDir.WEST:
        return (current_pos[0], current_pos[1] -1 )

def check_pixel(pixel_grid, r, c):
    if r < 0 or r >= len(pixel_grid):
        return 0
    if c < 0 or c >= len(pixel_grid[r]):
        return 0
    return pixel_grid[r][c]

def flood_clear_pixels(pixel_grid, r, c):
    pixel_stack = []
    pixel_stack.append((r, c))
    
    clear_val = pixel_grid[r][c]
    if clear_val == 0:
        return pixel_grid
    
    while len(pixel_stack) > 0:
        r, c = pixel_stack.pop()
        if check_pixel(pixel_grid, r, c) != clear_val:
            continue
        pixel_stack.append((r + 1, c))
        pixel_stack.append((r - 1, c))
        pixel_stack.append((r, c - 1))
        pixel_stack.append((r, c + 1))
        pixel_grid[r][c] = 0
    
    return pixel_grid

def contour_blob(pixel_grid, r, c):
    contour = []
    contour.append((r, c))
    
    current_pos = (r, c+1)
    last_move_dir = MoveDir.EAST
    
    while True:
        print("Iter Currnet Path {}, Last Move {}, Current pos {}".format(contour, last_move_dir.name, current_pos))
        if current_pos == (r, c):
            break
        for move_option in blob_contour_priority_dir_check[last_move_dir]:
            print("  Checking move option "+str(move_option))
            if move_option[1] is None:
                print("  Last Resort")
                break
            check_location = (current_pos[0] + move_option[1][0], current_pos[1] + move_option[1][1])
            print("  Checking pixel "+str(check_location))
            if check_pixel(pixel_grid, check_location[0], check_location[1]) != 0:
                print("    Got "+str(check_pixel(pixel_grid, check_location[0], check_location[1])))
                continue
            print("    Dir good")
            break
        contour.append(current_pos)
        current_pos = move_direction(current_pos, move_option[0])
        last_move_dir = move_option[0]
        pass
    
    pixel_grid = flood_clear_pixels(pixel_grid, r, c)
    return contour


def contour_pixel_grid(pixel_grid):
    """
    Arguments:
        pixel_grid (2d list[int]): A grid of integers representing the color group of each pixel
    
    Returns (list(list(int, int))):
        A list of bound loops for each destinct color group blob
    """
    blob_contours = []
    for r, row in enumerate(pixel_grid):
        for c, pixel in enumerate(row):
            if pixel == 0:
                continue
            blob_contours.append(contour_blob(pixel_grid, r, c))
            pass
    return blob_contours


def get_list_element_cyclic(list, i):
    return list[i % len(list)]


def get_contour_insets(contour):
    
    all_insets = []
    all_insets.append(contour.copy())
    
    inset = contour.copy()
    moved_mask = [True for _ in range(len(inset))]
    
    # all contour poinsts must be moved to make the inset
    while not all(moved_mask):
        inset_point_moved = False
        for point in inset:
            # if point is colinear with its neighbors, it can't be moved
            pass
        # if no points wer adjusted inward on this pass, then no further progress can be made, regardless if all points have been moved
        if not inset_point_moved:
            break
    
    pass


def main():
    pass

if __name__ == "__main__":
    sys.exit(main())