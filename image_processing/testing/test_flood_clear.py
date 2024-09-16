
from process_image import flood_clear_pixels

def test_pixel_flood_clear_basic():
    test_pixel_grid = [
        [0, 0, 0, 0],
        [0, 1, 1, 0],
        [0, 0, 0, 0],
    ]
    recieved_pixel_grid = flood_clear_pixels(test_pixel_grid, 1, 1)
    exp_pixel_grid = [
        [0, 0, 0, 0],
        [0, 0, 0, 0],
        [0, 0, 0, 0],
    ]
    assert recieved_pixel_grid == exp_pixel_grid


def test_pixel_flood_clear_on_zero():
    """
    Clearing from a starting point that is 0 should not change it
    """
    test_pixel_grid = [
        [0, 0, 0, 0],
        [0, 1, 1, 0],
        [0, 0, 0, 0],
    ]
    recieved_pixel_grid = flood_clear_pixels(test_pixel_grid, 0, 0)
    exp_pixel_grid = [
        [0, 0, 0, 0],
        [0, 1, 1, 0],
        [0, 0, 0, 0],
    ]
    assert recieved_pixel_grid == exp_pixel_grid


def test_pixel_flood_clear_to_bounds():
    """
    The function should be able to clear up to the edge
    """
    test_pixel_grid = [
        [0, 0, 1, 1],
        [0, 1, 1, 1],
        [0, 0, 0, 0],
    ]
    recieved_pixel_grid = flood_clear_pixels(test_pixel_grid, 1, 1)
    exp_pixel_grid = [
        [0, 0, 0, 0],
        [0, 0, 0, 0],
        [0, 0, 0, 0],
    ]
    assert recieved_pixel_grid == exp_pixel_grid


def test_pixel_flood_clear_no_diag():
    """
    The flood fill shoudl not jump diagonals
    """
    test_pixel_grid = [
        [1, 0, 0, 1],
        [0, 1, 1, 0],
        [1, 0, 0, 1],
    ]
    recieved_pixel_grid = flood_clear_pixels(test_pixel_grid, 1, 1)
    exp_pixel_grid = [
        [1, 0, 0, 1],
        [0, 0, 0, 0],
        [1, 0, 0, 1],
    ]
    assert recieved_pixel_grid == exp_pixel_grid


def test_pixel_flood_clear_differnt_val_neighbor():
    """
    The flood fill shoudl not jump diagonals
    """
    test_pixel_grid = [
        [0, 2, 3, 1],
        [0, 1, 1, 2],
        [5, 5, 5, 1],
    ]
    recieved_pixel_grid = flood_clear_pixels(test_pixel_grid, 1, 1)
    exp_pixel_grid = [
        [0, 2, 3, 1],
        [0, 0, 0, 2],
        [5, 5, 5, 1],
    ]
    assert recieved_pixel_grid == exp_pixel_grid
