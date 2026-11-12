import math


class Vector3D:
    def __init__(self, x, y, z):
        self.x = x
        self.y = y
        self.z = z

    def __repr__(self):
        return f"({self.x}, {self.y}, {self.z})"

    def __str__(self):
        return f"({self.x}, {self.y}, {self.z})"

    def __add__(self, other):
        if other.__class__ == Vector3D:
            return Vector3D(self.x + other.x, self.y + other.y, self.z + other.z)
        elif other.__class__ == int or other.__class == float or other.__class__ == complex:
            return Vector3D(self.x + other, self.y + other, self.z + other)
        else:
            raise TypeError("Unsupported operand type(s) for +: 'Vector3D' and '{}'".format(other.__class__.__name__))

    def __radd__(self, other):
        if other.__class__ == int or other.__class__ == float or other.__class__ == complex:
            return self.__add__(other)
        return None

    def __iadd__(self, other):
        self.x = self.__add__(other).x
        self.y = self.__add__(other).y
        self.z = self.__add__(other).z
        return self

    def __mul__(self, other):
        if other.__class__ == Vector3D:
            return Vector3D(self.x * other.x, self.y * other.y, self.z * other.z)
        elif other.__class__ == int or other.__class__ == float or other.__class__ == complex:
            return Vector3D(self.x * other, self.y * other, self.z * other)
        else:
            raise TypeError("Unsupported operand type(s) for *: 'Vector3D' and '{}'".format(other.__class__.__name__))

    def __rmul__(self, other):
        if other.__class__ == int or other.__class__ == float or other.__class__ == complex:
            return self.__mul__(other)
        return None

    def __imul__(self, other):
        self.x = self.__mul__(other).x
        self.y = self.__mul__(other).y
        self.z = self.__mul__(other).z
        return self

    def __eq__(self, other):
        return self.x == other.x and self.y == other.y and self.z == other.z

    def __ne__(self, other):
        return not self.__eq__(other)

    def __abs__(self):
        return math.sqrt(self.x ** 2 + self.y ** 2 + self.z ** 2)

    def __getattr__(self, name):
        if name == "X":
            return self.x
        elif name == "Y":
            return self.y
        elif name == "Z":
            return self.z
        else:
            raise AttributeError(f"'Vector3D' object has no attribute '{name}'")

    def __setattr__(self, name, value):
        if name == "X":
            self.x = value
        elif name == "Y":
            self.y = value
        elif name == "Z":
            self.z = value
        else:
            raise AttributeError(f"'Vector3D' object has no attribute '{name}'")

        def __iter__(self):
            return iter((self.x, self.y, self.z))
