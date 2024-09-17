
from process_image import get_blobs
from process_image import BlobTuple
from copy import deepcopy


def test_get_blobs_basic():
    test_pixel_grid = [
        [0, 0, 0, 0, 0],
        [0, 0, 1, 1, 0],
        [0, 0, 0, 0, 0],
    ]
    test_pixel_grid_unchanged = deepcopy(test_pixel_grid)
    recv_blobs = get_blobs(test_pixel_grid)
    expected_blobs = [
        BlobTuple(
            [
                [False, False, False, False, False],
                [False, False, True,  True,  False],
                [False, False, False, False, False],
            ],
            [(1,2), (1,3), (1,4), (2,4), (2,3), (2,2)],
            [],
        ),
    ]
    assert len(recv_blobs) == len(expected_blobs)
    assert recv_blobs == expected_blobs
    assert test_pixel_grid == test_pixel_grid_unchanged


def test_get_blobs_two():
    test_pixel_grid = [
        [0, 1],
        [1, 0],
    ]
    test_pixel_grid_unchanged = deepcopy(test_pixel_grid)
    recv_blobs = get_blobs(test_pixel_grid)
    expected_blobs = [
        BlobTuple(
            [
                [False, True ],
                [False, False],
            ],
            [(0,1), (0,2), (1,2), (1,1)],
            [],
        ),
        BlobTuple(
            [
                [False, False],
                [True,  False],
            ],
            [(1,0), (1,1), (2,1), (2,0)],
            [],
        ),
    ]
    assert len(recv_blobs) == len(expected_blobs)
    assert recv_blobs == expected_blobs
    assert test_pixel_grid == test_pixel_grid_unchanged


def test_get_two_blobs_adjacent():
    test_pixel_grid = [
        [1, 2],
    ]
    test_pixel_grid_unchanged = deepcopy(test_pixel_grid)
    recv_blobs = get_blobs(test_pixel_grid)
    expected_blobs = [
        BlobTuple(
            [
                [True, False],
            ],
            [(0,0), (0,1), (1,1), (1,0)],
            [],
        ),
        BlobTuple(
            [
                [False, True],
            ],
            [(0,1), (0,2), (1,2), (1,1)],
            [],
        ),
    ]
    assert len(recv_blobs) == len(expected_blobs)
    assert recv_blobs == expected_blobs
    assert test_pixel_grid == test_pixel_grid_unchanged


def test_get_blobs_one_with_void():
    test_pixel_grid = [
        [1, 1, 1],
        [1, 0, 1],
        [1, 1, 1],
    ]
    test_pixel_grid_unchanged = deepcopy(test_pixel_grid)
    recv_blobs = get_blobs(test_pixel_grid)
    expected_blobs = [
        BlobTuple(
            [
                [True,  True,  True],
                [True,  False, True],
                [True,  True,  True],
            ],
            [(0,0), (0,1), (0,2), (0,3), (1,3), (2,3), (3,3), (3,2), (3,1), (3,0), (2,0), (1,0)],
            [],
        ),
    ]
    assert len(recv_blobs) == len(expected_blobs)
    assert recv_blobs == expected_blobs
    assert test_pixel_grid == test_pixel_grid_unchanged


def test_get_blobs_nested():
    test_pixel_grid = [
        [1, 1, 1],
        [1, 2, 1],
        [1, 1, 1],
    ]
    test_pixel_grid_unchanged = deepcopy(test_pixel_grid)
    recv_blobs = get_blobs(test_pixel_grid)
    expected_blobs = [
        BlobTuple(
            [
                [True,  True,  True],
                [True,  False, True],
                [True,  True,  True],
            ],
            [(0,0), (0,1), (0,2), (0,3), (1,3), (2,3), (3,3), (3,2), (3,1), (3,0), (2,0), (1,0)],
            [
                BlobTuple(
                    [
                        [False, False, False],
                        [False, True,  False],
                        [False, False, False],
                    ],
                    [(1,1), (1,2), (2,2), (2,1)],
                    [],
                ),
            ],
        ),
    ]
    assert len(recv_blobs) == len(expected_blobs)
    assert recv_blobs == expected_blobs
    assert test_pixel_grid == test_pixel_grid_unchanged

