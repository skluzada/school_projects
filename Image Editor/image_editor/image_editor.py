import numpy as np
from PIL import Image

MIRROR_VERTICAL = 0
MIRROR_HORIZONTAL = 1

# coefficients by which the corresponding channels are multiplied when converting image from rgb to gray
TO_GRAY_COEFFICIENTS = {
    'RED': 0.2989,
    'GREEN': 0.5870,
    'BLUE': 0.1140
}


class ImageEditor:
    """
    A class used to perform some basic operations on images

    Attributes:
        image_data (np.array):
            Current state of the image, without brightness and contrast applied
            brightness and contrast are not applied, because information could be lost
            and we want to be able to keep all the information after for example setting brightness to maximum
        image_data_og (np.array):
            Serves as a backup to get to the original image
        brightness (int):
            Amount of brightness added to the image, default = 0, should be between -255 and 255
        contrast (float):
            Amount of contrast added to the image, default = 1, should be higher than 0
    """

    def __init__(self):
        self.image_data = None
        self.image_data_og = None
        self.brightness = 0
        self.contrast = 1

    def load_data(self, image_path):
        """
        Method used to load the image data

        :param image_path: (str) Absolute path of an image file
        :return: (bool) True for success, False otherwise
        """
        try:
            self.image_data = np.asarray(Image.open(image_path).convert('RGB'))
        except (OSError, AttributeError):
            return False
        self.image_data.setflags(write=True)
        self.image_data_og = self.image_data.copy()
        self.brightness = 0
        self.contrast = 1
        return True

    def get_data(self):
        """
        Method used to get the image data, with brightness and contrast applied

        :return: (np.array) Image data
        """
        return self.apply_contrast(self.apply_brightness(self.image_data.copy()))

    def save_image(self, image_path):
        """
        Method used to save the image data

        :param image_path: (string) Absolute path to which the image will be saved
        :return: (bool) True for success, False otherwise
        """
        try:
            Image.fromarray(self.get_data()).save(image_path)
        except ValueError:
            return False
        return True

    def reset(self):
        """
        Method used to set the image data back to the original
        """
        self.image_data = self.image_data_og.copy()
        self.brightness = 0
        self.contrast = 1

    def rotate(self, angle):
        """
        Method used to rotate the image by multiples of 90

        :param angle: (int) Angle to rotate the image by - has to be a multiple of 90
        """
        assert angle % 90 == 0  # GUI shouldn't let this happen
        self.image_data = np.rot90(self.image_data, k=angle / 90)

    def mirror(self, axis=MIRROR_HORIZONTAL):
        """
        Method used to mirror the image either vertically or horizontally

        :param axis: (int) Specify the axis around which the mirroring will be performed
                     has to be either MIRROR_HORIZONTAL (=1) or MIRROR_VERTICAL (=0)
        """
        self.image_data = np.flip(self.image_data, axis=axis)

    def negative(self):
        """
        Method used to transform the image data to negative
        """
        self.image_data = 255 - self.image_data

    def rgb_to_gray(self):
        """
        Method used to transform the image data from rgb to gray
        """
        if self.image_data.ndim != 3:
            # the image contains only one channel
            return

        if self.image_data.shape[2] < 3:
            # the image contains less than three channels
            return

        self.image_data = TO_GRAY_COEFFICIENTS['RED'] * self.image_data[:, :, 0] \
                          + TO_GRAY_COEFFICIENTS['GREEN'] * self.image_data[:, :, 1] \
                          + TO_GRAY_COEFFICIENTS['BLUE'] * self.image_data[:, :, 2]

    def set_brightness(self, new_brightness):
        """
        Method use to set the brightness attribute

        :param new_brightness: (int) New brightness value, should be between -255 and 255
        """
        self.brightness = new_brightness

    def apply_brightness(self, image_data):
        """
        Method to apply brightness to the passed image data (not the class's image_data attribute!)

        :param image_data: (np.array) Image data on which the brightness will be applied
        :return: (np.array) Image data with brightness applied
        """
        # convert to int16 as uint8 values could overflow
        image_data = np.clip(np.int16(image_data) + self.brightness, 0, 255)
        # convert back to uint8
        image_data = np.uint8(image_data)

        return image_data

    def set_contrast(self, new_contrast):
        self.contrast = new_contrast

    def apply_contrast(self, image_data):
        """
        Method to apply contrast to the passed image data (not the class's image_data attribute!)

        :param image_data: (np.array) Image data on which the contrast will be applied
        :return: (np.array) Image data with contrast applied
        """
        # convert to int16 as uint8 values could overflow
        image_data = np.clip(np.int16(image_data) * self.contrast, 0, 255)
        # convert back to uint8
        image_data = np.uint8(image_data)

        return image_data

    @staticmethod
    def convolve2d(channel, kernel):
        """
        Static method used to perform 2D convolution using Convolution theorem
        source: BI-SVZ, Lecture 7
        proof: https://en.wikipedia.org/wiki/Convolution_theorem

        :param channel: (np.array) 2D channel of an image
        :param kernel: (np.array) 2D filter to be applied
        :return: (np.array) result of convolution of params channel and kernel, with same size as param channel
        """
        assert channel.ndim == 2
        assert kernel.ndim == 2

        # we need to transform channel and kernel to same size, otherwise we wouldn't be able to get a dot product
        # size includes padding (np.fft.fft2 function will insert zero padding)
        size = np.array(channel.shape) + np.array(kernel.shape) - 1
        dtft_channel = np.fft.fft2(channel, size)
        dtft_kernel = np.fft.fft2(kernel, size)

        new_channel = np.fft.ifft2(dtft_channel * dtft_kernel)  # get convolution
        new_channel = np.real(new_channel)  # get back to the real world
        new_channel = np.clip(new_channel, 0, 255)  # there might be some overflows
        new_channel = np.uint8(new_channel)  # np.real returned float type values

        # we used padding, so we won't return the first and last row and column
        return new_channel[1:-1, 1:-1]

    def highlight_edges(self):
        """
        Method used to highlight edges using either laplacian filter or sharpen filter
        """
        # laplacian filter
        # sharpen_filter = np.array([[-1, -1, -1], [-1, 9, -1], [-1, -1, -1]])
        # this looks a lot better, than laplacian filter
        sharpen_filter = np.array([[0, -1, 0], [-1, 5, -1], [0, -1, 0]])

        if self.image_data.ndim == 2:
            # gray image
            self.image_data = self.convolve2d(self.image_data, sharpen_filter)

        elif self.image_data.shape[2] == 3:
            # rgb image
            # for each channel:
            for i in range(3):
                self.image_data[:, :, i] = self.convolve2d(self.image_data[:, :, i], sharpen_filter)

    def blur(self):
        """
        Method used to blur the image using either box blur filter or gaussian blur filter
        """
        # box blur filter
        # blur_filter = 1/9 * np.array([[1, 1, 1], [1, 1, 1], [1, 1, 1]])
        # gaussian blur filter
        blur_filter = 1 / 16 * np.array([[1, 2, 1], [2, 4, 2], [1, 2, 1]])

        if self.image_data.ndim == 2:
            # gray image
            self.image_data = self.convolve2d(self.image_data, blur_filter)

        elif self.image_data.shape[2] == 3:
            # rgb image
            # for each channel:
            for i in range(3):
                self.image_data[:, :, i] = self.convolve2d(self.image_data[:, :, i], blur_filter)
