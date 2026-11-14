-- 1
finDecRats = [(x, y) | x <- [0..], y <- [0..], gcd (x, y) == 1, areEqual (findPrimeDivs y) divsOf10] 
    where
        findDivs n = [x | x <- [2..(n - 1)], n `mod` x == 0]
        divsOf10 = findDivs 10
        findPrimeDivs n = filter isPrime (findDivs n)

        isPrime x = null [y | y <- [2..(x - 1)], x `mod` y == 0]
         
        areEqual [] [] = True
        areEqual (x : xs) (y : ys) = x `elem` (y : ys) && areEqual xs [z | z <- (y : ys), z /= x]

        gcd (0, _) = 1
        gcd (x, y) = head (filter (\num -> x `mod` num == 0 && y `mod` num == 0) (if x == 1 || y == 1 then [1..min x y] else [2..min x y]))

-- 3
s :: [(Integer, String, Integer)]
s = [(1, "A", 10), (1, "B", 5), (2, "A", 15), (3, "A", 6), (3, "A", 10), (4, "B", 10), (5, "B", 5), (6, "B", 6)]

maxDay :: Ord a1 => [(a2, b, a1)] -> a2
maxDay spending = head (map (\(name, _, _) -> name) (filter (\(day, name, sum) -> sum == maxSumSpent) spending))
    where
        maxSumSpent = maximum (map (\(day, name, sum) -> sum) spending)

topShop :: (Eq a1, Eq a2) => [(a1, a2, c)] -> a2
topShop spending = head (map (\(name, visits, days) -> name) (filter (\(name, visits, days) -> visits == maximum allShopsNumberOfVisits) allShopsWithVisits))
    where
        allShops = map (\(day, name, sum) -> name) spending

        removeDubs [] = []
        removeDubs (x : xs) = x : removeDubs [y | y <- xs, y /= x]

        getNumberOfDaysVisited shop = foldr (\(day, name1, sum) (name2, daysVisited, days) -> if name1 == shop && day `notElem` days then (name2, daysVisited + 1, day : days) else (name2, daysVisited, days)) (shop, 0, []) spending
        allShopsWithVisits = map getNumberOfDaysVisited (removeDubs allShops)
        allShopsNumberOfVisits = map (\(name, visits, days) -> visits) allShopsWithVisits

streak :: (Num a1, Num a2, Ord a1, Ord a2, Eq a3, Eq a4) => [(a4, a3, a2)] -> a1
streak spending = maximum (map (getMaxStreak . spentStreak) (removeDubs allShops)) --foldr (\(day, _, sum) lst -> if day != getDay (head lst) then (getSpecificDay day) : lst else lst) [] spending
    where 
        allShops = map (\(day, name, sum) -> name) spending

        removeDubs [] = []
        removeDubs (x : xs) = x : removeDubs [y | y <- xs, y /= x]

        getDay (day, _, _) = day

        getLastDay = getDay (last spending)

        getSpecificDay day sellerName = filter (\(currDay, name, _) -> currDay == day && name == sellerName) spending
        getAllSpendingsTo sellerName = filter (\(_, name, _) -> name == sellerName) spending
        calcDailySpendsHelper day sellerName = foldr (\(_, _, sum) spent -> sum + spent) 0 (filter (\(currDay, _, _) -> currDay == day) (getAllSpendingsTo sellerName))
        
        getAllDays = removeDubs (map (\(day, _, _) -> day) spending) 

        calcDailySpends sellerName = map (\day -> (day, calcDailySpendsHelper day sellerName)) getAllDays

        spentStreak sellerName = foldl (\(prevSpent, streak, maxStreak) (day, spent) -> if spent > prevSpent then (spent, streak + 1, maxStreak) else (0, 0, max streak maxStreak)) (0, 0, 0) (calcDailySpends sellerName)

        getMaxStreak (_, _, maxStreak) = maxStreak