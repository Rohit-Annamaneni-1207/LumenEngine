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


@pytest.mark.parametrize(
    ("delta", "expected"),
    [
        (20, np.array([[20, 120, 255]], dtype=np.uint8)),
        (-20, np.array([[0, 80, 230]], dtype=np.uint8)),
    ],
)
def test_adjust_brightness_grayscale(delta, expected):
    source = np.array([[0, 100, 250]], dtype=np.uint8)
    original = source.copy()

    result = lumen.adjust_brightness(source, delta)

    np.testing.assert_array_equal(result, expected)
    np.testing.assert_array_equal(source, original)

    assert result.dtype == np.uint8
    assert result.shape == source.shape
    assert result.flags.c_contiguous


def test_adjust_brightness_rgb():
    source = np.array(
        [
            [
                [0, 100, 250],
                [10, 200, 255],
            ]
        ],
        dtype=np.uint8,
    )

    expected = np.array(
        [
            [
                [10, 110, 255],
                [20, 210, 255],
            ]
        ],
        dtype=np.uint8,
    )

    result = lumen.adjust_brightness(source, 10)

    np.testing.assert_array_equal(result, expected)


def test_adjust_brightness_rejects_unsupported_channels():
    source = np.zeros((2, 3, 4), dtype=np.uint8)

    with pytest.raises(ValueError, match="grayscale or RGB"):
        lumen.adjust_brightness(source, 20)


def test_adjust_brightness_rejects_wrong_dtype():
    source = np.zeros((2, 3), dtype=np.float32)

    with pytest.raises(TypeError):
        lumen.adjust_brightness(source, 20)


def test_binary_threshold():
    source = np.array([[0, 127, 128, 129, 255]], dtype=np.uint8)
    original = source.copy()

    result = lumen.binary_threshold(source, 128)

    expected = np.array([[0, 0, 0, 255, 255]], dtype=np.uint8)
    np.testing.assert_array_equal(result, expected)
    np.testing.assert_array_equal(source, original)

    assert result.dtype == np.uint8
    assert result.shape == source.shape
    assert result.flags.c_contiguous


def test_binary_threshold_custom_max_value():
    source = np.array([[0, 128, 129, 255]], dtype=np.uint8)

    result = lumen.binary_threshold(source, 128, max_value=1)

    expected = np.array([[0, 0, 1, 1]], dtype=np.uint8)
    np.testing.assert_array_equal(result, expected)


def test_binary_threshold_rejects_rgb():
    source = np.zeros((2, 3, 3), dtype=np.uint8)

    with pytest.raises(ValueError, match="grayscale image"):
        lumen.binary_threshold(source, 128)


def test_binary_threshold_rejects_wrong_dtype():
    source = np.zeros((2, 3), dtype=np.float32)

    with pytest.raises(TypeError):
        lumen.binary_threshold(source, 128)
