-- 2.1
average :: Fractional a => a -> a -> a -> a
average x y z = (x + y + z) / 3

--2.2
isTriangle :: (Ord a, Num a) => a -> a -> a -> Bool
isTriangle a b c = a < b + c && b < a + c && c < a + b

-- 3.1
subtractFrom100 :: Int -> Int
subtractFrom100 = (100 -)

-- >>> subtractFrom100 37
-- 63

-- 3.2
subtract3 :: Int -> Int
subtract3 = subtract 3

-- >>> subtract3 37  -- => 34
-- 34

-- 3.3
isGreaterThan50 :: Integer -> Bool
isGreaterThan50 = (50 <)

-- >>> isGreaterThan50 75  -- => True
-- >>> isGreaterThan50 50  -- => False
-- >>> isGreaterThan50 25  -- => False
-- True
-- False
-- False

-- 4.1
circleArea :: Fractional a => a -> a
circleArea r = r ^^ 2 * pi
    where pi = 3.14159

-- >>> circleArea 5.0  -- => 78.5398
-- 78.53975

-- 4.2
cylinderVolume :: Fractional a => a -> a -> a
cylinderVolume r h = b * h
    where b = circleArea r

-- >>> cylinderVolume 3.0 5.0 -- => 141.37164
-- 141.37155

-- 4.3
-- vectorAngleCos a1 a2 b1 b2 = scal_a_b / (length_a * length_b)
--     where   scal_a_b = a1 * b1 + a2 * b2
--             length_a = sqrt(a1 ^ 2 + a2 ^ 2)
--             length_b = sqrt(b1 ^ 2 + b2 ^ 2)

vectorAngleCos :: Floating a => a -> a -> a -> a -> a
vectorAngleCos a1 a2 b1 b2 =
    let scal_a_b = a1 * b1 + a2 * b2
        length_a = sqrt(a1 ^ 2 + a2 ^ 2)
        length_b = sqrt(b1 ^ 2 + b2 ^ 2)
    in scal_a_b / (length_a * length_b)

-- >>> vectorAngleCos 1 2 3 4
-- 0.9838699100999074

-- 5.1
gradeLetter :: (Ord a, Fractional a) => a -> String
gradeLetter x
    | x >= 5.5 && x <= 6 = "Otli4en"
    | x >= 4.5 && x < 5.5 = "Mnogo dobur"
    | x >= 3.5 && x < 4.5 = "Dobur"
    | x >= 3 && x < 3.5 = "Sreden"
    | x >= 2 && x < 3 = "Slab"
    | otherwise = "Nevalidna ocenka"

-- 5.2
triangleType :: (Ord a, Num a) => a -> a -> a -> String
triangleType a b c
    | a == b && b == c = "Ravnostranen"
    | a == b || b == c || a == c && isTriangle a b c = "Ravnobedren"
    | isTriangle a b c = "Raznostranen"
    | otherwise = "Nevaliden triugulnik"

-- 6.1
safeDivide :: (Eq a, Fractional a) => a -> a -> a
safeDivide _ 0 = 0.0
safeDivide a b = a / b

-- 7.1
quadrant :: (Ord a1, Ord a2, Num a1, Num a2) => a1 -> a2 -> String
quadrant x y
    | x > 0 && y > 0 = "1"
    | x < 0 && y > 0 = "2"
    | x < 0 && y < 0 = "3"
    | x > 0 && y < 0 = "4"
    | x == 0 &&  y /= 0 || x /= 0 && y == 0 = "os"
    | otherwise = "centur"

-- 7.2
isPalindromeNumber :: Integral a => a -> Bool
isPalindromeNumber n = n == reverseNumber n 0
    where reverseNumber 0 acc = acc
          reverseNumber num acc = reverseNumber (num `div` 10) (acc * 10 + num `mod` 10)

-- isPalindromeNumber :: Show a => a -> Bool
-- isPalindromeNumber n = show n == reverse (show n)

-- >>> isPalindromeNumber 121   -- => True
-- >>> isPalindromeNumber 123   -- => False
-- >>> isPalindromeNumber 1221  -- => True
-- True
-- False
-- True

-- 7.3
triangleNum n = n * (n + 1) `div` 2

-- >>> triangleNum 1  -- => 1
-- >>> triangleNum 3  -- => 6
-- >>> triangleNum 5  -- => 15
-- 1
-- 6
-- 15

binomeCoef :: Int -> Int -> Int
bineomeCoef n 0 = 1
binomeCoef n k
  | n < k = 0
  | otherwise = binomeCoef (n - 1) k + binomeCoef (n - 1) (k - 1)

-- >>> binomeCoef 4 0  -- => 1

-- >>> binomeCoef 4 1  -- => 4

-- >>> binomeCoef 4 2  -- => 6

-- >>> binomeCoef 4 3  -- => 4

-- >>> binomeCoef 4 4  -- => 1

-- >>> binomeCoef 10 3 -- => 120

-- >>> binomeCoef 15 5 -- => 3003  