

from process_image import get_contour_insets

def test_get_inset_basic():
    # Contour a 3x3 square
    contour = [(0,0), (0,1), (0,2), (1,2), (2,2), (2,1), (2,0), (1,0)]
    insets = get_contour_insets(contour)
    expected_insets = [
        [(0,0), (0,1), (0,2), (1,2), (2,2), (2,1), (2,0), (1,0)], # Original 3x3 square
        [(1,1), (1,2), (2,2), (2,1)], # inner 1x1 square
    ]
    assert insets == expected_insets


def test_get_inset_double_basic():
    # Contour a 4x4 square
    contour = [(0,0), (0,1), (0,2), (0,3), (1,3), (2,3), (3,3), (3,2), (3,1), (3,0), (2,0), (1,0)]
    insets = get_contour_insets(contour)
    expected_insets = [
        [(0,0), (0,1), (0,2), (0,3), (1,3), (2,3), (3,3), (3,2), (3,1), (3,0), (2,0), (1,0)], # Original 4x4 square
        [(1,1), (1,2), (1,3), (2,3), (3,3), (3,2), (3,1), (2,1)], # Inset 2x2 square
        [(2,2)], # Inner most point
    ]
    assert insets == expected_insets
