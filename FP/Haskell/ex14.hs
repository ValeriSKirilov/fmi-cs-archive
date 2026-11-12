safeSqrt :: Double -> Maybe Double
safeSqrt num
    | num < 0   = Nothing
    | otherwise = Just $ sqrt num 

-- >>> Right 5 >>= \x -> Right (x * 2)
-- Right 10

-- >>> Left "error" >>= \x -> Right (x * 2)
-- Left "error"

-- >>> Right 5 >>= \_ -> Left "failed"
-- Left "failed"

newtype State s a = State { runState :: s -> (a, s) }

instance Functor (State s) where
    fmap :: (a -> b) -> State s a -> State s b
    fmap f (State func) = State $ \s -> let (a, newS) = func s in (f a , newS)
