-- 2
fixing :: [(String, Double, String)]
fixing = [("USD", 1.85, "BGN"), ("EUR", 1.95, "BGN"), ("GBP", 2.3, "BGN")]

canAffordOne (value, course) price wantedCourse fixing = any (\x -> x * price <= value) onlyConvertions
    where
        neededCourseConverts = filter (\(from, _, to) -> from == wantedCourse && to == course) fixing

        onlyConvertions = map (\(_, value, _) -> value) neededCourseConverts

canAfford banknotes fixing price course = any (\(from, rate, to) -> if from == course then 
                                                                    canAffordOne (head $ filter (\(v, c) -> c == to) (removeDublicateCurrencies allPossibleMoney)) price course fixing
                                                                    else 
                                                                    canAffordOne (head $ filter (\(v, c) -> c == from) (removeDublicateCurrencies allPossibleMoney)) price course fixing) neededTransfers 
    where
        waysToGetCourseFst = filter (\(from, _, _) -> from == course) fixing
        waysToGetCourseSnd = filter (\(_, _, to) -> to == course) fixing

        relatedToWantedCourse = map (\(_, _, to) -> to) waysToGetCourseFst ++ map (\(_, _, from) -> from) waysToGetCourseSnd

        convertFrom value currency = [(value * rate, to)| (from, rate, to) <- fixing, from == currency] ++
                                     [(value * rate, from)| (from, rate, to) <- fixing, to == currency] 

        allPossibleMoney = concatMap (uncurry convertFrom) banknotes

        removeDublicateCurrencies [] = []
        removeDublicateCurrencies ((value, currency):rest) = (sum (map fst (filter (\(v, c) -> c == currency) rest)), currency) : removeDublicateCurrencies (filter (\(v, c) -> c /= currency) rest)

        neededCurrency = filter (\(v, c) -> c `elem` relatedToWantedCourse) (removeDublicateCurrencies allPossibleMoney)

        neededTransfers = filter (\(from, _, to) -> from == course && to `elem` map snd neededCurrency || from `elem` map snd neededCurrency && to == course) fixing

-- 3
cycleStats :: [a1] -> [Int] -> [[a1] -> a2] -> [a2]
cycleStats d ws stats = helper neededNumbers neededWS stats ++ cycleStats (drop (length neededNumbers) d) (drop (length neededWS) ws) stats
    where
        neededWS = take (length stats) ws

        neededNumbers = take (sum neededWS) d

        helper [] _ _ = []
        helper _ [] _ = []
        helper _ _ [] = []
        helper nums (w:ws) (func:funcs) = func (take w nums) : helper (drop w nums) ws funcs