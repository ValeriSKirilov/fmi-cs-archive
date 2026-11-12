import math
from abc import ABC, abstractmethod


class Shape(ABC):
    def __init__(self, color):
        self._color = color

        @property
        def color(self):
            return self._color

        @abstractmethod
        def area(self):
            pass


class Rectangle(Shape):
    def __init__(self, height, width, color):
        super().__init__(color)

        self._width = width
        self._height = height

    @property
    def height(self):
        return self._height

    @property
    def width(self):
        return self._width

    def area(self):
        return self.width * self.height


class Circle(Shape):
    def __init__(self, radius, color):
        super().__init__(color)

        self._radius = radius

    @property
    def radius(self):
        return self._radius

    def calcArea(self):
        return math.pi * self.radius ** 2


class Shapes:
    def __init__(self, shapes=None):
        shapes = [] if shapes is None else shapes
        self._shapes = shapes

    def add_new_shape(self, shape):
        self._shapes.append(shape)

    def area_of_all_rectangles(self):
        return self.__area_of(Rectangle)

    def area_of_all_circles(self):
        return self.__area_of(Circle)

    def __area_of(self, shape_type):
        result = 0

        for shape in self._shapes:
            if isinstance(shape, shape_type):
                result += shape.area()

        return result

    def __getitem__(self, index):
        return self._shapes[index]
