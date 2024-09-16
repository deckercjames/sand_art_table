
from process_image import contour_pixel_grid


def test_basic_contour():
    test_pixel_grid = [
        [0, 0, 0, 0, 0],
        [0, 0, 1, 1, 0],
        [0, 0, 0, 0, 0],
    ]
    blob_contours = contour_pixel_grid(test_pixel_grid)
    assert type(blob_contours) == list
    assert len(blob_contours) == 1
    expected_result = [(1, 2), (1, 3), (1, 4), (2, 4), (2, 3), (2, 2)]
    assert blob_contours[0] == expected_result
    pass


def test_two_diag_contours():
    test_pixel_grid = [
        [0, 0, 0, 0],
        [0, 0, 1, 0],
        [0, 1, 0, 0],
        [0, 0, 0, 0],
    ]
    blob_contours = contour_pixel_grid(test_pixel_grid)
    assert type(blob_contours) == list
    expected_result = [
        [(1, 2), (1, 3), (2, 3), (2, 2)],
        [(2, 1), (2, 2), (3, 2), (3, 1)],
    ]
    assert blob_contours == expected_result
    pass