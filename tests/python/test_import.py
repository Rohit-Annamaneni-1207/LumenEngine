import numpy as np
import pytest

import lumen


def test_version_major():
    assert lumen.version_major() == 0


def test_invert_grayscale():
    source = np.array(
        [
            [0, 10, 255],
            [50, 100, 200],
        ],
        dtype=np.uint8,
    )

    original = source.copy()

    result = lumen.invert(source)

    expected = np.array(
        [
            [255, 245, 0],
            [205, 155, 55],
        ],
        dtype=np.uint8,
    )

    np.testing.assert_array_equal(result, expected)
    np.testing.assert_array_equal(source, original)

    assert result.dtype == np.uint8
    assert result.shape == source.shape
    assert result.flags.c_contiguous


def test_invert_rgb():
    source = np.array(
        [
            [
                [0, 10, 20],
                [100, 200, 255],
            ]
        ],
        dtype=np.uint8,
    )

    expected = np.array(
        [
            [
                [255, 245, 235],
                [155, 55, 0],
            ]
        ],
        dtype=np.uint8,
    )

    result = lumen.invert(source)

    np.testing.assert_array_equal(result, expected)


def test_invert_rejects_unsupported_channels():
    source = np.zeros((2, 3, 4), dtype=np.uint8)

    with pytest.raises(ValueError, match="grayscale or RGB"):
        lumen.invert(source)


def test_invert_rejects_wrong_dtype():
    source = np.zeros((2, 3), dtype=np.float32)

    with pytest.raises(TypeError):
        lumen.invert(source)