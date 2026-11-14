import math


class Player:
    def __init__(self, __name, hp=10, xp=0):
        self.__name = __name

        self.hp = hp
        if hp < 0:
            raise ValueError(
                f"Invalid value for hp: {hp}. HP must be greater than 0."
            )

        self.xp = xp
        if xp < 300:
            self.__level = 1
        else:
            self.__calculateLevel()

        self.isAlive = True

    def __repr__(self):
        return f"{self.__name} - {self.hp} HP - {self.xp} XP - Level {self.__level}"

    # def __str__(self):
    #     return f"{self.__name} - {self.hp} HP - {self.xp} XP - Level {self.__level}"

    def __calculateLevel(self):
        self.__level = 2 + math.floor(math.log2(self.xp // 300))

    def __onDamageMessageDisplay(self, damage):
        if damage > self.hp:
            print(f"{self.__name} has taken {damage} damage! You have died!")
        else:
            print(f"{self.__name} has taken {damage} damage! {self.hp} HP remain!")

    def takeDamage(self, damage):
        self.hp -= damage
        self.__onDamageMessageDisplay(damage)
        if self.hp <= 0:
            self.isAlive = False

    def giveXP(self, value):
        self.xp += value
        self.__calculateLevel()


testPlayer = Player("Test", 100, 500)
print(testPlayer)
testPlayer.takeDamage(100)
print(testPlayer)
testPlayer.giveXP(1000)
print(testPlayer)
