import sys

import qimage2ndarray
from PyQt5 import QtCore
from PyQt5.QtGui import QPixmap
from PyQt5.QtWidgets import QApplication, QMainWindow
from PyQt5.QtWidgets import QFileDialog
from PyQt5.uic import loadUi

from image_editor import image_editor


class ImageEditorGUI(QMainWindow):
    """
    A PyQT Image Editor GUI class

    GUI overlay was created in Qt Designer

    Attributes:
        self.image_editor (ImageEditor):
            Image editor to perform all the transformations on images
        self.actions_dict ({QAction: function}):
            Dictionary of QActions and according functions
        self.sliders_dict ({QSlider: function}):
            Dictionary of QSliders and according functions
    """
    def __init__(self):
        super(ImageEditorGUI, self).__init__()
        loadUi("./ImageEditor.ui", self)

        self.image_editor = image_editor.ImageEditor()

        self.actions_dict = {self.actionSave: self.save_image,
                             self.actionRotate_Left: self.rotate_left,
                             self.actionRotate_Right: self.rotate_right,
                             self.actionMirror_Horizontal: self.mirror_horizontal,
                             self.actionMirror_Vertical: self.mirror_vertical,
                             self.actionReset: self.reset,
                             self.actionTo_Gray: self.to_gray,
                             self.actionTo_Negative: self.to_negative,
                             self.actionBrightness: self.toggle_brightness_slider,
                             self.actionContrast: self.toggle_contrast_slider,
                             self.actionHighlight: self.highlight_edges,
                             self.actionBlur: self.blur}

        self.sliders_dict = {self.sliderBrightness: self.set_brightness,
                             self.sliderContrast: self.set_contrast}

        self.actionOpen.triggered.connect(self.load_image)
        self.actionExit.triggered.connect(self.close)

        self.connect_widgets()
        self.enable_actions(False)
        self.hide_sliders()

    def connect_widgets(self):
        """
        Method used to connect all widgets (QActions and QSliders) to according functions
        """
        for action, function in self.actions_dict.items():
            action.triggered.connect(function)

        for slider, function in self.sliders_dict.items():
            slider.valueChanged.connect(function)

    def enable_actions(self, enable):
        """
        Method used to enable or disable all actions from actions_dict
        The actions are disabled until user opens and image

        :param enable: (bool) True to enable all actions, False to disable all actions
        """
        for action in self.actions_dict:
            action.setEnabled(enable)

    def hide_sliders(self):
        """
        Method used to hide all sliders from sliders_dict
        Sliders should be hidden before they are toggled by an according function
        """
        for slider in self.sliders_dict:
            slider.hide()

    def reset_sliders(self):
        """
        Method used to reset all sliders to default value
        This should be done every time new image is loaded
        """
        for slider in self.sliders_dict:
            slider.setValue(0)

    def load_image(self):
        """
        Method used to ask the user which image file should be opened
        """
        image_path, _ = QFileDialog.getOpenFileName(self, "Open Image",
                                                    "./", "Image Files (*.png *.jpg *.jpeg *.bmp)")

        if not self.image_editor.load_data(image_path):
            return

        self.enable_actions(True)
        self.hide_sliders()
        self.reset_sliders()
        self.show_image()

    def save_image(self):
        """
        Method used to ask the user where should be the image file saved
        """
        image_path, _ = QFileDialog.getSaveFileName(self, "Save Image As",
                                                    "./", "PNG (*.png);;JPG (*.jpg);;JPEG (*.jpeg);;BMP(*.bmp))")
        self.image_editor.save_image(image_path)

    def show_image(self):
        """
        Method used to show image in it's current state
        Should be called every time an image transformation is called
        """
        image = qimage2ndarray.array2qimage(self.image_editor.get_data())
        pixmap = QPixmap.fromImage(image)

        # if the image is bigger than the label, scale it
        if pixmap.height() > self.label.height() or pixmap.width() > self.label.width():
            pixmap = pixmap.scaled(self.label.size(), QtCore.Qt.KeepAspectRatio)

        self.label.setPixmap(pixmap)
        self.label.show()

    def rotate_left(self):
        self.hide_sliders()
        self.image_editor.rotate(90)
        self.show_image()

    def rotate_right(self):
        self.hide_sliders()
        self.image_editor.rotate(-90)
        self.show_image()

    def mirror_horizontal(self):
        self.hide_sliders()
        self.image_editor.mirror(image_editor.MIRROR_HORIZONTAL)
        self.show_image()

    def mirror_vertical(self):
        self.hide_sliders()
        self.image_editor.mirror(image_editor.MIRROR_VERTICAL)
        self.show_image()

    def reset(self):
        self.image_editor.reset()
        self.hide_sliders()
        self.reset_sliders()
        self.show_image()

    def to_gray(self):
        self.hide_sliders()
        self.image_editor.rgb_to_gray()
        self.show_image()

    def to_negative(self):
        self.hide_sliders()
        self.image_editor.negative()
        self.show_image()

    def toggle_brightness_slider(self):
        if self.sliderBrightness.isVisible():
            self.sliderBrightness.hide()
        else:
            # hide all sliders before showing this one
            self.hide_sliders()
            self.sliderBrightness.show()

    def set_brightness(self):
        brightness = 5 * self.sliderBrightness.value()  # each step = 5 brightness
        self.image_editor.set_brightness(brightness)
        self.show_image()

    def toggle_contrast_slider(self):
        if self.sliderContrast.isVisible():
            self.sliderContrast.hide()
        else:
            # hide all sliders before showing this one
            self.hide_sliders()
            self.sliderContrast.show()

    def set_contrast(self):
        # each step = 0.1 contrast and + 10, because default slider value is 0
        contrast = (self.sliderContrast.value() + 10) / 10
        self.image_editor.set_contrast(contrast)
        self.show_image()

    def highlight_edges(self):
        self.hide_sliders()
        self.image_editor.highlight_edges()
        self.show_image()

    def blur(self):
        self.hide_sliders()
        self.image_editor.blur()
        self.show_image()


if __name__ == "__main__":
    app = QApplication(sys.argv)
    gui = ImageEditorGUI()
    gui.show()
    sys.exit(app.exec_())

