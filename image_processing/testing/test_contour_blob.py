
from process_image import contour_blob


def test_basic_contour_blob():
    test_pixel_grid = [
        [0, 0, 0, 0, 0],
        [0, 0, 1, 1, 0],
        [0, 0, 0, 0, 0],
    ]
    recv_blob_contour = contour_blob(test_pixel_grid, 1, 2)
    assert type(recv_blob_contour) == list
    expected_result = [(1, 2), (1, 3), (1, 4), (2, 4), (2, 3), (2, 2)]
    assert recv_blob_contour == expected_result
    pass


def test_contour_blob_with_diag_neighbor():
    test_pixel_grid = [
        [1, 0],
        [0, 1],
    ]
    recv_blob_contour = contour_blob(test_pixel_grid, 0, 0)
    assert type(recv_blob_contour) == list
    expected_result = [(0, 0), (0, 1), (1, 1), (1, 0)]
    assert recv_blob_contour == expected_result
    exp_pixel_grid = [
        [0, 0],
        [0, 1],
    ]
    assert test_pixel_grid == exp_pixel_grid
    pass


def test_contour_blob_all_angles():
    test_pixel_grid = [
        [0, 0, 0, 0, 0, 0],
        [0, 0, 0, 1, 0, 0],
        [0, 1, 1, 1, 1, 0],
        [0, 1, 1, 1, 1, 0],
        [0, 0, 0, 1, 0, 0],
        [0, 0, 0, 0, 0, 0],
    ]
    recv_blob_contour = contour_blob(test_pixel_grid, 1, 3)
    assert type(recv_blob_contour) == list
    expected_result = [(1, 3), (1, 4), (2, 4), (2, 5), (3, 5), (4, 5), (4, 4), (5, 4), (5, 3), (4, 3), (4, 2), (4, 1), (3, 1), (2, 1), (2, 2), (2, 3)]
    assert recv_blob_contour == expected_result
    exp_pixel_grid = [
        [0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0],
    ]
    assert test_pixel_grid == exp_pixel_grid
    
    
def test_basic_contour_edges():
    test_pixel_grid = [
        [1],
    ]
    recv_blob_contour = contour_blob(test_pixel_grid, 0, 0)
    assert type(recv_blob_contour) == list
    expected_result = [(0, 0), (0, 1), (1, 1), (1, 0)]
    assert recv_blob_contour == expected_result
    # check cleared with flood fill
    assert test_pixel_grid[0][0] == 0
